#ifndef PLAYER_BOOST_STATE_
#define PLAYER_BOOST_STATE_

#include "State/IState.h"
#include "../Player.h"

class PlayerBoostState : public IState {
public:
	PlayerBoostState(Player& owner);
	//ステートが開始された時に起こる
	virtual void on_enter() override;
	//ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time) override;
	//
	virtual void on_late_update(float delta_time) override;
	//ステート終了時に呼ばれる
	virtual void on_exit() override;
private:
	Player& owner_;
	Status& status_ = owner_.get_status();
};
#endif