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
#include "imgui/imgui.h"
//Todo:コメントアウトに規則を持たせる
//自分の高さ
const float PlayerHeight{ 1.0f };
//衝突判定用の半径
const float PlayerRadius{ 0.4f };

const float JumpPower{ 0.60f };

const float WalkSpeed{ 0.1f };

// 足元のオフセット
const float FootOffset{ 0.1f };

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
    mesh_{ Mesh_Player, (GSuint)Motion::MotionIdle, true},
    motion_{ (GSuint)Motion::MotionIdle },
    motion_loop_{ true },
    state_timer_{ 0.0f } {
    name_ = "Player";
    transform_.position(position);
    world_ = world;
    //衝突判定球の設定
    collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f,PlayerHeight,0.0f} };
    //状態を設定
    add_state();
    //初期状態を設定
    change_state(State::Move);
}

// 更新
void Player::update(float delta_time) {
    statemachine_.update(delta_time);
    // メッシュを更新
    mesh_.change_motion(motion_, motion_loop_);
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
    //接地チェック
    check_ground();
#ifndef DEBUG
    ImGui::Begin("Debug");
    bool ground = check_ground();
    ImGui::Text("is_ground: %s", ground ? "true" : "false");
    ImGui::DragFloat3("velocity", velocity_);
    ImGui::DragFloat3("position", transform_.position());
    auto motion = static_cast<int>(motion_);
    ImGui::DragInt("motion_num:", &motion);
    ImGui::DragFloat("motion_timer", &state_timer_);
    auto state = statemachine_.is_current_state((int)PlayerState::Move);
    ImGui::Checkbox("State", &state);
    ImGui::End();
    if (gsGetKeyTrigger(GKEY_UP)) ++motion_num;
    if (gsGetKeyTrigger(GKEY_DOWN)) --motion_num;
#endif
}

// 描画
void Player::draw() const {
    mesh_.draw();
    collider().draw();
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
    }
}

//移動
void Player::move(float delta_time) {
    // カメラの前方向ベクトルを取得
    GSvector3 forward = world_->camera()->transform().forward();
    forward.y = 0.0f;
    // カメラの右方向ベクトルを取得
    GSvector3 right = world_->camera()->transform().right();
    right.y = 0.0f;
    //入力値を取得(正規化して斜辺移動の値を一定にする)
    GSvector2 input = input_.get_left_stick_input().normalized();
    // キーの入力値から移動ベクトルを計算
    GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
    velocity += forward * input.y;
    velocity += right * input.x;
    velocity_.x = velocity.x * WalkSpeed * delta_time;
    velocity_.z = velocity.z * WalkSpeed * delta_time;
    transform_.translate(velocity_);
}

// 攻撃中
void Player::attack(float delta_time) {
    state_timer_ += delta_time;
    // 攻撃モーションの終了を待つ
    if (is_motion_end()) {
        change_state(State::Move);
    }
}

// ダメージ中
void Player::damage(float delta_time) {
    state_timer_ += delta_time;
    // ダメージモーションの終了を待つ
    if (is_motion_end()) {
        change_state(State::Move);
    }
}

void Player::jump(float jumpPower, float delta_time) {
    state_timer_+= delta_time;
    //ジャンプ力を設定(一度のみ)
    if (check_ground()) {
        velocity_.y = jumpPower;
    }
    // モーションの終了を待つ
    if (state_timer_ >= mesh_.motion_end_time() / 4) {
        change_state(State::Move);
    }
    transform_.translate(velocity_);
}

// アクターとの衝突処理
void Player::collide_actor(Actor& other) {
    // ｙ座標を除く座標を求める
    GSvector3 position = transform_.position();
    position.y = 0.0f;
    GSvector3 target = other.transform().position();
    target.y = 0.0f;
    // 相手との距離
    float distance = GSvector3::distance(position, target);
    // 衝突判定球の半径同士を加えた長さを求める
    float length = collider_.radius + other.collider().radius;
    // 衝突判定球の重なっている長さを求める
    float overlap = length - distance;
    // 重なっている部分の半分の距離だけ離れる移動量を求める
    GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
    transform_.translate(v, GStransform::Space::World);
    // フィールドとの衝突判定
    collide_field();
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
void Player::generate_bullet() {
    // 弾を生成する場所の距離
    const float GenerateDistance{ 0.5f };
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

// フィールドとの衝突判定
void Player::collide_field() {
    // 壁との衝突判定（球体との判定）
    GSvector3 center; // 押し戻し後の球体の中心座標
    if (world_->field()->collide(collider(), &center)) {
        // y座標は変更しない
        center.y = transform_.position().y;
        // 補正後の座標に変更する
        transform_.position(center);
    }
    // 地面との衝突判定（線分との交差判定）
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
    GSvector3 intersect; // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交点の位置からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
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
}

void Player::change_motion(Motion motion, bool loop) {
    motion_ = (GSuint)motion;
    motion_loop_ = loop;
    mesh_.change_motion(motion_, motion_loop_);
}

void Player::change_motion(int motion, bool loop) {
    motion_ = (GSuint)motion;
    motion_loop_ = loop;
    mesh_.change_motion(motion_, motion_loop_);
};

void Player::change_state(State state_num) {
    state_timer_ = 0;
    statemachine_.change_state((GSuint)state_num);
}

bool Player::is_motion_end() {
    return state_timer_ >= mesh_.motion_end_time();
}