#pragma once
#include "Actor.h"
#include <unordered_map>

#define ANIM_M Singleton<AnimationManager>::get_instance()

class Actor;
class Bomb;
class AnimationState;

class AnimationManager {
    friend class Singleton<AnimationManager>;

public:
    void InitializeStates(Actor* actor,std::string actioName);
    virtual void Update(Actor* actor);
    void TransitionToState(Actor* actor, const std::string& actionName);

private:
    AnimationManager();
    ~AnimationManager();

    std::unordered_map<std::string, std::shared_ptr<AnimationState>> mAllState;
};

class AnimationState {
    public:
        virtual ~AnimationState() = default;
        virtual void HandleInput(Actor* actor) = 0;
        virtual std::string GetActionName() const = 0;
};

class IdleState : public AnimationState {
public:
    void HandleInput(Actor* actor) override;

    std::string GetActionName() const override;
};

class WalkingState : public AnimationState {
public:
    void HandleInput(Actor* actor) override;
    std::string GetActionName() const override;
};

class FlightState : public AnimationState {
public:
    void HandleInput(Actor* actor) override;
    std::string GetActionName() const override;
};

class DeadState : public AnimationState {
public:
     void HandleInput(Actor* actor)override;
     std::string GetActionName() const override;
};

class LaserState : public AnimationState {
public:
    void HandleInput(Actor* actor)override;
    std::string GetActionName() const override;
};

class BombState : public AnimationState {
public:
    void HandleInput(Actor* actor)override;
    std::string GetActionName() const override;
private:
    Bomb *bomb;
};

class ExplosionState : public AnimationState {
public:
    void HandleInput(Actor* actor)override;
    std::string GetActionName() const override;
};

class BossIdleState : public AnimationState {
public:
    void HandleInput(Actor* actor)override;
    std::string GetActionName() const override;
};

class BossAttackState : public AnimationState {
public:
    void HandleInput(Actor* actor)override;
    std::string GetActionName() const override;
};