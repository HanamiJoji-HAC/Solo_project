#include "CameraLockOnState.h"

CameraLockOnState::CameraLockOnState(CameraTPS& owner) : owner_{ owner } {};

//ステートが開始された時に起こる
void CameraLockOnState::on_enter() {
	sencitivity_ = owner_.get_senc();
}
//ステートが実行中に毎フレーム呼ばれる
void CameraLockOnState::on_update(float delta_time) {
	// カメラスティックの入力を参照する
	GSvector2 stick_input = input_.get_right_stick_axis();
	// スティックのカメラ移動
	owner_.stick_controll_camera(stick_input, delta_time, sencitivity_);
	Actor* player = owner_.get_player();
	if (input_.get_action_input(InputAction::CAMERA_RESET)) {
		Actor* player = owner_.get_player();
		owner_.change_state(CameraState::Normal);
	}
	// ロックオンする敵を抽選する
	owner_.lock_on_actor("EnemyTag");
	// 通常カメラ処理
	owner_.update_lockon(delta_time);

}
//
void CameraLockOnState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void CameraLockOnState::on_exit() {

}