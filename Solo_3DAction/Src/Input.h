#ifndef INPUT_H_
#define INPUT_H_
#include <gslib.h>

enum class InputAction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	JUMP,	 //ジャンプ
	ATTACK,  //攻撃
	SETBOOST,
	BOOST,
	FIRE,
	L_UP,
	L_DOWN,
	L_LEFT,
	L_RIGHT
};
class Input {
public:
	// デフォルトコンストラクタ
	Input() = default;
	// デストラクタ
	~Input() = default;

	GSvector2 get_left_stick_input_value();
	float get_left_stick_input_angle();
	GSvector2 get_right_stick_input_value();
	float get_right_stick_input_angle();
	bool get_action_input(InputAction action);
	static Input& get_instance();
private:
	GSvector2 left_stick_input_;
	GSvector2 right_stick_input_;
	InputAction button_input_;
public:
	//コピー禁止
	Input(const Input& other) = delete;
	Input& operator = (const Input& other) = delete;
};
#endif

