#include "StateMachine.h"
#include "StateNull.h"

using namespace std;

static shared_ptr<StateNull> state_null_ = make_shared<StateNull>(); //ヌルステート
//コンストラクタ
StateMachine::StateMachine() :
	current_state_{ state_null_ } {
}
//デストラクタ
StateMachine::~StateMachine() {
	clear();
};
//更新
void StateMachine::update(float delta_time) {
	current_state_->on_update(delta_time);
};
//遅れて更新
void StateMachine::late_update(float delta_time) {};
//消去
void StateMachine::clear() {
	state_list_.clear();
};
//現在のステートを取得する
shared_ptr<IState> StateMachine::get_state() const {
	return current_state_;
};
//ステートを検索する
shared_ptr<IState> StateMachine::find_state(int state_num) {
	auto it = state_list_.find(state_num);
	if (it != state_list_.end()) {
		return it->second;
	}
	return nullptr;
};
//ステートを追加する
void StateMachine::add_state(int state_num, shared_ptr<IState> state_class) {
	state_list_[state_num] = state_class;
};
//ステートを変更する
void StateMachine::change_state(int next_state_num) {
	if (current_state_num_ == next_state_num) return;

	std::shared_ptr<IState> state = find_state(next_state_num);
	if (state == nullptr) return;
	current_state_->on_exit();
	current_state_ = state;
	current_state_->on_enter();
	current_state_num_ = next_state_num;
};
//ステートは現在の状態か？
bool StateMachine::is_current_state(int state_num) const {
	return current_state_num_ == state_num;
};

