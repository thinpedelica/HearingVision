#pragma once

#include "../base_scene.h"

class ZcomingScene : public BaseScene {
public:
    ZcomingScene();
    ~ZcomingScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr float kBaseZStep     = 100.f;
    static constexpr float kBaseThreshold = 0.15f;
    static constexpr float kDepthInterval = 5.f;
    static constexpr float kAdditionalZ   = 3000.f;

    static constexpr uint32_t kBaseGenCnt = 30;

    void updateZpos();
    void eraseOvers();
    void createTriangle(const float exp_ratio, const float angle, const float level);
    void createSquare(const float exp_ratio, const float angle, const float level);
    void createDepthLine(const float exp_ratio, const float angle, const float level);
    void createDepthCircle(const float exp_ratio, const float angle, const float level);
    void createDepthScrew(const float exp_ratio, const float angle, const float level);

    std::vector<ofVec3f> line_points_;
    std::vector<ofFloatColor> line_colors_;
    ofVbo vbo_line_;

    std::vector<ofVec3f> depth_line_points_;
    std::vector<ofFloatColor> depth_line_colors_;
    ofVbo vbo_depth_line_;

    float z_step_{0.f};
    float z_end_{-500.f};

    uint32_t cur_cnt_{0};
    uint32_t gen_cnt_{0};

    bool toggle_low_{ false };
    bool toggle_mid_{ false };
    bool toggle_high_{ false };

    ofFloatColor cur_color_;

    ofEasyCam cam_;

    float threshold_{0.f};
};
