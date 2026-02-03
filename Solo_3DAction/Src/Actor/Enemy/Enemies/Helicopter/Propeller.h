#ifndef PROPELLER_H_
#define PROPELLER_H_

#include "Actor/Actor.h"
#include "Rendering/AnimatedMesh.h"

class EnemyHeilcoptor;

class Propeller : public Actor {
public:
	// コンストラクタ
	Propeller(IWorld* world, const GSvector3& position, EnemyHeilcoptor* enemy_heil);
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;
private:
	// 回転速度
	float rotate_speed_{ 10.0f };
	EnemyHeilcoptor* heil_{ nullptr };
};
#endif