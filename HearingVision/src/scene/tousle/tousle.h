#pragma once

#include <memory>

#include "../base_scene.h"
#include "../util/counter.h"

#include "circle_drawer.h"

class TousleScene : public BaseScene {
public:
    TousleScene();
    ~TousleScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    enum class TousleMode {
        kMove,
        kVanish,
    };

    void changeMode(const SceneParam::TriggerState is_change);

    Counter counter_;

    TousleMode mode_{ TousleMode::kMove};

    std::unique_ptr<CircleDrawer> circle_drawer_ptr_;
};
