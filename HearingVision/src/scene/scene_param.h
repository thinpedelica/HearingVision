#pragma once

#include "ofMain.h"

class SceneParam {
public:
    enum class TriggerState {
        kOff,
        kOn,
        kWaitForOff,
    };

    SceneParam() : 
      alpha_(0.f),
      level_(0.f),
      speed_(0.f),
      gain_(0.f),
      color_(0.f),
      change_mode_(TriggerState::kOff),
      reset_(TriggerState::kOff) {
        // nop
    }

    float alpha_;
    float level_;
    float speed_;
    float gain_;
    float color_;
    TriggerState change_mode_;
    TriggerState reset_;
};
