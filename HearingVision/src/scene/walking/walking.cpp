#include "walking.h"

//--------------------------------------------------------------
WalkingScene::WalkingScene() {
    // nop
}

//--------------------------------------------------------------
WalkingScene::~WalkingScene() {
    // nop
}

//--------------------------------------------------------------
void WalkingScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    mesh_list_.resize(kAreaNum);
    vs_list_.resize(kAreaNum);
    is_list_.resize(kAreaNum);
    color_list_.resize(kAreaNum);
}

void WalkingScene::resetBuildings(const int index) {
    vs_list_.at(index).clear();
    is_list_.at(index).clear();
    color_list_.at(index).clear();
    draw_radius_ = kDrawRadiusMin;
}

void WalkingScene::updateBuildings(const int index) {
    r1(index, draw_radius_);
    mesh_list_.at(index).setVertexData(vs_list_.at(index).data(), vs_list_.at(index).size(), GL_STATIC_DRAW);
    mesh_list_.at(index).setIndexData(is_list_.at(index).data(), is_list_.at(index).size(), GL_STATIC_DRAW);
    mesh_list_.at(index).setColorData(color_list_.at(index).data(), color_list_.at(index).size(), GL_STATIC_DRAW);

    draw_radius_ += 50.f;
}

//--------------------------------------------------------------
void WalkingScene::update(SceneParam scene_param) {
    time_step_      = ofMap(scene_param.level_,     0.f, 1.f, kTimeStepBase,      kTimeStepBase * 10.f);
    time_draw_step_ = ofMap(scene_param.gain_, 0.f, 1.f, kTimeDrawStepBase, kTimeDrawStepBase * 10.f);

    color_level_ = scene_param.color_;

    time_ += time_step_;
    updateTimeToShift();
    updateTimeToDraw();
    updateCamPos();
}

void WalkingScene::updateTimeToShift() {
    time_to_shift_ += time_step_;
    if (time_to_shift_ > kShiftInterval) {
        time_to_shift_ = time_to_shift_ - kShiftInterval;
        updateDrawIndex();
        resetBuildings(draw_index_);
    }
}

void WalkingScene::updateTimeToDraw() {
    time_to_draw_ += time_draw_step_;
    if (time_to_draw_ > kDrawInterval) {
        time_to_draw_ = 0.f;
        updateBuildings(draw_index_);
    }
}

void WalkingScene::updateDrawIndex() {
    ++draw_index_;
    if (draw_index_ >= kAreaNum) {
        draw_index_ = 0;
    }
}

void WalkingScene::updateCamPos() {
    cam_pos_.set(250 * cos(time_), 50, 250 * sin(time_));
    cam_.setPosition(0, 0, 0);
    cam_.lookAt((cam_pos_).normalize(), ofVec3f(0, 1, 0));
    cam_.setFov(25.f);
}

void WalkingScene::r1(const int index, const float radius) {
    float angle_unit = ((2 * PI) / kAreaNum);
    float angle_start = angle_unit * index;
    float angle_step = angle_unit / 5.;

    for (int step = 1; step <= 5; ++step) {
        float angle = angle_start + angle_step * step;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        ofMatrix4x4 mat;
        mat.glTranslate(x, 0, z);
        r2(index, &mat, 0);
    }
}

void WalkingScene::r2(const int index, ofMatrix4x4 *mat, int level) {
    if (level == 5) return;

    float coin = ofRandom(1.0);
    ofMatrix4x4 _mat = *mat;

    float threshold = ofMap(pfft_->getLowVal(), 0.f, 0.3f, 0.3, 0.8);

    if (coin < 0.2) {
        return;
    } else if (coin < threshold) {
        _mat.glTranslate(0, 20, 0);
        _mat.glScale(1.2, 1.5, 1.2);
    } else {
        _mat.glTranslate(0, 10, 0);
        _mat.glScale(0.8, 0.8, 0.8);
    }

    r2(index, &_mat, ++level);
    r3(index, &_mat);
}

void WalkingScene::r3(const int index, ofMatrix4x4 *mat) {
    float y_ratio = ofMap(pfft_->getMidVal(), 0.f, 0.3f, 1.f, 3.f);

    for (int x = 0; x < 3; ++x) {
        for (int z = 0; z < 2; ++z) {
            for (int y = 0; y < 3; ++y) {
                ofMatrix4x4 _mat = *mat;
                _mat.glTranslate(20 * x, 10 * y_ratio * y, 20 * z);
                addBox(index, &_mat);
            }
        }
    }
}

void WalkingScene::addBox(const int index, ofMatrix4x4 *mat) {
    float coin = ofRandom(1.0);

    ofMesh _mesh;
    if (coin < 0.3) {
        _mesh = ofMesh::box(15, 5, 2, 1, 1, 1);
    } else if (coin < 0.6) {
        _mesh = ofMesh::box(3, 15, 2, 1, 1, 1);
    } else {
        _mesh = ofMesh::box(8, 15, 13, 1, 1, 1);
    }

    vector<ofVec3f> v = _mesh.getVertices();
    vector<ofIndexType> indices = _mesh.getIndices();
    int iNum = vs_list_.at(index).size();

    ofFloatColor fc;
    if (color_level_ < 0.1f) {
        fc.set(1.0, 0.04);
    } else {
        float h = ofMap(coin, 0.0, 1.0, 0, 255);
        float s = ofMap(color_level_, 0.1, 1.0, 0, 255);
        ofColor c = ofColor::fromHsb(h, s, 255, 8);
        fc.set(c);
    }

    for (int i = 0; i < v.size(); ++i) {
        vs_list_.at(index).push_back(v.at(i) * *mat);
        color_list_.at(index).push_back(fc);
    }

    for (int i = 0; i < indices.size(); ++i) {
        is_list_.at(index).push_back((int)indices.at(i) + iNum);
    }


}

//--------------------------------------------------------------
void WalkingScene::draw() {
    cam_.begin();

    drawBuildings();

    cam_.end();
}

void WalkingScene::drawBuildings() {
    std::vector<size_t> indexes = getDrawIndexes();
    for (const auto i : indexes) {
        ofFill();
        mesh_list_.at(i).drawElements(GL_TRIANGLES, is_list_.at(i).size());

        ofNoFill();
        mesh_list_.at(i).drawElements(GL_TRIANGLES, is_list_.at(i).size());
    }
}

std::vector<size_t> WalkingScene::getDrawIndexes() {
    std::vector<size_t> indexes;
    indexes.resize(3);
    indexes.at(2) = draw_index_;
    indexes.at(1) = draw_index_ - 1;
    indexes.at(0) = draw_index_ - 2;

    if (draw_index_ == 0) {
        indexes.at(1) = kAreaNum - 1;
        indexes.at(0) = kAreaNum - 2;
    }
    if (draw_index_ == 1) {
        indexes.at(0) = kAreaNum - 1;
    }

    return indexes;
}

//--------------------------------------------------------------
void WalkingScene::resize() {
    time_ = 0.f;
    for (int i = 0; i < kAreaNum; ++i) {
        resetBuildings(i);
    }
}
