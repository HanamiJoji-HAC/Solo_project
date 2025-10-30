#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_
#include "Actor/Actor.h"
#include "Input.h"
#include <string>
// 三人称カメラクラス
class CameraTPS : public Actor {
public:
	// コンストラクタ
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
public:
	Actor* get_player() const;
	void tpp_normal(Actor* other, float delta_time);
	bool start_debug{ false };
	void update_camera_rotation(GSvector2 stickInput, float deltaTime, float sensitivity,
		float& pitch, float& yaw);
private:
	Input& input_ = Input::get_instance();
	float smooth_time_{ 3.0f }; // 補間フレーム数
	GSvector3 debug_position{};
	float yaw_{};
	float pitch_{};
};

#endif

