#include "StatusInfoPanel.h"
#include "StatusManager.h"
#include "WeaponStatus.h"

StatusInfoPanel::StatusInfoPanel(const int width, const int height, int font ,int color)
{
	//パネルの枠組み設定
	mPanel.mLeftTop = Vector2D<int>(20,100);
	mPanel.mSize = Vector2D<int>(width,height);
	mPanel.mColor = UI::white;
	mPanel.mFillFlag = false;
	mFont = font;
	mColor = color;
	infoList.resize(3 + STATUS.GetEnemyCount());
}

StatusInfoPanel::~StatusInfoPanel()
{
	//文字ハンドルを消去
	DeleteFontToHandle(mFont);
}

void StatusInfoPanel::Update(float deltaTime){}

void StatusInfoPanel::Draw(float deltaTime)
{
	//表示するか
	if (!IsVisible()) { return; }
	//枠組みを描画
	DrawBox(mPanel.mLeftTop.x,mPanel.mLeftTop.y, mPanel.mLeftTop.x+mPanel.mSize.x, mPanel.mLeftTop.y + mPanel.mSize.y,mPanel.mColor,mPanel.mFillFlag);
	//スタート位置計算
	int startX = mPanel.mLeftTop.x + 10;
	int startY = mPanel.mLeftTop.y + 20;
	int i = 0;
	//情報を描画
	for(auto &x:infoList){
		int textY = startY + 10 * (1 + i * 1) + (i * 40);
		DrawStringToHandle(startX, textY, x.c_str(), mColor, mFont);
		i++;
	}
}

void StatusInfoPanel::SetStatusInfo()
{ 
	//現在の各キャラのステータスを取得
	auto playerStatus = STATUS.GetCurrentPlayerStatus();
	auto meleeStatus = std::static_pointer_cast<MeleeWeaponStatus>(STATUS.GetCurrentWeaponStatus("PlayerMeleeComponent"));
	auto gunStatus = std::static_pointer_cast<GunWeaponStatus>(STATUS.GetCurrentWeaponStatus("PlayerGunComponent"));
	infoList[0] = "自分の体力 : " + std::to_string(static_cast<int>(playerStatus.hp));
	float dps = std::floor(CalculateDPS(meleeStatus->mAttack, meleeStatus->mSwingSpeed) * 100.0f) / 100.0f;
	infoList[1] = "自分の近接DPS : " + std::to_string(dps);
	dps = std::floor(CalculateDPS(gunStatus->mAttack, gunStatus->mShotRate) * 100.0f) / 100.0f;
	infoList[2] = "自分の射撃DPS : " + std::to_string(dps);

	int i = 3;
	auto enemies = STATUS.GetAllCurrentEnemyStatus();
	//ステータスを表示テキストに設定
	for (auto& x : enemies) {
		int hp = static_cast<int>(x.hp);
		infoList[i] = "Lv" + std::to_string(x.level) + x.name + "の体力 : " + std::to_string(hp);
		i++;
	}
}

float StatusInfoPanel::CalculateDPS(float damage, float interval)
{
	float attackPerSecond = 1.0f / interval;// 攻撃レート（1秒間に行う攻撃の数
	return damage * attackPerSecond; // 秒間火力の計算
}
