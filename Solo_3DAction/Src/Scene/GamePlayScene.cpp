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
#include "Actor/Bullet/BulletInfo.h"

// 開始
void GamePlayScene::start() {
    // シーンフラグをオフにする
    is_end_ = false;

    // インダイレクトメッシュの読み込み
    gsLoadIndirectMesh(0, "Assets/Stage/Stage1/Ind/Stage1.ind");

    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Stage1, Octree_Stage1_Collider, Texture_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{
              &world_, GSvector3{ 0.0f, 2.0f, 4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });

    // プレーヤーを追加
    Character::Status player_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "Player");
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f }, player_status });
    // 大砲
    Character::Status enemy_cannon_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyCannon");
    world_.add_actor(new EnemyCannon{ &world_, GSvector3{ 0.0f, 0.0f, 10.0f }, enemy_cannon_status});
    // ドローン
    Character::Status enemy_drone_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyDrone");
    world_.add_actor(new EnemyDrone{ &world_, GSvector3{ 10.0f, 2.0f, 10.0f }, enemy_drone_status });
    // ロボット
    Character::Status enemy_robot_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyRobot");
    world_.add_actor(new EnemyRobot{ &world_, GSvector3{ 20.0f, 0.0f, 10.0f }, enemy_robot_status });
    // ヘリコプター
    Character::Status enemy_heil_status = json_.lode_status("Assets/Json/ActorStatusLoder.Json", "EnemyHeil");
    std::vector<GSvector3> way_points = json_.lode_way_points("Assets/Json/WayPointLoder.Json", "WayPoints01");
    world_.add_actor(new EnemyHeilcoptor{ &world_, GSvector3{ 0.0f, 5.0f, -75.0f }, enemy_heil_status, way_points});

    //bullet_status_.push_back(json_.lode_bullet_status("Assets/Json/BulletStatusLoader.json", "NormalBullet"));
    BulletInfo& bullet_info = BulletInfo::get_instance();
    bullet_info.load();
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
    // スカイボックスの描画
    gsDrawSkyboxCubemap(Texture_Skybox);
    // ワールドの描画
    world_.draw();
    // LODの更新
    gsUpdateIndirectMeshLod(0);
    // インダイレクトメッシュの描画
    gsDrawIndirectMesh(0);
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
    // インダイレクトメッシュの削除
    gsDeleteIndirectMesh(0);
    // ワールドを消去
    world_.clear();
}

// 弾情報を返却する
//const Bullet_Status& GamePlayScene::get_bullet_status(int bullet_id) const
//{
//    return bullet_status_[bullet_id];
//}
