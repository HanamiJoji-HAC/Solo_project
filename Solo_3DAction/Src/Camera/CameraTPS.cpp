#include "CameraTPS.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "Math/Line.h"
#include "imgui/imgui.h"
#include <algorithm>
#include "Actor/Enemy/Enemy.h"
#include "Camera/State/CameraNormalState.h"
#include "Camera/State/CameraLockOnState.h"
#include "Camera/State/CameraResetState.h"
#include "GameConfig.h"

#include "GSmathf.h"


const float MaxSpeed{ 20.0f }; // 移動スピードの最大値
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.5f, 0.0f };
// ロックオン時カメラの注視点の補正値
const GSvector3 LockOnReferencePointOffset{ 0.0f, 1.5f, 0.0f };
// プレーヤーからの相対座標
const GSvector3 NormalOffset{ 0.0f, 2.0f, -5.0f };
// ロックオン時プレーヤーからの相対座標
const GSvector3 LockOnOffest{ 0.0f, 2.5f, -7.0f };
// 距離限界
const float DistanceLimit{ 30.0f };
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
	player_offset_ = NormalOffset;
}

// 更新
void CameraTPS::update(float delta_time) {
	// プレーヤーを検索
	find_actor("Player");
	state_machine_.update(delta_time);

	yaw_ = wrap_yaw(yaw_);

	// TODO:ロックオンの解除
	// ロックオン中は常に一番近い敵にロックオンするように動的にロックオンする
#ifndef DEBUG
	ImGui::Begin("NormalCameraDebug");
	ImGui::DragFloat3("ReferencePointOffset", ReferencePointOffset);
	ImGui::DragFloat3("PlayerOffset", player_offset_);
	ImGui::DragFloat3("camera_position", transform().position());
	ImGui::DragFloat3("velocity", velocity_);
	ImGui::DragFloat("pitch", &pitch_);
	ImGui::DragFloat("yaw", &yaw_);
	ImGui::Text("is_reset: %s", is_reset() ? "true" : "false");
	ImGui::Text("is_lock_on: %s", is_lock_on() ? "true" : "false");
	auto current_state_num = (State)state_machine_.get_current_state();
	ImGui::Text("Now State is %s", current_state_to_string(current_state_num));
	ImGui::End();
#endif // !DEBUG

#ifndef DEBUG
	ImGui::Begin("LockOnDebug");
	ImGui::DragFloat3("LockOnReferencePointOffset", LockOnReferencePointOffset);
	ImGui::DragFloat3("lock_on_offest", LockOnOffest);
	ImGui::End();
#endif;
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
	stick_controll_camera(input_lstick, delta_time, Sencitivity);
	// カメラの設定
	update_normal(player, delta_time);
}

void CameraTPS::stick_controll_camera(GSvector2 stick_input, float delta_time, float sensitivity)
{
	// スティックの入力がなければ何もしない
	if (stick_input.x == 0 && stick_input.y == 0) return;

	// 感度・時間でスケーリング
	yaw_ -= stick_input.x * sensitivity * delta_time;
	pitch_ -= stick_input.y * sensitivity * delta_time;
	const float pitchMin = -45.0f;
	const float pitchMax = 59.0f;
	pitch_ = std::clamp(pitch_, pitchMin, pitchMax);
}

// カメラ内に対象がいるか？
bool CameraTPS::is_actor_on_screen(Actor* target) {
	// カメラの正面ベクトルとカメラから敵のベクトルの内積を取得
	GSvector3 forward_vector = (transform_.forward()).normalized();
	// エネミーの座標を取得
	GSvector3 other_pos = target->transform().position();
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
void CameraTPS::reset_camera(float delta_time) {
	is_reset_ = false;
	//if (is_lock_on()) {
	//	is_lock_on_ = false;
	//	is_reset_ = true;
	//}

	//// リセット用変数
	//float target_yaw = 0.0f;
	//float target_pitch = 0.0f;

	//// 補間速度
	//float speed = 2.0f;
	//// 徐々にリセットしていく
	//yaw_ = target_yaw;
	//pitch_ = target_pitch;
	//if (abs(yaw_) <= 0.05 && abs(pitch_) <= 0.05) {
	//	is_reset_ = false;
	//}

	//// カメラ処理
	//Actor* player = get_player();
	//update_normal(player, delta_time);
}

// 
void CameraTPS::reset_target() {
	target_ = nullptr;
}

// アクターをロックオンする
void CameraTPS::lock_on_actor(const std::string& tag) {
	// 指定したタグを持つアクターを検索
	auto actors = world_->find_actor_with_tag(tag);
	// ターゲットとの距離
	float to_target_distance = FLT_MAX;
	// エネミーの配列の数だけforループ
	for (auto* actor : actors) {
		// エネミーが視界にいるか？
		if (!is_actor_on_screen(actor)) continue;
		is_lock_on_ = true;
		// 敵と自分との距離を計算する
		GSvector3 target_vector = transform_.position() - actor->transform().position();
		float mag = target_vector.magnitude();

		// TODO:ロックオン条件の見直し　距離だけだと真横に敵がいたら真横向くでな
		// 前回のエネミーとの距離の近さを比較する
		if (mag < to_target_distance) {
			// ターゲットを更新する
			to_target_distance = mag;
			target_ = actor;
		}
	}
}

void CameraTPS::find_actor(const std::string& player_str) {
	player_ = world_->find_actor(player_str);
}

Actor* CameraTPS::get_player() const {
	return player_;
}

Actor* CameraTPS::get_target() const {
	return target_;
}

// ステートを変更する
void CameraTPS::change_state(State state_num) {
	state_machine_.change_state((GSuint)state_num);
}

// ステートの追加
void CameraTPS::add_state() {
	state_machine_.add_state((GSuint)State::Normal, std::make_shared<CameraNormalState>(*this));
	state_machine_.add_state((GSuint)State::LockOn, std::make_shared<CameraLockOnState>(*this));
	state_machine_.add_state((GSuint)State::Reset, std::make_shared<CameraResetState>(*this));
}

// オイラー角を計算する
void CameraTPS::update_lockon(float delta_time) {
	// プレイヤーを取得
	Actor* player = get_player();
	// ロックオン対象の敵を取得
	Actor* target = get_target();
	if (target == nullptr || player == nullptr) return;
	// ヨーを設定する
	set_lock_on_yaw(player, target,delta_time);
	// ピッチを設定する
	set_lock_on_pitch(player, target, delta_time);

	// 注視点の位置を求める（プレーヤーの目線位置からターゲットに向く）
	GSvector3 at = player->transform().position() + LockOnReferencePointOffset;
	GSvector3 velocity = velocity_;
	// 視点の位置を求める（プレーヤーの背後の座標）
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0) * player_offset_;
	// フィールドとの衝突判定
	Line line{ at, position };
	GSvector3 intersect;
	// フィールドに衝突していたらカメラ座標をクランプする
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	// 視点の位置を設定
	transform_.position(position);
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
}

void CameraTPS::update_normal(Actor* player, float delta_time) {
	// 注視点の位置を求める（プレーヤーの頭部の少し上あたりの座標）
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	GSvector3 velocity = velocity_;
	// 視点の位置を求める（プレーヤーの背後の座標）
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0) * player_offset_;
	// フィールドとの衝突判定
	Line line{ at, position };
	GSvector3 intersect;
	// フィールドに衝突していたらカメラ座標をクランプする
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}
	// スムースダンプによる滑らかな補間
	position = GSvector3::smoothDamp(transform_.position(), position, velocity,
		smooth_time_, MaxSpeed, delta_time);
	// 視点の位置を設定
	transform_.position(position);
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
}

// ヨー・ピッチの値を参照する
GSvector3 CameraTPS::get_yaw_pitch() const {
	return GSvector3{ yaw_, pitch_, 0.0f};
}

// ロックオン時のヨーを設定
void CameraTPS::set_lock_on_yaw(Actor* player, Actor* target, float delta_time) {
	// プレイヤーと敵とのベクトルを取得
	GSvector3 target_to_player = player->transform().position() - target->transform().position();
	// 鉛直成分を考慮しない
	target_to_player.y = 0;
	// 正規化
	target_to_player = target_to_player.normalized();
	// プレイヤーの背後ベクトルを取得
	GSvector3 back = GSvector3::back();

	// 180°~-180°の境界の補間値を算出する
	float angle_diff = GSmathf::deltaAngle(yaw_, GSvector3::signedAngle(back, target_to_player));
	// 補間時間(秒)
	const float speed = 2.0f;
	// ヨーを設定
	yaw_ += angle_diff * (speed * delta_time / cREF);
}

// ロックオン時のピッチを設定
void CameraTPS::set_lock_on_pitch(Actor* player, Actor* target, float delta_time) {
	// プレイヤーと敵とのベクトルを取得
	GSvector3 target_to_player = player->transform().position() - target->transform().position();
	// 水平距離を求める
	float horizontal = std::sqrt(target_to_player.x * target_to_player.x + target_to_player.z * target_to_player.z);
	// 角度差を求める
	float target_pitch = rad2deg(atan2(target_to_player.y, horizontal));
	// 差分
	float diff = target_pitch - pitch_;
	// 補間
	const float speed = 6.0f;
	pitch_ += diff * (speed * delta_time / cREF);
	// 可動域
	pitch_ = std::clamp(pitch_, -45.0f, 59.0f);
}

bool CameraTPS::is_lock_on() const {
	return is_lock_on_;
}

bool CameraTPS::is_reset() const {
	return is_reset_;
}

float CameraTPS::get_senc() const {
	return Sencitivity;
}

float CameraTPS::rad2deg(float rad)
{
	return rad * 180.0f / 3.1415926535f;
}

float CameraTPS::forward_angle_diff(GSvector3 target_vector)
{
	// 正面ベクトルと正規化された方向ベクトルを設定
	GSvector3 forward = transform().position().forward();
	forward.y = 0;
	target_vector.y = 0;
	target_vector.normalized();
	return abs(GSvector3::signedAngle(forward, target_vector));
}

// 現在のステートを文字列に変換
const char* CameraTPS::current_state_to_string(State state) {
	switch (state)
	{
	case CameraState::Null:			return "Null";
	case CameraState::Normal:       return "Normal";
	case CameraState::LockOn:		return "LockOn";
	case CameraState::Reset:		return "Reset";
	default:                        return "None";
	}
}

float CameraTPS::wrap_yaw(float yaw)
{
	while (yaw > 180.0f)  yaw -= 360.0f;
	while (yaw < -180.0f) yaw += 360.0f;
	return yaw;
}

GSvector3 CameraTPS::set_offset(State& state) {
	switch (state) {
	case CameraState::Normal: return NormalOffset;
	case CameraState::LockOn: return LockOnOffest;
	default:				  return NormalOffset;
	}
};