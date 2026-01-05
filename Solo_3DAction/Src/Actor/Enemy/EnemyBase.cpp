#include "EnemyBase.h"

// エネミー用抽象基底クラス

// 待機
void EnemyBase::Idle() {

}

// 移動
void EnemyBase::move(float delta_time, float move_speed) {

}

// 発射
void EnemyBase::Fire() {

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

}

// ターゲット方向の角度を求める（符号付き）
float EnemyBase::target_signed_angle() const{

}

// ターゲット方向の角度を求める（符号なし）
float EnemyBase::target_angle() const {

}
	
// ターゲットの距離を求める
float EnemyBase::target_distance() const {

}

// プレイヤーを取得する
void EnemyBase::get_player() const {

}