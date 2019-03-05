#pragma once

#include "../base_scene.h"

class PsychedelicaScene : public BaseScene {
public:
    PsychedelicaScene();
    ~PsychedelicaScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr float kLevelMin =  1.f;
    static constexpr float kLevelMax = 10.f;
    static constexpr size_t kHistoryMax = 100;

    ofShader shader_;
    float time_{0.f};
    float level_{kLevelMin};
    std::vector<float> high_;
    std::vector<float> mid_;
    std::vector<float> low_;
};
