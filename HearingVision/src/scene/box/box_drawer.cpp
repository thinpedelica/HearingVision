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

//--------------------------------------------------------------
void RollingBoxDrawer::setup() {
    // none
}

void RollingBoxDrawer::update() {
    roll_cam_.update();

    counter_.setThreshold(threshold_);
    if (counter_.update()) {
        updateBoxSize();
        updateCamPos();
    }
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


void RollingBoxDrawer::draw() {
    roll_cam_.begin();

    ofSetColor(255, ofMap(pfft_->getMidVal(), 0.0, 1.0, 64, 255));
    ofSetLineWidth(ofMap(pfft_->getLowVal(), 0.0, 1.0, 1.0, 10.0));
    for (auto& box : boxes_) {
        box.drawWireframe();
    }

    roll_cam_.end();
}

//--------------------------------------------------------------
