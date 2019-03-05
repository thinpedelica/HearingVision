#pragma once

#include "../base_scene.h"
#include <ofxRollingCam.h>

class RiverScene : public BaseScene {
public:
    RiverScene();
    ~RiverScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void resize();

private:
    static constexpr size_t kGridRow = 128;
    static constexpr size_t kGridCol = 128;
    static constexpr size_t kGridNum = kGridRow * kGridCol;
    static constexpr float  kGridRowHalf = kGridRow / 2;
    static constexpr float  kGridColHalf = kGridCol / 2;
    static constexpr float  kGridHeight = 10.f;

    static constexpr int kSamplingNum = 4;

    static constexpr float kFOVBase = 60.f;
    static constexpr float kCamPosBaseX = 0.f;
    static constexpr float kCamPosBaseY = -30.f;
    static constexpr float kCamPosBaseZ = 0.f;

    static constexpr float kTransCamSpeed = 1.f;

    void resetCamPos(const size_t index = 0);
    void initializeCamPosList();
    void translateCamPos(const SceneParam& scene_param);
    void translateCamPos();
    void updateFov();

    vector<vector<float> > fft_history_;
    std::vector<ofVec3f> vertex_list_;
    std::vector<ofPolyline> pline_row_;

    float fov_;
    ofVec3f target_;
    ofVec3f cam_pos_;

    ofEasyCam cam_;
    ofxRollingCam roll_cam_;
    struct CamPose {
        float fov;
        ofVec3f rotate;

        CamPose() : fov(60.0), rotate(0.0, 0.0, 0.0) {}
        CamPose(float f, ofVec3f r) : fov(f), rotate(r) {}
    };

    std::vector<CamPose> cam_pos_list_;
    size_t cam_pos_index_;

    float base_color_h_{ 0. };
    float color_s_{ 0. };
};
