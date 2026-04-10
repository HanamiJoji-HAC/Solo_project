#include "GunManager.h"
#include "IGun.h"

// デストラクタ
GunManager::~GunManager()
{
	clear();
}

// 初期化
void GunManager::clear()
{
	// 配列内の要素をすべて削除
	for (auto& ptr : guns_) {
		delete ptr; 
		ptr = nullptr;
	}
	guns_.clear();
}

// 銃を追加する
void GunManager::add_gun(IGun* gun_class)
{
	if (gun_class == nullptr) return;
	guns_.push_back(gun_class);
}

// 発射
void GunManager::fire(int gun_num, Actor* owner)
{
	IGun* gun = find(gun_num);
	if (gun != nullptr) {
		gun->fire(owner);
	};
}

// 描画
void GunManager::draw(int gun_num) const
{
	const IGun* gun = find(gun_num);
	if (gun != nullptr) {
		gun->draw();
	};
}

//	銃の要素数を返却する
int GunManager::gun_count() const
{
	return static_cast<int>(guns_.size());
}

// 銃の検索
IGun* GunManager::find(int gun_num)
{
	// 範囲外指定ならnull
	if (gun_num >= gun_count() || gun_num < 0) return nullptr;
	return guns_[gun_num];
}

// const用オーバーロード関数
const IGun* GunManager::find(int gun_num) const
{
	// 範囲外指定ならnull
	if (gun_num >= gun_count() || gun_num < 0) return nullptr;
	return guns_[gun_num];
}