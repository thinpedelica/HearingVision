#pragma once

#include "../base_scene.h"

class DebriScene : public BaseScene {
public:
    DebriScene();
    ~DebriScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr size_t kDebriNum = 6000;
    static constexpr float  kEarthRadius  = 256.f;
    static constexpr float  kFloatingDist =  30.f;
    static constexpr float  kSparkDist    = 100.f;
    static constexpr float  kGeoStationaryDistMin = 500.f;
    static constexpr float  kGeoStationaryDistMax = 550.f;
    static constexpr float  kGeoStationaryBand    =  15.f;
    static constexpr float  kLeoDistMin = 300.f;
    static constexpr float  kLeoDistMax = 350.f;

    float time_{0.f};
    float scale_{ kFloatingDist };
    float point_size_{1.f};
    bool normal_mode_{false};

    ofShader shader_;
    ofVboMesh vbo_mesh_;
    ofSpherePrimitive sphere_;
    ofImage img_;

    std::vector<ofVec3f> sats_;
    std::vector<ofFloatColor> color_list_;
    ofVbo vbo_;

    ofEasyCam cam_;
    ofVec3f cam_pos_;
};
