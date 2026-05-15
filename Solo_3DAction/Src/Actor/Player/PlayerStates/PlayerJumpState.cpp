#include "PlayerJumpState.h"

PlayerJumpState::PlayerJumpState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerJumpState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerJumpState::on_update(float delta_time) {
	owner_.update_gravity(delta_time, owner_status_.gravity_);
	// 移動
	if (owner_.get_previous_state() == (int)PlayerState::Move) {
		owner_.move(delta_time, owner_status_.air_move_speed_);
	}
	else if (owner_.get_previous_state() == (int)PlayerState::Boost) {
		owner_.boost(delta_time, owner_status_.boost_speed_);
	}
	// ブーストアクションがあれば遷移する
	if (input_.get_action_input(InputAction::QUICKBOOST)) {
 		owner_.change_state(PlayerState::QuickBoost);
		owner_.change_motion(PlayerMotion::MotionQuickBoostDown, false);
		return;
	}

	// 射撃
	if (input_.get_action_input(InputAction::FIRE)) {
		owner_.change_motion(0, PlayerMotion::MotionFire, false);
		owner_.change_state(PlayerState::Fire);
		return;
	}

	// 地上ならデフォルトステートへ遷移
	if (owner_.check_ground() && owner_.get_previous_state() == (int)PlayerState::Boost) {
		owner_.change_motion(PlayerMotion::MotionBoost, true);
		owner_.change_state(PlayerState::Boost);
		return;
	} else if(owner_.check_ground() && owner_.get_previous_state() == (int)PlayerState::Move) {
		owner_.change_state(PlayerState::Move);
	}
}

void PlayerJumpState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerJumpState::on_exit() {
}