#include "GunBase.h"

#include "Actor/Bullet/Bullets/NormalBullet/NormalBullet.h"

#include "GameConfig.h"
#include "Assets.h"
// コンストラクタ
GunBase::GunBase(IWorld* world, BulletInfo::Bullet_ID bullet_id) :
	world_{ world },
	bullet_id_{ bullet_id }
{
}
// 描画
void GunBase::draw() const
{
	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_.localToWorldMatrix());
	// メッシュの描画
	gsDrawMesh(mesh_id_);
	glPopMatrix();
}
// デフォルト発射
void GunBase::fire(Actor* owner)
{
    // 弾を生成
    auto* bullet = new NormalBullet(world_, owner->transform().position() + GSvector3{ 0.0f, 1.0, 0.0f}, owner, Mesh_Bullet, bullet_info_.get_bullet(bullet_id_));
	world_->add_actor(bullet);
}