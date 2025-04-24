#include "Map.h"
#include "DxLib.h"
#include "Actor.h"
#include "ActorManager.h"
#include "CharacterBase.h"
#include "GraphicManager.h"
#include "DebugHelper.h"

Map::Map(Vector2D<int>mapSize, Vector2D<float>cell):mMapSize(mapSize),mCell(cell)
{
	//�}�b�v�T�C�Y�v�Z
	mWorldHeight = mMapSize.y * mCell.y;
	mWorldWidth = mMapSize.x * mCell.x;
	//�^�C���z��ݒ�
	mTiles = std::vector<std::vector<Tile>>(mMapSize.y, std::vector<Tile>(mMapSize.x));
	//�e�^�C���̉摜�擾
	GRAPHIC_M.CreateGraphic(mGraphicName);
	GRAPHIC_M.ReadHandle(mTileHandles[TileType::GROUND],mGraphicName,"GROUND");
	GRAPHIC_M.ReadHandle(mTileHandles[TileType::WALL], mGraphicName, "WALL");
}

Map::~Map()
{
	//�摜����
	for(auto &x:mTileHandles){
		DeleteGraph(x.second);
	}
	mTileHandles.clear();
}

bool Map::Initialize(Vector2D<float>worldPosition)
{
	mWorldPosition = worldPosition;
	//�^�C���Z�b�g
	for(int i = 0;i<mMapSize.y;i++){
		for(int j = 0;j <mMapSize.x;j++){
			//�g�g�݂�wall�Ŗ��߂�
			if (i==mMapSize.y-1|| i == 0){
				SetTile(i,j,TileType::WALL);
			}else if(j==mMapSize.x-1|| j==0){
				SetTile(i, j, TileType::WALL);
			}else{
				SetTile(i, j, TileType::GROUND);
			}
		}
	}

	mInitialize = true;
	return true;
}

void Map::SetTile(int height,int width,TileType tile)
{
	//�^�C�����Ȃ��A�������͏c�����}�b�v�T�C�Y�𒴂��Ă��Ȃ���
	if(mTiles.size()==0 || height >= mMapSize.y || height < 0 ||width >= mMapSize.x||width < 0){return;}

	//�w�肵���c�A���̍��W�Ƀ^�C���ݒ�
	mTiles[height][width].setType(tile);
}

TileType Map::GetTile(int height,int width)const
{
	//�^�C�����Ȃ��A�������͏c�����}�b�v�T�C�Y�𒴂��Ă��Ȃ���
	if (mTiles.size() == 0 || height >= mMapSize.y || height < 0 || width >= mMapSize.x || width < 0) { return TileType::EMPTY; }

	//�^�C���̎�ގ擾
	return mTiles[height][width].getType();
}

const Vector2D<float> Map::GetTileWorldLocation(int height, int width) const
{
	//�^�C�����Ȃ��A�������͏c�����}�b�v�T�C�Y�𒴂��Ă��Ȃ���
	if (mTiles.size() == 0 || height >= mMapSize.y || height < 0 || width >= mMapSize.x || width < 0) { return Vector2D<float>(); }

	//�P�^�C���̍���̍��W���擾
	return Vector2D<float>(mCell.x*width,mCell.y*height);
}

TileType Map::GetTileOnWorldLocation(Vector2D<float> worldLocation)
{
	//�c�A���v�Z
	int height = static_cast<int>(worldLocation.y/ mCell.y);
	int width = static_cast<int>(worldLocation.x / mCell.x);

	//�^�C���̎�ގ擾	
	return GetTile(height,width);
}

void Map::calcMapPosition(Vector2D<float>&newPosition)
{
	// �X�N���[���̒�������ɂ��� 
	int width,height;
	GetWindowSize(&width,&height);
	Vector2D<float> screenCenter(width/2, height/2);

	// �v���C���[�̈ʒu����V�����}�b�v�̈ʒu���v�Z 
	mWorldPosition = newPosition - screenCenter;

	Vector2D<float> worldMapSize = (mMapSize*mCell);
	float screenWidth = screenCenter.x * 2.0f;
	float screenHeight = screenCenter.y * 2.0f;

	//newPosition = mWorldPosition + mapOffset;
	
	// �}�b�v�̋��E�𒴂��Ȃ��悤�ɐ���
	if (mWorldPosition.x < 0) {
		mWorldPosition.x = 0;
	}

	if (mWorldPosition.x > (worldMapSize.x - screenWidth)){
		mWorldPosition.x = worldMapSize.x - screenWidth;
	}
	
	if (mWorldPosition.y < 0) {
		mWorldPosition.y = 0;
	}
	
	if (mWorldPosition.y > (worldMapSize.y - screenHeight)) {
		mWorldPosition.y = worldMapSize.y - screenHeight;
	}
}

Vector2D<float> Map::GetLocalFromWorldPosition(Vector2D<float> worldPosition)
{
	return worldPosition - mWorldPosition;
}

Vector2D<float> Map::GetWorldFromLocalPosition(Vector2D<float> localPosition)
{
	return localPosition + mWorldPosition;
}

TileType Map::GetTileOnLocation(Vector2D<float>& location) const
{
	//���W�ɑΉ������^�C���̎�ގ擾
	auto roundlocation = Vector2D<float>::GetRoundValue(location);
	if(mTiles.size()==0){return TileType::EMPTY;}
	return mTiles.at((int)roundlocation.y).at((int)roundlocation.x).getType();
}

Vector2D<int> Map::GetMapSize() const
{
	return mMapSize;
}

Vector2D<float> Map::GetCellSize() const
{
	return mCell;
}

void Map::Draw()
{
	//�f�o�b�O�p�F
	auto wallColor = GetColor(0, 200, 0);
	//�e�^�C�����Ƃ̉摜�`��
	for (int i = 0;i < mMapSize.y;i++) {
		for (int j = 0;j < mMapSize.x;j++) {
			int x1 = static_cast<int>(j * mCell.x - mWorldPosition.x);
			int y1 = static_cast<int>(i * mCell.y - mWorldPosition.y);
			int x2 = static_cast<int>(j * mCell.x + mCell.x - mWorldPosition.x);
			int y2 = static_cast<int>(i * mCell.y + mCell.y - mWorldPosition.y);
			if(GetTile(i,j) == TileType::WALL){
				DrawExtendGraph(x1, y1,x2,y2, mTileHandles[TileType::WALL], TRUE);
			}else if(GetTile(i, j) == TileType::GROUND){
				DrawExtendGraph(x1, y1, x2, y2, mTileHandles[TileType::GROUND], TRUE);
			}
		}
	}
}
	
