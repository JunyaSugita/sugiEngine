#include "Fly.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "EffectManager.h"
#include "ParticleManager.h"
#include "Tutorial.h"
#include "NaviPointManager.h"
#include "ModelManager.h"
#include "ColliderManager.h"

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

	BaseEnemy::Initialize(name, pos);
	WorldTransUpdate();

	obj_.pos.y = FLY_Y;
	obj_.scale.x = 2;

	gap_ = 0.0f;
	col_.size = {2,1,2};

	obj_.obj->SetColor(COLOR_BODY);
	wingL_.obj->SetColor(COLOR_BODY);
	wingR_.obj->SetColor(COLOR_BODY);

	serial_ = 2;
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
	if (obj_.pos.y != FLY_Y) {
		Dead();
	}

	if (!isStop_) {
		Vector2 temp = {};
		//プレイヤー方向に壁が無ければプレイヤー方向に移動
		if (ColliderManager::GetInstance()->CanMoveToPlayer(col_.pos)) {
			temp.x = Player::GetInstance()->GetBoxCol().pos.x;
			temp.y = Player::GetInstance()->GetBoxCol().pos.z;

			//起動してなければ起動
			isStart_ = true;
		}
		else {
			if (isStart_) {
				Vector3 tempPos = ColliderManager::GetInstance()->CanMoveEnemyToNaviPoint(col_.pos);

				temp.x = tempPos.x;
				temp.y = tempPos.z;
			}
			else {
				return;
			}
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

	BaseCol::Update(obj_.pos,obj_.scale);
}

void Fly::Attack()
{
	attackTimer_ += SPEED_WING;
	wingL_.rot.x = EaseOut(attackTimer_, LEN_WING);
	wingR_.rot.x = -EaseOut(attackTimer_, LEN_WING);
}

void Fly::Down()
{
	if (col_.pos.y > 1) {
		col_.pos.y -= SPEED_DROP_DOWN;
	}
	else {
		col_.pos.y = 1;
	}
	obj_.pos = col_.pos;

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

void Fly::ResetShake()
{
	obj_.pos = col_.pos;
	obj_.pos.y = FLY_Y;
}
