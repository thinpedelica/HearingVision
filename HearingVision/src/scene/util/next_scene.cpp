#include "next_scene.h"
#include "ofMain.h"

void NextScene::setNextSceneList(NextSceneList& next_scene_list) {
    next_scene_list_ = next_scene_list;
}

void NextScene::draw() {
    std::string result;
    for (const auto& s : next_scene_list_) {
        result += s;
        result += ", ";
    }
    ofSetWindowTitle(result);
}
