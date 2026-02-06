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
	virtual bool search(float search_timier = 1.0f, float delta_time = 1.0f) override;
	// 発射
	virtual void fire() override;
	// 移動
	virtual void move(float delta_time, float move_speed) override;
	// 方向転換
	void turn_to(const GSvector3& target_pos, float rotate_speed, float delta_time);
public:
	// 回転は完了しているか？
	bool is_complete_turn(GSvector3 target_pos);
	// 回転量の取得
	float get_turn_angle(const GSvector3& target_pos);
	GSvector3 get_waypoint_pos() const;
	// 索敵範囲内か？
	bool is_search(float search_distance = 10.0f) const;
	// ステータスを取得する
	const Status& get_status() const;
	// ステートを変更する
	void change_state(EnemyState state);
	Actor* get_player();
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
	bool is_complete_turn_{ false };
};
#endif