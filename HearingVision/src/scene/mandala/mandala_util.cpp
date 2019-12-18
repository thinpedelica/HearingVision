#include "mandala_util.h"

void MandalaShape::changeColor(const float hue,
                               const float saturation,
                               const float brightness,
                               const float alpha) {
    color_.setHsb(hue, saturation, brightness, alpha);
}

void MandalaShape::changeColor(const float alpha) {
    color_.set(255, alpha);
}

void MandalaShape::changeSize(const float width,
                              const float height,
                              const float stroke_weight) {
    width_         = width;
    height_        = height;
    stroke_weight_ = stroke_weight;
}

void MandalaShape::draw() {
    ofNoFill();
    ofSetLineWidth(stroke_weight_);
    ofSetColor(color_);
    ofDrawEllipse(0.f, 0.f, width_, height_);
}

void MandalaDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_  = win_cache;
    base_color_ = 0.0;
}

void MandalaDrawer::update(const float base_color,
                           const float brightness_min,
                           const float brightness_max,
                           const float stroke_weight,
                           const float circle_div_base) {
    base_color_ = base_color;
    mstyle_.brightness_min_  = brightness_min;
    mstyle_.brightness_max_  = brightness_max;
    mstyle_.stroke_weight_   = stroke_weight;
    mstyle_.circle_div_base_ = circle_div_base;
}

void MandalaDrawer::draw() {
    ofPushMatrix();
    rotateCanvas(-18.0);

    for (int div_cnt = 1; div_cnt <= 6; ++div_cnt) {
        rotateCanvas(div_cnt * 3);
        float circle_size_base = win_cache_->getWidth() / mstyle_.circle_div_base_ +
                                 ofMap(ofRandom(1.0), 
                                       0.0,
                                       1.0,
                                       -win_cache_->getWidth() / (mstyle_.circle_div_base_ * 4.0),
                                        win_cache_->getWidth() / (mstyle_.circle_div_base_ * 4.0));
        float circle_size_mult = ofMap(ofRandom(1.0), 0.0, 1.0, 2.0, 10.0);
        int draw_cnt_max = div_cnt * 3;

        for (int draw_cnt = 0; draw_cnt < draw_cnt_max; ++draw_cnt) {
            float alpha = ofMap(ofRandom(1.0), 0.0, 1.0, 80.0, 255.0);
            if (base_color_ > 0.1f) {
                float hue = static_cast<int>((base_color_ * 255 + ofMap(ofRandom(1.0), 0.0, 1.0, -40.0, 40.0))) % 255;
                float saturation = ofMap(ofRandom(1.0), 0.0, 1.0, 150.0, 255.0);
                float brightness = ofMap(ofRandom(1.0), 0.0, 1.0, mstyle_.brightness_min_, mstyle_.brightness_max_);
                mshape_.changeColor(hue, saturation, brightness, alpha);
            } else {
                mshape_.changeColor(alpha);
            }

            float size = ofMap(ofRandom(1.0), 0.0, 1.0, circle_size_base, circle_size_base * circle_size_mult);
            mshape_.changeSize(size, size, mstyle_.stroke_weight_);

            float index_div = win_cache_->getWidth() / div_cnt;
            for (float index_w = 0.0; index_w < win_cache_->getWidth() + index_div; index_w += index_div) {
                for (float index_h = 0.0; index_h < win_cache_->getHeight() + index_div; index_h += index_div) {
                    ofPushMatrix();
                    ofTranslate(index_w, index_h);
                    mshape_.draw();
                    ofPopMatrix();
                }
            }
        }
    }
    ofPopMatrix();
}

void MandalaDrawer::rotateCanvas(const float deg) {
    ofTranslate(win_cache_->getWidth() * 0.5, win_cache_->getHeight() * 0.5);
    ofRotate(ofDegToRad(deg));
    ofTranslate(-win_cache_->getWidth() * 0.5, -win_cache_->getHeight() * 0.5);
}
