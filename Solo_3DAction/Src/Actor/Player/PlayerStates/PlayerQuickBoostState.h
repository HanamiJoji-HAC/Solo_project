#ifndef PLAYER_QUICK_BOOST_STATE_H_
#define PLAYER_QUICK_BOOST_STATE_H_

#include "State/IState.h"
#include "../Player.h"
#include "Input.h"
class PlayerQuickBoostState : public IState {
public:
	PlayerQuickBoostState(Player& owner);
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
	float left_stick_angle{};
	// オーナーのステータス
	const Status& owner_status_ = owner_.get_status();
	// ブースト値
	GSvector3 boost_value_{};
};
#endif