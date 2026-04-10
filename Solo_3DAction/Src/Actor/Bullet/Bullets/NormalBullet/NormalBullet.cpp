#include "NormalBullet.h"
#include "GameConfig.h"

NormalBullet::NormalBullet(IWorld* world, const GSvector3& position, const BulletInfo::Status& status, Actor* owner, unsigned int mesh_num) : 
	BulletBase(world, status, owner, mesh_num)
{
	// 名前を登録
	tag_ = "BulletTag";
	name_ = "NormalBullet";
	// 衝突判定用の球体を設定
	collider_ = BoundingSphere{ 0.2f };
	transform_.position(position);
	set_velocity();
}

// 更新
void NormalBullet::update(float delta_time)
{
	// 寿命が尽きたら死亡
	if (lifespan_timer_ <= 0.0f) {
		die();
		return;
	}
	// 寿命の更新
	lifespan_timer_ -= delta_time / cREF;

	// フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time / cREF;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// 交点の座標に補正
		transform_.position(intersect);
		// フィールドに衝突したら死亡
		die();
		return;
	}
	// 移動する（ワールド座標系基準
	transform_.translate(velocity_ * delta_time / cREF, GStransform::Space::World);
}

// 描画
void NormalBullet::draw() const
{
	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_.localToWorldMatrix());
	// メッシュの描画
	gsDrawMesh(Mesh_Bullet);
	glPopMatrix();
	collider().draw();
}

void NormalBullet::set_velocity()
{
	velocity_ = owner_->transform().forward() * status_.speed_;
}
