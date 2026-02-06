#include "EnemyCannon.h"

EnemyCannon::EnemyCannon(IWorld* world, const GSvector3& position, const Status& status) : 
	EnemyBase(status),
	mesh_{ Mesh_Cannon, Mesh_Cannon, Mesh_Cannon, 0, true },
		motion_{ 0 },
		motion_loop_{ true },
		state_{ State::Idle },
		state_timer_{ 0.0f },
		player_{ nullptr },
		health_{ 5 } {
		// ワールドの設定
		world_ = world;
		// タグ名の設定
		tag_ = "EnemyTag";
		// 名前の設定
		name_ = "Enemy";
		// 衝突判定球の設定
		collider_ = BoundingSphere{ 0, GSvector3{ 0.0f, 0, 0.0f} };
		// 座標の初期化
		transform_.position(position);
		// ワールド変換行列の初期化
		mesh_.transform(transform_.localToWorldMatrix());
}

void EnemyCannon::update(float delta_time)
{
}

void EnemyCannon::draw() const
{
}

void EnemyCannon::draw_gui() const
{
}

void EnemyCannon::react(Actor& other)
{
}

void EnemyCannon::change_state(State state, GSuint motion, bool loop)
{
}

void EnemyCannon::idle(float delta_time)
{
}

void EnemyCannon::walk(float delta_time)
{
}

void EnemyCannon::turn(float delta_time)
{
}

void EnemyCannon::attack(float delta_time)
{
}

void EnemyCannon::damage(float delta_time)
{
}

void EnemyCannon::down(float delta_time)
{
}

bool EnemyCannon::is_turn() const
{
	return false;
}

bool EnemyCannon::is_attack() const
{
	return false;
}

void EnemyCannon::collide_field()
{
}

void EnemyCannon::collide_actor(Actor& other)
{
}

void EnemyCannon::generate_attack_collider()
{
}
