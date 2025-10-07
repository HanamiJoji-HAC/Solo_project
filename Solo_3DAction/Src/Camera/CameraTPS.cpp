#include "CameraTPS.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "Math/Line.h"
#include "imgui/imgui.h"

// プレーヤーからの相対座標
GSvector3 PlayerOffset{ 0.0f, 3.0f, -5.0f };
// カメラの注視点の補正値
GSvector3 ReferencePointOffset{ 0.0f, 1.0f, 4.0f };
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
	ImGui::Begin("camera");
	ImGui::DragFloat3("camera", ReferencePointOffset);
	ImGui::End();
	//GSvector2 camera_input = input_.get_right_stick_input();
	//velocity_.x = camera_input.x;
	//velocity_.y = camera_input.y;
	//GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	//velocity += camera_input * delta_time;
	//transform_.position(velocity);
	//if (!gsGetKeyState(GKEY_P)) return;
	// プレーヤーを検索
	Actor* player = world_->find_actor("Player");
	if (player == nullptr) return;
	// 視点の位置を求める（プレーヤーの背後の座標）
	GSvector3 position = PlayerOffset * player->transform().localToWorldMatrix();
	// 注視点の位置を求める（プレーヤーの頭部の少し上あたりの座標）
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	// フィールドとの衝突判定
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}
	// スムースダンプによる滑らかな補間
	const float SmoothTime{ 3.0f }; // 補間フレーム数
	const float MaxSpeed{ 20.0f }; // 移動スピードの最大値
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);

	// 視点の位置を設定
	transform_.position(position);
	// 注視点を設定（注視点の方向に向きを変える）
	transform_.lookAt(at);
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