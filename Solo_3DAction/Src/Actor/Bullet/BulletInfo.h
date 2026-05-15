#ifndef BULLET_INFO_H_
#define BULLET_INFO_H_

#include <vector>
#include "Systems/LoadJson.h"

// 弾ステータス
struct Bullet_Status
{
	int atk_{};			// 攻撃力
	bool is_stun_{};	// スタン属性を持つか？
	float speed_{};		// 弾速
	float duration_{};	// 発射間隔
};
class BulletInfo {
public:
	enum Bullet_ID {
		Normal,
		Bazooka,
		Grenade,
		Missile
	};
public:
	// コンストラクタ
	BulletInfo() = default;
	// インスタンスを取得
	static BulletInfo& get_instance();
	// 弾のステータス情報を返却する
	Bullet_Status get_bullet(int bullet_id) const;
	// 弾のステータス情報を取得する
	void load();
private:
	// 弾管理変数
	std::vector<Bullet_Status> bullet_status_{};
	// Jsonクラス
	LoadJson& json_ = LoadJson::get_instance();
public:
	// コピー禁止
	BulletInfo(const BulletInfo&) = delete;
	BulletInfo& operator = (const BulletInfo&) = delete;
};
#endif