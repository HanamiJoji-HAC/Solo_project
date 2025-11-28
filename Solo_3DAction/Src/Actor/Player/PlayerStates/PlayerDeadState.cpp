#include "PlayerDeadState.h"
#include "../Player.h"
#include "imgui/imgui.h"

PlayerDeadState::PlayerDeadState(Player& owner) : owner_(owner) {

}
//ステートが開始された時に起こる
void PlayerDeadState::on_enter() {
}
//ステートが実行中に毎フレーム呼ばれる
void PlayerDeadState::on_update(float delta_time) {
	ImGui::Begin("DeadState");
	bool is_dead = owner_.is_dead();
	ImGui::Text("is_dead: %s", is_dead ? "true" : "false");
	bool is_motion_end = owner_.is_motion_end();
	ImGui::Text("is_end_motion: %s", is_motion_end ? "true" : "false");
	ImGui::End();
	if (!owner_.is_dead() && owner_.is_motion_end()) {
		owner_.die();
	}
}
//
void PlayerDeadState::on_late_update(float delta_time) {
}
//ステート終了時に呼ばれる
void PlayerDeadState::on_exit() {
}