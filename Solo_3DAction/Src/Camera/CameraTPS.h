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
		Reset,		// リセットステート
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
	void update_normal(Actor* player, float delta_time);
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
	// ロックオン時のヨーを設定
	void set_lock_on_yaw(Actor* player, Actor* target, float delta_time);
	// ロックオン時のピッチを設定
	void set_lock_on_pitch(Actor* player, Actor* target, float delta_time);
	// ステートの変更
	void change_state(State state_num);
	// ステートの追加
	void add_state();
public:
	// ヨーとピッチを取得する
	GSvector3 get_yaw_pitch() const;
	// プレイヤー情報を取得
	Actor* get_player() const;
	// ターゲット情報を取得
	Actor* get_target() const;
	// ロックオン状態か？
	bool is_lock_on() const;
	// リセット中か？
	bool is_reset() const;
	// 感度倍率を取得
	float get_senc() const;

private:
	// ラジアンを角度に変換する
	float rad2deg(float rad);
	// 符号付き2点間のベクトルと正面ベクトルとの角度差を算出する
	float forward_angle_diff(GSvector3 target_vector);
	// 現在のステートを文字列で取得する
	const char* current_state_to_string(State state);
	// ヨーの範囲を設定する
	float wrap_yaw(float yaw);
	// 1オフセット値を設定する
	GSvector3 set_offset(State& state);
private:
	// 入力クラス
	Input& input_ = Input::get_instance();
	// スムースダンプの補間フレーム数
	float smooth_time_{ 3.0f };
	// ヨー
	float yaw_{ 0.0f };
	// ピッチ
	float pitch_{ 0.0f };
	// ターゲット
	Actor* target_{ nullptr };
	// プレーヤー
	Actor* player_{ nullptr };
	// ロックオン中か？
	bool is_lock_on_{ false };
	// リセット中か？
	bool is_reset_{ false };
	// オフセット値
	GSvector3 player_offset_{};
};
using CameraState = CameraTPS::State;
using CameraAngleAxis = CameraTPS::AngleAxis;
#endif

