#include "box.h"

//--------------------------------------------------------------
BoxScene::BoxScene() {
    // nop
}

//--------------------------------------------------------------
BoxScene::~BoxScene() {
    // nop
}

//--------------------------------------------------------------
void BoxScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    drawer_prt_list_.push_back(std::make_unique<RollingBoxDrawer>());
    drawer_prt_list_.push_back(std::make_unique<EmptyBoxDrawer>());
    for (auto& drawer : drawer_prt_list_) {
        drawer->initialize(win_cache, pfft);
        drawer->setup();
    }
}

//--------------------------------------------------------------
void BoxScene::update(SceneParam scene_param) {
    if (scene_param.reset_ == SceneParam::TriggerState::kOn) {
        drawer_prt_list_.at(draw_list_index_)->reset();
    }

    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        ++draw_list_index_;
        if (draw_list_index_ == drawer_prt_list_.size()) {
            draw_list_index_ = 0;
        }
    }

    drawer_prt_list_.at(draw_list_index_)->setColor(scene_param.color_);
    drawer_prt_list_.at(draw_list_index_)->setLevel(scene_param.level_);
    drawer_prt_list_.at(draw_list_index_)->setThreshold(scene_param.gain_);
    drawer_prt_list_.at(draw_list_index_)->update();
}

//--------------------------------------------------------------
void BoxScene::draw() {
    drawer_prt_list_.at(draw_list_index_)->draw();
}

//--------------------------------------------------------------
NextScene& BoxScene::getNextScene() {
    return drawer_prt_list_.at(draw_list_index_)->getNextScene();
}
