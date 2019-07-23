#pragma once

#include <string>
#include <vector>

class NextScene {
public:
    using NextSceneList = std::vector<std::string>;

    NextScene() = default;
    ~NextScene() = default;

    void setNextSceneList(NextSceneList& next_scene_list);
    void draw();

private:
    NextSceneList next_scene_list_;
};