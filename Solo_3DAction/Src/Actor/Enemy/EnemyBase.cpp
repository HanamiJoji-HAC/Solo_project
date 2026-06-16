#include "EnemyBase.h"
#include "GSmathf.h"
#include <iostream>
#include "GameConfig.h"
#include "Math/MathUtility.h"
#include "imgui/imgui.h"

EnemyBase::EnemyBase(const Status& status) : Character(status){
}

void EnemyBase::Idle() {}

void EnemyBase::move(float delta_time, float move_speed) {}

void EnemyBase::fire(GunInfo::Gun_ID gun_id)
{
	guns_.fire(gun_id, this);
}

bool EnemyBase::search(float search_timer, float delta_time) {
	return false;
}

void EnemyBase::chase(Actor* player) {}

// 弾のリロード
void EnemyBase::reload(float cool_time) {}

// 銃の登録
void EnemyBase::add_gun(){}

// ターゲット方向の角度を求める（符号付き）
float EnemyBase::target_signed_angle() const {
	const Actor* player = find_player();
	if (player == nullptr) return 0.0f;
	GSvector3 to_target = player->transform().position() - transform_.position();
	GSvector3 forward = transform_.forward();
	// 高さ成分を考慮しない
	forward.y = 0.0f;
	to_target.y = 0.0f;
	// ベクトルの角度差を求める
	return GSvector3::signedAngle(forward, to_target);
}

// ターゲット方向の角度を求める（符号なし）
float EnemyBase::target_angle() const {
	return std::abs(target_signed_angle());
}
	
// ターゲットとの距離を求める
float EnemyBase::target_distance() const {
	const Actor* player = find_player();
	if (player == nullptr) return FLT_MAX;
	return GSvector3::distance(player->transform().position(), transform_.position());
}

Actor* EnemyBase::find_player() const {
	return world_->find_actor("Player");
}

void EnemyBase::clear_way_point()
{
	way_point_.clear();
}

void EnemyBase::change_way_point()
{
	// もし配列が登録されていなければ終了
	if (way_point_.empty()) return;

	// 座標カウントを更新
	current_way_point_count_++;

	// 次の目的座標が存在しない場合
	if (way_point_.size() <= current_way_point_count_) {
		// 目的座標をリセット
		current_way_point_count_ = 0;
		current_way_point_ = way_point_[current_way_point_count_];
		return;
	}
	// 次の目的座標に変更する
	current_way_point_ = way_point_[current_way_point_count_];
}

GSvector3 EnemyBase::get_current_way_point() const {
	return current_way_point_;
}

bool EnemyBase::is_arrive_way_point() const
{
	return GSvector3::distance(transform_.position(), current_way_point_) <= arrive_distance_;
}

int EnemyBase::get_way_point_num() const {
	return current_way_point_count_;
}

bool EnemyBase::is_dying() const
{
	//体力が残っているか？
	return status_.hp_ <= 0;
}

const Character::Status& EnemyBase::get_status() const
{
	return status_;
}

void EnemyBase::turn_to(Actor* target)
{
	if (is_complete_turn(target)) return;
	auto pos = target->transform().position();
	transform_.lookAt(pos);
}

void EnemyBase::turn_to(const GSvector3& target_pos, float delta_time)
{
	if (is_complete_turn(target_pos)) return;
	// 回転値を設定する
	auto pos = transform_.position();
	GSvector3 vec = target_pos - pos;
	GSquaternion rot = GSquaternion::lookRotation(vec);
	GSquaternion target_rot = GSquaternion::rotateTowards(GSquaternion(transform_.rotation()), rot, 360.0f * delta_time / cREF);
	transform_.rotation(target_rot);
}

bool EnemyBase::is_complete_turn(Actor* target)
{
	// 回転値を取得する
	auto yaw = MathUtility::get_yaw(this, target);
	auto pitch = MathUtility::get_pitch(this, target);
	// 回転が完了していたら終了
	//
	if (std::abs(yaw) <= 0.1f && std::abs(pitch) <= 0.1f) {
		return true;
	}
	return false;
}

bool EnemyBase::is_complete_turn(const GSvector3& target_pos)
{
	// 回転値を取得する
	auto yaw = MathUtility::get_yaw(this, target_pos);
	auto pitch = MathUtility::get_pitch(this, target_pos);
	// 回転が完了していたら終了
	//  
	if (std::abs(yaw) <= 0.1f && std::abs(pitch) <= 0.1f) {
		return true;
	}
	return false;
}
