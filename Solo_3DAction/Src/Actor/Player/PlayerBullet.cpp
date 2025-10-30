#include "PlayerBullet.h"
#include "Collision/AttackCollider.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "Math/Line.h"// コンストラクタ
PlayerBullet::PlayerBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity) :
	mesh_{ Mesh_Bullet, 0, 0, 0, false, 1, 0 }
{
	// ワールドを設定
	world_ = world;
	// タグ名
	tag_ = "PlayerAttackTag";
	// アクター名
	name_ = "PlayerBullet";
	// 移動量の初期化
	velocity_ = velocity;
	// 衝突判定用の球体を設定
	collider_ = BoundingSphere{ 0.2f };
	// 座標の初期化
	transform_.position(position);
	// 寿命
	lifespan_timer_ = 60.0f;
	//

}// 更新
void PlayerBullet::update(float delta_time) {
	// 寿命が尽きたら死亡
	if (lifespan_timer_ <= 0.0f) {
		die();
		return;
	}
	// 寿命の更新
	lifespan_timer_ -= delta_time;
	// フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// 交点の座標に補正
		transform_.position(intersect);
		// フィールドに衝突したら死亡
		die();
		return;
	}
	// 移動する（ワールド座標系基準）
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}
// 描画
void PlayerBullet::draw() const {
	// デバッグ表示
	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_.localToWorldMatrix());
	// メッシュの描画
	gsDrawMesh(Mesh_Bullet);
	glPopMatrix();
	collider().draw();
}// 衝突リアクション
void PlayerBullet::react(Actor& other) {
	// 衝突したら死亡
	die();
}