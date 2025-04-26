#include "CollisionManager.h"
#include "DebugHelper.h"
#include "Map.h"
#include <algorithm>
#include "CharacterBase.h"
#include "Vector2D.h"

void CollisionManager::Initialize(int gridSize, std::shared_ptr<Map>map)
{
	//マップの全体の幅を取得
	auto worldSize = map->GetMapSize() * map->GetCellSize();
	mWidth = (int)worldSize.x;
	mHeight = (int)worldSize.y;
	mGridSize = gridSize;
	//衝突判定用のグリッドを計算、作成
	mGrid.resize((mWidth / mGridSize) * (mHeight / mGridSize));
	mInitialize = true;
}

void CollisionManager::AddCharacter(const Vector2D<float>& position, std::shared_ptr<CharacterBase> character)
{
	if(!mInitialize) return;

	//衝突判定専用のグリッドに登録
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
	//衝突判定専用のグリッドから削除
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
	//衝突判定専用のグリッドに登録
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
	//衝突判定専用のグリッドから削除
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

	//距離が０なら、現在のグリッドから敵がいるか判定
	if(distance == 0.0){
		int index = GetGridIndex(position);
		return mGrid[index];
	}

	float dis = distance;

	//距離がキャラの衝突コリジョンよりも小さいなら、コリジョンの大きい幅にに距離合わせる
	if(dis>mWidth){
		dis = (float)mWidth;
	}
	if(dis>mHeight){
		dis = (float)mHeight;
	}

	//新しい座標から衝突用のグリッド計算
	std::vector<std::shared_ptr<CharacterBase>> result;
	int gridX = static_cast<int>(position.x) / mGridSize;
	int gridY = static_cast<int>(position.y) / mGridSize;
	
	int range = 0;
	if(dis<mGridSize){
		range = 1;
	}else{
		range = static_cast<int>(dis / mGridSize);
	}
	
	//周囲のグリッドにいる敵を取得
	for (int y = gridY - range; y <= gridY + range; ++y) {
		for (int x = gridX - range; x <= gridX + range; ++x) {
			int index = y * (mWidth / mGridSize) + x;
			if (index >= 0 && index < static_cast<int>(mGrid.size())) {
				Vector2D<float> left((float)x * mGridSize, (float)y * mGridSize);
				Vector2D<float> size((float)mGridSize,(float)mGridSize);
				auto draw = Collision::Rect(left,size);
				//DEBUG_HELPER.DrawCollision(draw);

				//周囲のキャラから除外するキャラを除く
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

	//距離が０なら、現在のグリッドから敵がいるか判定
	if (distance == 0.0) {
		int index = GetGridIndex(position);
		return mGrid[index];
	}

	float dis = distance;

	//距離がキャラの衝突コリジョンよりも小さいなら、コリジョンの大きい幅にに距離合わせる
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

	//近くに除外するキャラがいた場合、グリッドから除外するキャラとして変数で保存
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

	//周囲のグリッドにいる敵を取得
	bool isDetection = false;
	for (int y = gridY - range; y <= gridY + range; ++y) {
		for (int x = gridX - range; x <= gridX + range; ++x) {
			int index = y * (mWidth / mGridSize) + x;

			if (index < 0 || index >= static_cast<int>(mGrid.size())) {
				continue;
			}

			Vector2D<float> left((float)x * mGridSize, (float)y * mGridSize);
			Vector2D<float> size((float)mGridSize, (float)mGridSize);
			
			//除外するキャラのグリッドかどうか
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

			//除外するキャラのグリッドなら、キャラをグリッドのキャラ達から探して、除外する
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

	// インデックスの範囲チェックを追加 
	if (index < 0 || index >= static_cast<int>(mGrid.size())) {
		DEBUG_HELPER.Add("GetCellIndex: Position " + position.ToString() + " out of bounds with index ");
		return -1; // 範囲外の場合は無効なインデックスを返す 
	}

	return y * (mWidth / mGridSize) + x;
}

void CollisionManager::UpdateGridIndex(std::shared_ptr<CharacterBase> character)
{
	//キャラの座標から、衝突判定用のグリッドを更新
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

	//キャラがいるタイルたちのwidth,heightのインデックスを取得
	int leftWidth = (int)(collision.mLeftTop.x / cell.x);
	int rightWidth = (int)(collision.RightBottom().x / cell.x);
	int upHeight = (int)(collision.mLeftTop.y / cell.y);
	int downHeight = (int)(collision.RightBottom().y / cell.y);
	int width = rightWidth - leftWidth;
	int height = downHeight - upHeight;

	//キャラが属するタイル(3*3の最高9箇所、最低４箇所のタイルを判定する)
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
	//マップ内にいるか
	auto mapWorldSize = map->GetMapSize() * map->GetCellSize();
	return !(rect.mLeftTop.x < 0 ||
		rect.RightBottom().x >(mapWorldSize.x) ||
		rect.mLeftTop.y < 0 ||
		rect.RightBottom().y >(mapWorldSize.y));
}

bool CollisionManager::IsBulletInMap(Vector2D<float>& position, std::shared_ptr<Map> map)
{
	//マップ内にいるか
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

	//キャラがいるタイルたちのwidth,heightのインデックスを取得
	int leftWidth = (int)(rect.mLeftTop.x / cell.x);
	int rightWidth = (int)(rect.RightBottom().x / cell.x);
	int upHeight = (int)(rect.mLeftTop.y / cell.y);
	int downHeight = (int)(rect.RightBottom().y / cell.y);
	int width = rightWidth - leftWidth;
	int height = downHeight - upHeight;
	DebugHelper& dh = Singleton<DebugHelper>::get_instance();

	//キャラが属するタイル(3*3-1(中央)の最高８箇所、最低４箇所のタイルを判定する)
	for (int i = 0; i <= height; i++) {
		for (int j = 0; j <= width; j++) {
			//中央だけ判定しない
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

//ローカルに直して計算させて、ワールドで返す
bool CollisionManager::PushBackFromOut(Vector2D<float>& position, Collision::Rect& rect, std::shared_ptr<Map>map)
{
	auto mapWorldSize = map->GetMapSize() * map->GetCellSize();
	bool isOut = false;

	//めり込んだ分、各軸ごとに押し戻しを行う
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
		//左からあたったとしたときのめり込み量
		float penetrationFromLeft = newCollision.RightBottom().x - otherCollision.mLeftTop.x;
		newPosition.x -= penetrationFromLeft; //左へ戻す
	}else{
		//右からあたったとしたときのめり込み量
		float penetrationFromRight = otherCollision.RightBottom().x - newCollision.mLeftTop.x;
		newPosition.x += penetrationFromRight; //右へ戻す
	}

	if (vec.y >= 0) {
		//上から当たった時のめり込み量
		float penetrationFromTop = newCollision.RightBottom().y - otherCollision.mLeftTop.y;
		newPosition.y -= penetrationFromTop; //上へ戻す
	}else{
		//下から当たった時のめり込み量
		float penetrationFromBottom = otherCollision.RightBottom().y - newCollision.mLeftTop.y;
		newPosition.y += penetrationFromBottom; //下へ戻す
	}
	newCollision.mLeftTop = newPosition - (newCollision.mSize/2);
}

CollisionManager::~CollisionManager()
{
	RemoveAllCharacter();
}
