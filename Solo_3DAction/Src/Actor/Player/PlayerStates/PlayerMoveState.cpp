#include "PlayerMoveState.h"
#include "imgui/imgui.h"
#include <iostream>
PlayerMoveState::PlayerMoveState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerMoveState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerMoveState::on_update(float delta_time) {
    owner_.update_gravity(delta_time, owner_status_.gravity_);
	owner_.move(delta_time, owner_status_.walk_speed_);
#ifndef DEBUG
    ImGui::Begin("MoveState_LeftStickInput");
    left_stick_angle = input_.get_left_stick_input_angle();
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

    // ジャンプ
    if (input_.get_action_input(InputAction::JUMP) && owner_.check_ground()) {
        owner_.change_motion(PlayerMotion::MotionBoost, false);
        owner_.set_jump(owner_status_.jump_power_);
        owner_.change_state(PlayerState::Jump);
        return;
    }
    // 攻撃
    if (input_.get_action_input(InputAction::ATTACK)) {
        owner_.change_motion(PlayerMotion::MotionAttack, false);
        owner_.change_state(PlayerState::Attack);
        return;
    }
    // 射撃
    if (input_.get_action_input(InputAction::FIRE)) {
        owner_.change_motion(0, PlayerMotion::MotionFire, false);
        owner_.change_state(PlayerState::Fire);
        return;
    }
    // 使わないIdleステートにデバッグ挙動を追加する
#ifndef DEBUG
    if (gsGetKeyTrigger(GKEY_I)) {
        owner_.change_motion(PlayerMotion::MotionIdle, false);
        owner_.change_state(PlayerState::Idle);
    }
#endif // DEBUG
}
//
void PlayerMoveState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerMoveState::on_exit() {
}