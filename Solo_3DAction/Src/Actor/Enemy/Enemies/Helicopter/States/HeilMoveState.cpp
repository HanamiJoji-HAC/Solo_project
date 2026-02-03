#include "HeilMoveState.h"

HeilMoveState::HeilMoveState(EnemyHeilcoptor& owner) : owner_(owner){}

void HeilMoveState::on_enter()
{
}

void HeilMoveState::on_update(float delta_time)
{
	float turn_angle = owner_.turn_angle();
	if (std::abs(turn_angle) >= 0.1f) {
		owner_.turn(5.0f, delta_time);
		return;
	}
	owner_.move(delta_time, owner_status_.move_speed_);
}

void HeilMoveState::on_late_update(float delta_time)
{
}

void HeilMoveState::on_exit()
{
}
