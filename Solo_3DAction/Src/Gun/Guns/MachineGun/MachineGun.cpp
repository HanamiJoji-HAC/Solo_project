#include "MachineGun.h"
#include "Assets.h"

// コンストラクタ
//MachineGun::MachineGun(IWorld* world, BulletBase* bullet) : GunBase(world, bullet)
//{
//	mesh_id_ = Mesh_Bullet;
//}

MachineGun::MachineGun(IWorld* world, BulletInfo::Bullet_ID bullet_id) : GunBase(world, bullet_id)
{
	mesh_id_ = Mesh_Bullet;
}
