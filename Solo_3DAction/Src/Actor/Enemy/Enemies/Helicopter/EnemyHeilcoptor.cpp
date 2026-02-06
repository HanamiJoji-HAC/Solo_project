#include "EnemyHeilcoptor.h"
#include "GameConfig.h"
#include "imgui/imgui.h"
#include "Propeller.h"
#include "States/HeilMoveState.h"
#include "States/HeilAttackState.h"
#include "States/HeilSearchState.h"

EnemyHeilcoptor::EnemyHeilcoptor(IWorld* world, const GSvector3& position, const Status& status, const std::vector<GSvector3> way_points) : EnemyBase{ status }
{
	name_ = "EnemyHeilcoptor";
	tag_ = "EnemyTag";
	world_ = world;
	transform_.position(position);

	// プロペラを生成
	const GSvector3 prop_offest{ position.x, position.y, position.z };
	propeller_ = new Propeller(world_, prop_offest, this);
	world_->add_actor(propeller_);

	//衝突判定球の設定
	collider_ = BoundingSphere{ radius_ + 1.0f , GSvector3{0.0f, height_ ,0.0f} };
	height_ = 5.0f;

	// 目的座標の登録
	way_point_ = way_points;
	change_point();
	// ステートの追加
	add_state();
	// 初期ステートの設定
	change_state(State::Move);
}

void EnemyHeilcoptor::update(float delta_time)
{
	state_machine_.update(delta_time);
	ImGui::Begin("heil_debug");
	ImGui::DragFloat3("velocity",velocity_);
	ImGui::DragFloat3("position", transform_.position());
	int current_waypoint = get_current_point_num();
	ImGui::InputInt("waypoint", &current_waypoint);
	int current_state = state_machine_.get_current_state();
	ImGui::InputInt("NowState", &current_state);
	ImGui::End();
}

void EnemyHeilcoptor::draw() const
{
	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(transform_.localToWorldMatrix());
	// メッシュの描画
	gsDrawMesh(Mesh_Helicopter);
	glPopMatrix();
	collider().draw();
}

void EnemyHeilcoptor::draw_gui() const
{

}

void EnemyHeilcoptor::react(Actor& other)
{
	collide_celling();
	collide_field();
}

void EnemyHeilcoptor::move(float delta_time, float move_speed)
{
	// 目的地に到着しているか？
	if (is_arrive()) {
		change_point();
		return;
	}
	GSvector3 velocity;
	GSvector3 forward = transform_.forward();
	velocity += forward;
	velocity_ = velocity.normalized() * move_speed * (delta_time / cREF);

	transform_.translate(velocity_, GStransform::Space::World);
}

void EnemyHeilcoptor::turn_to(const GSvector3& target_pos, float rotate_speed, float delta_time)
{
	if (is_complete_turn(target_pos)) return;

	// 回転値を設定する
	float target_signed_angle = get_turn_angle(target_pos);
	float max_rotate = rotate_speed * delta_time / cREF;
	float rotation = std::clamp(
		target_signed_angle,
		-max_rotate,
		max_rotate
	);
	// 回転する
	transform_.rotate(0.0f, rotation, 0.0f);
}

bool EnemyHeilcoptor::search(float search_timer, float delta_time)
{
	return false;
}

void EnemyHeilcoptor::fire()
{
	generate_bullet_collider();
}

void EnemyHeilcoptor::add_state()
{
	state_machine_.add_state((int)EnemyState::Move, std::make_shared<HeilMoveState>(*this));
	state_machine_.add_state((int)EnemyState::Attack, std::make_shared<HeilAttackState>(*this));
	state_machine_.add_state((int)EnemyState::Search, std::make_shared<HeilSearchState>(*this));
}

// 弾判定の生成
void EnemyHeilcoptor::generate_bullet_collider() {
	// 弾を生成する場所の距離
	const float GenerateDistance{ 0.8f };
	// 生成する位置の高さの補正値
	const float GenerateHeight{ 1.5f };
	// 弾の移動スピード
	const float Speed{ 1.0f };
	// 生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// 生成位置の高さを補正する
	position.y += GenerateHeight;
	// 移動量の計算
	GSvector3 velocity = transform_.forward() * Speed;
	// 弾の生成
	world_->add_actor(new PlayerBullet{ world_, position, velocity });
}

bool EnemyHeilcoptor::is_complete_turn(GSvector3 target_pos)
{
	// 回転値を取得する
	float target_signed_angle = get_turn_angle(target_pos);
	// 回転が完了していたら終了
	if (std::abs(target_signed_angle) <= 0.1f) {
		is_complete_turn_ = true;
		return true;
	}
	 return is_complete_turn_ = false;
}

float EnemyHeilcoptor::get_turn_angle(const GSvector3& target_pos)
{
	// 現在の目的座標を取得
	GSvector3 current_vector = target_pos;
	// 自身の座標から目的座標に向かうベクトルを取得
	GSvector3 target_vector = current_vector - transform_.position();
	// 現在の正面ベクトルを取得する
	GSvector3 forward_vector = transform_.forward();
	// y軸成分は考慮しない
	target_vector.y = 0.0f;
	forward_vector.y = 0.0f;
	// 正規化
	target_vector.normalized();
	forward_vector.normalized();

	// 回転値を返却する
	return GSvector3::signedAngle(forward_vector, target_vector);
}

GSvector3 EnemyHeilcoptor::get_waypoint_pos() const
{
	return get_current_point();
}

bool EnemyHeilcoptor::is_search(float search_distance) const
{
	// 索敵条件を満たすか？
	const float angle = target_angle();
	const float distance = target_distance();
	return angle < 15.0f && distance < search_distance;
}

const Status& EnemyHeilcoptor::get_status() const
{
	return status_;
}

void EnemyHeilcoptor::change_state(EnemyState state)
{
	state_machine_.change_state((int)state);
}

Actor* EnemyHeilcoptor::get_player()
{
	return find_player();
}
