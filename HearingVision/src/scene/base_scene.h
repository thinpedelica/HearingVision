#pragma once

#include "ofMain.h"
#include "scene_param.h"
#include "util/next_scene.h"
#include "ofxProcessFFT.h"

class BaseScene {
public:
    BaseScene() {}
    virtual ~BaseScene() {}
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache) {
        pfft_      = pfft;
        win_cache_ = win_cache;
    }

    virtual void update(SceneParam scene_param) = 0;
    virtual void draw() = 0;
    virtual void reset() {}
    virtual void resize() {}

    virtual NextScene& getNextScene() { return next_scene_; };

protected:
    std::shared_ptr<ProcessFFT> pfft_;
    std::shared_ptr<ofRectangle> win_cache_;
    NextScene next_scene_;
};
