#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "ofxRollingCam.h"

#include <memory>

#include "../util/counter.h"

class BoxDrawer {
public:
    BoxDrawer() = default;
    virtual ~BoxDrawer() = default;

    virtual void initialize(std::shared_ptr<ofRectangle> win_cache,
                            std::shared_ptr<ProcessFFT> pfft);

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    virtual void reset();
    virtual void resize();
    virtual void setColor(const float color) { color_ = color; }
    virtual void setLevel(const float level) { level_ = level; }
    virtual void setThreshold(const float threshold) { threshold_ = threshold; }
protected:

    std::shared_ptr<ofRectangle> win_cache_;
    std::shared_ptr<ProcessFFT> pfft_;
    float color_{0.f};
    float level_{0.f};
    float threshold_{0.f};
};

class RollingBoxDrawer : public BoxDrawer {
public:
    RollingBoxDrawer() = default;
    virtual ~RollingBoxDrawer() = default;

    virtual void setup();
    virtual void update();
    virtual void draw();

private:
    static constexpr float    kCamSpeed    = 0.1f;
    static constexpr size_t   kBoxNum      = 50;
    static constexpr float    kBoxSizeBase = 500.0;

    void updateBoxSize();
    void updateCamPos();
    void updateColor();

    std::vector<ofBoxPrimitive> boxes_;
    ofxRollingCam roll_cam_;

    ofColor box_color_;
    Counter counter_;
};
