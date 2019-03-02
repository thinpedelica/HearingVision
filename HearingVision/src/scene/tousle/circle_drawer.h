#pragma once

#include "ofMain.h"
#include <memory>
#include <vector>

class Circle {
public:
    Circle(const ofVec2f& pos, float radius, float speed, ofColor& color) :
            pos_(pos), radius_(radius), speed_(speed), color_(color) {}
    ofVec2f pos_;
    float radius_;
    float speed_;
    ofColor color_;
};
using CircleList = std::vector<Circle>;

class CircleDrawer {
public:
    CircleDrawer() = default;
    virtual ~CircleDrawer() = default;
    virtual void setup(std::shared_ptr<ofRectangle> win_cache) = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    virtual void addCircle(const std::vector<float>& spectrum,
                           const float level) = 0;
};

class MovingCircleDrawer : public CircleDrawer {
public:
    MovingCircleDrawer() = default;
    ~MovingCircleDrawer() = default;
    void setup(std::shared_ptr<ofRectangle> win_cache) override;
    void update() override;
    void draw() override;

    void addCircle(const std::vector<float>& spectrum,
                   const float level) override;

private:
    bool isOutOfWindow(const Circle& circle) const;

    CircleList circle_list_; 
    std::shared_ptr<ofRectangle> win_cache_;
};

class VanishingCircleDrawer : public CircleDrawer {
public:
    VanishingCircleDrawer() = default;
    ~VanishingCircleDrawer() = default;
    void setup(std::shared_ptr<ofRectangle> win_cache) override;
    void update() override;
    void draw() override;

    void addCircle(const std::vector<float>& spectrum,
                   const float level) override;

private:

    CircleList circle_list_;
    std::shared_ptr<ofRectangle> win_cache_;
};