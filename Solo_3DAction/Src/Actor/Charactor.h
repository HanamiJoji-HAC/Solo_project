#ifndef CHARACTOR_H_
#define CHARACTOR_H_

#include "Actor/Actor.h"
#include "Collision/AttackCollider.h"
#include "Player/PlayerBullet.h"
#include "Input.h"

struct Status
{
	// ステータス構造体
	Status(
		int hp_ = 50,
		float energy_ = 100,
		float max_energy_ = 100,
		int melee_atk_ = 5,
		int ranged_atk_ = 1,
		float jump_power_ = 0.20f,
		float boost_speed_ = 5.0f,
		float walk_speed_ = 5.0f,
		float move_speed_ = 5.0f,
		float gravity_ = -0.40f,
		float invisible_timer_ = 3.0f,
		float default_inbisible_timer_ = 3.0f,
		float air_move_speed_ = 5.0f,
		float max_boost_speed_ = 15.0f,
		float quick_boost_speed_ = 3.0f
	) :
		hp_{ hp_ },
		energy_{ energy_ },
		max_energy_{ max_energy_ },
		melee_atk_{ melee_atk_ },
		ranged_atk_{ ranged_atk_ },
		jump_power_{ jump_power_ },
		boost_speed_{ boost_speed_ },
		walk_speed_{ walk_speed_ },
		move_speed_{ move_speed_ },
		gravity_{ gravity_ },
		invisible_timer_{ invisible_timer_ },
		default_inbisible_timer_{ default_inbisible_timer_ },
		air_move_speed_{ air_move_speed_ },
		max_boost_speed_{ max_boost_speed_ },
		quick_boost_speed_{ quick_boost_speed_ }
	{ }
	int hp_{ 0 };
	float energy_{ 0.0f };
	float max_energy_{ 0.0f };
	int melee_atk_{ 0 };
	int ranged_atk_{ 0 };
	float jump_power_{ 0.0f };
	float boost_speed_{ 0.0f };
	float walk_speed_{ 0.0f };
	float move_speed_{ 0.0f };
	float gravity_{ 0.0f };
	float invisible_timer_{ 0.0f };
	float default_inbisible_timer_{ 0.0f };
	float air_move_speed_{ 0.0f };
	float max_boost_speed_{ 0.0f };
	float quick_boost_speed_{ 0.0f };
};

class Charactor : public Actor {
public:
	Charactor(const Status& status);

	virtual ~Charactor() = default;
public:
	virtual void take_damage(Actor& other, int damage);
	virtual void update_gravity(float delta_time, float grav);
	virtual bool check_ground();
	// 無敵を設定する
	void set_invisible(bool is_invisible, float invisible_timer);
	// 無敵を設定する（タイマー式）
	void invisible(float delta_time);
	bool is_invisible();
protected:
	virtual void collide_actor(Actor& other);
	virtual void collide_field();
	virtual void generate_attack_collider();
	virtual void generate_bullet_collider();
	// 移動
	virtual void move(float delta_time, float move_speed);
	// ブースト
	virtual void boost(float delta_time, float boost_power);
protected:
	// ステータスパラメータ
	Status status_;
	// 身長
	float height_{ 1.0f };
	// 自身の判定
	float radius_{ 0.5f };
	// 足元のオフセット
	float foot_offset_{ 0.05f };
	// 死亡判定
	bool dead_{ false };
	Input& input_ = Input::get_instance();
	bool is_invisible_{ false };
};
#endif