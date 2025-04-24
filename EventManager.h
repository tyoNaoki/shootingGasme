#pragma once
#include "Singleton.h"
#include <functional>
#include <thread>
#include <vector>

#define EVENT_M Singleton<EventManager>::get_instance()

class Event {
public:
	Event(std::function<void()> func, float delayTime)
		: mFunc(func), mCondition([]() { return true; }), mCurrentTime(delayTime), mExecute(false) {}

	Event(std::function<void()> func, std::function<bool()> condition)
		: mFunc(func),mCondition(condition), mExecute(false) {}

	Event(std::function<void()> func, std::function<bool()> condition,float delayTime,bool isLoop)
		: mFunc(func), mCondition(condition),mCurrentTime(delayTime),mDelayTime(delayTime),mExecute(false),mIsLoop(isLoop) {}

	void update(float deltaTime);
	
	bool isExecuted() const;

private:
	std::function<void()> mFunc;
	std::function<bool()> mCondition;
	float mDelayTime = 0.0f;
	float mCurrentTime = 0.0f;
	bool mExecute;
	bool mIsLoop = false;
};

class EventManager
{
friend class Singleton<EventManager>;

public:
	void addEvent(std::function<void()> func, float delayTime);

	void addEvent(std::function<void()> func, std::function<bool()> condition);

	void addEvent(std::function<void()> func, std::function<bool()> condition, float delayTime, bool isLoop);

	void Update(float deltaTime);

	void Reset();

private:
	std::vector<Event> mEvents;
	std::vector<Event> mNewEvents;
};

