#pragma once

#include "../base_scene.h"

class WalkingScene : public BaseScene {
public:
    WalkingScene();
    ~WalkingScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void reset();

private:
    static constexpr size_t kAreaNum = 24;
    static constexpr float kTimeStepBase = 0.001f;
    static constexpr float kShiftInterval = (2.f * PI) / static_cast<float>(kAreaNum);

    static constexpr float kDrawRadiusMin = 250.f;

    void resetBuildings(const int index);
    void updateBuildings(const int index);
    void drawBuildings();

    void r1(const int index, const float radius);
    void r2(const int index, ofMatrix4x4 *mat, int level);
    void r3(const int index, ofMatrix4x4 *mat);
    void addBox(const int index, ofMatrix4x4 *mat);

    void updateTimeToShift();
    void updateTimeToDraw();
    void updateDrawIndex();
    void updateCamPos();

    std::vector<size_t> getDrawIndexes();

    std::vector<ofVbo> mesh_list_;
    std::vector<vector<ofVec3f> > vs_list_;
    std::vector<vector<ofIndexType> > is_list_;
    std::vector<std::vector<ofFloatColor> > color_list_;

    ofEasyCam cam_;
    ofVec3f cam_pos_;

    float time_{0.f};
    float time_step_{ 0.f };
    float time_to_draw_{ 0.f };
    float time_to_shift_{ 0.f };

    size_t draw_index_{0};
    float draw_radius_{ kDrawRadiusMin };
    float draw_interval_{0.f};

    float color_level_{ 0.f };
};
