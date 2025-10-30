#include "PlayerStunState.h"

PlayerStunState::PlayerStunState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerStunState::on_enter() {

}
//ステートが実行中に毎フレーム呼ばれる
void PlayerStunState::on_update(float delta_time) {
}
//
void PlayerStunState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerStunState::on_exit() {
}