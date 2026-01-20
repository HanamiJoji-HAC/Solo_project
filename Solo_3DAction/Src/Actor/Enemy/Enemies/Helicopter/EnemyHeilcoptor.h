#ifndef ENEMY_HEILCOPTOR
#define ENEMY_HEILCOPTOR

#include "Actor/Enemy/EnemyBase.h"

#include "Rendering/AnimatedMesh.h"
#include "Propeller.h"

// 敵クラス
class EnemyHeilcoptor : public EnemyBase {
public:
	// コンストラクタ
	EnemyHeilcoptor(IWorld* world, const GSvector3& position, const Status& status);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// GUI描画
	virtual void draw_gui() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
private:
	void move();
	// 状態タイマ
	float state_timer_;
	// プロペラクラス
	Propeller* propeller_{ nullptr };
};
#endif