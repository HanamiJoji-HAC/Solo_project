#include "PlayerDeadState.h"
#include "../Player.h"

PlayerDeadState::PlayerDeadState(Player& owner) : owner_(owner) {

}
//ステートが開始された時に起こる
void PlayerDeadState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerDeadState::on_update(float delta_time) {
	if (!owner_.is_dead() && owner_.is_motion_end()) {
		owner_.die();
	}
}
//
void PlayerDeadState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerDeadState::on_exit() {
}