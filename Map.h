#pragma once
#include <vector>
#include "Vector2D.h"
#include<unordered_map>

// タイルの種類を定義する
enum class TileType { EMPTY, GROUND, WALL};

class Actor;

// タイルクラス 
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
	//サイズ設定
	Map(Vector2D<int>mapSize,Vector2D<float>cell);

	~Map();
	//マップのタイル設定
	bool Initialize(Vector2D<float>worldPosition);
	//タイル(壁、地面)を設定
	void SetTile(int height,int width,TileType tile);
	//タイルを取得
	TileType GetTile(int height,int width)const; 
	//x(height,y(width)に対応した1タイルの左上座標を取得
	const Vector2D<float>GetTileWorldLocation(int height,int width) const;
	//ワールド座標上のタイル種類取得
	TileType GetTileOnWorldLocation(Vector2D<float>worldLocation);
	//mapのワールド座標を更新する
	void calcMapPosition(Vector2D<float>&newPosition);
	//ワールド座標をローカル座標に変換
	Vector2D<float>GetLocalFromWorldPosition(Vector2D<float>worldPosition);
	//ローカル座標をワールド座標に変換
	Vector2D<float>GetWorldFromLocalPosition(Vector2D<float>localPosition);
	//現在地から今いるタイルの種類を取得
	TileType GetTileOnLocation(Vector2D<float>& location)const;
	//マップのサイズ取得
	Vector2D<int> GetMapSize() const;
	//１タイルのサイズ取得
	Vector2D<float> GetCellSize() const;

	//マップの全タイル描画
	void Draw();

private:
	//マップの大きさ
	Vector2D<int> mMapSize;
	//マップの大きさ＊タイルサイズの全体の大きさ
	float mWorldWidth = 0;
	float mWorldHeight = 0;
	//１タイルの大きさ
	Vector2D<float>mCell;
	//全タイルの種類リスト
	std::vector<std::vector<Tile>>mTiles;
	//初期化フラグ
	bool mInitialize = false;
	//現在のプレイヤーのワールド座標
	Vector2D<float>mWorldPosition;
	//画像関係
	const std::string mGraphicName = "GameMap";
	std::unordered_map<TileType,int>mTileHandles;
};

