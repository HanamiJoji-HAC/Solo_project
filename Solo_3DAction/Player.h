#ifndef PLAYER_H_
#define PLAYER_H_

#include "Src/Actor.h"
#include "Src/AnimatedMesh.h"
#include "StateMachine.h"
// プレーヤークラス
class Player : public Actor {
public:
    // プレーヤーの状態
    enum class State {
        Idle,       // 待機中
        Move,		// 移動中
        Attack,		// 攻撃中
        Damage,		// ダメージ中
        Jump,		// ジャンプ中
        Fry,        //
        FryMove,
        Boost,
    };
    enum class Input {
        Flont,  //前移動
        Back,   //後ろ移動
        Left,   //左移動
        Right,  //右移動
        Jump,   //ジャンプ
        Attack  //攻撃
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
private:
    //状態更新
    void update_state(float delta_time);
    //状態遷移の指定
    void change_state(State state, GSuint motion, bool loop);
    //ジャンプ
    void jump(float jumpPower, float delta_time);
    //移動
    void move(float delta_time);
    //攻撃
    void attack(float delta_time);
    //ダメージ
    void damage(float delta_time);

    void collide_actor(Actor& other);

    void generate_attack_collider();
    void generate_bullet();
    void collide_field();
    bool is_idle();
    void add_state();
private:
    //アニメーションメッシュ
    AnimatedMesh mesh_{ 0 };
    //状態
    State state_{};
    //状態タイマ
    float state_timer_{ 0 };
    //モーション番号
    GSuint motion_;
    //モーションのループ指定
    bool motion_loop_;
    Input input_{};
    StateMachine statemachine_{};
};

#endif