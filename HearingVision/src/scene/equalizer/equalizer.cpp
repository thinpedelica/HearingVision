#include "equalizer.h"

//--------------------------------------------------------------
EqualizerScene::EqualizerScene() {
    // nop
}

//--------------------------------------------------------------
EqualizerScene::~EqualizerScene() {
    // nop
}

//--------------------------------------------------------------
void EqualizerScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    drawer_prt_list_.push_back(std::make_unique<CircularEqualizerDrawer>());
    drawer_prt_list_.push_back(std::make_unique<BoxEqualizerDrawer>());
    //drawer_prt_list_.push_back(std::make_unique<GridEqualizerDrawer>());
    drawer_prt_list_.push_back(std::make_unique<CylinderEqualizerDrawer>());
    for (auto& drawer : drawer_prt_list_) {
        drawer->setup(win_cache);
    }
}

//--------------------------------------------------------------
void EqualizerScene::update(SceneParam scene_param) {
    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        ++draw_list_index_;
        if (draw_list_index_ == drawer_prt_list_.size()) {
            draw_list_index_ = 0;
        }
    }

    drawer_prt_list_.at(draw_list_index_)->setColor(scene_param.color_);
    drawer_prt_list_.at(draw_list_index_)->setLevel(scene_param.level_);

    std::vector<float> spectrum = pfft_->getSpectrum();
    drawer_prt_list_.at(draw_list_index_)->update(spectrum);
}
//--------------------------------------------------------------
void EqualizerScene::draw() {
    drawer_prt_list_.at(draw_list_index_)->draw();
}

void EqualizerScene::resize() {
    for (auto& drawer : drawer_prt_list_) {
        drawer->resize();
    }
}
//--------------------------------------------------------------
