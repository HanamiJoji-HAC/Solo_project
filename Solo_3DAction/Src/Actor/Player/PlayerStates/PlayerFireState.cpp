#include "PlayerFireState.h"
#include "../Player.h"

PlayerFireState::PlayerFireState(Player& owner) : owner_(owner) {

}
//ステートが開始された時に起こる
void PlayerFireState::on_enter() {
	owner_.fire();
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerFireState::on_update(float delta_time) {
	owner_.move(delta_time, owner_status_.walk_speed_);
	// Todo:前回のステートに戻るを追加する
	if (owner_.is_motion_end()) {
		owner_.change_state(PlayerState::Move);
		return;
	}
}
//
void PlayerFireState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerFireState::on_exit() {
}