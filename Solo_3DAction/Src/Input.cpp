#include "Input.h"

void Input::update(float delta_time) {
	is_pad_ = gsXBoxGetPadCount() > 0;
	// コントローラー入力か？
	if (is_pad_) {
		// 左アナログスティックの入力情報を更新
		gsXBoxPadGetLeftAxis(0, &left_axis_);
		// 右アナログスティックの入力情報を更新
		gsXBoxPadGetRightAxis(0, &right_axis_);
	}
	else {
		// キーマウの入力情報を更新
		update_WASD_axis();
		update_mouce_axis();
	}
	set_left_stick_angle();
	set_right_stick_angle();
}

// 左スティックの角度を返却する
void Input::set_left_stick_angle() {
	GSvector2 left_stick_input = get_left_stick_axis();
	float stick_angle = atan2(left_stick_input.x, left_stick_input.y);
	//斜めに各方向の最大値を取る
	left_dir_ = (stick_angle / GS_PI) * 180 + 45;
}

void Input::set_right_stick_angle() {
	GSvector2 right_stick_input = get_right_stick_axis();
	float stick_angle = atan2(right_stick_input.x, right_stick_input.y);
	//斜めに各方向の最大値を取る
	right_dir_ = (stick_angle / GS_PI) * 180 + 45;
}

//ボタンの入力を参照する
bool Input::get_action_input(InputAction action) {
	switch (action)
	{
	case InputAction::JUMP:
		return is_pad_ ? gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A) : gsGetKeyTrigger(GKEY_SPACE);
	case InputAction::SETBOOST:
		return is_pad_ ? gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A) : gsGetKeyTrigger(GKEY_SPACE);
	case InputAction::BOOST:
		return is_pad_ ? gsXBoxPadButtonState(0, GS_XBOX_PAD_A) : gsGetKeyState(GKEY_SPACE);
	case InputAction::ATTACK:
		return is_pad_ ? gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_LEFT_SHOULDER) : gsGetMouseButtonTrigger(GMOUSE_BUTTON_1);
	case InputAction::FIRE:
		return is_pad_ ? gsXBoxPadButtonState(0, GS_XBOX_PAD_RIGHT_SHOULDER) : gsGetMouseButtonState(GMOUSE_BUTTON_2);
	case InputAction::QUICKBOOST:
		return is_pad_ ? gsXBoxPadButtonState(0, GS_XBOX_PAD_X) : gsGetKeyTrigger(GKEY_LSHIFT);
	case InputAction::LSTICK_UP:
		return left_dir_ <= 90 && left_dir_ >= 0;
	case InputAction::LSTICK_RIGHT:
		return left_dir_ <= 180 && left_dir_ > 90;
	case InputAction::LSTICK_DOWN:
		return left_dir_ < -90 || left_dir_ > 180;
	case InputAction::LSTICK_LEFT:
		return left_dir_ < 0 && left_dir_ >= -90;
	case InputAction::RSTICK_UP:
		return right_dir_ <= 90 && right_dir_ >= 0;
	case InputAction::RSTICK_RIGHT:
		return right_dir_ <= 180 && right_dir_ > 90;
	case InputAction::RSTICK_DOWN:
		return right_dir_ < -90 || right_dir_ > 180;
	case InputAction::RSTICK_LEFT:
		return right_dir_ < 0 && right_dir_ >= -90;
	case InputAction::IDLE:
		return left_axis_ == GSvector2{ 0, 0 };
	case InputAction::LOCK_ON:
		return is_pad_ ? gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT_THUMB) : gsGetMouseButtonState(GMOUSE_BUTTON_3);
	case InputAction::CAMERA_RESET:
		return is_pad_ ? gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT_THUMB) : gsGetMouseButtonState(GMOUSE_BUTTON_3);
	default:
		return false;
	}
	return false;
}

// 左スティックの状態を取得
const GSvector2& Input::get_left_stick_axis() const {
	return left_axis_;
}

// 右スティックの状態を取得
const GSvector2& Input::get_right_stick_axis() const {
	return right_axis_;
}

// WASDキーによる移動値の更新
void Input::update_WASD_axis() {
	left_axis_ = GSvector2{ 0.0, 0.0 };
	if (gsGetKeyState(GKEY_W)) {
		left_axis_.y += 1;
	}
	if (gsGetKeyState(GKEY_A)) {
		left_axis_.x += -1;
	}
	if (gsGetKeyState(GKEY_S)) {
		left_axis_.y += -1;
	}
	if (gsGetKeyState(GKEY_D)) {
		left_axis_.x += 1;
	}
}

// マウスの移動量の更新
void Input::update_mouce_axis() {
	int x, y;
	gsGetMouseVelocity(&x, &y, 0);
	right_axis_.x = x;
	// y移動量だけ逆なので反転
	right_axis_.y = -y;
}

float Input::get_left_dir() const {
	return left_dir_;
}

float Input::get_right_dir() const {
	return right_dir_;
}

bool Input::is_pad() const {
	return is_pad_;
}

//インスタンスを取得
Input& Input::get_instance() {
	static Input self;
	return self;
}