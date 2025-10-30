#include "PlayerJumpState.h"

PlayerJumpState::PlayerJumpState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerJumpState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerJumpState::on_update(float delta_time) {
	owner_.update_gravity(delta_time, status_.gravity_);
	owner_.update_state_timer(delta_time);
	//Todo:PlayerにairMove持たせた方が拡張性高い
	owner_.jump();
	owner_.move(delta_time, status_.move_speed_);
	//ジャンプ中に
	if (owner_.input_.get_action_input(InputAction::SETBOOST)) {
 		owner_.change_state(PlayerState::Boost);
	}
	if (owner_.check_ground()) {
		owner_.change_state(PlayerState::Move);
	}
}
//
void PlayerJumpState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerJumpState::on_exit() {
}