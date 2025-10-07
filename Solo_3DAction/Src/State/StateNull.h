#ifndef STATE_NULL_H_
#define STATE_NULL_H_

#include "IState.h"

// ヌルシーン
class StateNull : public IState {
public:
	//ステートが開始された時に起こる
	virtual void on_enter() override {}	
	//ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time) override {}
	//
	virtual void on_late_update(float delta_time) override {}
	//ステート終了時に呼ばれる
	virtual void on_exit() override {}
};
#endif

