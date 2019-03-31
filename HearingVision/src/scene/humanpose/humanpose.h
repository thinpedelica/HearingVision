#pragma once

#include "../base_scene.h"

#include <chrono>

class HumanPoseScene : public BaseScene {
public:
    HumanPoseScene();
    ~HumanPoseScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr uint32_t kBornNum = 14;
    static constexpr float kInvalidValue = -9990.f;

    static constexpr uint32_t kShadowLifeTime = 150;
    static constexpr float kShadowSlideX = -12.f;

    enum class DrawMode {
        kHumanOnly,
        kShadows,
        kHumanMany,
    };

    class HumanPose {
    public:
        HumanPose() : frame_no(0) {
            born_list.resize(kBornNum);
        }

        uint32_t frame_no;
        std::vector<ofVec2f> born_list;
    };

    bool initializeHumanPoseList();
    bool isValidBorn(const ofVec2f& born);

    void drawHumanPose(const float offset_x , const float offest_y);
    void drawHumanPoints(const size_t index);
    void drawHumanLines(const size_t index, const float alpha = 180.f);
    void drawHand(const size_t index, const bool is_right, const float alpha = 255.f);

    void drawHumanMany();

    void updateShadow();
    void drawShadow(const float offset_x, const float offest_y);

    void changeMode();
    bool isUpdateTiming();


    std::vector<HumanPose> human_pose_list_;

    using ShadowInfo = std::pair<size_t, float> ;
    std::vector<ShadowInfo> shadow_info_list_;

    uint32_t current_pose_{0};
    std::chrono::system_clock::time_point pre_time_;

    ofEasyCam cam_;

    DrawMode mode_{DrawMode::kHumanOnly};
    float level_{0.f};
};
