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
    static constexpr size_t kHistoryMax = 100;
    static constexpr size_t kHistoryMin = 5;

    std::vector<ofShader> shader_;
    float time_{0.f};
    float level_{0.f};
    float color_{0.f};

    std::vector<float> high_;
    std::vector<float> mid_;
    std::vector<float> low_;
    size_t history_size_{kHistoryMax};

    size_t shader_index_{0};
};
