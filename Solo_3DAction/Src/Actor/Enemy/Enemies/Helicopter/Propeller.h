#ifndef PROPELLER_H_
#define PROPELLER_H_

#include "Actor/Actor.h"
#include "Rendering/AnimatedMesh.h"

class Propeller : public Actor {
public:
	// コンストラクタ
	Propeller(IWorld* world, const GSvector3& position);
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;
private:
	//// 回転処理
	//void update_smooth_rotate(bool is_start = true, float start_rotate_speed = 0.0f, float acceleration = 1.0f, 
	//							float max_spped = 10.0f, float delta_time);
	// 回転速度
	float rotate_speed_{ 10.0f };
};
#endif