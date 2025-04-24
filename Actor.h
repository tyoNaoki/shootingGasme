#pragma once
<<<<<<< HEAD

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
#include "Vector2D.h"
#include "Game.h"
#include "DebugHelper.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Map.h"
#include "DxLib.h"
#include "AnimationState.h"
<<<<<<< HEAD
#include "ComponentManager.h"
=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

class AnimationState;
class Collision::Rect;

<<<<<<< HEAD
enum class CharacterType { PLAYER, ENEMY, BULLET,LASER,BOMB, ITEM,BOSS,EMPTY };
=======
enum class CharacterType { PLAYER, ENEMY, BULLET,LASER,BOMB, ITEM,BOSS, EMPTY };
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

class Actor
{
public:
	Actor();
	//初期化処理
	void Initialize(CharacterType ct,std::string typeName,int id, Vector2D<float> localPosition, Vector2D<float> worldPosition,float rot,bool visible);

	virtual bool IsInitialize() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Draw(const float deltaTime) = 0;

	//ローカル座標設定
<<<<<<< HEAD
	virtual void SetLocalPosition2D(Vector2D<float> newPosition);
=======
	virtual void SetLocalPosition2D(Vector2D<float> newPosition); 
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
	//ワールド座標設定
	virtual void SetWorldPosition2D(Vector2D<float> worldPosition);
	//更新前のワールド座標設定
	virtual void SetOldWorldPosition2D(Vector2D<float> worldPosition);

<<<<<<< HEAD
	void SetCollision(std::shared_ptr<Collision::Shape> collision);

	std::shared_ptr<Collision::Shape>&GetCollision()
	{
		return mActorCollision;
	};

	template<typename colType>
	std::shared_ptr<colType>GetCollision()
	{
		return std::dynamic_pointer_cast<colType>(mActorCollision);
	}

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
	//現在のコリジョンの所属グリッド取得、更新
	void SetGridIndex(int index);
	int GetGridIndex();

	//ローカル座標からワールド座標への変換
	virtual Vector2D<float> GetWorldFromLocalPosi(Vector2D<float> localPosition) const;
	//ワールド座標からローカル座標への変換
	virtual Vector2D<float> GetLocalFromWorldPosi(Vector2D<float> worldPosition) const;

	//プレイヤー、敵、弾丸などオブジェクトとしてのカテゴリ取得
	CharacterType GetActorType()const;
	//タイプネームとIDを組み合わせて取得(例:ゴブリン12)
	std::string GetName()const;
	//キャラのカテゴリ（ゴブリン、コウモリなど）
	std::string GetTypeName()const;
	//各キャラ毎の識別ID
	int GetId()const;

	void SetVelocity(Vector2D<float> vel); //速度設定
	void ApplyVelocity(); //速度を座標に適用する
	void SetRotation(float rot); //角度設定
	void SetRatationVelocity(float rot_Vel); //回転速度をセット(度)
	
	Vector2D<float>GetWorldPosition2D()const; //ワールド座標取得
	Vector2D<float>GetOldWorldPosition2D() const;
	int GetGridIndex() const;
	Vector2D<float>GetLocalPosition2D()const; //ローカル座標取得
	Vector2D<float>GetVelocity()const; //速度取得
	float GetRotation()const; //角度取得
	float GetDrawRotation()const; //描画角度を取得

	//現在のアクションを取得（攻撃、歩行など）
	std::shared_ptr<AnimationState>GetCurrentAnimState()const;
	//そのアクションが設定されているか
	bool HasAnimationState(std::string actionName);
	//アクション更新
	void SetAnimationState(std::shared_ptr<AnimationState>animState);

	//現在の画像フレーム.時間取得
	int GetCurrentAnimFrame()const;
	float GetCurrentAnimFrameAdjust()const;

	//死亡アニメーション開始、終了
	virtual void StartDeadAnimation();
	virtual void FinishDeadAnimation();

	bool IsReverse() const;//反転中かどうか
	bool IsMoving() const; //移動中か確認
	bool IsActive()const; 

	void SetActive(bool active);

<<<<<<< HEAD
	ComponentManager& GetComponentManager();

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
protected:
	Vector2D<float> mLocalPosition; //ローカル座標
	Vector2D<float> mWorldPosition; //ワールド座標
	Vector2D<float> mWorldOldPosition;

	Vector2D<float> mVelocity; //速度
	float mRotation; //角度
	float mRotation_vel; //回転速度(度)

	int mGridIndex = -1;

	bool mReverse = false; //左向きかどうか
	bool mIsMove = false; //動いているか
	bool mVisible; //見えるか

	//始めはIdleで初期化
	std::string mStartAnimStateName = "Idle";
	//画像
	int mCurrent_gHandle = -1;
	//現在の画像のフレーム数
	int mCurrent_AnimFrame = 0;
	//描画フレームの更新時間
	float mCurrent_AnimFrame_adjust = 0;

	//描画関係の位置やサイズ微調整
	Vector2D<float>mDrawOffset = Vector2D<float>(0, 0);
	Vector2D<float>mDrawExtendSize = Vector2D<float>(1, 1);
	Vector2D<float>mDrawSize = Vector2D<float>(1, 1);

private:
	//アニメーション関係
	std::shared_ptr<AnimationState>mCurrentAnimState;

	CharacterType mCharaType;

	//各キャラのカテゴリ（例：ゴブリン）
	std::string mTypeName;

	int mId = -1;//オブジェクト識別用

	//アクティブはゲーム内で動いていることを示す。非アクティブは、アクターマネージャーの専用データに格納される
	bool mActive = false;
<<<<<<< HEAD

	ComponentManager mComponentManager;

	std::shared_ptr<Collision::Shape> mActorCollision;
=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
};

