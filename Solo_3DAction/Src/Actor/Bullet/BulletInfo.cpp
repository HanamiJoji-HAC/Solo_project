#include "BulletInfo.h"

// インスタンスを取得
BulletInfo& BulletInfo::get_instance()
{
	static BulletInfo self;
	return self;
}

// 弾のステータス情報を取得する
Bullet_Status BulletInfo::get_bullet(int bullet_id) const
{
	return bullet_status_[bullet_id];
}

// 弾のステータス情報を読み込む
void BulletInfo::load()
{
	bullet_status_.push_back(json_.lode_bullet_status("Assets/Json/BulletStatusLoader.json", "NormalBullet"));
}
