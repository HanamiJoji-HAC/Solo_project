#include "Input.h"
//左スティックの入力を参照する(GSvector2)
GSvector2 Input::get_left_stick_input_value() {
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
	return left_stick_input;
}
// 左スティックの角度を返却する
float Input::get_left_stick_input_angle() {
	GSvector2 left_stick_input = get_left_stick_input_value();
	float stick_angle = atan2(left_stick_input.x, left_stick_input.y);
	//斜めに各方向の最大値を取る
	float dir = (stick_angle / GS_PI) * 180 + 45;
	return dir;
}

//右スティックの入力を参照する
GSvector2 Input::get_right_stick_input_value() {
	GSvector2 right_stick_input = GSvector2{ 0.0, 0.0 };
	if (gsGetKeyState(GKEY_UP)) {
		right_stick_input.y += 1;
	}
	if (gsGetKeyState(GKEY_LEFT)) {
		right_stick_input.x += -1;
	}
	if (gsGetKeyState(GKEY_DOWN)) {
		right_stick_input.y += -1;
	}
	if (gsGetKeyState(GKEY_RIGHT)) {
		right_stick_input.x += 1;
	}
	return right_stick_input;
}

float Input::get_right_stick_input_angle() {
	GSvector2 right_stick_input = get_right_stick_input_value();
	float stick_angle = atan2(right_stick_input.x, right_stick_input.y);
	//斜めに各方向の最大値を取る
	float dir = (stick_angle / GS_PI) * 180 + 45;
	return dir;
}

//ボタンの入力を参照する
bool Input::get_action_input(InputAction action) {
	int left_dir = get_left_stick_input_angle();
	int right_dir = get_right_stick_input_angle();
	switch (action)
	{
	case InputAction::JUMP:
		return gsGetKeyTrigger(GKEY_SPACE);
	case InputAction::SETBOOST:
		return gsGetKeyTrigger(GKEY_SPACE);
	case InputAction::BOOST:
		return gsGetKeyState(GKEY_SPACE);
	case InputAction::ATTACK:
		return gsGetMouseButtonTrigger(GMOUSE_BUTTON_1);
	case InputAction::UP:
		return left_dir <= 90 && left_dir >= 0 && gsGetKeyState(GKEY_W);
	case InputAction::RIGHT:
		return left_dir <= 180 && left_dir > 90 && gsGetKeyState(GKEY_D);
	case InputAction::DOWN:
		return left_dir < -90 || left_dir > 180 && gsGetKeyState(GKEY_S);
	case InputAction::LEFT:
		return left_dir < 0 && left_dir >= -90 && gsGetKeyState(GKEY_A);
	case InputAction::L_UP:
		return right_dir <= 90 && right_dir >= 0 && gsGetKeyState(GKEY_UP);
	case InputAction::L_RIGHT:
		return right_dir <= 180 && right_dir > 90 && gsGetKeyState(GKEY_DOWN);
	case InputAction::L_DOWN:
		return right_dir < -90 || right_dir > 180 && gsGetKeyState(GKEY_LEFT);
	case InputAction::L_LEFT:
		return right_dir < 0 && right_dir >= -90 && gsGetKeyState(GKEY_RIGHT);
	case InputAction::FIRE:
		return gsGetMouseButtonState(GMOUSE_BUTTON_2);
	default:
		return false;
	}
	return false;
}

//インスタンスを取得
Input& Input::get_instance() {
	static Input self;
	return self;
}