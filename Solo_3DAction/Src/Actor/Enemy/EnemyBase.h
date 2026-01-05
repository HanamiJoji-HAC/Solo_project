#ifndef ENEMY_BASE_H_
#define ENEMY_BASE_H_

#include "Actor/Charactor.h"

// エネミー用抽象基底クラス
class EnemyBase : public Charactor {
public:
	// 状態クラス
	enum class State {
		None,		// 空状態
		Idle,		// アイドル状態
		Move,		// 移動状態
		Chase,		// 追尾状態
		Attack,		// 攻撃状態
		CoolTime,	// クールタイム状態
		Stun,		// 気絶状態
		Dead,		// 死亡状態
	};
public:
	// デフォルトコンストラクタ
	EnemyBase() = default;
	// デストラクタ
	virtual ~EnemyBase() = default;
public:
	// 待機
	virtual void Idle();
	// 移動
	void move(float delta_time, float move_speed) override;
	// 発射
	virtual void Fire();
	// プレイヤーを捜索
	virtual void search();
	// プレイヤーを追尾
	virtual void chase(Actor* player);
	// リロード
	virtual void reload(float cool_time);

protected:
	// ステートを変更する
	virtual void change_state(State state_num);
	// ターゲット方向の角度を求める（符号付き）
	float target_signed_angle() const;
	// ターゲット方向の角度を求める（符号なし）
	float target_angle() const;
	// ターゲットの距離を求める
	float target_distance() const;
public:
	// プレイヤーを取得する
	void get_player() const;
private:
	//	プレイヤー取得用
	Actor* player_;
public:
	// コピー禁止
	EnemyBase(const EnemyBase&) = delete;
	EnemyBase& operator = (const EnemyBase&) = delete;
};
using EnemyState = EnemyBase::State;
#endif