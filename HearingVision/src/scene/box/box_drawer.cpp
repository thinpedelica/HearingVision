#include <cmath>

#include "box_drawer.h"

//--------------------------------------------------------------
void BoxDrawer::initialize(std::shared_ptr<ofRectangle> win_cache,
                            std::shared_ptr<ProcessFFT> pfft) {
    win_cache_    = win_cache;
    pfft_         = pfft;
}

void BoxDrawer::reset() {
    // non
}

void BoxDrawer::resize() {
    // non
}

NextScene& BoxDrawer::getNextScene() {
    return next_scene_;
}

//--------------------------------------------------------------
void RollingBoxDrawer::setup() {
    setupNextScene();
}

void RollingBoxDrawer::update() {
    roll_cam_.update();

    counter_.setThreshold(threshold_);
    if (counter_.update()) {
        updateBoxSize();
        updateCamPos();
    }

    updateColor();
}

void RollingBoxDrawer::updateBoxSize() {
    boxes_.resize(kBoxNum);
    for (auto& box : boxes_) {
        float size = kBoxSizeBase * pfft_->getMidVal() * ofRandom(5.0, 10.0);
        box.setPosition(ofRandom(-win_cache_->getWidth() * 0.8, win_cache_->getWidth() * 0.8),
                        ofRandom(-win_cache_->getWidth() * 0.8, win_cache_->getWidth() * 0.8),
                        ofRandom(-win_cache_->getWidth() * 0.8, win_cache_->getWidth() * 0.8));
        box.setWidth(size);
        box.setHeight(size);
        box.setDepth(size);
    }
}

void RollingBoxDrawer::updateCamPos() {
    roll_cam_.setRandomScale(0.5, 2.0);
    roll_cam_.setRandomPos(270);
}

void RollingBoxDrawer::updateColor() {
    float input = ofMap(pfft_->getMidVal(), 0.0, 0.3, 200, 255, true);
    if (color_ < 0.1f) {
        box_color_.set(255.f, input);
    } else {
        float hue = color_ * 255.f;
        box_color_.setHsb(hue, input, input, input);
    }
}

void RollingBoxDrawer::draw() {
    roll_cam_.begin();

    ofSetColor(box_color_);
    ofSetLineWidth(ofMap(pfft_->getLowVal(), 0.0, 0.5, 1.0, 10.0, true));
    for (auto& box : boxes_) {
        box.drawWireframe();
    }

    roll_cam_.end();
}

//--------------------------------------------------------------
void RollingBoxDrawer::setupNextScene() {
    NextScene::NextSceneList next_scene_list;
    next_scene_list.push_back("I4");
    next_scene_list.push_back("I5");
    next_scene_list.push_back("Q");
    next_scene_list.push_back("S");
    next_scene_list.push_back("X");

    next_scene_.setNextSceneList(next_scene_list);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void EmptyBoxDrawer::setup() {
    setupNextScene();
}

void EmptyBoxDrawer::update() {
    // line, face alpha by fft

    updateColor();
    updateRotateAngle(level_);
    updateTranslateDistance(level_);
    updateBoxSize();
}

void EmptyBoxDrawer::updateColor() {
    color_.set(255.f, 200.f);
}

void EmptyBoxDrawer::updateRotateAngle(const float level) {
    float ratio   = 0.05f * (level + 0.5f);
    rotate_angle_ = std::fmod(ofGetElapsedTimeMillis() * ratio, 360.f);
}

void EmptyBoxDrawer::updateTranslateDistance(const float level) {
    float angle_deg = std::fmod(360.f * translate_count_, 360.f);
    float angle_rad = ofDegToRad(angle_deg);
    float distance_ratio = (sin(angle_rad) + 1.0f) * 0.5f;

    translate_distance_ = kBoxDistanceBase - kBoxDistanceBase * distance_ratio;
    updateTranslateCount(level);
}

void EmptyBoxDrawer::updateTranslateCount(const float level) {
    float ratio = level + 0.5f;
    float threshold = kBoxDistanceBase * 0.5f;
    if (translate_distance_ > threshold) {
        float ratio_2 = ofMap(translate_distance_, threshold, kBoxDistanceBase, 1.0f, 0.4f);
        ratio *= ratio_2;
    }

    translate_count_ += (kTranslateCountBase * ratio);
}

void EmptyBoxDrawer::updateBoxSize() {
    float ratio = 1.0f;
    float threshold = kBoxDistanceBase * 0.4f;
    if (translate_distance_ < threshold) {
        float ratio_2 = ofMap(translate_distance_, 0.f, threshold, 0.9f, 1.0f);
        ratio *= ratio_2;
    }

    box_size_ = (kBoxSize * ratio);
}

void EmptyBoxDrawer::draw() {
    cam_.begin();

    ofSetColor(color_);
    ofNoFill();

    ofPushMatrix();
    ofTranslate(-translate_distance_, 0.f, 0.f);

    for (size_t i = 0; i < kBoxNum; ++i) {
        drawRotateBox();
        ofTranslate(translate_distance_, 0.f, 0.f);
    }

    ofPopMatrix();
    cam_.end();
}

void EmptyBoxDrawer::drawRotateBox() {
    ofPushMatrix();
    ofRotate(rotate_angle_, 1.f, 1.f, 1.f);
    ofDrawBox(box_size_);
    ofPopMatrix();
}

//--------------------------------------------------------------
void EmptyBoxDrawer::setupNextScene() {
    // TODO
}

//--------------------------------------------------------------
