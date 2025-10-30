#include "PlayerIdleState.h"

PlayerIdleState::PlayerIdleState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerIdleState::on_enter(){

}
//ステートが実行中に毎フレーム呼ばれる
void PlayerIdleState::on_update(float delta_time) {
#ifndef DEBUG
    if (gsGetKeyTrigger(GKEY_UP)) {
        ++motion_num_;
        owner_.change_motion(motion_num_, true);
    }
    if (gsGetKeyTrigger(GKEY_DOWN)) {
        --motion_num_;
        owner_.change_motion(motion_num_, true);
    };
//if (gsGetKeyTrigger(GKEY_LEFT)) {
//    --down_motion_num_;
//    mesh_.change_motion(1, down_motion_num_);
//};
//if (gsGetKeyTrigger(GKEY_RIGHT)) {
//    ++down_motion_num_;
//    mesh_.change_motion(1, down_motion_num_);
//};
    if (gsGetKeyTrigger(GKEY_I)) {
        owner_.change_motion(PlayerMotion::MotionIdle, false);
        owner_.change_state(PlayerState::Move);
    }
#endif // !DEBUG
}
//
void PlayerIdleState::on_late_update(float delta_time){
}
//ステート終了時に呼ばれる
void PlayerIdleState::on_exit(){
}