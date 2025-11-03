#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor/Actor.h"
#include "Actor/Charactor.h"
#include "Rendering/AnimatedMesh.h"
#include "State/StateMachine.h"
#include "Input.h"
// プレーヤークラス
class Player : public Charactor {
public:
    // プレーヤーの状態(必ずInputAction、add_state、Motion、current_state_to_stringに追加を行うこと。)
    enum class State {
        Null,       // 空ステート
        Idle,       // 待機中
        Move,		// 移動中
        Attack,		// 攻撃中
        GetDamage,	// ダメージ中
        Stun,       // 気絶
        Dead,       // 死亡
        Jump,		// ジャンプ中
        AirIdle,    // 空待機中
        AirMove,    // 空中移動中
        Boost,      // ブースト中
        Fire,       // 射撃中

    };
    // モーション番号
    enum class Motion {
        MotionIdle          = 37,          // アイドル
        MotionDamage        = 35,          // ダメージ
        MotionAttack        = 2,           // 攻撃
        MotionJump          = 40,          // ジャンプ
        MotionWalkForward   = 50,          // 歩き（前）
        MotionWalkBack      = 51,          // 歩き（後）
        MotionWalkLeft      = 53,          // 歩き（左）
        MotionWalkRight     = 54,          // 歩き（右）
        MotionBoost         = 34,          // 飛ぶ
        MotionDead          = 5,           // 死亡
        MotionFire          = 55,          // 射撃
        MotionAirIdle       = 18           // 空中アイドル
    };
public:
    // コンストラクタ
    Player(IWorld* world, const GSvector3& position, const Status& status);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // GUI描画
    virtual void draw_gui() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;
public:
    // 攻撃
    void attack();
    // 射撃
    void fire();
    // ジャンプ
    void set_jump(float jumpPower);
    // 移動
    void move(float delta_time,float move_speed) override;
    // ブースト
    void boost(float delta_time, float boost_power) override;
    // 死亡中
    bool is_dying() override;
    // ステートを変更する
    void change_state(State state_num);
    // モーションを変更する
    void change_motion(Motion motion, bool loop);
    // モーションを変更する(番号指定)
    void change_motion(int motion, bool loop);
    // モーションを変更する(レイヤー指定)
    void change_motion(int layer, Motion motion, bool loop);
    // モーションを変更する(レイヤー、番号指定)
    void change_motion(int layer, int motion, bool loop);
    // モーションは終了しているか？
    bool is_motion_end();
public:
    // 弾判定の生成
    void generate_bullet_collider() override;
    // 攻撃判定の生成
    void generate_attack_collider() override;
    // ステートの追加
    void add_state();
    // ステートタイマの更新
    void update_state_timer(float delta_time);
    // 現在のステートを文字列に変換
    const char* current_state_to_string(State state);
    // ステータスを参照する
    Status& get_status();
    const AnimatedMesh& get_mesh();
    int get_previous_state();
public:
    //Tips:変数はプライベートにしてゲッターを作る
    //モーションデバッグ用
    GSuint up_motion_num_{};
    GSuint down_motion_num_{};
private:
    float state_timer_{ 0 };
    //アニメーションメッシュ
    AnimatedMesh mesh_;
    //状態
    State state_{};
    //モーション番号
    GSuint motion_;
    //モーションのループ指定
    bool motion_loop_;
    StateMachine statemachine_{};
    GSvector2 move_input_value_{};
    // 入力クラス
    Input& input_ = Input::get_instance();
};
using PlayerMotion = Player::Motion;
using PlayerState = Player::State;
#endif