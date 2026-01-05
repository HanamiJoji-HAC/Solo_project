#ifndef ENEMY_CANNON_H_
#define ENEMY_CANNON_H_

#include "Actor/Enemy/EnemyBase.h"

#include "State/StateMachine.h"
#include "Rendering/AnimatedMesh.h"

// 敵クラス
class EnemyCannon : public EnemyBase {
public:
	// コンストラクタ
	EnemyCannon(IWorld* world, const GSvector3& position, const Status& status);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// GUI描画
	virtual void draw_gui() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
private:
	// 状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	// アイドル状態中
	void idle(float delta_time);
	// 移動中
	void walk(float delta_time);
	// 振り向き中
	void turn(float delta_time);
	// 攻撃中
	void attack(float delta_time);
	// ダメージ中
	void damage(float delta_time);
	// ダウン中
	void down(float delta_time);
	// 振り向き判定
	bool is_turn() const;
	// 攻撃判定
	bool is_attack() const;
	// 移動判定
	bool is_walk() const;
	// フィールドとの衝突処理
	void collide_field();
	// アクターとの衝突処理
	void collide_actor(Actor& other);
	// 攻撃判定を生成
	void generate_attack_collider();
private:
	StateMachine state_machine_;
	// アニメーションメッシュ
	AnimatedMesh mesh_;
	// モーション番号
	GSuint motion_;
	// モーションのループ指定
	bool motion_loop_;
	// 状態
	State state_;
	// 状態タイマ
	float state_timer_;
	// プレーヤー
	Actor* player_;
	// 体力
	int health_;
};
#endif