#pragma once
#include "Singleton.h"
#include <vector>
#include<unordered_map>
#include "GraphicBase.h"
#include <string>
#include "DxLib.h"
#include "CharacterBase.h"
#include "Singleton.h"
#include <algorithm>

#define GRAPHIC_M Singleton<GraphicManager>::get_instance()

typedef struct {
	int w, h;
}Size_t;

struct ImageInfo {
	int allNum;
	int xNum;
	int yNum;
	int xSize;
	int ySize;
};


struct ActionDict {
	//ActionDict(std::string name,std::string file,ImageInfo info,float speed):actionName(name),fileName(file),imageInfo(info),animSpeed(speed){}
	//ActionDict(std::string name,ActionDict actionDict,float speed) :actionName(name), fileName(""), imageInfo(actionDict.imageInfo), animSpeed(speed) {}
	
	std::string actionName;
	std::string fileName;
	ImageInfo imageInfo;
	float animSpeed = 1.0f;
	bool isLoop = true;
};


class ImageLoader {
public:
	std::unique_ptr<GraphicBase>loadImages(std::vector<ActionDict>&actionDict) {

		std::vector<int>handles;
		if(actionDict.size() == 0){
			//空画像作成
			return std::make_unique<GraphicBase>();
		}
		int size = 0;
		for(auto &x:actionDict){
			size +=x.imageInfo.allNum;
		}

		handles.resize(size);
		
		std::unordered_map<std::string,ActionInfo>data;
		int startIndex = 0;
		// 画像読み込みのロジックを実装
		for(auto &x:actionDict){
			int check = 0;
			//一枚絵、一枚のみ
			if(x.imageInfo.allNum==1){
				handles[startIndex] = LoadGraph(x.fileName.c_str());
				//読み込みエラー
				if (handles[startIndex] == -1) {
					check = -1;
					break;
				}
			}else if(x.imageInfo.xNum == 1&&x.imageInfo.yNum==1){ //一枚絵が複数ある場合
				//各アクションごとに読み込んでいく
				for (int i = 0; i < x.imageInfo.allNum; i++) {
					std::string readFileName = AddSuffixToFileName(x.fileName, std::to_string(i));
					handles[startIndex + i] = LoadGraph(readFileName.c_str());
					//読み込みエラー
					if (handles[startIndex + i] == -1) {
						check = -1;
						break;
					}
				}
			}else{ //一枚絵の中に複数の画像が含まれている場合
				std::vector<int>handle;
				handle.resize(x.imageInfo.allNum);
				check = LoadDivGraph(x.fileName.c_str(),x.imageInfo.allNum,x.imageInfo.xNum,x.imageInfo.yNum,x.imageInfo.xSize,x.imageInfo.ySize,handle.data());

				if(check != -1){
					check = replace_range_with_swap(handles,startIndex,startIndex+x.imageInfo.allNum-1,handle);
				}
			}

			//エラー読み込みの場合、追加せずにスキップ
			if(check == -1){
				startIndex += x.imageInfo.allNum;
				continue;
			}
			Vector2D<int>size(x.imageInfo.xSize, x.imageInfo.ySize);
			data[x.actionName] = ActionInfo{ startIndex,x.imageInfo.allNum-1,size,x.animSpeed,x.isLoop};
			startIndex += x.imageInfo.allNum;
		}
		
		return std::make_unique<GraphicBase>(actionDict.size(),handles,data);
	}

	//ファイル名に"."をつけて返す
	std::string AddSuffixToFileName(const std::string& filePath, const std::string& suffix) {
		size_t dotPos = filePath.find_last_of('.');
		if (dotPos == std::string::npos) {
		// 拡張子が見つからない場合のエラーハンドリング
			return "";
		}
		std::string newFilePath = filePath.substr(0, dotPos) + suffix + filePath.substr(dotPos);
		return newFilePath;
	}

	//vectorを別のvectorの中に追加する
	bool replace_range_with_swap(std::vector<int>& target, int start, int end, const std::vector<int>& replacement) {
		if (start < 0 || start >= (int)target.size() || end < 0 || end >= (int)target.size() || start > end) {
			return false;
		}
		
		if (end - start + 1 != (int)replacement.size()) {
			return false;
		}
		
		int replacementIndex = 0;
		
		for (int i = start; i <= end && replacementIndex < (int)replacement.size(); ++i, ++replacementIndex) {
			std::swap(target[i], const_cast<int&>(replacement[replacementIndex]));
		}
		return true;
	}

};

class GraphicManager
{
friend class Singleton<GraphicManager>;

public:
	bool Init(); //初期化

	void CreateGraphic(std::string name); //画像ロード

	bool ReadHandle(int &gHandle,const std::string& name,const std::string& actionName);

	bool ReadHandle(int& gHandle, const std::string& name, const std::string& actionName,const int interval);

	void initializeHandle(int& gHandle,int& cur_frame,float& cur_frameAdjust,Vector2D<float>&size,const std::string& name,const std::string& actionName); //キャラ画像初期化

	void updateHandle(const float deltaTime,int& currentHandle,int& currentframe,float& currentframeAdjust,const std::string& name,std::string actionName); //アニメーション更新

	void updateHandle(const float deltaTime, int& currentHandle, int& currentframe, float& currentframeAdjust, const std::string& name, std::string actionName,const float playSpeed); //プレイスピードを指定してアニメーション更新

	//別アクション遷移
	void TransitionToAction(int&currentHandle,int& currentframe,float& currentFrameAdjust,Vector2D<float>& size, const std::string name,const std::string newAction);

	bool IsReadGraphic(const std::string name);//読み込めているかどうか

	bool IsReadAction(const std::string name,const std::string actionName)const;//アクションが有効かどうか

	bool IsFinishAction(int currentframe,float currentframeAdjust,std::string name,std::string actionName)const;

private:
	GraphicManager() = default;
	~GraphicManager();

	//<キャラクター種類,画像情報>
	using create_GraphicTemplete = std::unordered_map <std::string,std::vector<ActionDict>>;
	create_GraphicTemplete mGraphicFiles;

	//<キャラクター種類,画像データ格納クラス>
	using character_graphic = std::unordered_map<std::string,std::unique_ptr<GraphicBase>>;
	character_graphic mGraphics;

	//画像読み込みクラス
	std::unique_ptr<ImageLoader>imageLoader;

	bool mInitialize = false;
};

