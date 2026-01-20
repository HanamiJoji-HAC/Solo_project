#include "EnemyRobot.h"

EnemyRobot::EnemyRobot(IWorld* world, const GSvector3& position, const Status& status) : EnemyBase(status),
mesh_{ Mesh_Robot, Mesh_Robot, Mesh_Robot, (GSuint)Motion::MotionIdle, true, 2 },
motion_{ (GSuint)Motion::MotionIdle },
motion_loop_{ true },
state_timer_{ 0 } {

	name_ = "EnemyDrone";
	tag_ = "EnemyTag";
	world_ = world;
	transform().position(position);
}

void EnemyRobot::update(float delta_time)
{
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
}

void EnemyRobot::draw() const
{
	mesh_.draw();
	collider().draw();
}

void EnemyRobot::draw_gui() const
{
}

void EnemyRobot::react(Actor& other) {

}