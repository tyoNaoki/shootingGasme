#include "EventManager.h"

void Event::update(float deltaTime)
{
	//まだ実行されていない、実行条件を満たしている
	if (!isExecuted()&& mCondition()) {
		mCurrentTime -= deltaTime;
		if (mCurrentTime <= 0.0f) {
			//ループなら、クールタイム設定
			if(mIsLoop){
				mFunc();
				mCurrentTime = mDelayTime;
			}else{//イベント実行、実行フラグを建てる
				mFunc();
				mExecute = true;
			}
		}
	}
}

bool Event::isExecuted() const
{
	return mExecute;
}

void EventManager::addEvent(std::function<void()> func, float delaySeconds)
{
	mNewEvents.emplace_back(func,delaySeconds);
}

void EventManager::addEvent(std::function<void()> func, std::function<bool()> condition)
{
	mNewEvents.emplace_back(func, condition);
}

void EventManager::addEvent(std::function<void()> func, std::function<bool()> condition,float delaySeconds,bool isLoop)
{
	mNewEvents.emplace_back(func, condition,delaySeconds,isLoop);
}

void EventManager::Update(float deltaTime)
{
	//イベント時間、条件判定などを更新
	for (auto it = mEvents.begin(); it != mEvents.end(); ) {
		it->update(deltaTime);
		if (it->isExecuted()) {
			it = mEvents.erase(it); // 実行済みのイベントを削除 
		}else {
			it++; 
		} 
	}

	//新しい追加イベントがあれば、ここで追加
	if(mNewEvents.size()!=0){
		mEvents.insert(mEvents.end(),mNewEvents.begin(),mNewEvents.end());
		mNewEvents.clear();
	}
}

void EventManager::Reset()
{
	mEvents.clear();
	mNewEvents.clear();
}
