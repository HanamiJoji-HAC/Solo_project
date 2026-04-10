#ifndef NORMAL_BULLET_H_
#define NORMAL_BULLET_H_

#include "../../BulletBase.h"

class NormalBullet : public BulletBase {
public:
	// コンストラクタ
	NormalBullet(IWorld* world, const GSvector3& position, const BulletInfo::Status& status, Actor* owner, unsigned int mesh_num);
	// 移動
	void update(float delta_time) override;
	// 描画
	void draw() const override;
	// ヒット処理
	void on_hit() override;
private:
	void set_velocity();
};
#endif