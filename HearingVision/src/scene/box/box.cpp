#include "box.h"

constexpr float    BoxScene::kCamSpeed;
constexpr size_t   BoxScene::kBoxNum;
constexpr float    BoxScene::kBoxSizeBase;

//--------------------------------------------------------------
BoxScene::BoxScene() {
    // nop
}

//--------------------------------------------------------------
BoxScene::~BoxScene() {
    // nop
}

//--------------------------------------------------------------
void BoxScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    roll_cam_.setup();
    roll_cam_.setCamSpeed(kCamSpeed);
}

//--------------------------------------------------------------
void BoxScene::update(SceneParam scene_param) {
    roll_cam_.update();

    counter_.setThreshold(scene_param.threshold_);
    bool ret = counter_.update();
    if (ret) {
        updateBoxSize();
        updateCamPos();
    }
}

void BoxScene::updateBoxSize() {
    boxes_.resize(kBoxNum);
    for (auto& box : boxes_) {
        float size = kBoxSizeBase * pfft_->getMidVal() * ofRandom(1.0, 10.0);
        box.setPosition(ofRandom(-win_cache_->getWidth() * 0.8, win_cache_->getWidth() * 0.8),
                        ofRandom(-win_cache_->getWidth() * 0.8, win_cache_->getWidth() * 0.8),
                        ofRandom(-win_cache_->getWidth() * 0.8, win_cache_->getWidth() * 0.8));
        box.setWidth(size);
        box.setHeight(size);
        box.setDepth(size);
    }
}

void BoxScene::updateCamPos() {
    roll_cam_.setRandomScale(0.1, 2.0);
    roll_cam_.setRandomPos(270);
}

//--------------------------------------------------------------
void BoxScene::draw() {
    roll_cam_.begin();

    ofSetColor(255, ofMap(pfft_->getMidVal(), 0.0, 1.0, 64, 255));
    ofSetLineWidth(ofMap(pfft_->getLowVal(), 0.0, 1.0, 1.0, 10.0));
    for (auto& box : boxes_) {
        box.drawWireframe();
    }

    roll_cam_.end();
}

//--------------------------------------------------------------
void BoxScene::reset() {
    // nop
}
