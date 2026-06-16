#ifndef MATH_UTILITY_H_
#define MATH_UTILITY_H_

class Actor;
struct GSvector3;
namespace MathUtility {
	// ラジアンを角度に変換
	float rad2deg(float rad);
	// ターゲット間のpitch角を取得する
	float get_pitch(Actor* self, Actor* target);
	float get_pitch(Actor* self, const GSvector3& target);
	// ターゲット間のyaw角を取得する
	float get_yaw(Actor* self, Actor* target);
	float get_yaw(Actor* self, const GSvector3& target);
}
#endif