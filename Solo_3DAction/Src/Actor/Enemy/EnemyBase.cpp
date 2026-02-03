#include "EnemyBase.h"

// コンストラクタ
EnemyBase::EnemyBase(const Status status) : Character(status){
}

// 待機
void EnemyBase::Idle() {
}

// 移動
void EnemyBase::move(float delta_time, float move_speed) {
}

// 発射
void EnemyBase::fire() {
}

// プレイヤーを捜索
void EnemyBase::search() {
}

// プレイヤーを追尾
void EnemyBase::chase(Actor* player) {
}

// リロード
void EnemyBase::reload(float cool_time) {
}

// ステートを変更する
void EnemyBase::change_state(State state_num) {
	state_machine_.change_state((int)state_num);
}

// ターゲット方向の角度を求める（符号付き）
float EnemyBase::target_signed_angle() const {
	// ターゲットがいなければ0を返す
	if (player_ == nullptr) return 0.0f;
	// ターゲット方向のベクトルを求める
	GSvector3 to_target = player_->transform().position() - transform_.position();
	// 前向き方向のベクトルを取得
	GSvector3 forward = transform_.forward();
	// ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;
	// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}

// ターゲット方向の角度を求める（符号なし）
float EnemyBase::target_angle() const {
	return std::abs(target_signed_angle());
}
	
// ターゲットの距離を求める
float EnemyBase::target_distance() const {
	// ターゲットがいなければ最大距離を返す
	if (player_ == nullptr) return FLT_MAX; // float型の最大値
	// ターゲットとの距離を計算する
	return GSvector3::distance(player_->transform().position(), transform_.position());
}

void EnemyBase::find_player(std::string player_str) {
	world_->find_actor(player_str);
}

// プレイヤーを取得する
Actor* EnemyBase::get_player() const {
	return player_;
}

void EnemyBase::clear_point()
{
	way_point_.clear();
}

void EnemyBase::change_point()
{
	// もし配列が登録されていなければ終了
	if (way_point_.empty()) return;

	// 座標カウントを更新
	current_point_count_++;

	// 次の目的座標が存在しない場合
	if (way_point_.size() <= current_point_count_) {
		// 目的座標をリセット
		current_point_count_ = 0;
		current_point_ = way_point_[current_point_count_];
		return;
	}
	// 次の目的座標に変更する
	current_point_ = way_point_[current_point_count_];
}

GSvector3 EnemyBase::get_current_point() const {
	return current_point_;
}

bool EnemyBase::is_arrive() const
{
	return GSvector3::distance(transform_.position(), current_point_) <= arrive_distance_;
}

int EnemyBase::get_current_point_num() const {
	return current_point_count_;
}