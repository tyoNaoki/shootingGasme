#pragma once
#include<memory>
#include<vector>
#include "Vector2D.h"
#include <unordered_map>

//画像データ情報
struct GraphicInfo {
	std::vector<const char*> mFileNames;
	int mAllNum;
	int mXnum;
	int mYnum;
	int mXSize;
	int mYSize;
};

struct ActionInfo {
	int startFrame;
	int interval;
	Vector2D<int> size;
	float playSpeed = 1.0f;
	bool IsLoop = true;
};

class GraphicBase
{
public:
	//GraphicBase(int* handle,const GraphicInfo&);
	GraphicBase(int actionAllNum,std::vector<int>& handle,const std::unordered_map<std::string,ActionInfo>actions);

	//空画像用
	GraphicBase();

	~GraphicBase();

	int GetGHandle(int frame); //画像取得
	int GetStartFrame(std::string actionName);
	int GetInterval(std::string actionName); //最大フレーム取得
	int GetActionAllNum()const; //アクションの種類の数取得
	Vector2D<int>GetSize(std::string actionName);//１アクションの描画枚数取得
	float GetPlaySpeed(std::string actionName);//描画スピード取得

	bool isDummy()const{return dummy;}//ダミー画像フラグ取得
	bool IsLoop(std::string actionName);//ループするか

	bool IsLoadGraphic(std::string actionName); //読み込めているかどうか

private:
	std::vector<int>graph;//画像

	int mInterval; //最大フレーム
	int mNumColumns;//アクションの種類の数
	int mActionAllNum;//全アクション描画合計数

	//見つからない場合、もしくは、サイズ0の場合、Dummyに差し替え
	std::unordered_map<std::string,ActionInfo>mActions;

	bool dummy = false;//ダミーフラグ
};

