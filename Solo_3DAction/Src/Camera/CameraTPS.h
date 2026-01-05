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
	// 状態
	enum class State {
		Null,       // 空ステート
		Normal,		// 通常ステート
		LockOn,		// ロックオンステート
	};
	// アングル
	enum class AngleAxis {
		Yaw,	// ヨー
		Pitch,	// ピッチ
		Roll	// ロール
	};
	// コンストラクタ
	CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
public:
	void set_camera(Actor* player, float delta_time);
	// 通常カメラ操作
	void tpp_normal(Actor* other, float delta_time);
	// ヨー・ピッチを計算する
	void stick_controll_camera(GSvector2 stick_input, float delta_time, float sensitivity);
	// カメラ内に対象がいるか？
	bool is_actor_on_screen(Actor* other);
	// カメラをリセットする
	void reset_camera(float delta_time);
	// ターゲットをリセットする	
	void reset_target();
	// アクターをロックオンする
	void lock_on_actor(const std::string& tag);
	// プレーヤーを検索
	void find_actor(const std::string& player);
	// ロックオン
	void update_lockon(float delta_time);
	GSvector3 get_yaw_pitch() const;
	// ロックオン時のヨーを設定
	void set_lock_on_yaw(Actor* player, Actor* target, float delta_time);
	// ロックオン時のピッチを設定
	void set_lock_on_pitch(Actor* player, Actor* target, float delta_time);
public:
	// ステートの変更
	void change_state(State state_num);
	// ステートの追加
	void add_state();
	// プレイヤー情報を取得
	Actor* get_player() const;
	// ターゲット情報を取得
	Actor* get_target() const;
	GSvector3 get_target_vector() const;
	// ロックオン状態か？
	bool is_lock_on() const;
	// リセット中か？
	bool is_reset() const;
	// 感度倍率を取得
	float get_senc() const;
private:
	// 入力クラス
	Input& input_ = Input::get_instance();
	// スムースダンプの補間フレーム数
	float smooth_time_{ 3.0f };
	// ヨー
	float yaw_{ 0.0f };
	// ピッチ
	float pitch_{ 0.0f };
	// ターゲットの対象
	Actor* target_ = nullptr;
	// プレーヤー
	Actor* player_;
	// ターゲットとのベクトル
	GSvector3 target_to_player_;
	bool is_reset_{ false };
	// HACK:
	float to_target_distance_;
	GSvector3 actor_position_;
	GSvector3 norma_forward_;
	GSvector3 euler_angle_{};
	GSvector3 desired_rotate_{};
	GSvector3 test_position_{};
};
using CameraState = CameraTPS::State;
using CameraAngleAxis = CameraTPS::AngleAxis;
#endif

