#ifndef PLAYER_MOVE_STATE
#define PLAYER_MOVE_STATE

#include "State/IState.h"
#include "../Player.h"
#include "Input.h"
class PlayerMoveState : public IState {
public:
	PlayerMoveState(Player& owner);
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
	//移動入力値
	//float left_stick_angle{};
	// オーナーのステータス
	const Status& owner_status_ = owner_.get_status();
};
#endif
