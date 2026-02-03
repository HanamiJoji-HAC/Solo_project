#include "EnemyHeilcoptor.h"
#include "GameConfig.h"
#include "imgui/imgui.h"
#include "Propeller.h"
#include "States/HeilMoveState.h"

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
	ImGui::DragFloat3("collider", collider_.center);
	ImGui::DragFloat3("way1", way_point_[0]);
	ImGui::DragFloat3("way2", way_point_[1]);
	ImGui::DragFloat3("way3", way_point_[2]);
	ImGui::DragFloat3("way4", way_point_[3]);
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

void EnemyHeilcoptor::turn(float rotate_speed, float delta_time)
{
	// 回転値を取得する
	float target_signed_angle = turn_angle();
	// 角度の分だけ回転する
	if (std::abs(target_signed_angle) <= 0.1f) return;
	// 1フレームで回転できる最大角度
	float max_rotate = rotate_speed * delta_time;
	// 回転値を設定する
	float rotation = std::clamp(
		target_signed_angle,
		-max_rotate,
		max_rotate
	);
	// 回転する
	transform_.rotate(0.0f, rotation, 0.0f);
}

float EnemyHeilcoptor::turn_angle()
{
	// 現在の目的座標を取得
	GSvector3 current_vector = get_current_point();
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
	// 自身の正面ベクトルと、目的ベクトルとの角度差を取得する
	return GSvector3::signedAngle(forward_vector, target_vector);
}



void EnemyHeilcoptor::search()
{
	// 索敵距離
	float search_distance = 10.0f;
	// 索敵条件を満たすか？
	auto is_search = [&]() -> bool {
		return target_angle() > 30.0f && target_distance() > search_distance;
	};

	if (!is_search()) return;
	change_state(State::Attack);
}

void EnemyHeilcoptor::fire()
{
	generate_bullet_collider();
}

void EnemyHeilcoptor::add_state()
{
	state_machine_.add_state((GSuint)State::Move, std::make_shared<HeilMoveState>(*this));
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

const Status& EnemyHeilcoptor::get_status() const
{
	return status_;
}
