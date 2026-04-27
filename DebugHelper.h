#pragma once
#include<vector>
#include<DxLib.h>
#include <string>
#include"Singleton.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "Collision.h"
#include "Map.h"

#define DEBUG_HELPER Singleton<DebugHelper>::get_instance()

struct debugMessage
{
	debugMessage():message(""),color(0),drawTime(1.0f){ ; }
	debugMessage(std::string text,float time,unsigned int c) :message(text), drawTime(time), color(c) { ; }
	std::string message;
	float drawTime;
	unsigned int color;
	float currentDrawTime = 0.0f;
};

struct debugDrawPoint
{
	debugDrawPoint():drawP(Vector2D<float>()),drawTime(0.0f),color(0){;}
	debugDrawPoint(Vector2D<float>p, float time, unsigned int c) :drawP(p), drawTime(time), color(c) { ; }
	debugDrawPoint(Vector2D<float>p,unsigned int c) :drawP(p),drawTime(0.0),color(c) { ; }
	debugDrawPoint(Vector2D<float>p) :drawP(p),drawTime(0.0),color(GetColor(255,0,0)) { ; }

	Vector2D<float>drawP;
	float drawTime = 3.0f;
	unsigned int color;
	float currentDrawTime = 0.0f;
};

struct debugRect {
	debugRect():rect(Collision::Rect()),drawTime(0.0f),color(0){;}
	debugRect(Collision::Rect &rect,float time,bool isHit):rect(rect),drawTime(time){
		if(isHit){
			color = GetColor(255, 0, 0);
		}else{
			color = GetColor(0, 255, 0);
		}
	}

	Collision::Rect rect;
	float drawTime;
	unsigned int color;
	float currentDrawTime = 0.0f;
};

struct debugCircle {
	debugCircle() :circle(Collision::Circle()), drawTime(0.0f), color(0) { ; }
	debugCircle(Collision::Circle& circle, float time, bool isHit) :circle(circle), drawTime(time) {
		if (isHit) {
			color = GetColor(255, 0, 0);
		}
		else {
			color = GetColor(0, 255, 0);
		}
	}

	Collision::Circle circle;
	float drawTime;
	unsigned int color;
	float currentDrawTime = 0.0f;
};

struct debugRotatedRect {
	debugRotatedRect() :rect(Collision::RotatedRect()), drawTime(0.0f), color(0) { ; }
	debugRotatedRect(Collision::RotatedRect& rect, float time, bool isHit) :rect(rect), drawTime(time) {
		if (isHit) {
			color = GetColor(255, 0, 0);
		}
		else {
			color = GetColor(0, 255, 0);
		}
	}

	Collision::RotatedRect rect;
	float drawTime;
	unsigned int color;
	float currentDrawTime = 0.0f;
};

class DebugHelper {
friend class Singleton<DebugHelper>;

	std::vector<debugMessage> List; //•`‰æ‚·‚é€–Ú
	std::vector<debugDrawPoint> drawPointList;
	std::vector<debugRect> drawRectList;
	std::vector<debugCircle> drawCircleList;
	std::vector<debugRotatedRect> drawRotatedRectList;
	std::string debugActorName = "EMPTY";
	int FontHandle = -1; //•`‰æ‚·‚éƒtƒHƒ“ƒg
	bool Initialize = false;
	std::shared_ptr<Map>debugMap;

	int testVolume = 100;
public:
	void SetMap(std::shared_ptr<Map>map) { //ƒtƒHƒ“ƒg‚ğg—p‚µ‚È‚¢ê‡‚Í‚¢‚ç‚È‚¢
		debugMap = map;
	}

	void Add(std::string text) { //•`‰æ‚·‚é€–Ú‚ğ’Ç‰Á
		List.emplace_back(text,0.0f, 0xffffff);
	}

	void Add(std::string text,float time) { //•`‰æ‚·‚é€–Ú‚ğ’Ç‰Á
		List.emplace_back(text, time, 0xffffff);
	}

	void Add(std::string text, float time,unsigned int color) { //•`‰æ‚·‚é€–Ú‚ğ’Ç‰Á
		List.emplace_back(text, time, color);
	}

	void Add() { //‹ós‚ğ’Ç‰Á
		List.emplace_back();
	}

	void Add(float time) { //‰½•bŠÔ‹ós‚ğ’Ç‰Á
		List.emplace_back("",time,0);
	}

	void AddDebugActorName(std::string actorName){
		if(debugActorName != "EMPTY") { return; }
		debugActorName = actorName;
	}

	void Add(Vector2D<float>p, float time, unsigned int c){
		if(debugMap){
			drawPointList.emplace_back(p, time, c);
		}
	}

	void Add(Vector2D<float>p,unsigned int c) {
		if (debugMap) {
			drawPointList.emplace_back(p, c);
		}
		
	}

	void Add(Vector2D<float>p) {
		if (debugMap) {
			drawPointList.emplace_back(p);
		}
	}

	void DrawCollision(Collision::Rect rect){
		if (debugMap) {
			drawRectList.emplace_back(rect,0.0f,false);
		}
	}

	void DrawCollision(Collision::Rect rect,float time,bool isHit){
		if (debugMap) {
			drawRectList.emplace_back(rect,time,isHit);
		}
	}

	void DrawCollision(Collision::Circle circle) {
		if (debugMap) {
			drawCircleList.emplace_back(circle, 0.0f, false);
		}
	}

	void DrawCollision(Collision::Circle circle, float time, bool isHit) {
		if (debugMap) {
			drawCircleList.emplace_back(circle, time, isHit);
		}
	}

	void DrawCollision(Collision::RotatedRect rect) {
		if (debugMap) {
			drawRotatedRectList.emplace_back(rect, 0.0f, false);
		}
	}

	void DrawCollision(Collision::RotatedRect rect, float time, bool isHit) {
		if (debugMap) {
			drawRotatedRectList.emplace_back(rect, time, isHit);
		}
	}

	bool IsDebugActor(const std::string name){
		if(debugActorName == "EMPTY") { return false; }
		return debugActorName == name;
	}

	void Update(const float deltaTime) {
		/*
		if(Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_1)){
			testVolume-=5;
			SOUND_M.ChangeSoundVolume("testSound",testVolume);
			Add("testVolume is " + std::to_string(testVolume),0.5f);
		}
		if (Singleton<InputManager>::get_instance().IsPushKeyNow(KEY_INPUT_2)) {
			testVolume += 5;
			SOUND_M.ChangeSoundVolume("testSound", testVolume);
			Add("testVolume is " + std::to_string(testVolume), 0.5f);
		}
		*/
		
		bool isPause;
		if(SCENE_M.GetCurrentScene()){
			isPause = SCENE_M.GetCurrentScene()->IsPaused();
		}else{
			isPause = false;
		}
		
		//ƒƒbƒZ[ƒWˆ—
		int i = 0;
		for (auto& t : List) {
			if (t.message == "") { //‹ósˆ—
				i++;
				continue;
			}
			int x, y, l;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127); //”wŒi‚Ì‹éŒ`‚ğ”¼“§–¾‚É‚·‚é
			GetDrawStringSizeToHandle(&x, &y, &l, t.message.c_str(), (int)t.message.length(), FontHandle); //Šes‚Ì‘å‚«‚³‚ğæ“¾
			DrawBox(0, i * y, x, i * y + y, 0x000000, TRUE); //”wŒi‚Ì‹éŒ`‚ğ•`‰æ
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //”¼“§–¾‚©‚ç’Êí‚É–ß‚·
			DrawStringToHandle(0, i * y, t.message.c_str(),t.color, FontHandle); //•¶š—ñ‚ğƒtƒHƒ“ƒg‚ğg‚Á‚Ä•`‰æ

			if(!isPause){
				t.currentDrawTime += deltaTime;
			}
			i++;
		}

		if(debugMap){
			//ƒ|ƒCƒ“ƒg•`‰æ
			for (auto& p : drawPointList) {
				Vector2D<float>draw = debugMap->GetLocalFromWorldPosition(p.drawP);

				int x = static_cast<int>(draw.x);
				int y = static_cast<int>(draw.y);
				DrawCircle(x,y,10,p.color);

				if (!isPause) {
					p.currentDrawTime += deltaTime;
				}
			}

			for(auto &r : drawRectList){
				Collision::Rect draw(debugMap->GetLocalFromWorldPosition(r.rect.mLeftTop),r.rect.mSize);

				int x1 = static_cast<int>(draw.mLeftTop.x);
				int y1 = static_cast<int>(draw.mLeftTop.y);
				int x2 = static_cast<int>(draw.RightBottom().x);
				int y2 = static_cast<int>(draw.RightBottom().y);
				DrawBox(x1, y1,x2,y2,r.color,FALSE);

				if (!isPause) {
					r.currentDrawTime += deltaTime;
				}
			}

			for (auto& c : drawCircleList) {
				Collision::Circle draw(debugMap->GetLocalFromWorldPosition(c.circle.mCenter), c.circle.mRadius);

				int x1 = static_cast<int>(draw.mCenter.x);
				int y1 = static_cast<int>(draw.mCenter.y);
				int r = static_cast<int>(draw.mRadius);
				DrawCircle(x1, y1, r, c.color, FALSE);

				if (!isPause) {
					c.currentDrawTime += deltaTime;
				}
			}

			for(auto &r : drawRotatedRectList){
				auto lines = r.rect.GetRotatedRectLines();

				for(int i=0;i<4;i++){
					Vector2D<float>localPosi = debugMap->GetLocalFromWorldPosition(lines[i].mStart);
					Vector2D<int>drawP1 = localPosi.ToInt();
					Vector2D<float>localPosi2 = debugMap->GetLocalFromWorldPosition(lines[i].mEnd);
					Vector2D<int>drawP2 = localPosi2.ToInt();
					DrawLine(drawP1.x,drawP1.y,drawP2.x,drawP2.y,r.color,1);
				}
				
				if (!isPause) {
					r.currentDrawTime += deltaTime;
				}
				
			}
		}

		//•`‰æ•b”‰z‚¦‚ğˆ—
		for(auto itr = List.begin();itr!=List.end();){
			if(itr->currentDrawTime > itr->drawTime){
				itr = List.erase(itr);
			}else{
				itr++;
			}
		}

		//•`‰æ•b”‰z‚¦‚ğˆ—
		for (auto itr = drawPointList.begin(); itr != drawPointList.end();) {
			if (itr->currentDrawTime > itr->drawTime) {
				itr = drawPointList.erase(itr);
			}else {
				itr++;
			}
		}

		for (auto itr = drawRectList.begin(); itr != drawRectList.end();) {
			if (itr->currentDrawTime > itr->drawTime) {
				itr = drawRectList.erase(itr);
			}
			else {
				itr++;
			}
		}

		for (auto itr = drawCircleList.begin(); itr != drawCircleList.end();) {
			if (itr->currentDrawTime > itr->drawTime) {
				itr = drawCircleList.erase(itr);
			}
			else {
				itr++;
			}
		}

		for (auto itr = drawRotatedRectList.begin(); itr != drawRotatedRectList.end();) {
			if (itr->currentDrawTime > itr->drawTime) {
				itr = drawRotatedRectList.erase(itr);
			}
			else {
				itr++;
			}
		}

		//List.clear(); //‚·‚×‚Ä•`‰æ‚µI‚¦‚½‚çAƒŠƒXƒg‚ğÁ‹‚·‚é
	}

private:
	DebugHelper(){ FontHandle = CreateFontToHandle("‚l‚rƒSƒVƒbƒN", 16, 2); };
	~DebugHelper(){
		DeleteFontToHandle(FontHandle);debugMap = nullptr;
	};
};