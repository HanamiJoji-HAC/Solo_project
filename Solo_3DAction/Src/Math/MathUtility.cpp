#include "MathUtility.h"

#include "Actor/Actor.h"

#include <algorithm>

const float PITCH_MAX = 45.0f;

float MathUtility::rad2deg(float rad)
{
	return rad * 180.0f / 3.1415926535f;
}

float MathUtility::get_pitch(Actor* self, Actor* target)
{
	GSvector3 pos = self->transform().position();
	GSvector3 forward = self->transform().forward();
	GSvector3 target_pos = target->transform().position();
	GSvector3 dir = (target_pos - pos).normalized();
	float current_pitch = rad2deg(atan2(forward.y, sqrt(forward.x * forward.x + forward.z * forward.z)));
	float target_pitch = rad2deg(atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z)));
	float diff = target_pitch - current_pitch;
	diff = std::clamp(diff, -PITCH_MAX, PITCH_MAX);
	// ‰EŽèÀ•WŒn‚È‚Ì‚Å”½“]
	return -diff;
	//GSvector3 pos = self->transform().position();
	//GSvector3 target_pos = target->transform().position();
	//GSvector3 dir = (target_pos - pos).normalized();
	//float xz = std::sqrt(dir.x * dir.x + dir.z * dir.z);
	//float pitch = rad2deg(atan2(dir.y, xz));
	//pitch = std::clamp(pitch, -PITCH_MAX, PITCH_MAX);
	//// ‰EŽèÀ•WŒn‚È‚Ì‚Å”½“]
	//return -pitch;
}

float MathUtility::get_pitch(Actor* self, const GSvector3& target_position)
{
	GSvector3 pos = self->transform().position();
	GSvector3 forward = self->transform().forward();
	GSvector3 target_pos = target_position;
	GSvector3 dir = (target_pos - pos).normalized();
	float current_pitch = rad2deg(atan2(forward.y, sqrt(forward.x * forward.x + forward.z * forward.z)));
	float target_pitch = rad2deg(atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z)));
	float diff = target_pitch - current_pitch;
	diff = std::clamp(diff, -PITCH_MAX, PITCH_MAX);
	// ‰EŽèÀ•WŒn‚È‚Ì‚Å”½“]
	return -diff;
}

float MathUtility::get_yaw(Actor* self, Actor* target)
{
	GSvector3 pos = self->transform().position();
	GSvector3 target_pos = target->transform().position();
	GSvector3 dir = (target_pos - pos);
	dir.y = 0;
	dir.normalized();
	GSvector3 forward = self->transform().forward();
	forward.y = 0;
	forward.normalized();
	return GSvector3::signedAngle(forward, dir);
}

float MathUtility::get_yaw(Actor* self, const GSvector3& target_position)
{
	GSvector3 pos = self->transform().position();
	GSvector3 target_pos = target_position;
	GSvector3 dir = (target_pos - pos);
	dir.y = 0;
	dir.normalized();
	GSvector3 forward = self->transform().forward();
	forward.y = 0;
	forward.normalized();
	return GSvector3::signedAngle(forward, dir);
}