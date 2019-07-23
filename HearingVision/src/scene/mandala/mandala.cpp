#include "mandala.h"

//--------------------------------------------------------------
MandalaScene::MandalaScene() {
    // nop
}
//--------------------------------------------------------------
MandalaScene::~MandalaScene() {
    // nop
}
//--------------------------------------------------------------
void MandalaScene::setup(std::shared_ptr<ProcessFFT> pfft,
                         std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;
    setupNextScene();

    mandala_drawer_.setup(win_cache_);
}

//--------------------------------------------------------------
void MandalaScene::update(SceneParam scene_param) {
    float base_color      = scene_param.color_ * 255.f;
    float brightness_min  = 32.f  + pfft_->getLowVal() * 128.f;
    float brightness_max  = 128.f + pfft_->getMidVal() * 256.f;
    float stroke_weight   = 0.2f  + pfft_->getLowVal() * 2.0f;
    float circle_div_base = 4.0f  + pfft_->getMidVal() * 16.0f + scene_param.level_ * 16.0f;
    mandala_drawer_.update(base_color,
                           brightness_min,
                           brightness_max,
                           stroke_weight,
                           circle_div_base);
}

//--------------------------------------------------------------
void MandalaScene::draw() {
    mandala_drawer_.draw();
}

//--------------------------------------------------------------
void MandalaScene::setupNextScene() {
    NextScene::NextSceneList next_scene_list;
    next_scene_list.push_back("C");
    next_scene_list.push_back("E3");
    next_scene_list.push_back("Q");

    next_scene_.setNextSceneList(next_scene_list);
}

//--------------------------------------------------------------
