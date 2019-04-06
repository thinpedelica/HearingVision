#include "equalizer_drawer.h"

//--------------------------------------------------------------
void CircularEqualizerDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
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
void CircularEqualizerDrawer::update(const std::vector<float>& spectrum) {
    float rad_step = 2.f * PI / 256;

    for (int i = 0; i < 256; ++i) {
        float x1 = kOutDirectRadius * sin(rad_step * i);
        float x2 = kOutDirectRadius * sin(rad_step * (i + 1));
        float y1 = kOutDirectRadius * cos(rad_step * i);
        float y2 = kOutDirectRadius * cos(rad_step * (i+1));
        float z1 = 0.f;
        float z2 = kBarHeight * spectrum.at(i);

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
void CircularEqualizerDrawer::draw() {
    cam_.begin();
    ofRotate(time_, 0.f, 0.f, 1.f);
    vbo_.draw(GL_TRIANGLES, 0, kPointNum);
    cam_.end();
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;

    initializeVertex();
    initializeColor();
}

void BoxEqualizerDrawer::initializeVertex() {
    vertexes_.resize(kPointNum);
    
    float width_step  = win_cache_->getWidth() / kBarColNum + 10;
    float height_step = win_cache_->getHeight() * 0.7 / kBarRowNum;

    for (size_t i = 0; i < kBarColNum; ++i) {
        float x1 = width_step * i + kOffsetXforBottom;
        float x2 = x1 + width_step - 10;
        
        for (size_t j = 0; j < kBarRowNum; ++j) {
            float y1 = win_cache_->getHeight() - (height_step * j);
            float y2 = y1 - height_step + 10;

            vertexes_.at(6 * (kBarRowNum * i + j) + 0).set(x1, y1, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 1).set(x2, y2, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 2).set(x1, y2, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 3).set(x1, y1, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 4).set(x2, y1, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 5).set(x2, y2, 0.f);
        }
    }

    vbo_.setVertexData(vertexes_.data(), kPointNum, GL_DYNAMIC_DRAW);
}

void BoxEqualizerDrawer::initializeColor() {
    color_list_.resize(kPointNum);

    ofFloatColor initial_color;
    initial_color.set(0.f, 0.f);
    for (auto& c : color_list_) {
        c.set(initial_color);
    }

    vbo_.setColorData(color_list_.data(), kPointNum, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::update(const std::vector<float>& spectrum) {
    size_t spectrum_step = spectrum.size() / kBarColNum;
    size_t spectrum_index = 0;

    ofFloatColor color_non;
    color_non.set(0.f, 0.f);
    float hue_step = 1.f / kBarRowNum;

    for (size_t i = 0; i < kBarColNum; ++i) {
        ofFloatColor color;
        float hue_base = 0.f;

        for (size_t j = 0; j < kBarRowNum; ++j) {
            float val = spectrum.at(spectrum_index);
            if (val > ((j + 1) * 0.025f)) {
                ofFloatColor color_upper;
                color.setHsb(hue_base, .7f, 1.f, 0.8f);
                color_upper.setHsb(hue_base + hue_step, .5f, 1.f, 0.8f);

                color_list_.at(6 * (kBarRowNum * i + j) + 0).set(color);
                color_list_.at(6 * (kBarRowNum * i + j) + 1).set(color_upper);
                color_list_.at(6 * (kBarRowNum * i + j) + 2).set(color_upper);
                color_list_.at(6 * (kBarRowNum * i + j) + 3).set(color);
                color_list_.at(6 * (kBarRowNum * i + j) + 4).set(color);
                color_list_.at(6 * (kBarRowNum * i + j) + 5).set(color_upper);
                hue_base += hue_step;
            } else {
                color_list_.at(6 * (kBarRowNum * i + j) + 0).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 1).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 2).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 3).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 4).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 5).set(color_non);
            }
        }

        spectrum_index += spectrum_step;
    }

    vbo_.updateColorData(color_list_.data(), kPointNum);
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::draw() {
   vbo_.draw(GL_TRIANGLES, 0, kPointNum);
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::resize() {
    initializeVertex();
}