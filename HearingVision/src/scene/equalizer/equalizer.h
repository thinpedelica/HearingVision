#pragma once

#include "../base_scene.h"

class EqualizerScene : public BaseScene {
public:
    EqualizerScene();
    ~EqualizerScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr size_t kBarNum = 256;
    static constexpr size_t kPointNum = 3 * 2 * kBarNum;
    static constexpr float kOutDirection    =  1.f;
    static constexpr float kInDirection     = -1.f;
    static constexpr float kOutDirectRadius = 200.f;
    static constexpr float kInDirectRadius  = 400.f;
    static constexpr float kBarHeight       = 1000.f;

    ofVbo vbo_;
    std::vector<ofVec3f> vertexes_;
    std::vector<ofFloatColor> color_list_;
    float time_{0.f};
    ofEasyCam cam_;

    float bar_direction_{ kOutDirection };
    float bar_radius_{ kOutDirectRadius };
};
