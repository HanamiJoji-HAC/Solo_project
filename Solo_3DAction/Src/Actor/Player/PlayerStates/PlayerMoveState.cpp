#include "PlayerMoveState.h"
#include "imgui/imgui.h"
#include <iostream>
PlayerMoveState::PlayerMoveState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerMoveState::on_enter() {
    grav_ = owner_.get_gravity();
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerMoveState::on_update(float delta_time) {
    owner_.update_gravity(delta_time, grav_);
	owner_.move(delta_time, status_.walk_speed_);
#ifndef DEBUG
    ImGui::Begin("MoveState_LeftStickInput");
    ImGui::DragFloat("angle", &left_stick_angle);
    ImGui::End();
    //デバッグ用:アニメーション番号確認
    //owner_.change_motion(owner_.motion_num_, true);
#endif
    //何もしていなければ待機
    if (input_.get_action_input(InputAction::UP)) {
        owner_.change_motion(PlayerMotion::MotionWalkForward, true);
    } else if (input_.get_action_input(InputAction::DOWN)) {
        owner_.change_motion(PlayerMotion::MotionWalkBack, true);
    } else if (input_.get_action_input(InputAction::LEFT)) {
        owner_.change_motion(PlayerMotion::MotionWalkLeft, true);
    } else if (input_.get_action_input(InputAction::RIGHT)) {
        owner_.change_motion(PlayerMotion::MotionWalkRight, true);
    } else {
        owner_.change_motion(PlayerMotion::MotionIdle, true);
    }

    //ジャンプ
    if (input_.get_action_input(InputAction::JUMP) && owner_.check_ground()) {
        owner_.change_motion(PlayerMotion::MotionBoost, false);
        owner_.set_jump(status_.jump_power_);
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
    //if (gsGetKeyTrigger(GKEY_1)) {
    //    owner_.change_motion(PlayerMotion::MotionDead, false);
    //    owner_.change_state(PlayerState::Dead);
    //}

    if (gsGetKeyTrigger(GKEY_I)) {
        owner_.change_motion(PlayerMotion::MotionIdle, false);
        owner_.change_state(PlayerState::Idle);
    }

}
//
void PlayerMoveState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerMoveState::on_exit() {
}