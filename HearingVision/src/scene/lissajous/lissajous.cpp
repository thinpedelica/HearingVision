#include "lissajous.h"

//--------------------------------------------------------------
LissajousScene::LissajousScene() {
    // nop
}

//--------------------------------------------------------------
LissajousScene::~LissajousScene() {
    // nop
}

//--------------------------------------------------------------
void LissajousScene::setup(std::shared_ptr<ProcessFFT> pfft,
                           std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;
    setupNextScene();
    roll_cam_.setup();
    roll_cam_.setCamSpeed(0.1);

    fbo_.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    window_num_list_.push_back(1);
    window_num_list_.push_back(2);
    window_num_list_.push_back(4);
    window_num_list_.push_back(8);
    window_num_index_ = 0;
    setDrawInfo(window_num_list_.at(window_num_index_));

    lissajous_points_.resize(kLissajousPointNum);
}

//--------------------------------------------------------------
void LissajousScene::update(SceneParam scene_param) {
    threshold_ = kThreshold - (kThreshold * scene_param.level_);

    if (pfft_->getLowVal() > threshold_) {
        roll_cam_.setRandomScale(kScaleMin, kScaleMax);
        roll_cam_.setRandomPos(270);
        updateLissajousParamsOverThreshold();
    } else {
        roll_cam_.setPos(roll_cam_.posT.x + 1, roll_cam_.posT.y + 1, roll_cam_.posT.z + 1);
        updateLissajousParamsUnderThreshold();
    }

    updateLissajousPoints();

    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        ++window_num_index_;
        if (window_num_index_ >= window_num_list_.size()) {
            window_num_index_ = 0;
        }
        setDrawInfo(window_num_list_.at(window_num_index_));
    }
}

void LissajousScene::updateLissajousParamsUnderThreshold() {
    freq_x_ = ofMap(ofGetFrameNum() % kFrameCycle, 1.0, kFrameCycle, 1.0, kFreqMaxX);
    freq_y_ = ofMap(ofGetFrameNum() % kFrameCycle, 1.0, kFrameCycle, 1.0, kFreqMaxY);
    freq_z_ = ofMap(ofGetFrameNum() % kFrameCycle, 1.0, kFrameCycle, 1.0, kFreqMaxZ);
    color_  = ofColor(255, 255, 255, 32);
}

void LissajousScene::updateLissajousParamsOverThreshold() {
    freq_x_ = ofMap(pfft_->getLowVal(),  0.0, 1.0, 1.0, kFreqMaxX);
    freq_y_ = ofMap(pfft_->getMidVal(),  0.0, 1.0, 1.0, kFreqMaxY);
    freq_z_ = ofMap(pfft_->getHighVal(), 0.0, 1.0, 1.0, kFreqMaxZ);
    color_  = ofColor(ofColor::fromHsb(ofRandom(0, 255), 255, 255), 32);
}

void LissajousScene::updateLissajousPoints() {
    float radius = win_cache_->getWidth() * kSizeRatio;

    float angle = 0;
    float angle_unit = TWO_PI / lissajous_points_.size();
    for (auto& lissajous_point : lissajous_points_) {
        float x = sin(angle * freq_x_) * sin(angle * 2) * radius;
        float y = sin(angle * freq_y_) * cos(angle * 2) * radius;
        float z = sin(angle * freq_z_) * radius;
        lissajous_point = ofVec3f(x, y, z);
        angle += angle_unit;
    }
}

//--------------------------------------------------------------
void LissajousScene::draw() {
    fbo_.begin();
    ofClear(0);
    roll_cam_.begin();
    ofBeginShape();
    ofSetColor(color_);
    ofFill();
    for (size_t i = 0; i < lissajous_points_.size() - 2; ++i) {
        if (i % 3 == 0) {
            ofVertex(0, 0, 0);
            ofVertex(lissajous_points_.at(i).x,     lissajous_points_.at(i).y,     lissajous_points_.at(i).z);
            ofVertex(lissajous_points_.at(i + 2).x, lissajous_points_.at(i + 2).y, lissajous_points_.at(i + 2).z);
        }
    }
    ofEndShape();
    roll_cam_.end();
    fbo_.end();

    ofSetColor(255);
    for (const auto& draw_info : draw_info_list_) {
        fbo_.draw(draw_info.x, draw_info.y, draw_info.w, draw_info.h);
    }
}

//--------------------------------------------------------------
void LissajousScene::resize() {
    fbo_.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    window_num_index_ = 0;
    setDrawInfo(window_num_list_.at(window_num_index_));
}

void LissajousScene::setDrawInfo(const size_t draw_num) {
    draw_info_list_.clear();
    switch (draw_num) {
        case 1:
            draw_info_list_.push_back(DrawInfo(0, 0, win_cache_->getWidth(), win_cache_->getHeight()));
            break;
        case 2:
            draw_info_list_.push_back(DrawInfo(0,                          0, win_cache_->getWidth() / 2, win_cache_->getHeight()));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() / 2, 0, win_cache_->getWidth() / 2, win_cache_->getHeight()));
            break;
        case 4:
            draw_info_list_.push_back(DrawInfo(0,                          0,                           win_cache_->getWidth() / 2, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() / 2, 0,                           win_cache_->getWidth() / 2, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(0,                          win_cache_->getHeight() / 2, win_cache_->getWidth() / 2, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() / 2, win_cache_->getHeight() / 2, win_cache_->getWidth() / 2, win_cache_->getHeight() / 2));
            break;
        case 8:
            draw_info_list_.push_back(DrawInfo(0,                              0, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() * 1 / 4, 0, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() * 2 / 4, 0, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() * 3 / 4, 0, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));

            draw_info_list_.push_back(DrawInfo(0,                              win_cache_->getHeight() / 2, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() * 1 / 4, win_cache_->getHeight() / 2, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() * 2 / 4, win_cache_->getHeight() / 2, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));
            draw_info_list_.push_back(DrawInfo(win_cache_->getWidth() * 3 / 4, win_cache_->getHeight() / 2, win_cache_->getWidth() / 4, win_cache_->getHeight() / 2));
            break;
    }
}

//--------------------------------------------------------------
void LissajousScene::setupNextScene() {
    // nop
}
