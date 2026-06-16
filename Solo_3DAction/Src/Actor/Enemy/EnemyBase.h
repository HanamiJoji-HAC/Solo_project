#ifndef ENEMY_BASE_H_
#define ENEMY_BASE_H_

#include "Actor/Character.h"
#include "Systems/LoadJson.h"

#include "Gun/GunManager.h"
#include "Gun/GunInfo.h"

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
		Search,		// 索敵状態
	};
public:
	// コンストラクタ
	EnemyBase(const Status& status);
public:
	// 待機
	virtual void Idle();
	// 移動
	virtual void move(float delta_time, float move_speed);
	// 発射
	virtual void fire(GunInfo::Gun_ID gun_id);
	// プレイヤーを捜索
	virtual bool search(float search_timer, float delta_time);
	// プレイヤーを追尾
	virtual void chase(Actor* player);
	// リロード
	virtual void reload(float cool_time);
	// 銃を登録する
	virtual void add_gun();
	// ステータスを取得する
	virtual const Status& get_status() const;

	void turn_to(Actor* target);
	void turn_to(const GSvector3& target_pos, float delta_time);
	bool is_complete_turn(Actor* target);
	bool is_complete_turn(const GSvector3& target_pos);

protected:
	// プレイヤーを検索する
	Actor* find_player() const;
	// ターゲット方向の角度を求める（符号付き）
	float target_signed_angle() const;
	// ターゲット方向の角度を求める（符号なし）
	float target_angle() const;
	// ターゲットの距離を求める
	float target_distance() const;
protected:
	// 目的座標を初期化する
	void clear_way_point();
	// 目的座標を変更する
	void change_way_point();
	// 現在の目的座標を返却する
	GSvector3 get_current_way_point() const;
	// 到着しているか？
	bool is_arrive_way_point() const;
	// 現在の目的座標番号を返す
	int get_way_point_num() const;
	// 死亡中
	bool is_dying() const;
protected:
	// 目的座標配列
	std::vector<GSvector3> way_point_;
	// 銃マネージャー
	GunManager guns_;
private:
	// 現在の目的座標
	GSvector3 current_way_point_;
	// 次の目的座標
	GSvector3 next_point_;
	// 到着判定距離
	float arrive_distance_{ 0.1f };
	// 到着ポイントカウンター
	int current_way_point_count_{ -1 };
	// ピッチ角差分計算用
	float pitch_{ 0.0f };
private:
	//	プレイヤー取得用
	Actor* player_ = nullptr;
	LoadJson& json_ = LoadJson::get_instance();
};
using EnemyState = EnemyBase::State;
#endif