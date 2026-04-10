#ifndef BULLET_BASE_H_
#define BULLET_BASE_H_

#include "Actor/Actor.h"
#include "BulletInfo.h"
#include "Assets.h"

// 前方宣言
class Actor;

// 弾用インターフェース
class BulletBase : public Actor {
public:
	// コンストラクタ
	BulletBase(IWorld* world, const BulletInfo::Status& status, Actor* owner, unsigned int mesh_num);
	// 更新
	void update(float delta_time) override;
	// 衝突判定
	void react(Actor& other) override;
	// 描画
	void draw() const override;
protected:
	// ヒット処理
	virtual void on_hit() {};
protected:
	// 弾の寿命
	float lifespan_timer_{};
	// 弾ステータス
	BulletInfo::Status status_{};
	// オーナー
	Actor* owner_{};
private:
	unsigned int mesh_num{};
};

#endif