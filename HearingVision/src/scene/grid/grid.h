#pragma once

#include "../base_scene.h"
#include <ofxRollingCam.h>

class GridScene : public BaseScene {
public:
    GridScene();
    ~GridScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void reset();

private:
    static constexpr size_t kGridRow = 128;
    static constexpr size_t kGridCol = 128;
    static constexpr size_t kGridNum = kGridRow * kGridCol;
    static constexpr float  kGridRowHalf = kGridRow / 2;
    static constexpr float  kGridColHalf = kGridCol / 2;
    static constexpr float  kGridHeight  = kGridRow / 4;

    static constexpr float  kPointSizeMin = 1.0f;
    static constexpr float  kPointSizeMax = 4.0f;

    static constexpr float kFOVBase = 60.f;
    static constexpr float kCamPosBaseX = 0.f;
    static constexpr float kCamPosBaseY = -kGridRowHalf;
    static constexpr float kCamPosBaseZ = kGridHeight / 2;

    static constexpr float kTransCamSpeed = 0.1f;

    void resetCamPos(const size_t index = 0);
    void initializeCamPosList();
    void translateCamPos();
    void updateFov();
    void changeMode(SceneParam scene_param);

    vector<vector<float> > fft_history_;

    std::vector<ofVbo> vbo_list_;
    std::vector<ofVec3f> vertex_list_;
    std::vector<ofFloatColor> color_list_;
    float point_size_;
    bool is_line_;

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
};
