#include "psychedelica.h"

#include <numeric>

//--------------------------------------------------------------
PsychedelicaScene::PsychedelicaScene() {
    // nop
}

//--------------------------------------------------------------
PsychedelicaScene::~PsychedelicaScene() {
    // nop
}

//--------------------------------------------------------------
void PsychedelicaScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    shader_.load("shader/psychedelica/draw");
}

//--------------------------------------------------------------
void PsychedelicaScene::update(SceneParam scene_param) {
    time_ += 0.01;

    high_.push_back(pfft_->getHighVal());
    mid_.push_back(pfft_->getMidVal());
    low_.push_back(pfft_->getLowVal());
    if (high_.size() > kHistoryMax) {
        high_.erase(high_.begin(), high_.begin() + 1);
        mid_.erase(mid_.begin(), mid_.begin() + 1);
        low_.erase(low_.begin(), low_.begin() + 1);
    }

    if (scene_param.level_ > 0.1f) {
        level_ = kLevelMax * scene_param.level_;
    } else {
        level_ = kLevelMin;
    }
}

//--------------------------------------------------------------
void PsychedelicaScene::draw() {

    shader_.begin();
    shader_.setUniform1f("time",  time_);
    shader_.setUniform1f("level", level_);
    shader_.setUniform1f("high",  std::accumulate(high_.begin(), high_.end(), 0.f) / static_cast<float>(high_.size()));
    shader_.setUniform1f("mid",   std::accumulate(mid_.begin(),  mid_.end(), 0.f)  / static_cast<float>(mid_.size()));
    shader_.setUniform1f("low",   std::accumulate(low_.begin(),  low_.end(), 0.f)  / static_cast<float>(low_.size()));
    shader_.setUniform2f("resolution", win_cache_->getWidth(), win_cache_->getHeight());
    ofDrawRectangle(0, 0, win_cache_->getWidth(), win_cache_->getHeight());
    shader_.end();
}

//--------------------------------------------------------------
void PsychedelicaScene::reset() {
    // nop
}