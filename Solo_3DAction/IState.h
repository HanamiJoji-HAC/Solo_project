#ifndef ISTATE_H_
#define ISTATE_H_

//ステート用の基底抽象クラス
class IState {
public:
	IState() = default;
	//仮想デストラクタ
	virtual ~IState() = default;
	//ステートが開始された時に起こる
	virtual void on_enter() = 0;
	//ステートが実行中に毎フレーム呼ばれる
	virtual void on_update(float delta_time) = 0;
	//
	virtual void on_late_update(float delta_time) = 0;
	//ステート終了時に呼ばれる
	virtual void on_exit() = 0;
public:
	//コピー禁止
	IState(const IState& other) = delete;
	IState& operator = (const IState& other) = delete;
};

#endif
