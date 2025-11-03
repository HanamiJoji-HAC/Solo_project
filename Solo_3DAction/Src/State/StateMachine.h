#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <unordered_map>
#include <memory>

//前方宣言
class IState;

class StateMachine {
public:
	//コンストラクタ
	StateMachine();
	//デストラクタ
	virtual ~StateMachine();
	//現在のステートを参照する
	std::shared_ptr<IState> get_state() const;
	//ステートを追加する
	void add_state(int state, std::shared_ptr<IState> state_class);
	//ステートを変更する
	void change_state(int next_stat);
	//ステートは現在の状態か？
	bool is_current_state(int state_num) const;
	//ステートを検索
	std::shared_ptr<IState> find_state(int state_num);
	//更新
	void update(float delta_time);
	//遅れて更新
	void late_update(float delta_time);
	//初期化
	void clear();
	//現在のステート番号を参照する
	int get_current_state();
	// 前回のステートを参照する
	int get_previous_state();
protected:
	std::unordered_map<int, std::shared_ptr<IState>> state_list_;
	int current_state_num_{ 0 };
	int previous_state_num_{ 0 };
private:
	std::shared_ptr<IState> current_state_;
	std::shared_ptr<IState> previous_state_;
public:
	//コピー禁止
	StateMachine(const StateMachine& other) = delete;
	StateMachine& operator = (const StateMachine& other) = delete;
};
#endif
