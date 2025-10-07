#ifndef INPUT_H_
#define INPUT_H_
#include <gslib.h>

enum class InputAction {
	FLONT,
	BACK,
	LEFT,
	RIGHT,
	JUMP,   //ジャンプ
	ATTACK  //攻撃
};

class Input {
public:
	// デフォルトコンストラクタ
	Input() = default;
	// デストラクタ
	~Input() = default;

	GSvector2 get_left_stick_input();
	GSvector2 get_right_stick_input();
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

