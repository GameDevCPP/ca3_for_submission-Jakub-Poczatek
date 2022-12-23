#pragma once
#include <json.h>
#include <fstream>

using Json = nlohmann::json;

struct JsonData{
    static Json playerData;
    static Json generalData;
    static Json pickups;
    static Json enemies;
    static Json audio;

    static void start(){
        std::ifstream f;
        f.open("../../res/json/player.json");
        playerData = Json::parse(f);
        f.close();

        f.open("../../res/json/general.json");
        generalData = Json::parse(f);
        f.close();

        f.open("../../res/json/pickups.json");
        pickups = Json::parse(f);
        f.close();

        f.open("../../res/json/enemies.json");
        enemies = Json::parse(f);
        f.close();

        f.open("../../res/json/audio.json");
        audio = Json::parse(f);
        f.close();
    }
};