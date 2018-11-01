#pragma once

#include "../base_scene.h"
#include "ofxRollingCam.h"

class BoxScene : public BaseScene {
public:
    BoxScene();
    ~BoxScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update();
    virtual void draw();
    virtual void reset();

private:
    static constexpr float    kCamSpeed = 0.1f;
    static constexpr size_t   kBoxNum = 100;
    static constexpr float    kBoxSizeBase = 500.0;
    static constexpr uint32_t kEventFrameCount = 100;

    void updateBoxSize();
    void updateCamPos();

    std::vector<ofBoxPrimitive> boxes_;
    ofxRollingCam roll_cam_;
};
