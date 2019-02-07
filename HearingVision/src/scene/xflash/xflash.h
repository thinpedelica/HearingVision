#pragma once

#include "../base_scene.h"
#include "../util/particle.h"

class XFlashScene : public BaseScene {
public:
    XFlashScene();
    ~XFlashScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void reset();

private:
    static constexpr int kBasePointNum =  500;
    static constexpr int kParticleNum  = 7000;

    void createBasePoint(const int num);
    void createDotPoint(const int num);
    void createParticle(const int num);

    void drawLineBetweenDots(const float level_val);
    void drawCircleOnDots(const float level_val);

    vector<ofVec2f> base_points_;
    vector<ofVec2f> dot_points_;
    std::unique_ptr<Particles> particles_ptr_;

    float friction_;
    float speed_;

};

