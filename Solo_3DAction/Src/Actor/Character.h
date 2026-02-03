#ifndef CHARACTER_H_
#define CHARACTER_H_

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
// playerとそれに干渉するアクターのためのクラス
class Character : public Actor {
public:
	Character(const Status& status);

	virtual ~Character() = default;
public:
	virtual void take_damage(Actor& other, int damage);				// 相手の体力を減らす処理
	virtual void update_gravity(float delta_time, float grav);		// 重力
	virtual bool check_ground();									// 接地判定
	void set_invisible(bool is_invisible, float invisible_timer); 	// 無敵を設定する
	void invisible(float delta_time);								// 無敵を設定する（タイマー式）
	bool is_invisible();											// 無敵中か？
protected:
	virtual void collide_actor(Actor& other);						// アクターとの衝突判定
	virtual void collide_field();									// 床との衝突判定
	virtual void collide_celling();									// 天井との衝突判定
	virtual void generate_attack_collider();						// 攻撃判定の生成
	virtual void generate_bullet_collider();						// 弾攻撃判定の生成
	virtual void move(float delta_time, float move_speed);			// 移動
	virtual void boost(float delta_time, float boost_power);		// ブースト
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
	// 無敵中か？
	bool is_invisible_{ false };
	// DEBUG:天井に触れているか？
	bool is_celling_{ false };
};
#endif