#ifndef GUN_MANAGER_H_
#define GUN_MANAGER_H_

#include<vector>

#include "Actor/Actor.h"

// 前方宣言
class IGun;

class GunManager {
public:
	// デフォルトコンストラクタ
	GunManager() = default;
	// デストラクタ
	~GunManager();
	// 初期化
	void clear();
	// 銃を追加する
	void add_gun(IGun* gun_class);
	// 発射
	void fire(int gun_num, Actor* owner);
	// 描画
	void draw(int gun_num) const;
	// 銃の要素数を返却する
	int gun_count() const;
private:
	// 銃リスト
	std::vector<IGun*> guns_;
private:
	// 銃の検索
	IGun* find(int gun_num);
	const IGun* find(int gun_num) const;
public:
	//コピー禁止
	GunManager(const GunManager& other) = delete;
	GunManager& operator = (const GunManager& other) = delete;
};
#endif