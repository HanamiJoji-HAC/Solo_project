#ifndef ENEMY_DRONE_H_
#define ENEMY_DRONE_H_

#include "Actor/Enemy/EnemyBase.h"

#include "State/StateMachine.h"
#include "Rendering/AnimatedMesh.h"

// 敵クラス
class EnemyDrone : public EnemyBase {
public:
	// モーション番号
	enum class Motion {
		MotionIdle = 37,          // アイドル
		MotionDamage = 35,          // ダメージ
		MotionAttack = 2,           // 攻撃
		MotionJump = 34,          // ジャンプ
		MotionWalkForward = 50,          // 歩き（前）
		MotionWalkBack = 51,          // 歩き（後）
		MotionWalkLeft = 53,          // 歩き（左）
		MotionWalkRight = 54,          // 歩き（右）
		MotionBoost = 18,          // ブースト
		MotionDead = 5,           // 死亡
		MotionFire = 55,          // 射撃
		MotionAirIdle = 18,          // 空中アイドル
		MotionQuickBoostUp = 7,
		MotionQuickBoostDown = 8,
		MotionQuickBoostLeft = 20,
		MotionQuickBoostRight = 22,
		MotionLanding = 19,
	};
public:
	// コンストラクタ
	EnemyDrone(IWorld* world, const GSvector3& position, const Status& status);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// GUI描画
	virtual void draw_gui() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
private:
	// アイドル状態中
	void idle(float delta_time);
	// 移動中
	void walk(float delta_time);
	// 振り向き中
	void turn(float delta_time);
	// 攻撃中
	void attack(float delta_time);
	// ダメージ中
	void damage(float delta_time);
	// ダウン中
	void down(float delta_time);
	// 振り向き判定
	bool is_turn() const;
	// 攻撃判定
	bool is_attack() const;
	// 移動判定
	bool is_walk() const;
	// 攻撃判定を生成
	void generate_attack_collider();
private:
	// アニメーションメッシュ
	AnimatedMesh mesh_;
	// モーション番号
	GSuint motion_;
	// モーションのループ指定
	bool motion_loop_;
	// 状態タイマ
	float state_timer_;
};
#endif