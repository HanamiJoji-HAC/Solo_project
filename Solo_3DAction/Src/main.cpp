#include <GSGame.h>
#include "Scene/SceneManager.h"
#include "Scene/GamePlayScene.h"
class MyGame : public gslib::Game {
public:
    // コンストラクタ
    MyGame() : gslib::Game{ 1920, 1080 } {
    }
    // 開始
    void start() override {
        scene_manager_.add("GamePlayScene", new GamePlayScene());
        scene_manager_.change("GamePlayScene");
    }
    // 更新
    void update(float delta_time) override {
        scene_manager_.update(delta_time);
    }
    // 描画
    void draw() override {
        scene_manager_.draw();
    }
    // 終了
    void end() override {
        scene_manager_.end();
        scene_manager_.clear();
    }
private:
    // シーンマネージャー
    SceneManager scene_manager_;
};

int main() {
	return MyGame().run();
}