#pragma once

#include "../base_scene.h"

class FractalScene : public BaseScene {
public:
    FractalScene();
    ~FractalScene();
    // virtual void setup(std::shared_ptr<ProcessFFT> pfft,
    //                    std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr size_t kRow = 4;
    static constexpr size_t kCol = 8;
    static constexpr float kMinLineWidth   =  2.0f;
    static constexpr float kMaxLineWidth   = 10.0f;
    static constexpr float kNarrowingRatio =  0.1f;

    void divideRect(float x, float y, float w, float h, int n);
};
