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
      reset_(TriggerState::kOff) {
        // nop
    }

    float alpha_;
    float level_;
    TriggerState reset_;
};
