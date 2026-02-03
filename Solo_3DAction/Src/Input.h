#ifndef INPUT_H_
#define INPUT_H_
#include <gslib.h>

enum class InputAction {
	LSTICK_UP,		// 左スティック上
	LSTICK_DOWN,	// 左スティック下
	LSTICK_LEFT,	// 左スティック左
	LSTICK_RIGHT,	// 左スティック右
	RSTICK_UP,		// 右スティック上
	RSTICK_DOWN,	// 右スティック下
	RSTICK_LEFT,	// 右スティック左
	RSTICK_RIGHT,	// 右スティック右
	IDLE,			// 待機
	JUMP,			// ジャンプ
	ATTACK,			// 攻撃
	SETBOOST,		// ブースト設定
	BOOST,			// ブースト
	FIRE,			// 射撃
	QUICKBOOST,		// クイックブースト
	LOCK_ON,		// ロックオン
	CAMERA_RESET	// カメラリセット
};
class Input {
public:
	// デフォルトコンストラクタ
	Input() = default;
	// デストラクタ
	~Input() = default;
	// インプットクラスのインスタンスを取得
	static Input& get_instance();

	void update(float delta_time);

	// 左スティックの角度を設定
	void set_left_stick_angle();
	// 右スティックの角度を設定
	void set_right_stick_angle();

	// WASDキーによる移動値の更新
	void update_WASD_axis();
	// マウスの移動量の更新
	void update_mouce_axis();
public:
	// ボタンの入力情報を取得
	bool get_action_input(InputAction action) const;
	// 左スティックの状態を取得
	const GSvector2& get_left_stick_axis() const;
	// 右スティックの状態を取得
	const GSvector2& get_right_stick_axis() const;
	// 左スティックの角度を取得
	float get_left_dir() const;
	// 右スティックの角度を取得
	float get_right_dir() const;
	// コントローラー入力か？
	bool is_pad() const;
private:
	GSvector2 left_axis_{};
	GSvector2 right_axis_{};
	float left_dir_{};
	float right_dir_{};
	bool is_pad_{ false };
public:
	//コピー禁止
	Input(const Input& other) = delete;
	Input& operator = (const Input& other) = delete;
};
#endif