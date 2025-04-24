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
	//����������
	void Initialize(CharacterType ct,std::string typeName,int id, Vector2D<float> localPosition, Vector2D<float> worldPosition,float rot,bool visible);

	virtual bool IsInitialize() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Draw(const float deltaTime) = 0;

	//���[�J�����W�ݒ�
<<<<<<< HEAD
	virtual void SetLocalPosition2D(Vector2D<float> newPosition);
=======
	virtual void SetLocalPosition2D(Vector2D<float> newPosition); 
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
	//���[���h���W�ݒ�
	virtual void SetWorldPosition2D(Vector2D<float> worldPosition);
	//�X�V�O�̃��[���h���W�ݒ�
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
	//���݂̃R���W�����̏����O���b�h�擾�A�X�V
	void SetGridIndex(int index);
	int GetGridIndex();

	//���[�J�����W���烏�[���h���W�ւ̕ϊ�
	virtual Vector2D<float> GetWorldFromLocalPosi(Vector2D<float> localPosition) const;
	//���[���h���W���烍�[�J�����W�ւ̕ϊ�
	virtual Vector2D<float> GetLocalFromWorldPosi(Vector2D<float> worldPosition) const;

	//�v���C���[�A�G�A�e�ۂȂǃI�u�W�F�N�g�Ƃ��ẴJ�e�S���擾
	CharacterType GetActorType()const;
	//�^�C�v�l�[����ID��g�ݍ��킹�Ď擾(��:�S�u����12)
	std::string GetName()const;
	//�L�����̃J�e�S���i�S�u�����A�R�E�����Ȃǁj
	std::string GetTypeName()const;
	//�e�L�������̎���ID
	int GetId()const;

	void SetVelocity(Vector2D<float> vel); //���x�ݒ�
	void ApplyVelocity(); //���x�����W�ɓK�p����
	void SetRotation(float rot); //�p�x�ݒ�
	void SetRatationVelocity(float rot_Vel); //��]���x���Z�b�g(�x)
	
	Vector2D<float>GetWorldPosition2D()const; //���[���h���W�擾
	Vector2D<float>GetOldWorldPosition2D() const;
	int GetGridIndex() const;
	Vector2D<float>GetLocalPosition2D()const; //���[�J�����W�擾
	Vector2D<float>GetVelocity()const; //���x�擾
	float GetRotation()const; //�p�x�擾
	float GetDrawRotation()const; //�`��p�x���擾

	//���݂̃A�N�V�������擾�i�U���A���s�Ȃǁj
	std::shared_ptr<AnimationState>GetCurrentAnimState()const;
	//���̃A�N�V�������ݒ肳��Ă��邩
	bool HasAnimationState(std::string actionName);
	//�A�N�V�����X�V
	void SetAnimationState(std::shared_ptr<AnimationState>animState);

	//���݂̉摜�t���[��.���Ԏ擾
	int GetCurrentAnimFrame()const;
	float GetCurrentAnimFrameAdjust()const;

	//���S�A�j���[�V�����J�n�A�I��
	virtual void StartDeadAnimation();
	virtual void FinishDeadAnimation();

	bool IsReverse() const;//���]�����ǂ���
	bool IsMoving() const; //�ړ������m�F
	bool IsActive()const; 

	void SetActive(bool active);

<<<<<<< HEAD
	ComponentManager& GetComponentManager();

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
protected:
	Vector2D<float> mLocalPosition; //���[�J�����W
	Vector2D<float> mWorldPosition; //���[���h���W
	Vector2D<float> mWorldOldPosition;

	Vector2D<float> mVelocity; //���x
	float mRotation; //�p�x
	float mRotation_vel; //��]���x(�x)

	int mGridIndex = -1;

	bool mReverse = false; //���������ǂ���
	bool mIsMove = false; //�����Ă��邩
	bool mVisible; //�����邩

	//�n�߂�Idle�ŏ�����
	std::string mStartAnimStateName = "Idle";
	//�摜
	int mCurrent_gHandle = -1;
	//���݂̉摜�̃t���[����
	int mCurrent_AnimFrame = 0;
	//�`��t���[���̍X�V����
	float mCurrent_AnimFrame_adjust = 0;

	//�`��֌W�̈ʒu��T�C�Y������
	Vector2D<float>mDrawOffset = Vector2D<float>(0, 0);
	Vector2D<float>mDrawExtendSize = Vector2D<float>(1, 1);
	Vector2D<float>mDrawSize = Vector2D<float>(1, 1);

private:
	//�A�j���[�V�����֌W
	std::shared_ptr<AnimationState>mCurrentAnimState;

	CharacterType mCharaType;

	//�e�L�����̃J�e�S���i��F�S�u�����j
	std::string mTypeName;

	int mId = -1;//�I�u�W�F�N�g���ʗp

	//�A�N�e�B�u�̓Q�[�����œ����Ă��邱�Ƃ������B��A�N�e�B�u�́A�A�N�^�[�}�l�[�W���[�̐�p�f�[�^�Ɋi�[�����
	bool mActive = false;
<<<<<<< HEAD

	ComponentManager mComponentManager;

	std::shared_ptr<Collision::Shape> mActorCollision;
=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
};

