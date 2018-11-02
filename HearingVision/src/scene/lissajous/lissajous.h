#pragma once

#include "../base_scene.h"
#include <ofxRollingCam.h>

class LissajousScene : public BaseScene {
public:
    LissajousScene();
    ~LissajousScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update();
    virtual void draw();
    virtual void reset();

private:
    static constexpr float kThreshold = 0.3f;
    static constexpr int   kLissajousPointNum = 480;
    static constexpr float kSizeRatio = 0.25f;
    static constexpr float kScaleMin  = 0.2f;
    static constexpr float kScaleMax  = 2.0f;
    static constexpr int   kFrameCycle = 1260;
    static constexpr float kFreqMaxX   = 1.0f;
    static constexpr float kFreqMaxY   = 2.0f;
    static constexpr float kFreqMaxZ   = 4.0f;

    void updateLissajousParamsUnderThreshold();
    void updateLissajousParamsOverThreshold();
    void updateLissajousPoints();

    ofxRollingCam roll_cam_;

    float threshold_;

    std::vector<ofVec3f> lissajous_points_;
    float freq_x_;
    float freq_y_;
    float freq_z_;
    ofColor color_;
};
