#include "circle_drawer.h"

//--------------------------------------------------------------
void MovingCircleDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;
}

//--------------------------------------------------------------
void MovingCircleDrawer::update() {
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

//--------------------------------------------------------------
void MovingCircleDrawer::draw() {
    ofSetColor(255, 64);
    ofSetLineWidth(2.f);
    ofNoFill();
    for (auto& circle : circle_list_) {
        ofDrawCircle(circle.pos_, circle.radius_);
    }
}

//--------------------------------------------------------------
void MovingCircleDrawer::addCircle(const std::vector<float>& spectrum,
                                   const float level) {
    float sampling_step = 10.f - (9.f * level);
    ofColor color(255, 64);
    for (size_t i = 0; i < spectrum.size(); i += sampling_step) {
        if (spectrum.at(i) > 0.2) {
            float speed_direct = 1.0;
            if (ofRandomf() > 0.5) speed_direct = -1.0;
            Circle circle(ofVec2f(win_cache_->getWidth() * 0.5,
                                  win_cache_->getHeight() * 0.75 + ofRandomf() * 100.f),
                          ofMap(spectrum.at(i), 0.1f, 0.5f, 50.f, 200.f),
                          10.f * spectrum.at(i) * speed_direct,
                          color);
            circle_list_.emplace_back(circle);
        }
    }
}

//--------------------------------------------------------------
bool MovingCircleDrawer::isOutOfWindow(const Circle& circle) const {
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
void VanishingCircleDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;
}

//--------------------------------------------------------------
void VanishingCircleDrawer::update() {
    // TODO
}

//--------------------------------------------------------------
void VanishingCircleDrawer::draw() {
    // TODO
}

//--------------------------------------------------------------
void VanishingCircleDrawer::addCircle(const std::vector<float>& spectrum,
                                      const float level) {
    // TODO
}

