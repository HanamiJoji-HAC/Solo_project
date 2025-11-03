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
		owner_.change_motion(0, PlayerMotion::MotionFire, false);
	}
	else {
		// 前回のステートに遷移
		auto previous_state = owner_.get_previous_state();
		owner_.change_state(static_cast<Player::State>(previous_state));
	}
	owner_.move(delta_time, owner_status_.air_move_speed_);
}
//
void PlayerBoostState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerBoostState::on_exit() {
}