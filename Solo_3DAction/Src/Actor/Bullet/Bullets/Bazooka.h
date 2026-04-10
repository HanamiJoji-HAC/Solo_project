#ifndef BAZOOKA_H_
#define BAZOOKA_H_

#include "../BulletBase.h"

class Bazooka : public BulletBase {
	// コンストラクタ
	Bazooka();
	// 更新
	virtual void update(float delta_time) override;
};
#endif