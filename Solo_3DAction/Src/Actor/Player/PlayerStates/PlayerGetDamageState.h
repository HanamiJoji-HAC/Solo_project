#ifndef PLAYER_GET_DAMAGE_STATE
#define PLAYER_GET_DAMAGE_STATE

#include "State/IState.h"
#include "../Player.h"

class PlayerGetDamageState : public IState {
public:
	PlayerGetDamageState(Player& owner);
	//ステートが開始された時に起こる
	virtual void on_enter() override;
	//ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time);
	//
	virtual void on_late_update(float delta_time) override;
	//ステート終了時に呼ばれる
	virtual void on_exit() override;
private:
	Player& owner_;
	const Status& owner_status_ = owner_.get_status();
};
#endif