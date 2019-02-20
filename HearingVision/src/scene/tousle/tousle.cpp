#include "tousle.h"


//--------------------------------------------------------------
TousleScene::TousleScene() {
    // nop
}

//--------------------------------------------------------------
TousleScene::~TousleScene() {
    // nop
}

//--------------------------------------------------------------
void TousleScene::setup(std::shared_ptr<ProcessFFT> pfft,
                        std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

}

//--------------------------------------------------------------
void TousleScene::update(SceneParam scene_param) {
    counter_.setThreshold(scene_param.threshold_);
    bool ret = counter_.update();
    if (ret) {
        addCircleList();
    }

    sampling_step_ = 10.f - (9.f * scene_param.level_);

    updateCircleList();
}

void TousleScene::addCircleList() {
    std::vector<float> values = pfft_->getSpectrum();
    for (size_t i = 0; i < values.size(); i += sampling_step_) {
        if (values.at(i) > 0.2) {
            float speed_direct = 1.0;
            if (ofRandomf() > 0.5) speed_direct = -1.0;
            Circle circle(ofVec2f(win_cache_->getWidth() * 0.5,
                                  win_cache_->getHeight() * 0.75 + ofRandomf() * 100.f),
                          ofMap(values.at(i), 0.1f, 0.5f, 50.f, 200.f),
                          10.f * values.at(i) * speed_direct);
            circle_list_.push_back(circle);
        }
    }
}

void TousleScene::updateCircleList() {
    for (auto& circle : circle_list_) {
        circle.pos_.x += circle.speed_;
        if (isOutOfWindow(circle)) {
            // delete
        }
    }

    auto itr = circle_list_.begin();
    while (itr != circle_list_.end()) {
        itr->pos_.x += itr->speed_;
        if (isOutOfWindow(*itr)) {
            itr = circle_list_.erase(itr);
        } else {
            ++itr;
        }
    }
}

bool TousleScene::isOutOfWindow(const Circle& circle) const {
    if (circle.speed_ > 0) {
        float x = circle.pos_.x - circle.radius_;
        if (x > win_cache_->getWidth()) {
            return true;
        }
    } else {
        float x = circle.pos_.x + circle.radius_;
        if (x < 0) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------
void TousleScene::draw() {
    drawCircleList();
}

void TousleScene::drawCircleList() {
    ofSetColor(255, 64);
    ofSetLineWidth(2.f);
    ofNoFill();
    for (auto& circle : circle_list_) {
        ofDrawCircle(circle.pos_, circle.radius_);
    }
}

//--------------------------------------------------------------
void TousleScene::reset() {
    // nop
}
