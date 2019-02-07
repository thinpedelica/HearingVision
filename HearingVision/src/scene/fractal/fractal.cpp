#include "fractal.h"

constexpr size_t FractalScene::kRow;
constexpr size_t FractalScene::kCol;
constexpr float FractalScene::kMinLineWidth;
constexpr float FractalScene::kMaxLineWidth;
constexpr float FractalScene::kNarrowingRatio;

//--------------------------------------------------------------
FractalScene::FractalScene() {
    // nop
}

//--------------------------------------------------------------
FractalScene::~FractalScene() {
    // nop
}

//--------------------------------------------------------------
void FractalScene::update(SceneParam scene_param) {
    // nop
}

//--------------------------------------------------------------
void FractalScene::draw() {
    std::vector<float> spectrum = pfft_->getSpectrum();

    float box_w = win_cache_->getWidth() / kCol;
    float box_h = win_cache_->getHeight() / kRow;
    size_t step = spectrum.size() / (kCol * kRow);

    for (size_t i = 0; i < kRow; ++i) {
        float top = box_h * i;

        for (size_t j = 0; j < kCol; ++j) {
            size_t index = ((kCol * i) + j) * step;
            float left = box_w * j;
            float line_width = ofMap(spectrum.at(index), 0.0, 1.0, kMinLineWidth, kMaxLineWidth);
            divideRect(left, top, box_w, box_h, line_width);
        }
    }
}

void FractalScene::divideRect(float x, float y, float w, float h, int n) {
    float randw = ofRandom(w);
    float randh = ofRandom(h);
    ofSetLineWidth(n);
    ofSetColor(255);
    ofNoFill();

    if (w > h) {
        ofDrawLine(x + randw, y, x + randw, y + h);    // vertical
        if (n > 1) {
            divideRect(x,         y, randw,     h, n - kNarrowingRatio);    // left box
            divideRect(x + randw, y, w - randw, h, n - kNarrowingRatio);    // right box
        }
    } else {
        ofDrawLine(x, y + randh, x + w, y + randh);    // holizontal
        if (n > 1) {
            divideRect(x, y,         w, randh,     n - kNarrowingRatio);    // top box
            divideRect(x, y + randh, w, h - randh, n - kNarrowingRatio);    // bottom box
        }
    }
}

//--------------------------------------------------------------
void FractalScene::reset() {
    // nop
}