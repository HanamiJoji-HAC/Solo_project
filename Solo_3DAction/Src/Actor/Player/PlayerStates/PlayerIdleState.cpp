#include "PlayerIdleState.h"

PlayerIdleState::PlayerIdleState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerIdleState::on_enter(){

}
//ステートが実行中に毎フレーム呼ばれる
void PlayerIdleState::on_update(float delta_time) {
#ifndef DEBUG
    if (gsGetKeyTrigger(GKEY_I)) {
        owner_.change_motion(PlayerMotion::MotionIdle, false);
        owner_.change_state(PlayerState::Move);
    }
    //デバッグ用:アニメーション番号確認
    //owner_.change_motion(owner_.motion_num_, true);
#endif // !DEBUG
}
//
void PlayerIdleState::on_late_update(float delta_time){
}
//ステート終了時に呼ばれる
void PlayerIdleState::on_exit(){
}