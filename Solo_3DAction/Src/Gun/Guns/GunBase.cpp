#include "GunBase.h"

#include "Actor/Bullet/Bullets/NormalBullet/NormalBullet.h"

#include "GameConfig.h"
// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
GunBase::GunBase(IWorld* world, BulletInfo::Bullet_ID bullet_id) :
	world_{ world },
	bullet_id_{ bullet_id }
{
}
// •`‰æ
void GunBase::draw() const
{
	glPushMatrix();
	// À•W•ÏŠ·s—ñ‚ðÝ’è‚·‚é
	glMultMatrixf(transform_.localToWorldMatrix());
	// ƒƒbƒVƒ…‚Ì•`‰æ
	gsDrawMesh(mesh_id_);
	glPopMatrix();
}
// ”­ŽË
void GunBase::fire(Actor* owner)
{
    // ID‚É‰ž‚¶‚Ä’e‚ð¶¬‚·‚é
    BulletBase* bullet = nullptr;

    switch (bullet_id_)
    {
    case BulletInfo::Normal:
        // Json‚©‚ç’eî•ñ‚ð“Ç‚Ýž‚Þ
        BulletInfo::Status normal_bullet_status = json_.lode_bullet_status("Assets/Json/BulletStatusLoader.json", "NormalBullet");
        // ’e‚ð¶¬
        bullet = new NormalBullet(world_, owner->transform().position() + GSvector3{ 0.0f, 1.0, 0.0f}, normal_bullet_status, owner, Mesh_Bullet);
        break;
    }

    world_->add_actor(bullet);
}