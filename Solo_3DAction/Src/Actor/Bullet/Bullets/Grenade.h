#ifndef GRENADE_H_
#define GRENADE_H_

#include "../BulletBase.h"

class Grenade : public BulletBase {
public:
	// コンストラクタ
	Grenade();
	// 更新
	virtual void update(float delta_time) override;
};
#endif