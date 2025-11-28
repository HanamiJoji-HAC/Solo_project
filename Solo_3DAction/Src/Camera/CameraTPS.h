#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_
#include "Actor/Actor.h"
#include "Input.h"

#include "Actor/ActorManager.h"
#include "State/StateMachine.h"

#include <string>
// 三人称カメラクラス
class CameraTPS : public Actor {
public:
	enum class State {
		Null,       // 空ステート
		Normal,		// 通常ステート
		LockOn,		// ロックオンステート
	};
	// コンストラクタ
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
public:
	// 通常カメラ操作
	void tpp_normal(Actor* other, float delta_time);
	// ヨー・ピッチを計算する
	void update_tpp_normal(GSvector2 stick_input, float delta_time, float sensitivity);
	// カメラの滑らかな補間
	GSvector3 smooth_damp(float speed, float smooth_time, GSvector3 position, GSvector3 velocity, float delta_time);
	// カメラ内に対象がいるか？
	bool is_actor_on_screen(Actor* other);
	// カメラをリセットする
	void reset_camera(Actor* player);
	// ターゲットをリセットする	
	void reset_actor();
	// アクターをロックオンする
	void lock_on_actor(const std::string& tag);
	// プレーヤーを検索
	void find_actor(const std::string& player);
public:
	void change_state(State state_num);
	void add_state();
	Actor* get_player();
private:
	Input& input_ = Input::get_instance();
	float smooth_time_{ 3.0f }; // 補間フレーム数
	GSvector3 debug_position{};
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };
	ActorManager actor_manager_;
	// ロックオン状態か？
	bool is_lock_on_{ false };
	// ターゲットの対象
	Actor* target_ = nullptr;
	// プレーヤー
	Actor* player_;
};
using CameraState = Camera::State;
#endif

