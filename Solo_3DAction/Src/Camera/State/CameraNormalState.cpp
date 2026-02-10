#include "CameraNormalState.h"
CameraNormalState::CameraNormalState(CameraTPS& owner) : owner_{ owner } {};
//ステートが開始された時に起こる
void CameraNormalState::on_enter() {

}
//ステートが実行中に毎フレーム呼ばれる
void CameraNormalState::on_update(float delta_time) {
	// プレーヤーを検索
	Actor* player = owner_.get_player();
	if (player == nullptr) return;
	// 通常カメラ処理
	owner_.tpp_normal(player, delta_time);
	// ロックオンに移行
	if (input_.get_action_input(InputAction::LOCK_ON)) {
		owner_.change_state(CameraState::LockOn);
	}
}
//
void CameraNormalState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void CameraNormalState::on_exit() {

}