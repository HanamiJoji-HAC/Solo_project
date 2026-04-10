#include "HeilAttackState.h"
#include "GameConfig.h"
HeilAttackState::HeilAttackState(EnemyHeilcoptor& owner) : owner_(owner) {}

void HeilAttackState::on_enter()
{
	timer_ = 0.0f;
	is_fired_ = false;
}

void HeilAttackState::on_update(float delta_time)
{
	Actor* player = owner_.get_player();
	GSvector3 target_pos = player->transform().position();
	// プレイヤーに回転する
	owner_.turn_to(player->transform().position(), 60.0f, delta_time);
	// 射撃後、索敵状態に遷移する
	if (owner_.is_complete_turn(target_pos)) {
		// 射撃を実行
		if (!is_fired_) {
			owner_.fire(GunInfo::MachineGun);
			is_fired_ = true;
		}
		// タイマーの実行
		timer_ += delta_time / cREF;
		if (timer_ >= 3.0f) {
			owner_.change_state(EnemyState::Search);
		}
	}
}

void HeilAttackState::on_late_update(float delta_time)
{
}

void HeilAttackState::on_exit()
{
}
