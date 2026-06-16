#include "CannonBody.h"
#include "EnemyCannon.h"

#include "imgui/imgui.h"
#include "Assets.h"

CannonBody::CannonBody(IWorld* world, const GSvector3& position, EnemyCannon* cannon) :
	cannon_{ cannon }
{
	name_ = "EnemyCannon";
	tag_ = "EnemyTag";
	world_ = world;
	transform_.position(position);
}

void CannonBody::update(float delta_time)
{}

void CannonBody::draw() const
{
	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_.localToWorldMatrix());
	// メッシュの描画
	gsDrawMesh(Mesh_Cannon_Body);
	glPopMatrix();
	collider().draw();
}

