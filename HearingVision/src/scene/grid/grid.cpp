#include "grid.h"

constexpr size_t GridScene::kGridCol;
constexpr size_t GridScene::kGridRow;
constexpr size_t GridScene::kGridNum;
constexpr float  GridScene::kGridColHalf;
constexpr float  GridScene::kGridRowHalf;
constexpr float  GridScene::kGridHeight;
constexpr float  GridScene::kPointSizeMin;
constexpr float  GridScene::kPointSizeMax;
constexpr float  GridScene::kFOVBase;
constexpr float  GridScene::kCamPosBaseX;
constexpr float  GridScene::kCamPosBaseY;
constexpr float  GridScene::kCamPosBaseZ;
constexpr float  GridScene::kTransCamSpeed;

//--------------------------------------------------------------
GridScene::GridScene() {
    // nop
}
//--------------------------------------------------------------
GridScene::~GridScene() {
    // nop
}
//--------------------------------------------------------------
void GridScene::setup(std::shared_ptr<ProcessFFT> pfft,
                      std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    fft_history_.resize(kGridRow);
    for (auto& fft_item : fft_history_) {
        fft_item.assign(kGridCol, 0.0);
    }

    vbo_list_.resize(kGridRow);
    vertex_list_.resize(kGridNum);
    color_list_.resize(kGridNum);
    for (int i = 0; i < kGridCol; ++i) {
        for (int j = 0; j < kGridRow; ++j) {
            float x = static_cast<float>(i) - kGridColHalf;
            float y = static_cast<float>(j) - kGridRowHalf;
            vertex_list_.at(kGridCol * i + j).set(x, y);
            color_list_.at(kGridCol * i + j).set(0.5, 0.8, 1.0, 0.2);
        }
    }

    for (auto& vbo : vbo_list_) {
        vbo.setVertexData(vertex_list_.data(), kGridRow, GL_DYNAMIC_DRAW);
        vbo.setColorData(color_list_.data(),   kGridRow, GL_DYNAMIC_DRAW);
    }

    point_size_ = 0.0f;
    is_line_    = false;

    roll_cam_.setup();
    roll_cam_.setCamSpeed(kTransCamSpeed);
    initializeCamPosList();
    resetCamPos();
}

//--------------------------------------------------------------
void GridScene::update(SceneParam scene_param) {
    fft_history_.push_back(pfft_->getSpectrum());
    fft_history_.erase(fft_history_.begin(), fft_history_.begin() + 1);

    changeMode(scene_param);

    for (int i = 0; i < kGridCol; ++i) {
        for (int j = 0; j < kGridRow; ++j) {
            float fft_val = fft_history_.at(j).at(i);
            float x = static_cast<float>(i) - kGridColHalf;
            float y = static_cast<float>(j) - kGridRowHalf;
            float z = fft_val * kGridHeight;

            vertex_list_.at(kGridCol * i + j) = ofVec3f(x, y, z);
            color_list_.at(kGridCol * i + j).set(0.5 + 0.5 * fft_val,
                                                 0.8,
                                                 1.0,
                                                 0.2 + 0.8 * fft_val);
        }
    }

    size_t list_index = 0;
    for (auto& vbo : vbo_list_) {
        vbo.updateVertexData(&vertex_list_.at(kGridRow * list_index), kGridRow);
        vbo.updateColorData(&color_list_.at(kGridRow * list_index),   kGridRow);
        ++list_index;
    }

    point_size_ = ofMap(pfft_->getLowVal(), 0.0f, 1.0f, kPointSizeMin, kPointSizeMax);

    roll_cam_.update();
    updateFov();
}

//--------------------------------------------------------------
void GridScene::draw() {
    ofPushMatrix();
    ofTranslate(win_cache_->getWidth()  * 0.5f,
                win_cache_->getHeight() * 0.5f);

    cam_.begin();
    cam_.setFov(fov_);
    cam_.lookAt(target_);
    cam_.setPosition(cam_pos_);
    roll_cam_.begin();

    glPointSize(point_size_);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (auto& vbo : vbo_list_) {
        if (is_line_) {
            vbo.draw(GL_LINE_STRIP, 0, kGridCol);
        } else {
            vbo.draw(GL_POINTS, 0, kGridCol);
        }
    }

    roll_cam_.end();
    cam_.end();
    ofPopMatrix();
}

//--------------------------------------------------------------
void GridScene::resetCamPos(const size_t index) {
    target_  = ofVec3f(win_cache_->getWidth()  * 0.5f, 
                       win_cache_->getHeight() * 0.5f);
    cam_pos_ = ofVec3f(kCamPosBaseX + win_cache_->getWidth()  * 0.5f, 
                       kCamPosBaseY + win_cache_->getHeight() * 0.5f,
                       kCamPosBaseZ);

    cam_pos_index_ = index;
    fov_ = cam_pos_list_.at(cam_pos_index_).fov;
    translateCamPos();
}

void GridScene::initializeCamPosList() {
    cam_pos_list_.push_back(CamPose( 60.0, ofVec3f(  0.0, 0.0,   0.0)));
    cam_pos_list_.push_back(CamPose( 30.0, ofVec3f(-10.0, 0.0,   0.0)));
    cam_pos_list_.push_back(CamPose(150.0, ofVec3f(  0.0, 0.0,   0.0)));
    cam_pos_list_.push_back(CamPose(120.0, ofVec3f(  0.0, 0.0, 150.0)));
    cam_pos_list_.push_back(CamPose(160.0, ofVec3f(  0.0, 0.0, 150.0)));
    cam_pos_list_.push_back(CamPose(120.0, ofVec3f(-15.0, 0.0,   0.0)));
    cam_pos_list_.push_back(CamPose( 90.0, ofVec3f(  0.0, 0.0,   0.0)));
}

void GridScene::translateCamPos() {
    roll_cam_.setPos(cam_pos_list_.at(cam_pos_index_).rotate.x,
                     cam_pos_list_.at(cam_pos_index_).rotate.y,
                     cam_pos_list_.at(cam_pos_index_).rotate.z);
}

void GridScene::updateFov() {
    fov_ -= (fov_ - cam_pos_list_.at(cam_pos_index_).fov) * kTransCamSpeed;
}

//--------------------------------------------------------------
void GridScene::changeMode(SceneParam scene_param) {
    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        ++cam_pos_index_;
        if (cam_pos_index_ >= cam_pos_list_.size()) {
            cam_pos_index_ = 0;
        }
        translateCamPos();
    }

    if (scene_param.level_ > 0.5) {
        is_line_ = true;
    } else {
        is_line_ = false;
    }
}

//--------------------------------------------------------------
void GridScene::reset() {
    resetCamPos(cam_pos_index_);
}
