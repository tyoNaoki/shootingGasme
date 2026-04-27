#include "EffectManager.h"
#include "GraphicManager.h"
#include "DebugHelper.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Map.h"
#include "DxLib.h"

void EffectManager::Initialize()
{
	effectLineDict.emplace("PlayerMelee",EffectLineInfo(GetColor(255,255,255),10,0.1f,1.0f));
}

void EffectManager::LoadEffects()
{
	GRAPHIC_M.CreateGraphic(graphicFileName);
}

void EffectManager::AddEffect(std::string name,Vector2D<float>start,Vector2D<float>end)
{
	auto itr = effectLineDict.find(name);
	if(itr!=effectLineDict.end()){
		effectLineList.push_back(EffectLine(itr->second,start,end));
	}
}

void EffectManager::AddEffectRota3(std::string effectName, Vector2D<float> drawPosi, Vector2D<float> graphCenter, Vector2D<float> exRate, float radian, bool transFrag, bool reverseX, bool reverseY,float drawTime)
{
	if(GRAPHIC_M.IsReadAction(graphicFileName,effectName)){
		int handle;
		GRAPHIC_M.ReadHandle(handle,graphicFileName,effectName);
		auto effect = EffectRotaGraph(effectName,drawPosi,graphCenter,exRate,radian,handle,transFrag,reverseX,reverseY, drawTime);
		effectRotaGraphList.push_back(effect);
	}
}

void EffectManager::Update(float deltaTime)
{
	//•`‰æ•b”‰z‚¦‚ðˆ—
	for (auto itr = effectLineList.begin(); itr != effectLineList.end();) {
		itr->currentTime-=deltaTime;
		if (itr->currentTime <=0.0f) {
			itr = effectLineList.erase(itr);
		}
		else {
			
			itr++;
		}
	}

	//•`‰æ•b”‰z‚¦‚ðˆ—
	for (auto itr = effectRotaGraphList.begin(); itr != effectRotaGraphList.end();) {
		itr->currentTime -= deltaTime;
		if (itr->currentTime <= 0.0f) {
			itr = effectRotaGraphList.erase(itr);
		}
		else {
			GRAPHIC_M.updateHandle(deltaTime,itr->gHandle,itr->currentFrame,itr->currentFrame_Adjust,graphicFileName,itr->effectName);
			itr++;
		}
	}
}

void EffectManager::Draw(float deltaTime)
{
	auto scene = SCENE_M.GetCurrentScene();

	if(!scene){return;}

	for(auto &x:effectLineList){	

		float currentThickness = x.currentTime <= 0.0f ? 0.0f : x.effectLineInfo.startThickness - x.effectLineInfo.startThickness*(1.0f-(x.currentTime/ x.effectLineInfo.drawTime));
		//float currentThickness = x.effectLineInfo.startThickness;

		Vector2D<int>localStart = scene->GetMap()->GetLocalFromWorldPosition(x.drawStart).ToInt();
		Vector2D<int>localEnd = scene->GetMap()->GetLocalFromWorldPosition(x.drawEnd).ToInt();

		DrawLine(localStart.x, localStart.y, localEnd.x, localEnd.y,x.effectLineInfo.drawColor,currentThickness);
	}

	for(auto &x:effectRotaGraphList){
		auto localPosi = scene->GetMap()->GetLocalFromWorldPosition(x.drawPosi).ToInt();
		DrawRotaGraph3(localPosi.x,localPosi.y,x.graphCenterPosi.x,x.graphCenterPosi.y,x.drawExRate.x,x.drawExRate.y,x.radian ,x.gHandle,x.transFrag,x.reverseXFlag,x.reverseYFlag);
	}
}

void EffectManager::Finitialze()
{
	effectLineList.clear();
	effectRotaGraphList.clear();
}

EffectManager::~EffectManager()
{
	Finitialze();
	effectLineDict.clear();
}
