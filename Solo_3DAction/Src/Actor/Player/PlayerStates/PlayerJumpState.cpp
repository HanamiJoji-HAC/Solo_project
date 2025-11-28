#include "PlayerJumpState.h"

PlayerJumpState::PlayerJumpState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerJumpState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerJumpState::on_update(float delta_time) {
	owner_.update_gravity(delta_time, owner_status_.gravity_);
	// 移動
	owner_.move(delta_time, owner_status_.air_move_speed_);
	// ブーストアクションがあれば遷移する
	if (input_.get_action_input(InputAction::SETBOOST)) {
 		owner_.change_state(PlayerState::Boost);
		owner_.change_motion(PlayerMotion::MotionBoost, true);
		return;
	}

	// 射撃
	if (input_.get_action_input(InputAction::FIRE)) {
		owner_.change_motion(0, PlayerMotion::MotionFire, false);
		//owner_.change_state(PlayerState::Fire);
		owner_.change_state(PlayerState::Fire);
		return;
	}

	// 地上ならデフォルトステートへ遷移
	if (owner_.check_ground()) {
		owner_.change_state(PlayerState::Move);
		return;
	}
}

void PlayerJumpState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerJumpState::on_exit() {
}