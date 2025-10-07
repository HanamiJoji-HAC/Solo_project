#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor/Actor.h"
#include "Rendering/AnimatedMesh.h"
#include "State/StateMachine.h"
#include "Input.h"
// プレーヤークラス
class Player : public Actor {
public:
    // プレーヤーの状態
    enum class State {
        Idle,       // 待機中
        Move,		// 移動中
        Attack,		// 攻撃中
        GetDamage,		// ダメージ中
        Stun,
        Dead,
        Jump,		// ジャンプ中
        Fall,
        Fry,        //
        FryMove,
        Boost,
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
        MotionFry = 34,      //飛ぶ
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
    Player(IWorld* world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;
public:
    //攻撃
    void attack(float delta_time);
    //ジャンプ
    void jump(float jumpPower, float delta_time);
    //移動
    void move(float delta_time);
    void generate_bullet();
    void change_state(State state_num);
    bool is_motion_end();
    //モーションを変更する
    void change_motion(Motion motion, bool loop);
    //モーションを変更する(番号指定)
    void change_motion(int motion, bool loop);
    //Tips:変数はプライベートにしてゲッターを作る
    float grav{ -0.01f };
    //状態タイマ
    float state_timer_{ 0 };
    //モーションデバッグ用
    int motion_num{ 0 };
private:
    //ダメージ
    void damage(float delta_time);
    void collide_actor(Actor& other);
    void generate_attack_collider();
    void collide_field();
    void add_state();
private:
    //アニメーションメッシュ
    AnimatedMesh mesh_{ 0 };
    //状態
    State state_{};
    //モーション番号
    GSuint motion_;
    //モーションのループ指定
    bool motion_loop_;
    Input& input_ = Input::get_instance();
    StateMachine statemachine_{};
    GSvector2 move_input_value_{};
};
using PlayerMotion = Player::Motion;
using PlayerState = Player::State;
#endif