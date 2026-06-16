#ifndef CANNONBODY_H_
#define CANNONBODY_H_

#include "Actor/Actor.h"
#include "Rendering/AnimatedMesh.h"

class EnemyCannon;

class CannonBody : public Actor {
public:
	// コンストラクタ
	CannonBody(IWorld* world, const GSvector3& position, EnemyCannon* cannon);
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;
private:
	EnemyCannon* cannon_{ nullptr };
};
#endif