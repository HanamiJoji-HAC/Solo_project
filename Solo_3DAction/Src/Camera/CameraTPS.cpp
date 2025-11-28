#include "CameraTPS.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "Math/Line.h"
#include "imgui/imgui.h"
#include <algorithm>
#include "Actor/Enemy/Enemy.h"
#include "Camera/State/CameraNormalState.h"
#include "Camera/State/CameraLockOnState.h"


const float MaxSpeed{ 20.0f }; // 移動スピードの最大値
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.5f, 0.0f };
// プレーヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f, 2.0f, -5.0f };
// 距離限界
const float DistanceLimit{ 100.0f };
// カメラ感度倍率
const float Sencitivity{ 2.0f };
// コンストラクタ
CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at) {
	// ワールドを設定
	world_ = world;
	// タグの設定
	tag_ = "CameraTag";
	// 名前の設定
	name_ = "Camera";
	// 視点の位置を設定
	transform_.position(position);
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
	//状態を設定
	add_state();
	//初期状態を設定
	change_state(State::Normal);
}

// 更新
void CameraTPS::update(float delta_time) {
	state_machine_.update(delta_time);

	if (input_.get_action_input(InputAction::CAMERA_RESET)) {
		reset_camera(player_);
	}
	// TODO:ロックオンの解除
	// ロックオン中は常に一番近い敵にロックオンするように動的にロックオンする
#ifndef DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("look_point", ReferencePointOffset);
	ImGui::DragFloat3("position", PlayerOffset);
	ImGui::End();
#endif // !DEBUG
}

// 描画
void CameraTPS::draw() const {
	// 視点の位置
	GSvector3 eye = transform_.position();
	// 注視点の位置
	GSvector3 at = eye + transform_.forward();
	// カメラの設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z, // 視点の位置
		at.x, at.y, at.z, // 注視点の位置
		0.0f, 1.0f, 0.0f // 視点の上方向
	);
}

void CameraTPS::tpp_normal(Actor* player, float delta_time) {
	// 左スティックの状態を取得
	GSvector2 input_lstick = input_.get_right_stick_axis();
	// ヨー・ピッチをスティックの状態に応じて計算する
	update_tpp_normal(input_lstick, delta_time, Sencitivity);

	// 注視点の位置を求める（プレーヤーの頭部の少し上あたりの座標）
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	GSvector3 velocity = velocity_;
	// 視点の位置を求める（プレーヤーの背後の座標）
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0) * PlayerOffset;
	// フィールドとの衝突判定
	Line line{ at, position };
	GSvector3 intersect;
	// フィールドに衝突していたらカメラ座標をクランプする
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}
	// スムースダンプによる滑らかな補間
	position = smooth_damp(MaxSpeed, smooth_time_, position, velocity, delta_time);
	// 視点の位置を設定
	transform_.position(position);
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);

#ifndef DEBUG
	ImGui::Begin("tpp_normal");
	ImGui::DragFloat3("camera_position", position);
	ImGui::DragFloat3("velocity", velocity);
	ImGui::DragFloat("pitch", &pitch_);
	ImGui::DragFloat("yaw", &yaw_);
	ImGui::End();
#endif // !DEBUG
}

void CameraTPS::update_tpp_normal(GSvector2 stick_input, float delta_time, float sensitivity)
{
	// スティックの入力がなければ何もしない
	if (stick_input.x == 0 && stick_input.y == 0) return;

	// 感度・時間でスケーリング
	yaw_ -= stick_input.x * sensitivity * delta_time;
	pitch_ -= stick_input.y * sensitivity * delta_time;

	// pitch角を制限（上向き・下向きの限界）
	const float pitchMin = -45.0f;
	const float pitchMax = 59.0f;
	pitch_ = std::clamp(pitch_, pitchMin, pitchMax);
}

// カメラの滑らかな補間
GSvector3 CameraTPS::smooth_damp(float speed, float smooth_time, GSvector3 position, GSvector3 velocity, float delta_time) {
	//// スムースダンプによる滑らかな補間
	return GSvector3::smoothDamp(transform_.position(), position, velocity,
		smooth_time_, MaxSpeed, delta_time);
}

// カメラ内に対象がいるか？
bool CameraTPS::is_actor_on_screen(Actor* other) {
	// カメラの正面ベクトルとカメラから敵のベクトルの内積を取得
	GSvector3 forward_vector = (transform_.forward()).normalized();
	// エネミーの座標を取得
	GSvector3 other_pos = other->transform().position();
	// エネミーとカメラ座標のベクトルの向きを取得
	GSvector3 other_vector = (other_pos - transform_.position()).normalized();
	// 正面ベクトルとエネミー間のベクトルの内積を計算する
	float dot = GSvector3::dot(forward_vector, other_vector);
	// 自身と敵の距離を取得
	float mag = (transform_.position() - other_pos).magnitude();
	// REVIEW:dot = 0~1なら画面内、負数ならカメラより後ろに存在する
	// REVIEW:mag = DistanceLimitの距離まで判別可能
	return dot >= 0.7f && mag <= DistanceLimit;
}

// カメラをデフォルトにリセットする
void CameraTPS::reset_camera(Actor* player) {
	// プレイヤーのヨーに合わせる
	yaw_ = player->transform().eulerAngles().y;
	pitch_ = 0.0f;
}

// 
void CameraTPS::reset_actor() {
	target_ = nullptr;
}

// アクターをロックオンする
void CameraTPS::lock_on_actor(const std::string& tag) {
	// 指定したタグを持つアクターを検索
	// 参照ループだからちょっとだけ無茶しても大丈夫
	auto actors = world_->find_actor_with_tag(tag);
	// ターゲットとの距離
	float to_target_distance = FLT_MAX;
	// エネミーの配列の数だけforループ
	for (auto* actor : actors) {
		// エネミーが視界にいるか？
		if (!is_actor_on_screen(actor)) continue;
		is_lock_on_ = true;
		// 敵と自分との距離を計算する
		float mag = (transform_.position() - actor->transform().position()).magnitude();
		// 前回のエネミーとの距離の近さを比較する
		if (mag < to_target_distance) {
			// ターゲットを更新する
			to_target_distance = mag;
			target_ = actor;
		}
	}
}

void CameraTPS::find_actor(const std::string& player) {
	player_ = world_->find_actor(player);
}

Actor* CameraTPS::get_player() {
	return player_;
}

// ステートを変更する
void CameraTPS::change_state(State state_num) {
	state_machine_.change_state((GSuint)state_num);
}

// ステートの追加
void CameraTPS::add_state() {
	state_machine_.add_state((GSuint)State::Normal, std::make_shared<CameraNormalState>(*this));
	state_machine_.add_state((GSuint)State::LockOn, std::make_shared<CameraLockOnState>(*this));
}