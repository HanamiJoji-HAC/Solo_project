#include "Actor.h"

const float FootOffset{ 0.1f };
// 更新
void Actor::update(float) {}

// 遅延更新
void Actor::late_update(float) {}

// 描画
void Actor::draw() const {}

// 半透明オブジェクトの描画
void Actor::draw_transparent() const{}

// GUIの描画
void Actor::draw_gui() const {}

// 衝突リアクション
void Actor::react(Actor&) {}

// メッセージ処理
void Actor::handle_message(const std::string& message, void* param) {}

// 衝突判定
void Actor::collide(Actor& other) {
	// どちらのアクターも衝突判定が有効か？
	if (enable_collider_ && other.enable_collider_) {
		// 衝突判定をする
		if (is_collide(other)) {
			// 衝突した場合は、お互いにリアクションをする
			react(other);
			other.react(*this);
		}
	}
}

// 死亡する
void Actor::die() {
	dead_ = true;
}

// 衝突しているか？
bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

void Actor::check_ground() {
	//地面との衝突判定(線分との交差判定)
 	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f,-FootOffset,0.0f };
	GSvector3 intersect; //地面との交点

	if (world_->field()->collide(line, &intersect)) {
		//フラグを変更
		is_ground = true;
		//交点の位置からy座標のみ補正する
		position.y = intersect.y;
		////座標を変更する
		transform_.position(position);
		//重力を初期化する
		velocity_.y = 0.0f;
	}
	else {
		is_ground = false;
	}
}

// 死亡しているか？
bool Actor::is_dead() const {
	return dead_;
}

// 名前を取得
const std::string& Actor::name() const {
	return name_;
}

// タグ名を取得
const std::string& Actor::tag() const {
	return tag_;
}

// トランスフォームを取得(const版)
const GStransform& Actor::transform() const {
	return transform_;
}

// トランスフォームを取得
GStransform& Actor::transform() {
	return transform_;
}

// 移動量を取得
GSvector3 Actor::velocity() const {
	return velocity_;
}

// 衝突判定データを取得
BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.localToWorldMatrix());
}

//重力計算をする
void Actor::updateGravity(float delta_time, float grav) {
	if (is_ground) return;
	velocity_.y += delta_time * grav;
}