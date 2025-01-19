#pragma once
#include "Singleton.h"
#include "Collision.h"
#include <vector>

class Map;
class Actor;
class CharacterBase;

#define COLLISION_M Singleton<CollisionManager>::get_instance()

class CollisionManager
{
friend class Singleton<CollisionManager>;

public:
	void Initialize(int gridSize,std::shared_ptr<Map>map);

	void AddCharacter(const Vector2D<float>& position, std::shared_ptr<CharacterBase> character);

	void RemoveCharacter(const Vector2D<float>& position, std::shared_ptr<CharacterBase> character);

	void AddIndex(const int index, std::shared_ptr<CharacterBase> character);

	void RemoveIndex(const int index, std::shared_ptr<CharacterBase> character);

	void RemoveAllCharacter();

	const std::vector<std::shared_ptr<CharacterBase>> GetNearCharacters(const Vector2D<float>& position,const float distance,const std::vector<std::weak_ptr<CharacterBase>>&characterToIgnores);

	const std::vector<std::shared_ptr<CharacterBase>> DetectionNearCharacters(Vector2D<float>& position,Collision::Rect&collision,const float distance, const std::vector<std::weak_ptr<CharacterBase>>& characterToIgnores);

	int GetGridIndex(const Vector2D<float>& position) const;

	void UpdateGridIndex(std::shared_ptr<CharacterBase>character);

	//��Q������
	bool IsCollidingWalls(Vector2D<float>& worldP, Collision::Rect& collision,std::shared_ptr<Map>map);

	//�}�b�v���ɂ��邩�ǂ���
	bool IsCharacterInMap(Vector2D<float>& position, Collision::Rect& rect, std::shared_ptr<Map>map);

	//�}�b�v���ɂ��邩�ǂ���
	bool IsBulletInMap(Vector2D<float>& position,std::shared_ptr<Map>map);

	//�}�b�v�̊O�ɂ��邩�ǂ���
	bool PushBackFromWalls(Vector2D<float>& worldP, Collision::Rect& rect, std::shared_ptr<Map>map);

	//�}�b�v�O�̃L�����𒆂ɉ����߂�
	bool PushBackFromOut(Vector2D<float>& position, Collision::Rect& rect, std::shared_ptr<Map>map);

	bool IsCollidingCharacter(std::shared_ptr<CharacterBase>self,std::shared_ptr<CharacterBase> target);

	void PushBackActor(const Vector2D<float>&oldPosition,Vector2D<float>&newPosition,Collision::Rect&newCollision,const Collision::Rect&otherCollision);
	
private:
	CollisionManager() = default;
	~CollisionManager();

	bool mInitialize = false;

	int mWidth;
	int mHeight;
	int mGridSize;

	std::shared_ptr<Actor>debugActor;

	std::vector<std::vector<std::shared_ptr<CharacterBase>>>mGrid;
};

