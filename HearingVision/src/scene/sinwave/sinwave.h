#pragma once

#include "../base_scene.h"

class SinWaveScene : public BaseScene {
public:
    SinWaveScene();
    ~SinWaveScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr float kBaseScale = 0.5f;
    static constexpr float kMaxScale  = 3.0f;
    static constexpr float kBaseWaves = 0.2f;
    static constexpr float kMaxWaves  = 4.0f;
    static constexpr float kBaseSpeed = 0.5f;
    static constexpr float kMaxSpeed  = 5.0f;

    ofShader shader_;
    float time_{0.0f};
    float scale_;
    float waves_;
    float speed_;
    float r_;
    float g_;
    float b_;
};
