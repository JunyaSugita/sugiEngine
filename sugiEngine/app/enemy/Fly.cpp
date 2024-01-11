#include "Fly.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "NaviPointManager.h"
#include "ModelManager.h"

void Fly::Initialize(std::string name, Vector3 pos)
{
	wingL_.Initialize("box");
	wingL_.worldTrans.parent_ = &obj_.worldTrans;
	wingL_.pos = { 0,0,-1 };
	wingL_.rot = { 0,0,0 };
	wingL_.scale = { 0.8f,0.3f,1 };

	wingR_.Initialize("box");
	wingR_.worldTrans.parent_ = &obj_.worldTrans;
	wingR_.pos = { 0,0,1 };
	wingR_.rot = { 0,0,0 };
	wingR_.scale = { 0.8f,0.3f,1 };


	life_ = MAX_HP;
	angleSpeed_ = SPEED_ANGLE;
	height_ = HEIGHT_COL;

	BaseEnemy::Initialize(name, pos);
	WorldTransUpdate();

	obj_.pos.y = FLY_Y;
	obj_.scale.x = 2;

	cols_.gap.y = 0.0f;
	cols_.col.size.x = 2;
	cols_.col.size.y = 1;
	cols_.col.size.z = 2;

	obj_.obj->SetColor(COLOR_BODY);
	wingL_.obj->SetColor(COLOR_BODY);
	wingR_.obj->SetColor(COLOR_BODY);
}

void Fly::Draw()
{
	BaseEnemy::Draw();
	wingL_.Draw();
	wingR_.Draw();
}

void Fly::WorldTransUpdate()
{
	wingL_.Update();
	wingR_.Update();

	BaseEnemy::WorldTransUpdate();
}

void Fly::Move()
{
	ColliderManager* colM = ColliderManager::GetInstance();
	NaviPointManager* navePointM = NaviPointManager::GetInstance();

	if (obj_.pos.y != FLY_Y) {
		isDead_ = true;
	}

	if (!isStop_) {
		Vector2 temp;
		//プレイヤー方向に壁が無ければプレイヤー方向に移動
		if (colM->CanMovePlayerVec(obj_.pos)) {
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;

			//起動してなければ起動
			isStart_ = true;
		}
		else {
			int32_t point = colM->CanMoveNaviPointVec(obj_.pos);
			//ナビポイントが見つからなければ移動しない
			if (point == -1) {
				return;
			}
			//isStartがfalseなら止まる
			if (!isStart_) {
				return;
			}

			temp.x = navePointM->GetNaviPoint(point).pos.x;
			temp.y = navePointM->GetNaviPoint(point).pos.z;
		}

		toPlayer = Vector2(temp.x - obj_.pos.x, temp.y - obj_.pos.z);
		toPlayer.normalize();

		obj_.pos.x += toPlayer.x * SPEED_MOVE * slow_;
		obj_.pos.z += toPlayer.y * SPEED_MOVE * slow_;
	}
	else {
		isStop_ = false;
	}

	//最後にスピード減少を初期化
	slow_ = 1;
}

void Fly::DontMoveUpdate()
{
	if (debuff_.isIce) {
		obj_.pos.y -= SPEED_DROP;
	}

	if (obj_.pos.y <= 0) {
		debuff_.iceTime = 0;
	}

	cols_.col.pos = obj_.pos;
}

void Fly::Attack()
{
	attackTimer_ += SPEED_WING;
	wingL_.rot.x = EaseOut(attackTimer_, LEN_WING);
	wingR_.rot.x = -EaseOut(attackTimer_, LEN_WING);
}

void Fly::Down()
{
	if (cols_.col.pos.y > 1) {
		cols_.col.pos.y -= SPEED_DROP_DOWN;
	}
	else {
		cols_.col.pos.y = 1;
	}
	obj_.pos = cols_.col.pos;

	//最後
	BaseEnemy::Down();
}

void Fly::WeakBodyColor()
{
	if (Player::GetInstance()->GetPresetSpell() == MAGIC::ICE_BOLT) {
		obj_.obj->SetColor(COLOR_WEAK_BODY);
		wingL_.obj->SetColor(COLOR_WEAK_BODY);
		wingR_.obj->SetColor(COLOR_WEAK_BODY);
		obj_.obj->SetIsBloom();
	}
	else {
		obj_.obj->SetColor(COLOR_BODY);
		wingL_.obj->SetColor(COLOR_BODY);
		wingR_.obj->SetColor(COLOR_BODY);
		obj_.obj->SetIsBloom(false);
	}

	if (isDown_) {
		obj_.obj->SetColor(COLOR_BODY);
		obj_.obj->SetIsBloom(false);
	}
}
