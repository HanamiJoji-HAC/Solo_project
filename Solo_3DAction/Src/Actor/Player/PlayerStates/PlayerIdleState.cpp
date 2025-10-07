#include "PlayerIdleState.h"

PlayerIdleState::PlayerIdleState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerIdleState::on_enter(){

}
//ステートが実行中に毎フレーム呼ばれる
void PlayerIdleState::on_update(float delta_time) {
}
//
void PlayerIdleState::on_late_update(float delta_time){
}
//ステート終了時に呼ばれる
void PlayerIdleState::on_exit(){
}