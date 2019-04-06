#pragma once
#include "ofMain.h"

class Counter {
public:
    Counter() = default;
    ~Counter() = default;

    void setThreshold(const float val) {
        threshold_ = kThreshold - val;
    }

    float getThreshold() { return threshold_; }

    bool update() {
        count_ += kUpdateCount;
        if (count_ > threshold_) {
            count_ = 0.f;
            return true;
        } else {
            return false;
        }
    }

    bool isOver(const float ratio) {
        if (count_ > (threshold_ * ratio)) {
            return true;
        } else {
            return false;
        }
    }

private:
    static constexpr float kThreshold = 1.f;
    static constexpr float kUpdateCount = 0.01f;

    float count_{0.f};
    float threshold_{kThreshold};
};
