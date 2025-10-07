#include "Input.h"

//左スティックの入力を参照する
GSvector2 Input::get_left_stick_input() {
	GSvector2 left_stick_input = GSvector2{ 0.0, 0.0 };
	if (gsGetKeyState(GKEY_W)) {
		left_stick_input.y += 1;
	}
	if (gsGetKeyState(GKEY_A)) {
		left_stick_input.x += -1;
	}
	if (gsGetKeyState(GKEY_S)) {
		left_stick_input.y += -1;
	}
	if (gsGetKeyState(GKEY_D)) {
		left_stick_input.x += 1;
	}
	left_stick_input_ = left_stick_input;
	return left_stick_input_;
}

//右スティックの入力を参照する
GSvector2 Input::get_right_stick_input() {
	if (gsGetKeyState(GKEY_UP)) {
		right_stick_input_.y += 1;
	}
	if (gsGetKeyState(GKEY_LEFT)) {
		right_stick_input_.x += -1;
	}
	if (gsGetKeyState(GKEY_DOWN)) {
		right_stick_input_.y += -1;
	}
	if (gsGetKeyState(GKEY_RIGHT)) {
		right_stick_input_.x += 1;
	}
	//right_stick_input_ = right_stick_input;
	return right_stick_input_;
}

//ボタンの入力を参照する
bool Input::get_action_input(InputAction action) {
	switch (action)
	{
	case InputAction::JUMP:
		return gsGetKeyTrigger(GKEY_SPACE);
	case InputAction::ATTACK:
		return gsGetMouseButtonTrigger(GMOUSE_BUTTON_2);
	default:
		return false;
	}
	return false;
}

Input& Input::get_instance() {
	static Input self;
	return self;
}