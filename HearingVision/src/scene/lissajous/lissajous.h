#pragma once

#include "../base_scene.h"
#include <ofxRollingCam.h>

class LissajousScene : public BaseScene {
public:
    LissajousScene();
    ~LissajousScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void reset();

private:
    static constexpr float kThreshold = 0.3f;
    static constexpr int   kLissajousPointNum = 480;
    static constexpr float kSizeRatio = 0.25f;
    static constexpr float kScaleMin  = 0.2f;
    static constexpr float kScaleMax  = 5.0f;
    static constexpr int   kFrameCycle = 1260;
    static constexpr float kFreqMaxX   = 1.0f;
    static constexpr float kFreqMaxY   = 2.0f;
    static constexpr float kFreqMaxZ   = 4.0f;

    void updateLissajousParamsUnderThreshold();
    void updateLissajousParamsOverThreshold();
    void updateLissajousPoints();

    ofxRollingCam roll_cam_;

    float threshold_{kThreshold};

    std::vector<ofVec3f> lissajous_points_;
    float freq_x_;
    float freq_y_;
    float freq_z_;
    ofColor color_;

    struct DrawInfo {
        float x, y, w, h;

        DrawInfo(const float x_, const float y_, const float w_, const float h_) :
            x(x_), y(y_), w(w_), h(h_) {
            // nop
        }
    };
    ofFbo fbo_;
    std::vector<size_t> window_num_list_;
    std::vector<DrawInfo> draw_info_list_;
    size_t window_num_index_;

    void setDrawInfo(const size_t draw_num);
};
