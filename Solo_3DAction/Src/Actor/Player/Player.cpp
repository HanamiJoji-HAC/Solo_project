//Todo:名前順とか命名規則決める
#include "Assets.h"
#include "Collision/AttackCollider.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "PlayerStates/PlayerAttackState.h"
#include "PlayerStates/PlayerDeadState.h"
#include "PlayerStates/PlayerGetDamageState.h"
#include "PlayerStates/PlayerIdleState.h"
#include "PlayerStates/PlayerJumpState.h"
#include "PlayerStates/PlayerMoveState.h"
#include "PlayerStates/PlayerStunState.h"
#include "PlayerStates/PlayerBoostState.h"
#include "imgui/imgui.h"
#include "GameConfig.h"
//Todo:コメントアウトに規則を持たせる
//Status:身長
const float PlayerHeight{ 1.0f };
//Status:衝突判定用の半径
const float PlayerRadius{ 0.4f };

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position, const Status& status) : Charactor(status),
    mesh_{ Mesh_Player, Mesh_Player, Mesh_Player, (GSuint)Motion::MotionIdle, true, 2 },
    motion_{ (GSuint)Motion::MotionIdle },
    motion_loop_{ true },
    state_timer_{ 0 } 
{
    name_ = "Player";
    transform_.position(position);
    world_ = world;
    //衝突判定球の設定
    collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f, height_ ,0.0f} };
    //状態を設定
    add_state();
    //初期状態を設定
    change_state(State::Move);
    // 下半身のボーンＩＤ
    static const GSuint lower_body_bone_id[] =
    { 0, 1, 2, 69, 70, 71, 72, 73, 74, 75, 76, 77 };
    // 下半身ボーンをレイヤー１に設定（それ以外の上半身ボーンはレイヤー０）
    mesh_.set_layer_indices(1, lower_body_bone_id, 12);
}

// 更新
void Player::update(float delta_time) {
    statemachine_.update(delta_time);
    if (gsGetKeyTrigger(GKEY_UP)) {
        ++up_motion_num_;
        mesh_.change_motion(0, up_motion_num_);
    }
    if (gsGetKeyTrigger(GKEY_DOWN)) {
        --up_motion_num_;
        mesh_.change_motion(0, up_motion_num_);
    };
    if (gsGetKeyTrigger(GKEY_LEFT)) {
        --down_motion_num_;
        mesh_.change_motion(1, down_motion_num_);
    };
    if (gsGetKeyTrigger(GKEY_RIGHT)) {
        ++down_motion_num_;
        mesh_.change_motion(1, down_motion_num_);
    };
    // メッシュを更新
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());

    //接地チェック
    check_ground();
#ifndef DEBUG
    ImGui::Begin("Player_motion_state");
    auto motion = static_cast<int>(motion_);
    ImGui::DragInt("motion_num:", &motion);
    float end_motion_time = mesh_.motion_end_time();
    ImGui::DragFloat("end_motion_time", &end_motion_time);
    float motion_timer = mesh_.current_motion_time();
    ImGui::DragFloat("motion_time", &motion_timer);
    bool motion_loop = motion_loop_;
    ImGui::Checkbox("motion_loop_", &motion_loop);
    auto current_state_num = (State)statemachine_.get_current_state();
    ImGui::Text("Now State is %s", current_state_to_string(current_state_num));
    bool motion_end = is_motion_end();
    ImGui::Text("is_end_motion: %s", motion_end ? "true" : "false");
    int up_motion_num = (int)up_motion_num_;
    ImGui::DragInt("up_motion_num", &up_motion_num);
    int down_motion_num = (int)down_motion_num_;
    ImGui::DragInt("down_motion_num", &down_motion_num);
    ImGui::End();
#endif

#ifndef DEBUG
    ImGui::Begin("PlayerStatus");
    ImGui::DragFloat3("velocity", velocity_);
    ImGui::DragFloat3("position", transform_.position());
    ImGui::DragInt("HP", &status_.hp_);
    ImGui::DragInt("MATK", &status_.melee_atk_);
    ImGui::DragInt("RATK", &status_.ranged_atk_);
    ImGui::DragFloat("energy", &status_.energy_);
    ImGui::DragFloat("max_energy", &status_.max_energy_);
    ImGui::DragFloat("walk_speed", &status_.walk_speed_);
    ImGui::DragFloat("move_speed", &status_.move_speed_);
    ImGui::DragFloat("boost_speed", &status_.boost_speed_);
    ImGui::DragFloat("jump_power", &status_.jump_power_);
    ImGui::DragFloat("gravity_", &status_.gravity_);
    bool ground = check_ground();
    ImGui::Text("is_ground: %s", ground ? "true" : "false");
    ImGui::End();
#endif // !DEBUG

}

// 描画
void Player::draw() const {
    mesh_.draw();
    collider().draw();
}
void Player::draw_gui() const
{
    GSuint bone_num = mesh_.bone_count(0);
    for (int i = 0; i <= 10; ++i) {
        GSvector2 p;
        GSvector3 o = mesh_.bone_matrices(i).position();
        gsCalculateScreen(&p, &o);
        gsTextPos(p.x, p.y);
        std::string text = std::to_string(i);
        gsDrawText(text.c_str());
    }
}
// 衝突リアクション
void Player::react(Actor& other) {
    // ダメージ中はリアクションしない
    if (state_ == State::GetDamage) return;
    // 敵と衝突した場合はダメージ中に遷移
    if (other.tag() == "EnemyTag") {
        // ダメージ効果音を再生
        //gsPlaySE(Se_PlayerDamage);
        // ダメージ中は衝突判定を無効にする
        enable_collider_ = false;
        // ダメージ中に遷移する
        change_state(State::GetDamage);
        change_motion(PlayerMotion::MotionDamage, true);
    }
}

//移動
void Player::move(float delta_time, float move_speed) {
    // Lスティックの入力情報を取得
    input_.get_left_stick_input_angle();
    // カメラの前方向ベクトルを取得
    GSvector3 forward = world_->camera()->transform().forward();
    forward.y = 0.0f;
    // カメラの右方向ベクトルを取得
    GSvector3 right = world_->camera()->transform().right();
    right.y = 0.0f;
    // 入力値を取得
    GSvector2 input = input_.get_left_stick_input_value();

    // キーの入力値から移動ベクトルを計算
    GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
    velocity += forward * input.y;
    velocity += right * input.x;
    velocity = velocity.normalized() * move_speed * (delta_time / cREF);

    // 回転処理
    if (velocity.length() != 0.0f) {
        // 向きの補間
        GSquaternion rotation =
            GSquaternion::rotateTowards(
                transform_.rotation(),
                GSquaternion::lookRotation(forward), 12.0f * delta_time);
        transform_.rotation(rotation);
    }

    // 移動する
    velocity_.x = velocity.x;
    velocity_.z = velocity.z;
    transform_.translate(velocity, GStransform::Space::World);
}

// 攻撃中
void Player::attack() {
    generate_bullet_collider();
}
// ジャンプ
void Player::jump() {
    transform_.translate(velocity_, GStransform::Space::World);
}
//ジャンプ中
void Player::set_jump(float jumpPower) {
    //ジャンプ力を設定
    velocity_.y = jumpPower;
    transform_.translate(velocity_, GStransform::Space::World);
}

void Player::boost(float delta_time, float boost_power) {
    int boost_input = input_.get_action_input(InputAction::BOOST);
    velocity_.y = boost_power * (delta_time / cREF) * boost_input;
    //アクションが実行されている間は持続的に上昇
    transform_.translate(velocity_);
}

void Player::generate_attack_collider() {
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 1.0f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 0.70f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackColliderDelay{ 3.0f };
    // 攻撃判定の寿命
    const float AttackColliderLifeSpan{ 15.0f };

    // 衝突判定を出現させる座標を求める（前方の位置）
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // 高さの補正（足元からの高さ）
    position.y += AttackColliderHeight;
    // 衝突判定用の球を作成
    BoundingSphere collider{ AttackColliderRadius, position };
    // 衝突判定を出現させる
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerTag", "PlayerAttack", tag_, AttackColliderLifeSpan, AttackColliderDelay });
}

// 弾の生成
void Player::generate_bullet_collider() {
    // 弾を生成する場所の距離
    const float GenerateDistance{ 0.8f };
    // 生成する位置の高さの補正値
    const float GenerateHeight{ 1.5f };
    // 弾の移動スピード
    const float Speed{ 1.0f };
    // 生成位置の計算
    GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
    // 生成位置の高さを補正する
    position.y += GenerateHeight;
    // 移動量の計算
    GSvector3 velocity = transform_.forward() * Speed;
    // 弾の生成
    world_->add_actor(new PlayerBullet{ world_, position, velocity });
}

//状態を追加する
void Player::add_state() {
    statemachine_.add_state((GSuint)State::Attack, std::make_shared<PlayerAttackState>(*this));
    statemachine_.add_state((GSuint)State::Dead, std::make_shared<PlayerDeadState>(*this));
    statemachine_.add_state((GSuint)State::GetDamage, std::make_shared<PlayerGetDamageState>(*this));
    statemachine_.add_state((GSuint)State::Idle, std::make_shared<PlayerIdleState>(*this));
    statemachine_.add_state((GSuint)State::Jump, std::make_shared<PlayerJumpState>(*this));
    statemachine_.add_state((GSuint)State::Move, std::make_shared<PlayerMoveState>(*this));
    statemachine_.add_state((GSuint)State::Stun, std::make_shared<PlayerStunState>(*this));
    statemachine_.add_state((GSuint)State::Boost,std::make_shared<PlayerBoostState>(*this));
}

void Player::change_motion(Motion motion, bool loop) {
    state_timer_ = 0;
    motion_ = (GSuint)motion;
    motion_loop_ = loop;
    mesh_.change_motion(motion_, motion_loop_);
}

void Player::change_motion(int motion, bool loop) {
    state_timer_ = 0;
    motion_ = (GSuint)motion;
    motion_loop_ = loop;
    mesh_.change_motion(motion_, motion_loop_);
};

void Player::change_state(State state_num) {
    statemachine_.change_state((GSuint)state_num);
}

bool Player::is_motion_end() {
    return mesh_.is_motion_end();
}

float Player::get_gravity() {
    return status_.gravity_;
}

void Player::update_state_timer(float delta_time) {
    state_timer_ += delta_time;
}

const char*Player::current_state_to_string(State state) {
    switch (state)
    {
    case Player::State::Idle:       return "Idle";
    case Player::State::Move:       return "Move";
    case Player::State::Attack:     return "Attack";
    case Player::State::GetDamage:  return "GetDamage";
    case Player::State::Stun:       return "Stun";
    case Player::State::Dead:       return "Dead";
    case Player::State::Jump:       return "Jump";
    case Player::State::AirIdle:    return "AirIdle";
    case Player::State::AirMove:    return "AirMove";
    case Player::State::Boost:      return "Boost";
    default:                        return "None";
    }
}
float Player::get_move_speed() {
    return status_.move_speed_;
}

Status& Player::get_status() {
    return status_;
}
