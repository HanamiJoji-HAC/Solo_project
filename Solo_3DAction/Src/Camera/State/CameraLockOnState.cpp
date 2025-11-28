#include "CameraLockOnState.h"

CameraLockOnState::CameraLockOnState(CameraTPS& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void CameraLockOnState::on_enter() {
	owner_.lock_on_actor("EnemyTag");
}
//ステートが実行中に毎フレーム呼ばれる
void CameraLockOnState::on_update(float delta_time) {
	// TODO:ロックオン処理
}
//
void CameraLockOnState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void CameraLockOnState::on_exit() {

}