#pragma once
#include "Singleton.h"
#include "Vector2D.h"
#include <unordered_map>
#include <vector>

#define EFFECT_M Singleton<EffectManager>::get_instance()

struct EffectLineInfo
{
	EffectLineInfo(unsigned int color,int thickNess,float time,float speed):drawColor(color),startThickness(thickNess),drawTime(time),drawSpeed(speed){;}
	EffectLineInfo():drawColor(0),startThickness(0),drawTime(0),drawSpeed(0){;}

	unsigned int drawColor;
	int startThickness;
	float drawTime;
	float drawSpeed = 1.0f;
};

struct EffectLine
{
	EffectLine(EffectLineInfo lineInfo,Vector2D<float>start,Vector2D<float>end):effectLineInfo(lineInfo),drawStart(start),drawEnd(end),currentTime(lineInfo.drawTime){}

	Vector2D<float>drawStart;
	Vector2D<float>drawEnd;
	float currentTime;

	EffectLineInfo effectLineInfo;
};

struct EffectRotaGraph
{
	EffectRotaGraph(std::string effectName,Vector2D<float>drawPosition, Vector2D<float>graphCenterPosition,Vector2D<float>exRate,float radian,int&gHandle,bool transFrag,bool reverseX,bool reverseY,float drawTime) :effectName(effectName),drawPosi(drawPosition),graphCenterPosi(graphCenterPosition), drawExRate(exRate),radian(radian),gHandle(gHandle),transFrag(transFrag),reverseXFlag(reverseX),reverseYFlag(reverseY),currentTime(drawTime) {}

	std::string effectName;
	Vector2D<float>drawPosi;
	Vector2D<float>graphCenterPosi;
	Vector2D<float>drawExRate;
	float radian;
	int gHandle;
	bool transFrag;
	bool reverseXFlag;
	bool reverseYFlag;

	int currentFrame = 0;
	float currentFrame_Adjust = 0.0f;

	float currentTime;
};

class EffectManager
{
friend class Singleton<EffectManager>;

public:
	void Initialize();

	void LoadEffects();

	void AddEffect(std::string name,Vector2D<float>start,Vector2D<float>end);

	void AddEffectRota3(std::string effectName,Vector2D<float>drawPosi, Vector2D<float>graphCenter,Vector2D<float>exRate,float radian,bool transFrag,bool reverseX,bool reverseY,float drawTime);

	void Update(float deltaTime);

	void Draw(float deltaTime);

	void Finitialze();

private:
	bool mInitialize = false;

	const std::string graphicFileName = "Effect";

	EffectManager() = default;
	~EffectManager();

	std::unordered_map<std::string, EffectLineInfo>effectLineDict;

	std::vector<std::string>effectRotaLoadGraphList;

	std::vector<EffectLine>effectLineList;

	std::vector<EffectRotaGraph>effectRotaGraphList;

	
};

