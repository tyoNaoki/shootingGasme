#include "CollisionManager.h"
#include "DebugHelper.h"
#include "Map.h"
#include <algorithm>
#include "CharacterBase.h"
#include "Vector2D.h"

void CollisionManager::Initialize(int gridSize, std::shared_ptr<Map>map)
{
	//�}�b�v�̑S�̂̕����擾
	auto worldSize = map->GetMapSize() * map->GetCellSize();
	mWidth = (int)worldSize.x;
	mHeight = (int)worldSize.y;
	mGridSize = gridSize;
	//�Փ˔���p�̃O���b�h���v�Z�A�쐬
	mGrid.resize((mWidth / mGridSize) * (mHeight / mGridSize));
	mInitialize = true;
}

void CollisionManager::AddCharacter(const Vector2D<float>& position, std::shared_ptr<CharacterBase> character)
{
	if(!mInitialize) return;

	//�Փ˔����p�̃O���b�h�ɓo�^
	int index = GetGridIndex(position);
	if (index >= 0 && index < (int)mGrid.size()) {
		mGrid[index].push_back(character);
		character->SetGridIndex(index);
	}else{
		Singleton<DebugHelper>::get_instance().Add(character->GetName() + "CollisionManager AddActor Invalid mGrid index " + std::to_string(index),3.0f,GetColor(255, 0, 0));
	}
}

void CollisionManager::RemoveCharacter(const Vector2D<float>& position, std::shared_ptr<CharacterBase> character)
{
	if (!mInitialize) return;
	//�Փ˔����p�̃O���b�h����폜
	int index = GetGridIndex(position);
	if (index >= 0 && index < (int)mGrid.size()) {
		auto& cell = mGrid[index];
		cell.erase(std::remove(cell.begin(), cell.end(), character), cell.end());
	}else{
		Singleton<DebugHelper>::get_instance().Add(character->GetName() +"CollisionManager RemoveActor Invalid mGrid index " + std::to_string(index),3.0f,GetColor(255,0,0));
	}
}

void CollisionManager::AddIndex(const int index, std::shared_ptr<CharacterBase> character)
{
	if (!mInitialize) return;
	//�Փ˔����p�̃O���b�h�ɓo�^
	if (index >= 0 && index < (int)mGrid.size()) {
		mGrid[index].push_back(character);
		character->SetGridIndex(index);
	}
	else {
		Singleton<DebugHelper>::get_instance().Add(character->GetName() + "CollisionManager AddActor Invalid mGrid index " + std::to_string(index), 3.0f, GetColor(255, 0, 0));
	}
}

void CollisionManager::RemoveIndex(const int index, std::shared_ptr<CharacterBase> character)
{
	if (!mInitialize) return;
	//�Փ˔����p�̃O���b�h����폜
	if (index >= 0 && index < (int)mGrid.size()) {
		auto& cell = mGrid[index];
		cell.erase(std::remove(cell.begin(), cell.end(), character), cell.end());
	}
}

void CollisionManager::RemoveAllCharacter()
{
	mGrid.clear();
}

const std::vector<std::shared_ptr<CharacterBase>> CollisionManager::GetNearCharacters(const Vector2D<float>& position,const float distance, const std::vector<std::weak_ptr<CharacterBase>>& characterToIgnores)
{
	if (!mInitialize) return std::vector<std::shared_ptr<CharacterBase>>();

	//�������O�Ȃ�A���݂̃O���b�h����G�����邩����
	if(distance == 0.0){
		int index = GetGridIndex(position);
		return mGrid[index];
	}

	float dis = distance;

	//�������L�����̏Փ˃R���W���������������Ȃ�A�R���W�����̑傫�����ɂɋ������킹��
	if(dis>mWidth){
		dis = (float)mWidth;
	}
	if(dis>mHeight){
		dis = (float)mHeight;
	}

	//�V�������W����Փ˗p�̃O���b�h�v�Z
	std::vector<std::shared_ptr<CharacterBase>> result;
	int gridX = static_cast<int>(position.x) / mGridSize;
	int gridY = static_cast<int>(position.y) / mGridSize;
	
	int range = 0;
	if(dis<mGridSize){
		range = 1;
	}else{
		range = static_cast<int>(dis / mGridSize);
	}
	
	//���͂̃O���b�h�ɂ���G���擾
	for (int y = gridY - range; y <= gridY + range; ++y) {
		for (int x = gridX - range; x <= gridX + range; ++x) {
			int index = y * (mWidth / mGridSize) + x;
			if (index >= 0 && index < static_cast<int>(mGrid.size())) {
				Vector2D<float> left((float)x * mGridSize, (float)y * mGridSize);
				Vector2D<float> size((float)mGridSize,(float)mGridSize);
				auto draw = Collision::Rect(left,size);
				//DEBUG_HELPER.DrawCollision(draw);

				//���͂̃L�������珜�O����L����������
				auto& grid = mGrid[index];
				grid.erase(std::remove_if(grid.begin(), grid.end(), [&characterToIgnores](const std::shared_ptr<Actor>& actor) { return std::any_of(characterToIgnores.begin(), characterToIgnores.end(), [&actor](const std::weak_ptr<Actor>& weakActor) { if (auto sharedActor = weakActor.lock()) { return sharedActor->GetName() == actor->GetName(); } return false; }); }), grid.end());
				result.insert(result.end(), grid.begin(), grid.end());
			}
		}
	}

	return result;
}

const std::vector<std::shared_ptr<CharacterBase>> CollisionManager::DetectionNearCharacters(Vector2D<float>& position, Collision::Rect& collision, const float distance, const std::vector<std::weak_ptr<CharacterBase>>& characterToIgnores)
{
	if (!mInitialize) return std::vector<std::shared_ptr<CharacterBase>>();

	//�������O�Ȃ�A���݂̃O���b�h����G�����邩����
	if (distance == 0.0) {
		int index = GetGridIndex(position);
		return mGrid[index];
	}

	float dis = distance;

	//�������L�����̏Փ˃R���W���������������Ȃ�A�R���W�����̑傫�����ɂɋ������킹��
	if (dis > mWidth) {
		dis = (float)mWidth;
	}
	if (dis > mHeight) {
		dis = (float)mHeight;
	}

	std::vector<std::shared_ptr<CharacterBase>> result;
	int gridX = static_cast<int>(position.x) / mGridSize;
	int gridY = static_cast<int>(position.y) / mGridSize;

	//int range = static_cast<int>(dis / mGridSize);
	int range = 0;
	if (dis < mGridSize) {
		range = 1;
	}
	else {
		range = static_cast<int>(dis / mGridSize);
	}

	//�߂��ɏ��O����L�����������ꍇ�A�O���b�h���珜�O����L�����Ƃ��ĕϐ��ŕۑ�
	std::vector<std::shared_ptr<CharacterBase>>Ignores;
	std::vector<int>IgnoresIndex;
	for(auto &x:characterToIgnores){
		auto ignoreChara = x.lock();
		int underIndex = (gridY - range) * (mWidth / mGridSize) + (gridX - range);
		int upIndex = (gridY + range) * (mWidth / mGridSize) + (gridX + range);
		if(underIndex<=ignoreChara->GetGridIndex()&& ignoreChara->GetGridIndex()<=upIndex){
			Ignores.push_back(x.lock());
			IgnoresIndex.push_back(ignoreChara->GetGridIndex());
		}
	}

	//���͂̃O���b�h�ɂ���G���擾
	bool isDetection = false;
	for (int y = gridY - range; y <= gridY + range; ++y) {
		for (int x = gridX - range; x <= gridX + range; ++x) {
			int index = y * (mWidth / mGridSize) + x;

			if (index < 0 || index >= static_cast<int>(mGrid.size())) {
				continue;
			}

			Vector2D<float> left((float)x * mGridSize, (float)y * mGridSize);
			Vector2D<float> size((float)mGridSize, (float)mGridSize);
			
			//���O����L�����̃O���b�h���ǂ���
			bool isCheck = false;
			std::vector<std::weak_ptr<CharacterBase>>checkIgnores;
			int i = 0;
			for(auto &x:IgnoresIndex){
				if(index == x){
					checkIgnores.push_back(characterToIgnores[i]);
					isCheck = true;
				}
				i++;
			}
			
			auto& grid = mGrid[index];

			//���O����L�����̃O���b�h�Ȃ�A�L�������O���b�h�̃L�����B����T���āA���O����
			if(isCheck){
				grid.erase(std::remove_if(grid.begin(), grid.end(), [&checkIgnores](const std::shared_ptr<Actor>& actor) { return std::any_of(checkIgnores.begin(), checkIgnores.end(), [&actor](const std::weak_ptr<Actor>& weakActor) { if (auto sharedActor = weakActor.lock()) { return sharedActor->GetName() == actor->GetName(); } return false; }); }), grid.end());
			}
			
			result.insert(result.end(), grid.begin(), grid.end());
		}
	}
	return result;
}

int CollisionManager::GetGridIndex(const Vector2D<float>& position) const
{
	if (!mInitialize) return -1;

	int x = static_cast<int>(position.x) / mGridSize;

	int y = static_cast<int>(position.y) / mGridSize;

	int index = y * (mWidth / mGridSize) + x;

	// �C���f�b�N�X�͈̔̓`�F�b�N��ǉ� 
	if (index < 0 || index >= static_cast<int>(mGrid.size())) {
		DEBUG_HELPER.Add("GetCellIndex: Position " + position.ToString() + " out of bounds with index ");
		return -1; // �͈͊O�̏ꍇ�͖����ȃC���f�b�N�X��Ԃ� 
	}

	return y * (mWidth / mGridSize) + x;
}

void CollisionManager::UpdateGridIndex(std::shared_ptr<CharacterBase> character)
{
	//�L�����̍��W����A�Փ˔���p�̃O���b�h���X�V
	int oldIndex = character->GetGridIndex();
	int newIndex = GetGridIndex(character->GetWorldPosition2D());
	if(oldIndex==-1){
		//RemoveIndex(oldIndex, character);
		AddIndex(newIndex, character);
		return;
	}else{
		RemoveIndex(oldIndex, character);
		AddIndex(newIndex, character);
		return;
		if (oldIndex != newIndex) {
			//RemoveActor(oldPosition, actor);
			//AddActor(newPosition, actor);
			RemoveIndex(oldIndex, character);
			AddIndex(newIndex, character);
			return;
			//actor->SetOldWorldPosition2D(newPosition);
		}
	}
}

bool CollisionManager::IsCollidingWalls(Vector2D<float>& worldP, Collision::Rect& collision, std::shared_ptr<Map>map)
{
	auto cell = map->GetCellSize();

	//�L����������^�C��������width,height�̃C���f�b�N�X���擾
	int leftWidth = (int)(collision.mLeftTop.x / cell.x);
	int rightWidth = (int)(collision.RightBottom().x / cell.x);
	int upHeight = (int)(collision.mLeftTop.y / cell.y);
	int downHeight = (int)(collision.RightBottom().y / cell.y);
	int width = rightWidth - leftWidth;
	int height = downHeight - upHeight;

	//�L������������^�C��(3*3�̍ō�9�ӏ��A�Œ�S�ӏ��̃^�C���𔻒肷��)
	for (int i = 0; i <= height; i++) {
		for (int j = 0; j <= width; j++) {
			auto worldLeftTop = Vector2D<float>(cell.x * (leftWidth + j), cell.y * (upHeight + i));
			if (map->GetTile(upHeight + i, leftWidth + j) == TileType::WALL) {
				return true;
			}
		}
	}

	return false;
}

bool CollisionManager::IsCharacterInMap(Vector2D<float>& position, Collision::Rect& rect, std::shared_ptr<Map> map)
{
	//�}�b�v���ɂ��邩
	auto mapWorldSize = map->GetMapSize() * map->GetCellSize();
	return !(rect.mLeftTop.x < 0 ||
		rect.RightBottom().x >(mapWorldSize.x) ||
		rect.mLeftTop.y < 0 ||
		rect.RightBottom().y >(mapWorldSize.y));
}

bool CollisionManager::IsBulletInMap(Vector2D<float>& position, std::shared_ptr<Map> map)
{
	//�}�b�v���ɂ��邩
	auto mapWorldSize = map->GetMapSize() * map->GetCellSize();
	return !(position.x < 0 ||
		position.x >(mapWorldSize.x) ||
		position.y < 0 ||
		position.y >(mapWorldSize.y));
}

bool CollisionManager::PushBackFromWalls(Vector2D<float>& worldP, Collision::Rect& rect, std::shared_ptr<Map>map)
{
	bool colliding = false;
	auto cell = map->GetCellSize();

	//�L����������^�C��������width,height�̃C���f�b�N�X���擾
	int leftWidth = (int)(rect.mLeftTop.x / cell.x);
	int rightWidth = (int)(rect.RightBottom().x / cell.x);
	int upHeight = (int)(rect.mLeftTop.y / cell.y);
	int downHeight = (int)(rect.RightBottom().y / cell.y);
	int width = rightWidth - leftWidth;
	int height = downHeight - upHeight;
	DebugHelper& dh = Singleton<DebugHelper>::get_instance();

	//�L������������^�C��(3*3-1(����)�̍ō��W�ӏ��A�Œ�S�ӏ��̃^�C���𔻒肷��)
	for (int i = 0; i <= height; i++) {
		for (int j = 0; j <= width; j++) {
			//�����������肵�Ȃ�
			if (i == 1 && j == 1 && height == 2 && width == 2) { continue; }
			//auto drawRect = rect;
			auto worldLeftTop = Vector2D<float>(cell.x * (leftWidth + j), cell.y * (upHeight + i));
			
			if (map->GetTile(upHeight + i, leftWidth + j) == TileType::WALL) {
				Collision::Rect tileRect(worldLeftTop, cell);
				Collision::PushBackRect(rect, tileRect);
				worldP = rect.mLeftTop + (rect.mSize / 2);
				colliding = true;
			}
		}
	}

	return colliding;
}

//���[�J���ɒ����Čv�Z�����āA���[���h�ŕԂ�
bool CollisionManager::PushBackFromOut(Vector2D<float>& position, Collision::Rect& rect, std::shared_ptr<Map>map)
{
	auto mapWorldSize = map->GetMapSize() * map->GetCellSize();
	bool isOut = false;

	//�߂荞�񂾕��A�e�����Ƃɉ����߂����s��
	if (rect.mLeftTop.x < 0) {
		position.x = (rect.mSize.x / 2.0f);
		isOut = true;
	}
	else if (rect.RightBottom().x > mapWorldSize.x) {
		position.x = mapWorldSize.x - (rect.mSize.x / 2);
		isOut = true;
	}

	if (rect.mLeftTop.y < 0) {
		position.y = (rect.mSize.y / 2.0f);
		isOut = true;
	}
	else if (rect.RightBottom().y > mapWorldSize.y) {
		position.y = mapWorldSize.y - (rect.mSize.y / 2.0f);
		isOut = true;
	}

	rect.mLeftTop = position - (rect.mSize / 2);
	return isOut;
}

bool CollisionManager::IsCollidingCharacter(std::shared_ptr<CharacterBase> self, std::shared_ptr<CharacterBase> target)
{
	auto rect1 = self->GetCollision<Collision::Rect>();
	auto rect2 = target->GetCollision<Collision::Rect>();
	
	return Collision::IsColliding(*rect1,*rect2);
}

void CollisionManager::PushBackActor(const Vector2D<float>& oldPosition, Vector2D<float>& newPosition, Collision::Rect& newCollision, const Collision::Rect& otherCollision)
{
	auto vec = newPosition - oldPosition;
	
	if(vec.x>=0){
		//�����炠�������Ƃ����Ƃ��̂߂荞�ݗ�
		float penetrationFromLeft = newCollision.RightBottom().x - otherCollision.mLeftTop.x;
		newPosition.x -= penetrationFromLeft; //���֖߂�
	}else{
		//�E���炠�������Ƃ����Ƃ��̂߂荞�ݗ�
		float penetrationFromRight = otherCollision.RightBottom().x - newCollision.mLeftTop.x;
		newPosition.x += penetrationFromRight; //�E�֖߂�
	}

	if (vec.y >= 0) {
		//�ォ�瓖���������̂߂荞�ݗ�
		float penetrationFromTop = newCollision.RightBottom().y - otherCollision.mLeftTop.y;
		newPosition.y -= penetrationFromTop; //��֖߂�
	}else{
		//�����瓖���������̂߂荞�ݗ�
		float penetrationFromBottom = otherCollision.RightBottom().y - newCollision.mLeftTop.y;
		newPosition.y += penetrationFromBottom; //���֖߂�
	}
	newCollision.mLeftTop = newPosition - (newCollision.mSize/2);
}

CollisionManager::~CollisionManager()
{
	RemoveAllCharacter();
}
