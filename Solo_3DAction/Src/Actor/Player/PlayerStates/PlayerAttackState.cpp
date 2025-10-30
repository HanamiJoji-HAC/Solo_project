#include "PlayerAttackState.h"
#include "../Player.h"

PlayerAttackState::PlayerAttackState(Player& owner) : owner_( owner ) {

}
//ステートが開始された時に起こる
void PlayerAttackState::on_enter() {
	owner_.attack();
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerAttackState::on_update(float delta_time) {
	if (owner_.is_motion_end()) {
		owner_.change_state(PlayerState::Move);
		return;
	}
}
//
void PlayerAttackState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerAttackState::on_exit() {
}