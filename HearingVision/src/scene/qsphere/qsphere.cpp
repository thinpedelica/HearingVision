#include "qsphere.h"


//--------------------------------------------------------------
QSphereScene::QSphereScene() {
    // nop
}

//--------------------------------------------------------------
QSphereScene::~QSphereScene() {
    // nop
}

//--------------------------------------------------------------
void QSphereScene::setup(std::shared_ptr<ProcessFFT> pfft,
                         std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;
    setupNextScene();

    cur_points_.resize(kPointsNum);
    target_points_.resize(kPointsNum);
    connect_points_list_.resize(kPointsNum);
    for (auto& connect_points : connect_points_list_) {
        connect_points.resize(kLinesPerPoint);
    }
    conectLines();

    color_list_.resize(kPointsNum);
    for (size_t index = 0; index < kPointsNum; ++index) {
        color_list_.at(index).set(1.0f);
    }

    point_vbo_.setVertexData(cur_points_.data(), kPointsNum, GL_DYNAMIC_DRAW);
    point_vbo_.setColorData(color_list_.data(),  kPointsNum, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void QSphereScene::update(SceneParam scene_param) {
    if ((ofGetFrameNum() % kEventFrameCount) == 0) {
        createTargetPoints(scene_param.level_);
    }
    updatePointsPos();
    updatePointsColor(scene_param.color_);
    point_vbo_.updateVertexData(cur_points_.data(), kPointsNum);
    point_vbo_.updateColorData(color_list_.data(), kPointsNum);
}

void QSphereScene::updatePointsPos() {
    for (size_t index = 0; index < kPointsNum; ++index) {
        cur_points_.at(index) = 
            cur_points_.at(index).getInterpolated(target_points_.at(index), 0.3);
    }
}

void QSphereScene::updatePointsColor(const float base_color) {
    ofColor color(255, 128);
    for (size_t index = 0; index < kPointsNum; ++index) {
        if (base_color < 0.1f) {
            color_list_.at(index) = color;
        } else {
            float dist = ofDist(0, 0, 0, cur_points_.at(index).x, cur_points_.at(index).y, cur_points_.at(index).z);
            float h_ratio = dist / kPointRangeMax;
            color.setHue(256.f * base_color - 30.f * h_ratio);
            color.setSaturation(150.f + 100.f * h_ratio);
            color.setBrightness(150.f + 100.f * h_ratio);
            color_list_.at(index) = color;
        }
    }
}

void QSphereScene::conectLines() {
    for (size_t i = 0; i < kPointsNum; ++i) {
        for (size_t j = 0; j < kLinesPerPoint; ++j) {
            size_t point = static_cast<size_t>(ofRandom(0.f, kPointsNum));
            connect_points_list_.at(i).at(j) = point;
        }
    }
}

void QSphereScene::createTargetPoints(const float level) {
    std::vector<float> spectrum = pfft_->getSpectrum();

    size_t index = 0;
    float angle_step = 360.f / target_points_.size();
    float z_max      = kPointRangeMax * pfft_->getMidVal() + 
                       kPointRangeMax * level;
    float z_step     = z_max / kPointsNum * 2.0f;
    for (size_t index = 0; index < kPointsNum; ++index) {
        float r = kPointRangeMax * spectrum.at(index) +
                  kPointRangeMax * level;
        float angle = angle_step * index;
        target_points_.at(index).x = r * sin(angle);
        target_points_.at(index).y = r * cos(angle);
        target_points_.at(index).z = z_step * index - z_max;
    }
}

//--------------------------------------------------------------
void QSphereScene::draw() {
    ofTranslate(win_cache_->getWidth() * 0.5, win_cache_->getHeight() * 0.5);
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    glPointSize(kPointSize);
    point_vbo_.draw(GL_POINTS, 0, kPointsNum);

    ofSetLineWidth(kLineWidth);
    for (size_t i = 0; i < kPointsNum; ++i) {
        ofSetColor(color_list_.at(i));
        for (size_t j = 0; j < connect_points_list_.at(i).size(); ++j) {
            ofDrawLine(cur_points_.at(i),
                       cur_points_.at(connect_points_list_.at(i).at(j)));
        }
    }
}

//--------------------------------------------------------------
void QSphereScene::setupNextScene() {
    NextScene::NextSceneList next_scene_list;
    next_scene_list.push_back("B1");
    next_scene_list.push_back("C");
    next_scene_list.push_back("D");
    next_scene_list.push_back("M");

    next_scene_.setNextSceneList(next_scene_list);
}
