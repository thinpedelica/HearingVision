#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"

#include <functional>
#include <memory>

#include "ofxAssimpModelLoader.h"
#include "../util/counter.h"

class IdolDrawer {
public:
    IdolDrawer() = default;
    virtual ~IdolDrawer() = default;
    virtual void initialize(std::shared_ptr<ofRectangle> win_cache,
                            std::shared_ptr<ProcessFFT> pfft,
                            std::shared_ptr<ofxAssimpModelLoader> model_loader);

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    virtual void reset();
    virtual void resize();
    virtual void setColor(const float color) { color_ = color; }
    virtual void setLevel(const float level) { level_ = level; }
    virtual void setThreshold(const float threshold) { threshold_ = threshold; }

protected:
    static constexpr int kRotateForBaseX = 0;
    static constexpr int kRotateForBaseZ = 1;
    static constexpr int kRotateForDrawZ = 2;

    void setCamCenter();
    void setCamCenterFar();
    void setCamCenterNear();
    void setCamLookDown();
    void setCamLookDownFace();
    void setCamLookUp();
    void setCamLookUpSide();
    void setCamSideUp();
    void setCamSideBody();
    void setCamDiagonallyAbove();

    void rotateBody(const float angle_deg);
    void updatePointSize(const float input_level);

    void lightBegin(const float color);
    void lightEnd();
    void drawIdol(const float color);

    std::shared_ptr<ofRectangle> win_cache_;
    std::shared_ptr<ProcessFFT> pfft_;
    std::shared_ptr<ofxAssimpModelLoader> model_loader_;

    ofLight point_light_1_;
    ofLight point_light_2_;
    ofLight point_light_3_;
    ofMaterial material_;
    ofEasyCam cam_;

    float color_{0.f};
    float level_{0.f};
    float threshold_{0.f};

private:
    enum class DrawColorMode {
        kNoLight,
        kWhite,
        kShadow,
        kShadow2,
        kColor,
    };

    DrawColorMode drawColorMode(const float color);
};

class SoloIdolDrawer : public IdolDrawer {
public:
    SoloIdolDrawer() = default;
    ~SoloIdolDrawer() = default;

    virtual void setup() override;
    virtual void update() override;
    virtual void draw() override;
private:
    float rorate_count_{0.f};
};

class TrioIdolDrawer : public IdolDrawer {
public:
    TrioIdolDrawer() = default;
    ~TrioIdolDrawer() = default;

    virtual void setup() override;
    virtual void update() override;
    virtual void draw() override;
private:
    float rorate_count_{0.f};
};

class MoveIdolDrawer : public IdolDrawer {
public:
    MoveIdolDrawer() = default;
    ~MoveIdolDrawer() = default;

    virtual void setup() override;
    virtual void update() override;
    virtual void draw() override;
private:
    struct Position {
        ofVec3f position_;
        float rotation_;
    };

    std::vector<Position> position_list_;

    void initializePositionList();
    void updatePositionList();
};

class MultiAngleIdolDrawer : public IdolDrawer {
public:
    MultiAngleIdolDrawer() = default;
    ~MultiAngleIdolDrawer() = default;

    virtual void setup();
    virtual void update();
    virtual void draw();
private:
    static constexpr uint32_t kAngleNum = 8;

    void initializeAngleList();
    void drawIdol(const uint32_t angle_no, const float color);

    Counter counter_;
    uint32_t angle_{0};
    std::vector<std::function<void()>> set_angles_;
};
