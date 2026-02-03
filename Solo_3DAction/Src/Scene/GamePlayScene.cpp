#include "GamePlayScene.h"
#include "Assets.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Actor/Enemy/Enemies/Cannon/EnemyCannon.h"
#include "Actor/Enemy/Enemies/Drone/EnemyDrone.h"
#include "Actor/Enemy/Enemies/Robot/EnemyRobot.h"
#include "Actor/Enemy/Enemies/Helicopter/EnemyHeilcoptor.h"
#include "Camera/CameraTPS.h"
#include "Light/Light.h"
#include "World/Field.h"

#include <GSstandard_shader.h>

// 開始
void GamePlayScene::start() {
    // シーンフラグをオフにする
    is_end_ = false;
    gsInitDefaultShader();
    // プレイヤーメッシュの読み込み
    gsLoadSkinMesh(Mesh_Player, "Assets/Player/Player.mshb");
    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Cannon, "Assets/Enemy/Enemy_Cannon/Enemy_Cannon.mshb");
    gsLoadSkinMesh(Mesh_Cannon, "Assets/Enemy/Enemy_Cannon/Enemy_Cannon_Body.mshb");
    gsLoadSkinMesh(Mesh_Drone, "Assets/Enemy/Enemy_Drone/Enemy_Drone.mshb");
    gsLoadSkinMesh(Mesh_Robot, "Assets/Enemy/Enemy_Robot/Enemy_Robot.mshb");
    gsLoadMesh(Mesh_Helicopter, "Assets/Enemy/Enemy_Helicoptor/Enemy_Helicoptor_Body.mshb");
    gsLoadMesh(Mesh_Heil_Prop, "Assets/Enemy/Enemy_Helicoptor/Enemy_Heilcoptor_Propeller.mshb");
    // 弾メッシュの読み込み
    gsLoadMesh(Mesh_Bullet, "Assets/Bullet/Bullet.mshb");
    // スカイボックス用テクスチャの読み込み
    gsLoadTexture(Texture_Skybox, "Assets/Skybox/default_skybox.dds");
    // ステージオクトリーの読み込み
    gsLoadOctree(Octree_Test_Stage, "Assets/Stage/Stage1/Octree/Stage1_Octree.oct");
    // ステージ衝突判定用オクトリーの読み込み
    gsLoadOctree(Octree_Test_Stage_Collider, "Assets/Stage/Stage1/Collider/Stage1_Collider.oct");
    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Test_Stage, Octree_Test_Stage_Collider, Texture_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{
              &world_, GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });

    Status player_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "Player");
    // プレーヤーを追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 20.0f, -30.0f }, player_status });
    // エネミーを追加
    Status enemy_cannon_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyCannon");
    world_.add_actor(new EnemyCannon{ &world_, GSvector3{ 0.0f, 0.0f, 10.0f }, enemy_cannon_status});
    Status enemy_drone_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyDrone");
    world_.add_actor(new EnemyDrone{ &world_, GSvector3{ 10.0f, 2.0f, 10.0f }, enemy_drone_status });
    Status enemy_robot_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyRobot");
    world_.add_actor(new EnemyRobot{ &world_, GSvector3{ 20.0f, 0.0f, 10.0f }, enemy_robot_status });
    Status enemy_heil_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyHeil");
    std::vector<GSvector3> way_points = json_.lode_way_points("Assets/Json/WayPointLoder.Json", "WayPoints01");
    world_.add_actor(new EnemyHeilcoptor{ &world_, GSvector3{ 0.0f, 10.0f, -75.0f }, enemy_heil_status, way_points});
}

// 更新
void GamePlayScene::update(float delta_time) {
    world_.update(delta_time);
    // Todo:ゲームクリア条件に変更する(現状はプレイヤーまたは敵の全滅)
    if (world_.count_actor_with_tag("PlayerTag") <= 0 || world_.count_actor_with_tag("EnemyTag") <= 0) {
        is_end_ = true;
        return;
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
