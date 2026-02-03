#ifndef LOAD_JSON_H_
#define LOAD_JSON_H_

#include <fstream>
#include <iostream>
#include "Actor/Character.h"

// 外部Jsonツールの参照
#include "External/json.hpp"
using json = nlohmann::json;
// Jsonの読み込みに関連する処理を保持しているクラス
class LoadJson {
public:
	// デフォルトコンストラクタ
	LoadJson() = default;
	// デストラクタ
	~LoadJson() = default;

	// インスタンスを取得
	static LoadJson& get_instance();

	// ステータス情報を取得する
	Status lode_status(const std::string& file_path, const std::string& key);
	// 目的座標情報を取得する
	std::vector<GSvector3> lode_way_points(const std::string& file_path, const std::string& key);

private:

public:
	// コピー禁止
	LoadJson(const LoadJson&) = delete;
	LoadJson& operator = (const LoadJson&) = delete;
};

#endif