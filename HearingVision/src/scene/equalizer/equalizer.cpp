#include "equalizer.h"

//--------------------------------------------------------------
EqualizerScene::EqualizerScene() {
    // nop
}

//--------------------------------------------------------------
EqualizerScene::~EqualizerScene() {
    // nop
}

//--------------------------------------------------------------
void EqualizerScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    vertexes_.resize(kPointNum);
    color_list_.resize(kPointNum);

    float hue_step = 1.f / static_cast<float>(kBarNum);
    for (size_t i = 0; i < kBarNum; ++i) {
        ofFloatColor color;
        color.setHsb(hue_step * i, 1.f, 1.f, 0.8f);
        for (size_t j = 0; j < 6; ++j) {
            color_list_.at(6 * i + j).set(color);
        }
    }

    vbo_.setVertexData(vertexes_.data(), kPointNum, GL_DYNAMIC_DRAW);
    vbo_.setColorData(color_list_.data(), kPointNum, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void EqualizerScene::update(SceneParam scene_param) {
    std::vector<float> vals = pfft_->getSpectrum();
    float rad_step = 2.f * PI / 256;

    for (int i = 0; i < 256; ++i) {
        float x1 = kRadius * sin(rad_step * i);
        float x2 = kRadius * sin(rad_step * (i + 1));
        float y1 = kRadius * cos(rad_step * i);
        float y2 = kRadius * cos(rad_step * (i+1));
        float z1 = 0.f;
        float z2 = kBarHeight * vals.at(i);

        vertexes_.at(6 * i + 0).set(x1, y1, z1);
        vertexes_.at(6 * i + 1).set(x2, y2, z1);
        vertexes_.at(6 * i + 2).set(x2, y2, z2);
        vertexes_.at(6 * i + 3).set(x1, y1, z1);
        vertexes_.at(6 * i + 4).set(x2, y2, z2);
        vertexes_.at(6 * i + 5).set(x1, y1, z2);
    }
    vbo_.updateVertexData(vertexes_.data(), kPointNum);

    time_ += 0.1f;
}
//--------------------------------------------------------------
void EqualizerScene::draw() {
    cam_.begin();
    ofRotate(time_, 0.f, 0.f, 1.f);
    vbo_.draw(GL_TRIANGLES, 0, kPointNum);
    cam_.end();
}

//--------------------------------------------------------------
void EqualizerScene::reset() {
    // nop
}
