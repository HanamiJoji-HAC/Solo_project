#ifndef ENEMY_HEILCOPTOR
#define ENEMY_HEILCOPTOR

#include "Actor/Enemy/EnemyBase.h"

#include "Rendering/AnimatedMesh.h"

class Propeller;

// 敵クラス
class EnemyHeilcoptor : public EnemyBase {
public:
	// コンストラクタ
	EnemyHeilcoptor(IWorld* world, const GSvector3& position, const Status& status, const std::vector<GSvector3> way_points);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// GUI描画
	virtual void draw_gui() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
public:
	// 索敵
	virtual void search() override;
	// 発射
	virtual void fire() override;
	// 移動
	virtual void move(float delta_time, float move_speed) override;
	// 方向転換
	void turn(float rotate_speed, float delta_time);
	// 回転値の取得
	float turn_angle();
public:
	// ステータスを取得する
	const Status& get_status() const;
private:
	//	ステートの追加
	void add_state();
	// 弾の生成
	void generate_bullet_collider();
private:
	// 状態タイマ
	float state_timer_{ 0.0f };
	// プロペラクラス
	Propeller* propeller_{ nullptr };
};
#endif