#ifndef ENEMY_EnemyRobot_H_
#define ENEMY_EnemyRobot_H_

#include "Actor/Enemy/EnemyBase.h"

#include "State/StateMachine.h"
#include "Rendering/AnimatedMesh.h"

// 敵クラス
class EnemyRobot : public EnemyBase {
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
	EnemyRobot(IWorld* world, const GSvector3& position, const Status& status);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// GUI描画
	virtual void draw_gui() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
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