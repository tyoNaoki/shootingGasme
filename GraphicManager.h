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
			//��摜�쐬
			return std::make_unique<GraphicBase>();
		}
		int size = 0;
		for(auto &x:actionDict){
			size +=x.imageInfo.allNum;
		}

		handles.resize(size);
		
		std::unordered_map<std::string,ActionInfo>data;
		int startIndex = 0;
		// �摜�ǂݍ��݂̃��W�b�N������
		for(auto &x:actionDict){
			int check = 0;
			//�ꖇ�G�A�ꖇ�̂�
			if(x.imageInfo.allNum==1){
				handles[startIndex] = LoadGraph(x.fileName.c_str());
				//�ǂݍ��݃G���[
				if (handles[startIndex] == -1) {
					check = -1;
					break;
				}
			}else if(x.imageInfo.xNum == 1&&x.imageInfo.yNum==1){ //�ꖇ�G����������ꍇ
				//�e�A�N�V�������Ƃɓǂݍ���ł���
				for (int i = 0; i < x.imageInfo.allNum; i++) {
					std::string readFileName = AddSuffixToFileName(x.fileName, std::to_string(i));
					handles[startIndex + i] = LoadGraph(readFileName.c_str());
					//�ǂݍ��݃G���[
					if (handles[startIndex + i] == -1) {
						check = -1;
						break;
					}
				}
			}else{ //�ꖇ�G�̒��ɕ����̉摜���܂܂�Ă���ꍇ
				std::vector<int>handle;
				handle.resize(x.imageInfo.allNum);
				check = LoadDivGraph(x.fileName.c_str(),x.imageInfo.allNum,x.imageInfo.xNum,x.imageInfo.yNum,x.imageInfo.xSize,x.imageInfo.ySize,handle.data());

				if(check != -1){
					check = replace_range_with_swap(handles,startIndex,startIndex+x.imageInfo.allNum-1,handle);
				}
			}

			//�G���[�ǂݍ��݂̏ꍇ�A�ǉ������ɃX�L�b�v
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

	//�t�@�C������"."�����ĕԂ�
	std::string AddSuffixToFileName(const std::string& filePath, const std::string& suffix) {
		size_t dotPos = filePath.find_last_of('.');
		if (dotPos == std::string::npos) {
		// �g���q��������Ȃ��ꍇ�̃G���[�n���h�����O
			return "";
		}
		std::string newFilePath = filePath.substr(0, dotPos) + suffix + filePath.substr(dotPos);
		return newFilePath;
	}

	//vector��ʂ�vector�̒��ɒǉ�����
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
	bool Init(); //������

	void CreateGraphic(std::string name); //�摜���[�h

	bool ReadHandle(int &gHandle,const std::string& name,const std::string& actionName);

	bool ReadHandle(int& gHandle, const std::string& name, const std::string& actionName,const int interval);

	void initializeHandle(int& gHandle,int& cur_frame,float& cur_frameAdjust,Vector2D<float>&size,const std::string& name,const std::string& actionName); //�L�����摜������

	void updateHandle(const float deltaTime,int& currentHandle,int& currentframe,float& currentframeAdjust,const std::string& name,std::string actionName); //�A�j���[�V�����X�V

	void updateHandle(const float deltaTime, int& currentHandle, int& currentframe, float& currentframeAdjust, const std::string& name, std::string actionName,const float playSpeed); //�v���C�X�s�[�h���w�肵�ăA�j���[�V�����X�V

	//�ʃA�N�V�����J��
	void TransitionToAction(int&currentHandle,int& currentframe,float& currentFrameAdjust,Vector2D<float>& size, const std::string name,const std::string newAction);

	bool IsReadGraphic(const std::string name);//�ǂݍ��߂Ă��邩�ǂ���

	bool IsReadAction(const std::string name,const std::string actionName)const;//�A�N�V�������L�����ǂ���

	bool IsFinishAction(int currentframe,float currentframeAdjust,std::string name,std::string actionName)const;

private:
	GraphicManager() = default;
	~GraphicManager();

	//<�L�����N�^�[���,�摜���>
	using create_GraphicTemplete = std::unordered_map <std::string,std::vector<ActionDict>>;
	create_GraphicTemplete mGraphicFiles;

	//<�L�����N�^�[���,�摜�f�[�^�i�[�N���X>
	using character_graphic = std::unordered_map<std::string,std::unique_ptr<GraphicBase>>;
	character_graphic mGraphics;

	//�摜�ǂݍ��݃N���X
	std::unique_ptr<ImageLoader>imageLoader;

	bool mInitialize = false;
};

