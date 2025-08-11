#include "Player.h"

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) {
    name_ = "Player";
    mesh_ = { 0, 0, true};
    transform_.position(position);
}

// 更新
void Player::update(float delta_time) {
    mesh_.update(delta_time);
    mesh_.transform(transform_.localToWorldMatrix());

}

// 描画
void Player::draw() const {
    mesh_.draw();
}

// 衝突リアクション
void Player::react(Actor& other) {}