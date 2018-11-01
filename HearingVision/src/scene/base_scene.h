#pragma once

#include "ofMain.h"
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
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void reset() = 0;

protected:
    std::shared_ptr<ProcessFFT> pfft_;
    std::shared_ptr<ofRectangle> win_cache_;
};
