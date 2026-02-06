#include "HeilSearchState.h"

HeilSearchState::HeilSearchState(EnemyHeilcoptor& owner) : owner_(owner) {}

void HeilSearchState::on_enter()
{
}

void HeilSearchState::on_update(float delta_time)
{
	// õ“G¬Œ÷‚È‚çUŒ‚A¸”s‚È‚ç„‰ñ‚É–ß‚é
	if (owner_.is_search()) {
		owner_.change_state(EnemyState::Attack);
	} else {
		owner_.change_state(EnemyState::Move);
	}
}

void HeilSearchState::on_late_update(float delta_time)
{
}

void HeilSearchState::on_exit()
{
}
