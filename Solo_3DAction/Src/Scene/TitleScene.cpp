#include "TitleScene.h"

// 開始
void TitleScene::start() {
}

// 更新
void TitleScene::update(float delta_time) {
}

// 描画
void TitleScene::draw() const {
    world_.draw();
}

// 終了しているか？
bool TitleScene::is_end() const {
    return is_end_;
}

// 次のシーンを返す
std::string TitleScene::next() const {
    return "GamePlayScene";
}

// 終了
void TitleScene::end() {
    // ワールドを消去
    world_.clear();
}