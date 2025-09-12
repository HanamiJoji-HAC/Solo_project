#ifndef PLAYER_BULLET_H_
#define PLAYER_BULLET_H_
#include "Actor.h"

// プレーヤーの弾クラス
class PlayerBullet : public Actor {
public:
	// コンストラクタ
	PlayerBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
private:
	// 寿命
	float lifespan_timer_;
};
#endif
