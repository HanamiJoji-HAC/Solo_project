#include "PlayerBoostState.h"

PlayerBoostState::PlayerBoostState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerBoostState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerBoostState::on_update(float delta_time) 
{
	owner_.move(delta_time, status_.move_speed_);
	if (owner_.input_.get_action_input(InputAction::BOOST)) {
		owner_.boost(delta_time, status_.boost_speed_);
	}
	owner_.update_gravity(delta_time, status_.gravity_);
	if (owner_.check_ground()) {
		owner_.change_state(PlayerState::Move);
	}
}
//
void PlayerBoostState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerBoostState::on_exit() {
}