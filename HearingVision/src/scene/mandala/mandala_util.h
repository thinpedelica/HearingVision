#pragma once

#include "ofMain.h"

class MandalaStyle {
public:
    MandalaStyle() : 
        brightness_min_(0.f),
        brightness_max_(0.f),
        color_min_(0.f),
        color_max_(0.f),
        stroke_weight_(0.f),
        circle_div_base_(0.f) {
        // nop
    };

    float brightness_min_;
    float brightness_max_;
    float color_min_;
    float color_max_;
    float stroke_weight_;
    float circle_div_base_;
};

class MandalaShape {
public:
    MandalaShape() : 
        color_(0),
        width_(0.f),
        height_(0.f),
        stroke_weight_(0.f) {
        // nop
    };

    void changeColor(const float hue,
                     const float saturation,
                     const float brightness,
                     const float alpha);
    void changeSize(const float width, 
                    const float height, 
                    const float stroke_weight);

    void draw();

private:
    ofColor color_;
    float width_;
    float height_;
    float stroke_weight_;
};

class MandalaDrawer {
public:
    void setup(std::shared_ptr<ofRectangle> win_cache);

    void update(const float base_color,
                const float brightness_min,
                const float brightness_max,
                const float stroke_weight,
                const float circle_div_base);

    void draw();

private:
    void rotateCanvas(const float deg);
    float base_color_;
    MandalaStyle mstyle_;
    MandalaShape mshape_;

    std::shared_ptr<ofRectangle> win_cache_;
};