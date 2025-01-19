#include "EventManager.h"

void Event::update(float deltaTime)
{
	//�܂����s����Ă��Ȃ��A���s�����𖞂����Ă���
	if (!isExecuted()&& mCondition()) {
		mCurrentTime -= deltaTime;
		if (mCurrentTime <= 0.0f) {
			//���[�v�Ȃ�A�N�[���^�C���ݒ�
			if(mIsLoop){
				mFunc();
				mCurrentTime = mDelayTime;
			}else{//�C�x���g���s�A���s�t���O�����Ă�
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
	//�C�x���g���ԁA��������Ȃǂ��X�V
	for (auto it = mEvents.begin(); it != mEvents.end(); ) {
		it->update(deltaTime);
		if (it->isExecuted()) {
			it = mEvents.erase(it); // ���s�ς݂̃C�x���g���폜 
		}else {
			it++; 
		} 
	}

	//�V�����ǉ��C�x���g������΁A�����Œǉ�
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
