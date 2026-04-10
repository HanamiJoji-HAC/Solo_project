#ifndef MISSLE_H_
#define MISSLE_H_

#include "../BulletBase.h"

class Missle : public BulletBase {
public:
	// コンストラクタ
	Missle();
	// 更新
	virtual void update(float delta_time) override;
};
#endif