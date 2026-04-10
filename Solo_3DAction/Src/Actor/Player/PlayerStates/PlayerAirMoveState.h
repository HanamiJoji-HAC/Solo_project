#ifndef PLAYER_AIR_MOVE_STATE
#define PLAYER_AIR_MOVE_STATE

#include "State/IState.h"
#include "../Player.h"
#include "Input.h"
class PlayerAirMoveState : public IState {
public:
	PlayerAirMoveState(Player& owner);
	// ステートが開始された時に起こる
	virtual void on_enter() override;
	// ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time) override;
	// ステートの遅延実行
	virtual void on_late_update(float delta_time) override;
	// ステート終了時に呼ばれる
	virtual void on_exit() override;
private:
	// オーナー
	Player& owner_;
	// 入力クラス
	Input& input_ = Input::get_instance();
	// オーナーのステータス
	const Character::Status& owner_status_ = owner_.get_status();
};
#endif