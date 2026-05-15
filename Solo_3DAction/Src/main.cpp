#include <GSGameEx.h>
#include "Scene/SceneManager.h"
#include "Scene/GamePlayScene.h"
#include "Scene/LoadGameScene.h"
#include "GameConfig.h"
#include "Input.h"
#include <GSstandard_shader.h>
#include <GSEffect.h>

class MyGame : public gslib::Game {
public:
    // コンストラクタ
    MyGame(int screen_width, int screen_height, bool enable_full_screen, float target_refresh_rate)
        : gslib::Game{ screen_width, screen_height, enable_full_screen, target_refresh_rate } {
    }
    // 開始
    void start() override {
        // デフォルトシェーダーを初期化
        gsInitDefaultShader();
        // エフェクトの初期化
        gsInitEffect();

        scene_manager_.add("LoadGamePlayScene", new LoadGameScene());
        scene_manager_.add("GamePlayScene", new GamePlayScene());
        scene_manager_.change("LoadGamePlayScene");
    }
    // 更新
    void update(float delta_time) override {
        scene_manager_.update(delta_time);
        input_.update(delta_time);
    }
    // 描画
    void draw() override {
        scene_manager_.draw();
    }
    // 終了
    void end() override {
        scene_manager_.end();
        scene_manager_.clear();

        // エフェクトの終了
        gsFinishEffect();
    }
private:
    // シーンマネージャー
    SceneManager scene_manager_;
    // 入力
    Input& input_ = Input::get_instance();

};

int main() {
#ifdef _DEBUG
    return MyGame(1920, 1080, false, cREF).run();
#else
    return MyGame(1920, 1080, true, cREF).run();
#endif
}