#include "PlayerBoostState.h"

PlayerBoostState::PlayerBoostState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerBoostState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerBoostState::on_update(float delta_time) 
{
	owner_.update_gravity(delta_time, owner_status_.gravity_);
	// ブースト
	if (input_.get_action_input(InputAction::BOOST)) {
		owner_.boost(delta_time, owner_status_.boost_speed_);
		owner_.move(delta_time, owner_status_.air_move_speed_);
	}
	// 地上にいるか？
	else if(owner_.check_ground()) {
		owner_.change_state(PlayerState::Move);
	}
	// 空中にいるか？
	else {
		owner_.change_state(PlayerState::Jump);
		return;
	}

	// 射撃
	if (input_.get_action_input(InputAction::FIRE)) {
		owner_.change_motion(0, PlayerMotion::MotionFire, false);
		owner_.change_state(PlayerState::Fire);
		//owner_.fire();
		return;
	}
	// 地上ならデフォルトステートへ遷移
	if (owner_.check_ground()) {
		owner_.change_state(PlayerState::Move);
		return;
	}
}
//
void PlayerBoostState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerBoostState::on_exit() {
}