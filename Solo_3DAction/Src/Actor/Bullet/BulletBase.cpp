#include "BulletBase.h"
#include "Actor/Character.h"

// 弾の寿命
float LifeSpanTimer{ 5.0f };

// コンストラクタ
BulletBase::BulletBase(IWorld* world, const BulletInfo::Status& status, Actor* owner, unsigned int mesh_num) : 
	status_{ status },
	owner_{ owner },
	mesh_num{ mesh_num }
{
	world_ = world;
	lifespan_timer_ = LifeSpanTimer;
}

// 更新処理
void BulletBase::update(float delta_time){}

// 衝突判定
void BulletBase::react(Actor& other)
{
	if (other.tag() == owner_->tag() || tag_ == other.tag()) return;
	Character* c = dynamic_cast<Character*>(&other);
	c->take_damage(status_.atk_);
	// TODO:エフェクト、効果音　ヒット処理
	on_hit();
	die();
}

// 描画
void BulletBase::draw() const {}
