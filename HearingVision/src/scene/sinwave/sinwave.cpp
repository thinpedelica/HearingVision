#include "sinwave.h"

//--------------------------------------------------------------
SinWaveScene::SinWaveScene() {
    // nop
}

//--------------------------------------------------------------
SinWaveScene::~SinWaveScene() {
    // nop
}

//--------------------------------------------------------------
void SinWaveScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    shader_.load("shader/sinwave/draw");
}

//--------------------------------------------------------------
void SinWaveScene::update(SceneParam scene_param) {
    time_ += 0.01;

    scale_ = kBaseScale + kMaxScale * scene_param.threshold_;
    waves_ = kBaseWaves + kBaseWaves * scene_param.threshold_;
    speed_ = kBaseSpeed + kMaxSpeed * scene_param.level_;

    float hue =128.f;
    if (scene_param.color_ > 0.f) {
        hue = ofMap(scene_param.color_, 0.f, 1.f, 128.f, 230.f);
    }
    ofColor c;
    c.setHsb(hue, 128.f, 128.f);
    r_ = static_cast<float>(c.r) / 255.f;
    g_ = static_cast<float>(c.g) / 255.f;
    b_ = static_cast<float>(c.b) / 255.f;

}

//--------------------------------------------------------------
void SinWaveScene::draw() {

    shader_.begin();
    shader_.setUniform1f("time",  time_);
    shader_.setUniform1f("scale", scale_);
    shader_.setUniform1f("waves", waves_);
    shader_.setUniform1f("speed", speed_);
    shader_.setUniform3f("color", r_, g_, b_);
    shader_.setUniform2f("resolution", win_cache_->getWidth(), win_cache_->getHeight());
    ofDrawRectangle(0, 0, win_cache_->getWidth(), win_cache_->getHeight());
    shader_.end();
}

//--------------------------------------------------------------
