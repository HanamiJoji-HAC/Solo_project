#include "PlayerGetDamageState.h"
#include "../Player.h"

PlayerGetDamageState::PlayerGetDamageState(Player& owner) : owner_(owner) {

}
//ステートが開始された時に起こる
void PlayerGetDamageState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerGetDamageState::on_update(float delta_time) {
    if (owner_.is_motion_end()) {
        owner_.change_state(PlayerState::Move);
    }
}
//
void PlayerGetDamageState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerGetDamageState::on_exit() {
}