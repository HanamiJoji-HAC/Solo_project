#include "GamePlayScene.h"
#include "Actor/Player/Player.h"
#include "Actor/Enemy/Enemy.h"
#include "World/Field.h"
#include "Light/Light.h"
#include "Camera/CameraTPS.h"
#include "Assets.h"
#include <GSstandard_shader.h>
// 開始
void GamePlayScene::start() {
    // シーンフラグをオフにする
    is_end_ = false;
    gsInitDefaultShader();
    // プレイヤーメッシュの読み込み
    gsLoadSkinMesh(Mesh_Player, "Assets/Player/Player.mshb");
    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Enemy, "Assets/Enemy/ghoul.msh");
    // 弾メッシュの読み込み
    gsLoadSkinMesh(Mesh_Bullet, "Assets/Bullet/Bullet.msh");
    // スカイボックス用テクスチャの読み込み
    gsLoadTexture(Texture_Skybox, "Assets/Skybox/default_skybox.dds");
    // ステージオクトリーの読み込み
    gsLoadOctree(Octree_Test_Stage, "Assets/Octree/Octree.oct");
    // ステージ衝突判定用オクトリーの読み込み
    gsLoadOctree(Octree_Test_Stage_Collider, "Assets/Octree/Collider.oct");
    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Test_Stage, Octree_Test_Stage_Collider, Texture_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{
              &world_, GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });
    Status player_status;
    // プレーヤーを追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 1.0f, 0.0f }, player_status });
    world_.add_actor(new Enemy{ &world_, GSvector3{ 0.0f, 1.0f, 10.0f } });
}

// 更新
void GamePlayScene::update(float delta_time) {
    world_.update(delta_time);
    if (gsGetKeyTrigger(GKEY_C)) {
        is_end_ = true;
    }
}

// 描画
void GamePlayScene::draw() const {
    gsDrawSkyboxCubemap(Texture_Skybox);
    world_.draw();
}

// 終了しているか？
bool GamePlayScene::is_end() const {
    return is_end_;
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
    return "GamePlayScene";
}

// 終了
void GamePlayScene::end() {
    // ワールドを消去
    world_.clear();
}