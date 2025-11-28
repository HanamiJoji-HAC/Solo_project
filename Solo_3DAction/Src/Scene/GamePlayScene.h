#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include "IScene.h"
#include "World/World.h"
#include "Actor/Charactor.h"

// ゲームプレイシーン
class GamePlayScene : public IScene {
public:
    // 開始
    virtual void start() override;
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 終了しているか？
    virtual bool is_end() const override;
    // 次のシーンを返す
    virtual std::string next() const override;
    // 終了
    virtual void end() override;
    // Jsonファイルからステータス情報を取得する
    Status lode_status_from_json(const std::string& file_path, const std::string& key);
private:
    // ワールドクラス
    World   world_;
    // シーンが終了しているか？
    bool is_end_{ false };
};

#endif
