#ifndef IGUN_H_
#define IGUN_H_

#include "Actor/Bullet/BulletBase.h"
#include "Input.h"
// ステート用の基底抽象クラス
class IGun {
public:
	// デフォルトコンストラクタ
	IGun() = default;
	// 仮想デストラクタ
	virtual ~IGun() = default;
	// 描画
	virtual void draw() const = 0;
	// 発射
	virtual void fire(Actor* owner) = 0;
public:
	//コピー禁止
	IGun(const IGun& other) = delete;
	IGun& operator = (const IGun& other) = delete;
};

#endif