#include "LoadGameScene.h"
#include <GSgameEx.h>
#include "Assets.h"

#include <GSstandard_shader.h>

const int TOTAL_ASSET_NUM{ 11 };
void LoadGameScene::start()
{
	is_end_ = false;
	loaded_count_ = 0;
	// ロードを別スレッドで開始
	gslib::Game::run_thread([=] {load(); });
}

void LoadGameScene::update(float delta_time)
{
}

void LoadGameScene::draw() const
{
	// ロードの進捗状況を画面に表示
	gsTextPos(10, 10);
	gsDrawText("ロード状況：%d/%d", loaded_count_, TOTAL_ASSET_NUM);
}

bool LoadGameScene::is_end() const
{
	return is_end_;
}

std::string LoadGameScene::next() const
{
	return "GamePlayScene";
}

void LoadGameScene::end()
{
}

void LoadGameScene::load()
{
    // プレイヤーメッシュの読み込み
    gsLoadSkinMesh(Mesh_Player, "Assets/Player/Player.mshb");                                       ++loaded_count_;
    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Cannon, "Assets/Enemy/Enemy_Cannon/Enemy_Cannon.mshb");                     ++loaded_count_;
    gsLoadSkinMesh(Mesh_Cannon, "Assets/Enemy/Enemy_Cannon/Enemy_Cannon_Body.mshb");                ++loaded_count_;
    gsLoadSkinMesh(Mesh_Drone, "Assets/Enemy/Enemy_Drone/Enemy_Drone.mshb");                        ++loaded_count_;
    gsLoadSkinMesh(Mesh_Robot, "Assets/Enemy/Enemy_Robot/Enemy_Robot.mshb");                        ++loaded_count_;
    gsLoadMesh(Mesh_Helicopter, "Assets/Enemy/Enemy_Helicoptor/Enemy_Helicoptor_Body.mshb");        ++loaded_count_;
    gsLoadMesh(Mesh_Heil_Prop, "Assets/Enemy/Enemy_Helicoptor/Enemy_Heilcoptor_Propeller.mshb");    ++loaded_count_;
    // 弾メッシュの読み込み
    gsLoadMesh(Mesh_Bullet, "Assets/Bullet/Bullet.mshb");                                           ++loaded_count_;
    // スカイボックス用テクスチャの読み込み
    gsLoadTexture(Texture_Skybox, "Assets/Skybox/default_skybox.dds");                              ++loaded_count_;
    // ステージオクトリーの読み込み  
    gsLoadOctree(Octree_Stage1, "Assets/Stage/Stage1/Octree/Stage1_Octree.oct");                ++loaded_count_;
    // ステージ衝突判定用オクトリーの読み込み
    gsLoadOctree(Octree_Stage1_Collider, "Assets/Stage/Stage1/Collider/Stage1_Collider.oct");   ++loaded_count_;
	// シーン終了フラグON
	is_end_ = true;
}
