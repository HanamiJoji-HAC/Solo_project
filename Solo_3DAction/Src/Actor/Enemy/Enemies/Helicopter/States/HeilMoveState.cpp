#include "HeilMoveState.h"

#include "imgui/imgui.h"

HeilMoveState::HeilMoveState(EnemyHeilcoptor& owner) : owner_(owner){}

void HeilMoveState::on_enter()
{
}

void HeilMoveState::on_update(float delta_time)
{
	// õ“G¬Œ÷‚ÅUŒ‚ó‘Ô‚É‘JˆÚ
	if(owner_.is_search(30.0f)) {
		owner_.change_state(EnemyState::Attack);
		return;
	};

	GSvector3 waypoint_pos = owner_.get_waypoint_pos();
#ifdef _DEBUG
	ImGui::Begin("Heil");
	ImGui::DragFloat3("WP_pos", waypoint_pos);
	ImGui::End();
#endif
	// „‰ñ’†‚Ì•ûŒü“]Š·
	if (!owner_.is_complete_turn(waypoint_pos)) {
		owner_.turn_to(waypoint_pos, delta_time);
		return;
	}

	// ˆÚ“®
	owner_.move(delta_time, owner_status_.move_speed_);
}

void HeilMoveState::on_late_update(float delta_time)
{
}

void HeilMoveState::on_exit()
{
}
