#include "PlayerQuickBoostState.h"
#include "../Player.h"

PlayerQuickBoostState::PlayerQuickBoostState(Player& owner) : owner_(owner) {
}
// ステートが開始された時に起こる
void PlayerQuickBoostState::on_enter() {
	owner_.set_quick_boost(owner_status_.quick_boost_speed_);
}
// ステートが実行中に毎フレーム呼ばれる
void PlayerQuickBoostState::on_update(float delta_time) {
	owner_.quick_boost(delta_time);

}
// 
void PlayerQuickBoostState::on_late_update(float delta_time) {
}
// ステート終了時に呼ばれる
void PlayerQuickBoostState::on_exit() {
}