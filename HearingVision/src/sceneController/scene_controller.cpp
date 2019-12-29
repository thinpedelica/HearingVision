#include "scene_controller.h"

const std::string SceneController::kLabelScene = "/Scene";

//--------------------------------------------------------------
SceneController::SceneController() {
    scene_ids_.resize(2);
}

//--------------------------------------------------------------
SceneController::~SceneController() {
    exit();
}

//--------------------------------------------------------------
void SceneController::setup() {
    receiver_.setup(kListenPort);
    setupKeyMap();
}

void SceneController::setupKeyMap() {
    int key = 'a';
    for (int i = 0; i < 26; ++i) {
        key_map_[i] = key;
        ++key;
    }
}

//--------------------------------------------------------------
void SceneController::update() {
    updateOscMessage();
}

//--------------------------------------------------------------
void SceneController::updateOscMessage() {
    while (receiver_.hasWaitingMessages()) {
        ofxOscMessage msg;
        receiver_.getNextMessage(msg);
        std::string addr = msg.getAddress();
        if (addr == kLabelScene) {
            int scene_index = msg.getArgAsInt32(0);
            int key_index   = msg.getArgAsInt32(1);
            setScene(scene_index, key_index);
        }
    }
}

void SceneController::setScene(const int scene_index, const int key_index) {
    if (scene_index >= scene_ids_.size()) {
        return;
    }
    if (key_index >= key_map_.size()) {
        return;
    }

    scene_ids_.at(scene_index) = key_map_.at(key_index);
}

//--------------------------------------------------------------
int SceneController::get(const size_t index) const {
    if (index >= scene_ids_.size()) {
        return 0;
    }

    return scene_ids_.at(index);
}

//--------------------------------------------------------------
void SceneController::exit() {
    // nop
}
