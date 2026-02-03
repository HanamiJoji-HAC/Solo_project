#ifndef ENEMY_BASE_H_
#define ENEMY_BASE_H_

#include "Actor/Character.h"
#include "Systems/LoadJson.h"

// エネミー用抽象基底クラス
class EnemyBase : public Character {
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
	// コンストラクタ
	EnemyBase(const Status status);
public:
	// 待機
	virtual void Idle();
	// 移動
	virtual void move(float delta_time, float move_speed);
	// 発射
	virtual void fire();
	// プレイヤーを捜索
	virtual void search();
	// プレイヤーを追尾
	virtual void chase(Actor* player);
	// リロード
	virtual void reload(float cool_time);
protected:
	// ステートを変更する
	void change_state(State state_num);
	// プレイヤーを検索する
	void find_player(std::string player_str);
	// ターゲット方向の角度を求める（符号付き）
	float target_signed_angle() const;
	// ターゲット方向の角度を求める（符号なし）
	float target_angle() const;
	// ターゲットの距離を求める
	float target_distance() const;

protected:
	// 目的座標配列
	std::vector<GSvector3> way_point_;
	// 目的座標を初期化する
	void clear_point();
	// 目的座標を変更する
	void change_point();
	// 現在の目的座標を返却する
	GSvector3 get_current_point() const;
	// 到着しているか？
	bool is_arrive() const;
	// 現在の目的座標番号を返す
	int get_current_point_num() const;
private:
	// 現在の目的座標
	GSvector3 current_point_;
	// 次の目的座標
	GSvector3 next_point_;
	// 到着判定距離
	float arrive_distance_{ 0.1f };
	int current_point_count_{ -1 };
public:
	// プレイヤーを取得する
	Actor* get_player() const;
private:
	//	プレイヤー取得用
	Actor* player_ = nullptr;
	LoadJson& json_ = LoadJson::get_instance();
};
using EnemyState = EnemyBase::State;
#endif