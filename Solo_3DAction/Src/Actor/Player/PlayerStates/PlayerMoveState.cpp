#include "PlayerMoveState.h"
#include "Rendering/Layer.h"
#include "imgui/imgui.h"
#include <iostream>
PlayerMoveState::PlayerMoveState(Player& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void PlayerMoveState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerMoveState::on_update(float delta_time) {
    // 重力の更新
    owner_.update_gravity(delta_time, owner_status_.gravity_);
    // 移動
	owner_.move(delta_time, owner_status_.walk_speed_);

#ifndef DEBUG
    ImGui::Begin("MoveState_LeftStickInput");
    float left_stick_angle = input_.get_left_dir();
    ImGui::DragFloat("angle", &left_stick_angle);
    // TODO
    //GSvector2 input_value = input_.get_left_stick_input_value().normalized();
    //input_value.x = -input_value.x;
    //ImGui::DragFloat2("angle", input_value);
    ImGui::End();
    //デバッグ用:アニメーション番号確認
    //owner_.change_motion(owner_.motion_num_, true);
#endif

    // 移動中のアニメーション遷移。何もしていなければアイドルモーション
    if (input_.get_action_input(InputAction::IDLE)) {
        owner_.change_motion(PlayerMotion::MotionIdle, true);
    } else if (input_.get_action_input(InputAction::LSTICK_DOWN)) {
        owner_.change_motion(PlayerMotion::MotionWalkBack, true);
    } else if (input_.get_action_input(InputAction::LSTICK_LEFT)) {
        owner_.change_motion(PlayerMotion::MotionWalkLeft, true);
    } else if (input_.get_action_input(InputAction::LSTICK_RIGHT)) {
        owner_.change_motion(PlayerMotion::MotionWalkRight, true);
    } else if(input_.get_action_input(InputAction::LSTICK_UP)) {
        owner_.change_motion(PlayerMotion::MotionWalkForward, true);
    }

    // ジャンプ
    if (input_.get_action_input(InputAction::JUMP) && owner_.check_ground()) {
        owner_.change_motion(PlayerMotion::MotionJump, false);
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
        //owner_.is_motion_end((GSuint)Layer::Upper_Body);
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

    // HACK
    GSvector2 left_stick_value = input_.get_left_stick_axis();
    if (input_.get_action_input(InputAction::QUICKBOOST) && left_stick_value != GSvector2{ 0, 0 }) {
        //owner_.change_motion(PlayerMotion::MotionJump, false);
        owner_.change_state(PlayerState::QuickBoost);
        if (input_.get_action_input(InputAction::LSTICK_UP)) {
            owner_.change_motion(PlayerMotion::MotionQuickBoostUp, false);
        }
        else if (input_.get_action_input(InputAction::LSTICK_LEFT)) {
            owner_.change_motion(PlayerMotion::MotionQuickBoostLeft, false);
        }
        else if (input_.get_action_input(InputAction::LSTICK_DOWN)) {
            owner_.change_motion(PlayerMotion::MotionQuickBoostDown, false);
        }
        else if (input_.get_action_input(InputAction::LSTICK_RIGHT)) {
            owner_.change_motion(PlayerMotion::MotionQuickBoostRight, false);
        }
    }
}
//
void PlayerMoveState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerMoveState::on_exit() {
}