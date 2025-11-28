#include "PlayerFireState.h"
#include "../Player.h"
#include "Rendering/Layer.h"

PlayerFireState::PlayerFireState(Player& owner) : owner_(owner) {

}
//ステートが開始された時に起こる
void PlayerFireState::on_enter() {
	owner_.fire();
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerFireState::on_update(float delta_time) {
	owner_.move(delta_time, owner_status_.walk_speed_);
	owner_.update_gravity(delta_time, owner_status_.gravity_);
	if (input_.get_action_input(InputAction::BOOST)) {
		owner_.boost(delta_time, owner_status_.boost_speed_);
	}
	// 射撃モーションが終了しているか？
	if (owner_.is_motion_end((GSuint)Layer::Upper_Body)) {
		//auto previous_state = owner_.get_previous_state();
		//owner_.change_state(static_cast<Player::State>(previous_state));
		// TODO:前回のモーションを取得する
		//auto prev_motion = owner_.get_prev_motion();
		//owner_.change_motion(prev_motion, false);
		// TODO:射撃モーションが終わるまで遷移しないので、着地ギリギリに射撃すると挙動が悪い。
		if (owner_.check_ground()) {
			owner_.change_state(PlayerState::Move);
			owner_.change_motion(PlayerMotion::MotionIdle, true);
			return;
		}
		else {
			owner_.change_state(PlayerState::Jump);
			owner_.change_motion(PlayerMotion::MotionAirIdle, true);
			return;
		}
	}

}
//
void PlayerFireState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerFireState::on_exit() {
}