#ifndef CANNON_MOVE_STATE
#define CANNON_MOVE_STATE

#include "State/IState.h"
#include "../EnemyCannon.h"

// キャノンの移動ステートを管理するクラス
class EnemyCannon : public IState {
public:
	EnemyCannon(EnemyCannon& owner);
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
	EnemyCannon& owner_;
	// オーナーのステータス
	const Character::Status& owner_status_ = owner_.get_status();
};
#endif