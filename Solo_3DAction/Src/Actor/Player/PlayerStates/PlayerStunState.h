#ifndef PLAYER_STUN_STATE
#define PLAYER_STUN_STATE

#include "State/IState.h"
#include "../Player.h"

class PlayerStunState : public IState {
public:
	PlayerStunState(Player& owner);
	//ステートが開始された時に起こる
	virtual void on_enter();
	//ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time);
	//
	virtual void on_late_update(float delta_time);
	//ステート終了時に呼ばれる
	virtual void on_exit();
private:
	Player& owner_;
};
#endif