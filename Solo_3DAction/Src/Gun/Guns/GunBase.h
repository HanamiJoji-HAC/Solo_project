#ifndef GUN_BASE_H_
#define GUN_BASE_H_

#include "../IGun.h"
#include "World/IWorld.h"
#include "Rendering/AnimatedMesh.h"
#include "Systems/LoadJson.h"

class GunBase : public IGun {
public:
	// コンストラクタ
	GunBase(IWorld* world, BulletInfo::Bullet_ID bullet_id);
	// 描画
	void draw() const override;
	// 発射
	void fire(Actor* owner) override;
protected:
	// トランスフォーム
	GStransform transform_{};
	// メッシュ情報
	unsigned int mesh_id_{};
	// ワールド
	IWorld* world_{ nullptr };
	// 弾ID
	BulletInfo::Bullet_ID bullet_id_;
	LoadJson& json_ = LoadJson::get_instance();
private:
};

#endif