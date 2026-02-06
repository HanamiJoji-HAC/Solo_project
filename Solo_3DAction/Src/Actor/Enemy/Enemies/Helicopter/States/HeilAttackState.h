#ifndef HEIL_ATTACK_STATE
#define HEIL_ATTACK_STATE

#include "State/IState.h"
#include "../EnemyHeilcoptor.h"

// ヘリクラスの移動ステートを管理するクラス
class HeilAttackState : public IState {
public:
	HeilAttackState(EnemyHeilcoptor& owner);
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
	EnemyHeilcoptor& owner_;
	// オーナーのステータス
	const Status& owner_status_ = owner_.get_status();
	float timer_{ 0.0f };
	bool is_fired_{ false };
};
#endif