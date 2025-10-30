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
    // プレーヤーの状態
    enum class State {
        Idle,       // 待機中
        Move,		// 移動中
        Attack,		// 攻撃中
        GetDamage,	// ダメージ中
        Stun,       // 気絶
        Dead,       // 死亡
        Jump,		// ジャンプ中
        AirIdle,    // 空待機中
        AirMove,    // 空中移動中
        Boost,      //ブースト中
    };
    // モーション番号
    enum class Motion {
        MotionIdle = 37,     //アイドル
        MotionDamage = 35,     //ダメージ
        MotionAttack = 55,     //攻撃
        MotionJump = 40,     //ジャンプ
        MotionWalkForward = 50,     //歩き（前）
        MotionWalkBack = 51,     //歩き（後）
        MotionWalkLeft = 53,     //歩き（左）
        MotionWalkRight = 54,     //歩き（右）
        MotionBoost = 34,      //飛ぶ
        MotionDead = 5,
        //MotionMove = 52,       // 走り
        //MotionDown = 3,      // ダウン
        //MotionGetUp = 31,     // 起き上がり
        //MotionJumpStart = 2,  // ジャンプ開始
        //MotionLanding = 31,    // 着地中
        //MotionJumpAttack = 32,// 空中攻撃中
        //MotionAttack2 = 4,
        //MotionAttack3 = 32
    };
public:
    // コンストラクタ
    Player(IWorld* world, const GSvector3& position, const Status& status);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    virtual void draw_gui() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;
public:
    //攻撃
    void attack();
    //ジャンプ
    void jump();
    void set_jump(float jumpPower);
    //移動
    void move(float delta_time,float move_speed) override;
    void boost(float delta_time, float boost_power) override;
    //死亡
    //void dead();
    //弾の生成
    //状態を変更する
    void change_state(State state_num);
    //モーションを変更する
    void change_motion(Motion motion, bool loop);
    //モーションを変更する(番号指定)
    void change_motion(int motion, bool loop);
    //モーションは終了しているか？
    bool is_motion_end();
    //変数を取得する
    float get_gravity();
public:
    void generate_bullet_collider() override;
    void generate_attack_collider() override;
    void add_state();
    void update_state_timer(float delta_time);
    //float get_boost_power();
    const char* current_state_to_string(State state);
    float get_move_speed();
    Status& get_status();
public:
    //Tips:変数はプライベートにしてゲッターを作る
    //モーションデバッグ用
    GSuint up_motion_num_{};
    GSuint down_motion_num_{};
    Input& input_ = Input::get_instance();
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
};
using PlayerMotion = Player::Motion;
using PlayerState = Player::State;
#endif