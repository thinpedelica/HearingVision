#pragma once

#include "../base_scene.h"
#include "../util/counter.h"

class TousleScene : public BaseScene {
public:
    TousleScene();
    ~TousleScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void reset();

private:
    class Circle {
    public:
        Circle(const ofVec2f& pos, float radius, float speed) :
             pos_(pos), radius_(radius), speed_(speed) {}
        ofVec2f pos_;
        float radius_;
        float speed_;
    };
    using CircleList = std::vector<Circle>;

    void addCircleList();
    void updateCircleList();
    void drawCircleList();

    bool isOutOfWindow(const Circle& circle) const;

    CircleList circle_list_; 

    Counter counter_;
};
