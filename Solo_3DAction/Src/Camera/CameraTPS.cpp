#include "CameraTPS.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "Math/Line.h"
#include "imgui/imgui.h"
#include <algorithm>


const float MaxSpeed{ 20.0f }; // 移動スピードの最大値
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.0f, 0.0f };
// プレーヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f, 3.0f, -5.0f };
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
}

// 更新
void CameraTPS::update(float delta_time) {

	// プレーヤーを検索
	Actor* player = get_player();

	if (player == nullptr) return;
#ifndef DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("look_point", ReferencePointOffset);
	ImGui::DragFloat3("position", PlayerOffset);
	ImGui::End();
#endif // !DEBUG

	if (gsGetKeyTrigger(GKEY_I)) {
		start_debug = true;
		return;
	}

	if (start_debug) return;

	tpp_normal(player, delta_time);
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

Actor* CameraTPS::get_player() const {
	return world_->find_actor("Player");
}

void CameraTPS::tpp_normal(Actor* other, float delta_time) {
	input_.get_right_stick_input_angle();
	GSvector2 input_lstick = input_.get_right_stick_input_value();
	update_camera_rotation(input_lstick, delta_time, Sencitivity, pitch_, yaw_);

	// 注視点の位置を求める（プレーヤーの頭部の少し上あたりの座標）
	GSvector3 at = other->transform().position() + ReferencePointOffset;
	GSvector3 velocity = velocity_;
	// 視点の位置を求める（プレーヤーの背後の座標）
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0) * PlayerOffset;
	// フィールドとの衝突判定
	Line line{ at, position };
	GSvector3 intersect;
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

#ifndef DEBUG
	ImGui::Begin("tpp_normal");
	ImGui::DragFloat3("camera_position", position);
	ImGui::DragFloat3("velocity", velocity);
	ImGui::DragFloat("pitch", &pitch_);
	ImGui::DragFloat("yaw", &yaw_);
	ImGui::End();
#endif // !DEBUG
}

void CameraTPS::update_camera_rotation(GSvector2 stickInput, float delta_time, float sensitivity,
	float& pitch, float& yaw)
{
	// スティックの入力がなければ何もしない
	if (stickInput.x == 0 && stickInput.y == 0) return;

	// 感度・時間でスケーリング
	yaw -= stickInput.x * sensitivity * delta_time;
	pitch -= stickInput.y * sensitivity * delta_time;

	// pitch角を制限（上向き・下向きの限界）
	const float pitchMin = -45.0f;
	const float pitchMax = 59.0f;
	pitch = std::clamp(pitch, pitchMin, pitchMax);
}