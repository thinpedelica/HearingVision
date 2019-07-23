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
    setupNextScene();

    shader_.resize(5);
    shader_.at(0).load("shader/psychedelica/crystal/draw");
    shader_.at(1).load("shader/psychedelica/mandala/draw");
    shader_.at(2).load("shader/psychedelica/voronoi/draw");
    shader_.at(3).load("shader/psychedelica/cocoon/draw");
    shader_.at(4).load("shader/psychedelica/tunnel/draw");
}

//--------------------------------------------------------------
void PsychedelicaScene::update(SceneParam scene_param) {
    time_ += 0.01;
    level_ = scene_param.level_;
    color_ = scene_param.color_;

    high_.push_back(pfft_->getHighVal());
    mid_.push_back(pfft_->getMidVal());
    low_.push_back(pfft_->getLowVal());

    if (scene_param.threshold_ > 0.1f) {
        history_size_ = kHistoryMax * scene_param.threshold_;
    } else {
        history_size_ = kHistoryMin;
    }

    if (high_.size() > history_size_) {
        high_.erase(high_.begin(), high_.begin() + 1);
        mid_.erase(mid_.begin(), mid_.begin() + 1);
        low_.erase(low_.begin(), low_.begin() + 1);
    }

    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        ++shader_index_;
        if (shader_index_ == shader_.size()) {
            shader_index_ = 0;
        }
    }
}

//--------------------------------------------------------------
void PsychedelicaScene::draw() {

    shader_.at(shader_index_).begin();
    shader_.at(shader_index_).setUniform1f("time",  time_);
    shader_.at(shader_index_).setUniform1f("level", level_);
    shader_.at(shader_index_).setUniform1f("color", color_);
    shader_.at(shader_index_).setUniform1f("high",  std::accumulate(high_.begin(), high_.end(), 0.f) / static_cast<float>(high_.size()));
    shader_.at(shader_index_).setUniform1f("mid",   std::accumulate(mid_.begin(),  mid_.end(), 0.f)  / static_cast<float>(mid_.size()));
    shader_.at(shader_index_).setUniform1f("low",   std::accumulate(low_.begin(),  low_.end(), 0.f)  / static_cast<float>(low_.size()));
    shader_.at(shader_index_).setUniform2f("resolution", win_cache_->getWidth(), win_cache_->getHeight());
    ofDrawRectangle(0, 0, win_cache_->getWidth(), win_cache_->getHeight());
    shader_.at(shader_index_).end();
}

//--------------------------------------------------------------
void PsychedelicaScene::setupNextScene() {
    NextScene::NextSceneList next_scene_list;
    next_scene_list.push_back("E1");
    next_scene_list.push_back("E3");

    next_scene_list.push_back("/");

    next_scene_list.push_back("/");

    next_scene_list.push_back("/");
    next_scene_list.push_back("C");

    next_scene_list.push_back("/");
    next_scene_list.push_back("C");
    next_scene_list.push_back("I1");
    next_scene_list.push_back("I2");

    next_scene_.setNextSceneList(next_scene_list);
}
