#include "Charactor.h"
#include "GameConfig.h"

Charactor::Charactor(const Status& status) : status_(status) {}

void Charactor::take_damage(Actor& other, int damage) {
	status_.hp_ = CLAMP(status_.hp_ - damage, 0, INT_MAX);
}

void Charactor::update_gravity(float delta_time, float gravity) {
	velocity_.y += (delta_time / cREF) * gravity;
	transform_.translate(0.0f, velocity_.y, 0.0f);
    collide_field();
};

void Charactor::move(float delta_time, float move_speed) {}

void Charactor::boost(float delta_time, float boost_power) {}

void Charactor::collide_actor(Actor& other) {
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
};

void Charactor::collide_field() {
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
    line.end = position + GSvector3{ 0.0f, -foot_offset_, 0.0f };
    GSvector3 intersect; // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交点の位置からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
    // 地面との衝突判定（線分との交差判定）
    //GSvector3 position = transform_.position();
    //Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f, 2.0f, 0.0f };
    //GSvector3 intersect; // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交点の位置からy座標のみ補正する
        //position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
};

bool Charactor::check_ground() {
    //地面との衝突判定(線分との交差判定)
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f,-foot_offset_,0.0f };
    GSvector3 intersect; //地面との交点

    if (world_->field()->collide(line, &intersect)) {
        //交点の位置からy座標のみ補正する
        position.y = intersect.y;
        ////座標を変更する
        transform_.position(position);
        //重力を初期化する
        velocity_.y = 0.0f;
        return true;
    }
    return false;
}

void Charactor::generate_attack_collider() {}

void Charactor::generate_bullet_collider() {}

void Charactor::set_invisible(bool is_invisible, float invisible_timer) {
    status_.invisible_timer_ = invisible_timer;
    is_invisible_ = is_invisible;
}

void Charactor::invisible(float delta_time) {
    if (status_.invisible_timer_ <= 0) {
        set_invisible(false, status_.default_inbisible_timer_);
        return;
    }
    status_.invisible_timer_ -= (delta_time / cREF);
}

bool Charactor::is_invisible() {
    return is_invisible_;
}