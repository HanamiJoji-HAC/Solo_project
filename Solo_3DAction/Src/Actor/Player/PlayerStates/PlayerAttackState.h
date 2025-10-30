#ifndef PLAYER_ATTACK_STATE
#define PLAYER_ATTACK_STATE

#include "State/IState.h"
#include "../Player.h"

class PlayerAttackState : public IState {
public:
	PlayerAttackState(Player& owner);
	//ステートが開始された時に起こる
	void on_enter() override;
	//ステートが実行中に毎フレーム呼ばれる
	void on_update(float delta_time) override;
	//
	void on_late_update(float delta_time) override;
	//ステート終了時に呼ばれる
	void on_exit() override;
private:
	Player& owner_;
	Status& status_ = owner_.get_status();
};

#endif