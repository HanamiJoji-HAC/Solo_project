#include "LoadJson.h"

LoadJson& LoadJson::get_instance()
{
    static LoadJson self;
    return self;
}

Status LoadJson::lode_status(const std::string& file_path, const std::string& key)
{
    std::ifstream file(file_path);
    // ファイルパスが存在しない場合は終了
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << file_path << std::endl;
        return{};
    }

    json j;
    file >> j;
    // キーが存在しない場合は終了
    if (!j.contains(key)) {
        std::cerr << "Invalid waypoint key: " << key << std::endl;
        return {};
    }

    Status status;
    status.hp_ = j[key].value("hp_", 10);
    status.energy_ = j[key].value("energy_", 0.0f);
    status.max_energy_ = j[key].value("max_energy_", 0.0f);
    status.melee_atk_ = j[key].value("melee_atk_", 0);
    status.ranged_atk_ = j[key].value("ranged_atk_", 0);
    status.jump_power_ = j[key].value("jump_power_", 0.0f);
    status.boost_speed_ = j[key].value("boost_speed_", 0.0f);
    status.walk_speed_ = j[key].value("walk_speed_", 0.0f);
    status.move_speed_ = j[key].value("move_speed_", 0.0f);
    status.gravity_ = j[key].value("gravity_", 0.0f);
    status.invisible_timer_ = j[key].value("invisible_timer_", 0.0f);
    status.default_inbisible_timer_ = j[key].value("default_inbisible_timer_", 0.0f);
    status.air_move_speed_ = j[key].value("air_move_speed_", 0.0f);
    status.max_boost_speed_ = j[key].value("max_boost_speed_", 0.0f);
    status.quick_boost_speed_ = j[key].value("quick_boost_speed_", 0.0f);
    return status;
}

std::vector<GSvector3> LoadJson::lode_way_points(const std::string& file_path, const std::string& key)
{
    std::ifstream file(file_path);
    // ファイルパスが存在しない場合は終了
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << file_path << std::endl;
        return{};
    }

    json j;
    file >> j;

    std::vector<GSvector3> way_points;
    // キーが存在しない、配列でない時は終了
    if (!j.contains(key) || !j[key].is_array()) {
        std::cerr << "Invalid waypoint key: " << key << std::endl;
        return {};
    }

    for (const auto& point : j[key]) {
        // 配列でない、要素数が3でないときは未処理
        if (!point.is_array() || point.size() != 3) {
            std::cerr << "Invalid waypoint format" << std::endl;
            continue;
        }
        // GSfloat型にキャストする
        GSvector3 way_point;
        way_point.x = point[0].get<GSfloat>();
        way_point.y = point[1].get<GSfloat>();
        way_point.z = point[2].get<GSfloat>();

        way_points.push_back(way_point);
    }
    return way_points;
}
