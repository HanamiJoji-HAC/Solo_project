#include "GamePlayScene.h"
#include "Assets.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Player/Player.h"
#include "Camera/CameraTPS.h"
#include "Light/Light.h"
#include "World/Field.h"

#include <GSstandard_shader.h>

#include <iostream>
#include <fstream>

#include "External/json.hpp"
using json = nlohmann::json;

// 開始
void GamePlayScene::start() {
    // シーンフラグをオフにする
    is_end_ = false;
    gsInitDefaultShader();
    // プレイヤーメッシュの読み込み
    gsLoadSkinMesh(Mesh_Player, "Assets/Player/Player.mshb");
    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Enemy, "Assets/Enemy/Enemy_Cannon/Enemy_Cannon.mshb");

    // 弾メッシュの読み込み
    gsLoadSkinMesh(Mesh_Bullet, "Assets/Bullet/Bullet.msh");
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
    Status player_status = lode_status_from_json("Assets/Json/ActorStatusLoder.Json", "Player");
    // プレーヤーを追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 1.0f, 0.0f }, player_status });
    world_.add_actor(new Enemy{ &world_, GSvector3{ 0.0f, 0.0f, 10.0f } });
    world_.add_actor(new Enemy{ &world_, GSvector3{ 20.0f, 0.0f, 10.0f } });
    world_.add_actor(new Enemy{ &world_, GSvector3{ 40.0f, 0.0f, 10.0f } });

    
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
// Jsonファイルからステータス情報を取得する
Status GamePlayScene::lode_status_from_json(const std::string& file_path, const std::string& key) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << file_path << std::endl;
        return{};
    }

    json j;
    file >> j;

    Status status;
    status.hp_                      = j[key]["hp_"];
    status.energy_                  = j[key]["energy_"];
    status.max_energy_              = j[key]["max_energy_"];
    status.melee_atk_               = j[key]["melee_atk_"];
    status.ranged_atk_              = j[key]["ranged_atk_"];
    status.jump_power_              = j[key]["jump_power_"];
    status.boost_speed_             = j[key]["boost_speed_"];
    status.walk_speed_              = j[key]["walk_speed_"];
    status.move_speed_              = j[key]["move_speed_"];
    status.gravity_                 = j[key]["gravity_"];
    status.invisible_timer_         = j[key]["invisible_timer_"];
    status.default_inbisible_timer_ = j[key]["default_inbisible_timer_"];
    status.air_move_speed_          = j[key]["air_move_speed_"];
    status.max_boost_speed_         = j[key]["max_boost_speed_"];
    status.quick_boost_speed_       = j[key]["quick_boost_speed_"];
    return status;
}