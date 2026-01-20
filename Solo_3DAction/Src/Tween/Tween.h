#ifndef TWEEN_H_
#define TWEEN_H_

#include <list>
#include "TweenUnit.h"

// Tweenモーションを管理するstaticクラス
// 1つ1つの動きはTweenUnitクラスが行う。
// 本クラスはTweenモーションの管理を行う。

class Tween {
public:
	// TweenUnitを管理下にする
	static void add(TweenUnit* unit);
	// 管理ユニットの更新
	static void update(float delta_time);
	// 管理ユニットの削除
	static void clear();
	// float型の値をTweenする
	static TweenUnit& value(float from, float to, float duration, std::function<void(float)>update_func);
	// vector3型の値をTweenする
	static TweenUnit& vector3(const GSvector3& from, const GSvector3& to, float duration, std::function<void(const GSvector3&)>update_func);
	// 指定時間後に処理を実行する
	static TweenUnit& dalay_call(float delta_time, std::function<void()> callback);
	// 指定された名前のTweenをキャンセル(削除)する
	static TweenUnit& cancel(const std::string& name);
private:
	// インスタンス生成禁止
	Tween() = delete;
	// リスト
	static std::list<TweenUnit*> units_;
};

#endif