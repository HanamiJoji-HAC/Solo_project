#include "CameraResetState.h"
CameraResetState::CameraResetState(CameraTPS& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void CameraResetState::on_enter() {

}
//ステートが実行中に毎フレーム呼ばれる
void CameraResetState::on_update(float delta_time) {
	if(!owner_.is_reset()) {
		owner_.change_state(CameraState::Normal);
	}

	owner_.reset_camera(delta_time);
}
//
void CameraResetState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void CameraResetState::on_exit() {

}