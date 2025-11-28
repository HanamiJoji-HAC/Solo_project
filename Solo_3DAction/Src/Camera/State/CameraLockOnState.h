#ifndef CAMERA_LOCK_ON_STATE_H_
#define CAMERA_LOCK_ON_STATE_H_

#include "State/IState.h"
#include "../CameraTPS.h"

class CameraLockOnState : public IState {
public:
	CameraLockOnState(CameraTPS& owner);
	//ステートが開始された時に起こる
	virtual void on_enter();
	//ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time);
	//
	virtual void on_late_update(float delta_time);
	//ステート終了時に呼ばれる
	virtual void on_exit();
private:
	CameraTPS& owner_;
};
#endif