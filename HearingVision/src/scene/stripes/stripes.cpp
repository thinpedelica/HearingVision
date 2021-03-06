#include "stripes.h"

//--------------------------------------------------------------
StripesScene::StripesScene() {
    // nop
}

//--------------------------------------------------------------
StripesScene::~StripesScene() {
    // nop
}

//--------------------------------------------------------------
void StripesScene::setup(std::shared_ptr<ProcessFFT> pfft,
                         std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;
    setupNextScene();

    std::shared_ptr<BloomLineDrawer> bloom_line_drawer_ptr = std::make_shared<BloomLineDrawer>();
    bloom_line_drawer_ptr->setup();

    drawer_prt_list_.push_back(std::make_unique<BarStripesDrawer>());
    drawer_prt_list_.push_back(std::make_unique<PointStripesDrawer>());
    drawer_prt_list_.push_back(std::make_unique<FractalStripesDrawer>());
    drawer_prt_list_.push_back(std::make_unique<HorizontalStripesDrawer>());
    for (auto& drawer : drawer_prt_list_) {
        drawer->initialize(win_cache, bloom_line_drawer_ptr);
        drawer->setup();
    }
}

//--------------------------------------------------------------
void StripesScene::update(SceneParam scene_param) {
    if (scene_param.reset_ == SceneParam::TriggerState::kOn) {
        drawer_prt_list_.at(draw_list_index_)->reset();
    }

    if (drawer_prt_list_.at(draw_list_index_)->isDrawEnd()) {
        ++draw_list_index_;
        if (draw_list_index_ == drawer_prt_list_.size()) {
            draw_list_index_ = 0;
        }
        drawer_prt_list_.at(draw_list_index_)->reset();
    }

    drawer_prt_list_.at(draw_list_index_)->setColor(scene_param.color_);
    drawer_prt_list_.at(draw_list_index_)->setLevel(scene_param.level_);
    drawer_prt_list_.at(draw_list_index_)->setThreshold(scene_param.gain_);

    std::vector<float> spectrum = pfft_->getSpectrum();
    drawer_prt_list_.at(draw_list_index_)->update(spectrum);
}

//--------------------------------------------------------------
void StripesScene::draw() {
    drawer_prt_list_.at(draw_list_index_)->draw();
}

void StripesScene::resize() {
    for (auto& drawer : drawer_prt_list_) {
        drawer->resize();
    }
}

//--------------------------------------------------------------
void StripesScene::setupNextScene() {
    NextScene::NextSceneList next_scene_list;
    next_scene_list.push_back("A");
    next_scene_list.push_back("B1");
    next_scene_list.push_back("B2");
    next_scene_list.push_back("G");
    next_scene_list.push_back("X");

    next_scene_.setNextSceneList(next_scene_list);
}
