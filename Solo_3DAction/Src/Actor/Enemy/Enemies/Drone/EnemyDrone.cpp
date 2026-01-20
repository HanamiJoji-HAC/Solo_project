#include "EnemyDrone.h"

EnemyDrone::EnemyDrone(IWorld* world, const GSvector3& position, const Status& status) : EnemyBase(status),
	mesh_{ Mesh_Drone, Mesh_Drone, Mesh_Drone, (GSuint)Motion::MotionIdle, true, 2 },
	motion_{ (GSuint)Motion::MotionIdle },
	motion_loop_{ true },
	state_timer_{ 0 } {

	name_ = "EnemyDrone";
	tag_ = "EnemyTag";
	world_ = world;
	transform_.position(position);
}

void EnemyDrone::update(float delta_time)
{
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
}

void EnemyDrone::draw() const
{
	mesh_.draw();
	collider().draw();
}

void EnemyDrone::draw_gui() const
{
}

void EnemyDrone::react(Actor& other)
{
}

void EnemyDrone::idle(float delta_time)
{
}

void EnemyDrone::walk(float delta_time)
{
}

void EnemyDrone::turn(float delta_time)
{
}

void EnemyDrone::attack(float delta_time)
{
}

void EnemyDrone::damage(float delta_time)
{
}

void EnemyDrone::down(float delta_time)
{
}

bool EnemyDrone::is_turn() const
{
	return false;
}

bool EnemyDrone::is_attack() const
{
	return false;
}

bool EnemyDrone::is_walk() const
{
	return false;
}

void EnemyDrone::generate_attack_collider()
{
}
