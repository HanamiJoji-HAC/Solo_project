#ifndef PLAYER_JUMP_STATE
#define PLAYER_JUMP_STATE

#include "State/IState.h"
#include "../Player.h"

class PlayerJumpState : public IState {
public:
	// コンストラクタ
	PlayerJumpState(Player& owner);
	// ステートが開始された時に起こる
	virtual void on_enter() override;
	// ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time) override;
	//
	virtual void on_late_update(float delta_time) override;
	// ステート終了時に呼ばれる
	virtual void on_exit() override;
private:
	// プレイヤーを取得
	Player& owner_;
	// オーナーのステータス
	const Status& owner_status_ = owner_.get_status();
	// 入力クラス
	Input& input_ = Input::get_instance();
};
#endif