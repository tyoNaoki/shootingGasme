#pragma once
#include <vector>
#include "Vector2D.h"
#include<unordered_map>

// �^�C���̎�ނ��`����
enum class TileType { EMPTY, GROUND, WALL};

class Actor;

// �^�C���N���X 
class Tile 
{ 
public: 
	Tile(TileType type = TileType::EMPTY) : mType(type) {} 
	TileType getType() const { return mType; } 
	void setType(TileType type) { mType = type; } 
private: 
	TileType mType; 
};

class Map
{
public:
	//�T�C�Y�ݒ�
	Map(Vector2D<int>mapSize,Vector2D<float>cell);

	~Map();
	//�}�b�v�̃^�C���ݒ�
	bool Initialize(Vector2D<float>worldPosition);
	//�^�C��(�ǁA�n��)��ݒ�
	void SetTile(int height,int width,TileType tile);
	//�^�C�����擾
	TileType GetTile(int height,int width)const; 
	//x(height,y(width)�ɑΉ�����1�^�C���̍�����W���擾
	const Vector2D<float>GetTileWorldLocation(int height,int width) const;
	//���[���h���W��̃^�C����ގ擾
	TileType GetTileOnWorldLocation(Vector2D<float>worldLocation);
	//map�̃��[���h���W���X�V����
	void calcMapPosition(Vector2D<float>&newPosition);
	//���[���h���W�����[�J�����W�ɕϊ�
	Vector2D<float>GetLocalFromWorldPosition(Vector2D<float>worldPosition);
	//���[�J�����W�����[���h���W�ɕϊ�
	Vector2D<float>GetWorldFromLocalPosition(Vector2D<float>localPosition);
	//���ݒn���獡����^�C���̎�ނ��擾
	TileType GetTileOnLocation(Vector2D<float>& location)const;
	//�}�b�v�̃T�C�Y�擾
	Vector2D<int> GetMapSize() const;
	//�P�^�C���̃T�C�Y�擾
	Vector2D<float> GetCellSize() const;

	//�}�b�v�̑S�^�C���`��
	void Draw();

private:
	//�}�b�v�̑傫��
	Vector2D<int> mMapSize;
	//�}�b�v�̑傫�����^�C���T�C�Y�̑S�̂̑傫��
	float mWorldWidth = 0;
	float mWorldHeight = 0;
	//�P�^�C���̑傫��
	Vector2D<float>mCell;
	//�S�^�C���̎�ރ��X�g
	std::vector<std::vector<Tile>>mTiles;
	//�������t���O
	bool mInitialize = false;
	//���݂̃v���C���[�̃��[���h���W
	Vector2D<float>mWorldPosition;
	//�摜�֌W
	const std::string mGraphicName = "GameMap";
	std::unordered_map<TileType,int>mTileHandles;
};

