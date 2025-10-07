#include "PlayerMoveState.h"

PlayerMoveState::PlayerMoveState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerMoveState::on_enter() {

}
//ステートが実行中に毎フレーム呼ばれる
void PlayerMoveState::on_update(float delta_time) {
    owner_.updateGravity(delta_time, owner_.grav);
	owner_.move(delta_time);
    //何もしていなければ待機
    owner_.change_motion(PlayerMotion::MotionIdle, true);
    //デバッグ用:アニメーション番号確認
    //owner_.change_motion(owner_.motion_num, true);
    //ジャンプ
    if (input_.get_action_input(InputAction::JUMP) && owner_.check_ground()) {
        owner_.change_motion(PlayerMotion::MotionJump, false);
        owner_.change_state(PlayerState::Jump);
        return;
    }
    //攻撃
    if (input_.get_action_input(InputAction::ATTACK)) {
        owner_.change_motion(PlayerMotion::MotionAttack, false);
        owner_.change_state(PlayerState::Attack);
        return;
    }
    //死亡
    if (gsGetKeyTrigger(GKEY_1)) {
        owner_.change_motion(PlayerMotion::MotionDead, false);
        owner_.change_state(PlayerState::Dead);
    }

}
//
void PlayerMoveState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerMoveState::on_exit() {
}