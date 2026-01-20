#include "Propeller.h"

#include "EnemyHeilcoptor.h"
#include "imgui/imgui.h"

Propeller::Propeller(IWorld* world, const GSvector3& position)
{
	name_ = "EnemyHeilcoptor";
	tag_ = "EnemyTag";
	world_ = world;
	transform_.position(position);
}

void Propeller::update(float delta_time)
{
#ifdef _DEBUG
	ImGui::Begin("Propeller");
	ImGui::DragFloat3("Position", transform_.position());
	ImGui::DragFloat("rotate_speed", &rotate_speed_);
	ImGui::End();
#endif
	transform_.eulerAngles(transform_.eulerAngles() + GSvector3(0, rotate_speed_, 0) * delta_time);
}

void Propeller::draw() const
{
	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_.localToWorldMatrix());
	// メッシュの描画
	gsDrawMesh(Mesh_Heil_Prop);
	glPopMatrix();
	collider().draw();
}

