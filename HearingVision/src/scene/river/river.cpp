#include "river.h"

constexpr size_t RiverScene::kGridCol;
constexpr size_t RiverScene::kGridRow;
constexpr size_t RiverScene::kGridNum;
constexpr float  RiverScene::kGridColHalf;
constexpr float  RiverScene::kGridRowHalf;
constexpr float  RiverScene::kGridHeight;

constexpr float  RiverScene::kFOVBase;
constexpr float  RiverScene::kCamPosBaseX;
constexpr float  RiverScene::kCamPosBaseY;
constexpr float  RiverScene::kCamPosBaseZ;
constexpr float  RiverScene::kTransCamSpeed;

//--------------------------------------------------------------
RiverScene::RiverScene() {
    // nop
}
//--------------------------------------------------------------
RiverScene::~RiverScene() {
    // nop
}
//--------------------------------------------------------------
void RiverScene::setup(std::shared_ptr<ProcessFFT> pfft,
                      std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    fft_history_.resize(kGridRow);
    for (auto& fft_item : fft_history_) {
        fft_item.assign(kGridCol, 0.0);
    }

    pline_row_.resize(kGridRow);
    vertex_list_.resize(kGridNum);

    roll_cam_.setup();
    roll_cam_.setCamSpeed(kTransCamSpeed);
    initializeCamPosList();
    resetCamPos();
}

//--------------------------------------------------------------
void RiverScene::update(SceneParam scene_param) {
    fft_history_.push_back(pfft_->getSpectrum());
    fft_history_.erase(fft_history_.begin(), fft_history_.begin() + 1);

    if (scene_param.reset_ == SceneParam::TriggerState::kOn) {
        reset();
    }

    translateCamPos(scene_param);
    base_color_ = 255. * scene_param.color_;

    for (int i = 0; i < kGridCol; ++i) {
        for (int j = 0; j < kGridRow; ++j) {
            float fft_val = fft_history_.at(j).at(i);
            float x = (static_cast<float>(i) - kGridColHalf) * 5.;
            float y = (static_cast<float>(j) - kGridRowHalf) * 0.1;
            float z = fft_val * kGridHeight;

            vertex_list_.at(i + kGridCol * j) = ofVec3f(x, y, z);
        }
    }

    for (int i = 0; i < kGridRow; ++i) {
        pline_row_.at(i).clear();

        ofVec3f start_pos = vertex_list_.at(kGridCol * i);
        ofVec3f end_pos   = vertex_list_.at(kGridCol * (i + 1) -1);
        start_pos.z = 0.f;
        end_pos.z   = 0.f;
        pline_row_.at(i).curveTo(start_pos);
        for (int j = kSamplingNum * 10; j < kGridCol; j += kSamplingNum) {
            pline_row_.at(i).curveTo(vertex_list_.at(kGridCol * i + j));
        }
        pline_row_.at(i).curveTo(end_pos);
    }

    roll_cam_.update();
    updateFov();
}

//--------------------------------------------------------------
void RiverScene::draw() {
    cam_.begin();
    cam_.setFov(fov_);
    cam_.lookAt(target_);
    cam_.setPosition(cam_pos_);
    roll_cam_.begin();

    ofEnableAlphaBlending();
    for (int i = 0; i < kGridRow; ++i) {
        ofSetColor(ofColor::fromHsb(base_color_ + i / 4., 255, 255, 160));
        pline_row_.at(i).draw();
    }

    roll_cam_.end();
    cam_.end();
}

//--------------------------------------------------------------
void RiverScene::resetCamPos(const size_t index) {
    target_  = ofVec3f(win_cache_->getWidth()  * 0.5f, 
                       win_cache_->getHeight() * 0.5f);
    cam_pos_ = ofVec3f(kCamPosBaseX + win_cache_->getWidth()  * 0.5f,
                       kCamPosBaseY + win_cache_->getHeight() * 0.5f,
                       kCamPosBaseZ);

    cam_pos_index_ = index;
    fov_ = cam_pos_list_.at(cam_pos_index_).fov;
    translateCamPos();
}

void RiverScene::initializeCamPosList() {
    cam_pos_list_.push_back(CamPose( 60.0, ofVec3f(   0.0,    0.0,   0.0)));
    cam_pos_list_.push_back(CamPose(110.0, ofVec3f(-145.0,  -30.0,   0.0)));
    cam_pos_list_.push_back(CamPose( 60.0, ofVec3f(-145.0, -145.0,   0.0)));
    cam_pos_list_.push_back(CamPose( 60.0, ofVec3f(   0.0,    0.0, 140.0)));
    cam_pos_list_.push_back(CamPose( 45.0, ofVec3f(  20.0,    0.0,  65.0)));
}

void RiverScene::translateCamPos(const SceneParam& scene_param) {
    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        ++cam_pos_index_;
        if (cam_pos_index_ >= cam_pos_list_.size()) {
            cam_pos_index_ = 0;
        }
        translateCamPos();
    }
}

void RiverScene::translateCamPos() {
    roll_cam_.setPos(cam_pos_list_.at(cam_pos_index_).rotate.x,
                     cam_pos_list_.at(cam_pos_index_).rotate.y,
                     cam_pos_list_.at(cam_pos_index_).rotate.z);
}

void RiverScene::updateFov() {
    fov_ -= (fov_ - cam_pos_list_.at(cam_pos_index_).fov) * kTransCamSpeed;
}

//--------------------------------------------------------------
void RiverScene::reset() {
    resetCamPos(cam_pos_index_);
}
