#pragma once

#include "../base_scene.h"

class QSphereScene : public BaseScene {
public:
    QSphereScene();
    ~QSphereScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr uint32_t kEventFrameCount = 5;
    static constexpr size_t kPointsNum     = 256;
    static constexpr size_t kLinesPerPoint =   8;

    static constexpr float kPointSize = 4.f;
    static constexpr float kLineWidth = 0.2f;
    static constexpr float kPointRangeMax = 1000.f;

    void updatePointsPos();
    void updatePointsColor(const float base_color);
    void conectLines();
    void createTargetPoints(const float level);

    void setupNextScene();

    using ConnectPoints = std::vector<size_t>;
    using ConnectPointsList = std::vector<ConnectPoints>;

    std::vector<ofVec3f> cur_points_;
    std::vector<ofVec3f> target_points_;
    std::vector<ofFloatColor> color_list_;
    ConnectPointsList connect_points_list_;
    ofVbo point_vbo_;
};
