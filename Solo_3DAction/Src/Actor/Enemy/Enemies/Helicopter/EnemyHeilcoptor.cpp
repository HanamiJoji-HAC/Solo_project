#include "EnemyHeilcoptor.h"

EnemyHeilcoptor::EnemyHeilcoptor(IWorld* world, const GSvector3& position, const Status& status) : EnemyBase{ status }
{
	name_ = "EnemyHeilcoptor";
	tag_ = "EnemyTag";
	world_ = world;
	transform_.position(position);

	const GSvector3 prop_offest{ position.x, position.y, position.z };
	propeller_ = new Propeller(world_, prop_offest);
	world_->add_actor(propeller_);
	propeller_->transform().setParent(&transform());
}

void EnemyHeilcoptor::update(float delta_time)
{
	transform_.translate(0, 0.01 * delta_time, 0);
}

void EnemyHeilcoptor::draw() const
{
	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_.localToWorldMatrix());
	// メッシュの描画
	gsDrawMesh(Mesh_Helicopter);
	glPopMatrix();
	collider().draw();
}

void EnemyHeilcoptor::draw_gui() const
{
}

void EnemyHeilcoptor::react(Actor& other)
{
}
