//Todo:名前順とか命名規則決める
#include "Player.h"
#include "imgui/imgui.h"
#include "Src/Assets.h"
#include "Src/AttackCollider.h"
#include "Src/PlayerBullet.h"
#include "PlayerIdleState.h"
//Todo:コメントアウトに規則を持たせる
//自分の高さ
const float PlayerHeight{ 1.0f };
//衝突判定用の半径
const float PlayerRadius{ 0.4f };

const float JumpPower{ 0.60f };

const float grav{ -0.020f };

const float WalkSpeed{ 0.1f };

// 足元のオフセット
const float FootOffset{ 0.1f };
//モーションデバッグ用
int motion_num{ 0 };

// モーション番号
enum {
    MotionIdle        = 37,     //アイドル
    MotionDamage      = 35,     //ダメージ
    MotionAttack      = 57,     //攻撃
    MotionJump        = 40,     //ジャンプ
    MotionWalkForward = 50,     //歩き（前）
    MotionWalkBack    = 51,     //歩き（後）
    MotionWalkLeft    = 53,     //歩き（左）
    MotionWalkRight   = 54,     //歩き（右）
    MotionFry         = 34      //飛ぶ
    //MotionMove = 52,       // 走り
    //MotionDown = 3,      // ダウン
    //MotionGetUp = 31,     // 起き上がり
    //MotionJumpStart = 2,  // ジャンプ開始
    //MotionLanding = 31,    // 着地中
    //MotionJumpAttack = 32,// 空中攻撃中
    //MotionAttack2 = 4,
    //MotionAttack3 = 32
};

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
    mesh_{ Mesh_Player, MotionIdle, true},
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Move },
    state_timer_{ 0.0f } {
    name_ = "Player";
    transform_.position(position);
    world_ = world;
    //衝突判定球の設定
    collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f,PlayerHeight,0.0f} };
    add_state();
    statemachine_.change_state((GSuint)State::Attack);
}

// 更新
void Player::update(float delta_time) {
    statemachine_.update(delta_time);
    //重力
    updateGravity(delta_time, grav);
    //状態の更新
    update_state(delta_time);

    // メッシュを更新
    mesh_.change_motion(motion_, motion_loop_);
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
    //接地チェック
    check_ground();

    //デバッグ用↓
    ImGui::Begin("Debug");
    ImGui::Text("is_ground: %s", is_ground ? "true" : "false");
    ImGui::DragFloat3("velocity", velocity_);
    ImGui::DragFloat3("position", transform_.position());
    ImGui::DragInt("motion_num:", &motion_num);
    int state = (int)state_;
    ImGui::DragInt("State", &state);
    state_ = (State)state;
    ImGui::End();
    if (gsGetKeyTrigger(GKEY_UP)) ++motion_num;
    if (gsGetKeyTrigger(GKEY_DOWN)) --motion_num;
}

// 描画
void Player::draw() const {
    mesh_.draw();
    collider().draw();
}

// 衝突リアクション
void Player::react(Actor& other) {
    // ダメージ中はリアクションしない
    if (state_ == State::Damage) return;
    // 敵と衝突した場合はダメージ中に遷移
    if (other.tag() == "EnemyTag") {
        // ダメージ効果音を再生
        //gsPlaySE(Se_PlayerDamage);
        // ダメージ中は衝突判定を無効にする
        enable_collider_ = false;
        // ダメージ中に遷移する
        change_state(State::Damage, MotionDamage, false);
    }
}

//移動
void Player::move(float delta_time) {
    //ジャンプ
    if (is_ground && gsGetKeyTrigger(GKEY_SPACE)) {
        change_state(State::Jump, MotionJump, false);
        return;
    }
    //攻撃
    if (is_ground && gsGetMouseButtonState(GMOUSE_BUTTON_2)) {
        change_state(State::Attack, MotionAttack, false);
        generate_bullet();
        return;
    }
    //跳躍
    if (is_ground && gsGetKeyTrigger(GKEY_LSHIFT)) {
        change_state(State::Fry, MotionFry, false);
        return;
    }
    //何もしていなければ待機
    GSuint motion{ MotionIdle };

    //移動処理
    // カメラの前方向ベクトルを取得
    GSvector3 forward = world_->camera()->transform().forward();
    forward.y = 0.0f;
    // カメラの右方向ベクトルを取得
    GSvector3 right = world_->camera()->transform().right();
    right.y = 0.0f;
    // キーの入力値から移動ベクトルを計算
    GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
    if (gsGetKeyState(GKEY_W)) {
        velocity += forward;
        motion = MotionWalkForward;
    }
    if (gsGetKeyState(GKEY_S)) {
        velocity -= forward;
        motion = MotionWalkBack;
    }
    if (gsGetKeyState(GKEY_A)) {
        velocity -= right;
        motion = MotionWalkLeft;
    }
    if (gsGetKeyState(GKEY_D)) {
        velocity += right;
        motion = MotionWalkRight;
    }
    velocity = velocity.normalized() * WalkSpeed * delta_time;
    velocity_.x = velocity.x;
    velocity_.z = velocity.z;

    change_state(State::Move, motion, false);
    transform_.translate(velocity_);
}
// 状態の更新
void Player::update_state(float delta_time) {
    // 状態遷移
    switch (state_) {
    case State::Idle:                                   break;
    case State::Move:   move(delta_time);               break;
    case State::Jump:   jump(JumpPower, delta_time);    break;
    case State::Attack: attack(delta_time);             break;
    case State::Damage: damage(delta_time);             break;
    }
    // 状態タイマの更新
    state_timer_ += delta_time;
}

// 状態の変更
void Player::change_state(State state, GSuint motion, bool loop) {
    motion_ = motion;
    motion_loop_ = loop;
    state_ = state;
    state_timer_ = 0.0f;
}

bool Player::is_idle() {
    if (is_ground && velocity_.x == 0 && velocity_.y == 0 && velocity_.z == 0) {
        return true;
    }
}

// 攻撃中
void Player::attack(float delta_time) {
    // 攻撃モーションの終了を待つ
    if (state_timer_ >= mesh_.motion_end_time()) {
        move(delta_time);
    }
}

// ダメージ中
void Player::damage(float delta_time) {
    // ダメージモーションの終了を待つ
    if (state_timer_ >= mesh_.motion_end_time()) {
        move(delta_time);
    }
}

void Player::jump(float jumpPower, float delta_time) {
    //ジャンプ力を設定(一度のみ)
    if (is_ground) {
        velocity_.y = jumpPower;
    }
    // 攻撃モーションの終了を待つ
    if (state_timer_ >= mesh_.motion_end_time() / 4) {
        move(delta_time);
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
    //// フィールドとの衝突判定
    //collide_field();
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

void Player::add_state() {
    statemachine_.add_state((GSuint)State::Attack, std::make_shared<PlayerIdleState>(*this));
}