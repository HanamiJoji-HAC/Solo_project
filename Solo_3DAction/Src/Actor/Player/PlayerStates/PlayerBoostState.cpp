#include "PlayerBoostState.h"

PlayerBoostState::PlayerBoostState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerBoostState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerBoostState::on_update(float delta_time) 
{
	// 操作がなければ移動ステートに戻る
	if (input_.get_action_input(InputAction::IDLE) && owner_.get_boost_velocity().length() < 0.01f) {
		owner_.change_motion(PlayerMotion::MotionIdle, true);
		owner_.change_state(PlayerState::Move);
		return;
	} else {
		// ブースト
		owner_.boost(delta_time, owner_status_.boost_speed_);
	}

	// 射撃
	if (input_.get_action_input(InputAction::FIRE)) {
		owner_.change_motion(0, PlayerMotion::MotionFire, false);
		owner_.fire(GunInfo::MachineGun);
	}
	// 地上ならジャンプ可能
	if (input_.get_action_input(InputAction::JUMP) && owner_.check_ground()) {
		owner_.change_motion(PlayerMotion::MotionJump, false);
		owner_.set_jump(owner_status_.jump_power_);
		owner_.change_state(PlayerState::Jump);
		return;
	}
}
//
void PlayerBoostState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerBoostState::on_exit() {
}