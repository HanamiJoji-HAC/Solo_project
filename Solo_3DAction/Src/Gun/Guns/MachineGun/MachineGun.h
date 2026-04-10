#ifndef MACHINE_GUN_H_
#define MACHINE_GUN_H_

#include "../GunBase.h"
#include "Input.h"
#include "Actor/Bullet/Bullets/NormalBullet/NormalBullet.h"

class MachineGun : public GunBase {
public:
	// コンストラクタ
	MachineGun(IWorld* world, BulletInfo::Bullet_ID bullet_id);
private:
};
#endif