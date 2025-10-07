#include "PlayerJumpState.h"

PlayerJumpState::PlayerJumpState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerJumpState::on_enter() {

}
//ステートが実行中に毎フレーム呼ばれる
void PlayerJumpState::on_update(float delta_time) {
	owner_.updateGravity(delta_time, owner_.grav);
	owner_.jump(0.25f, delta_time);
	owner_.attack(delta_time);
	owner_.move(delta_time);
}
//
void PlayerJumpState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerJumpState::on_exit() {
}