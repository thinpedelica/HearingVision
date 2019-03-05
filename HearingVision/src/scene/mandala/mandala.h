#pragma once

#include "../base_scene.h"
#include "mandala_util.h"

class MandalaScene : public BaseScene {
public:
    MandalaScene();
    ~MandalaScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    MandalaDrawer mandala_drawer_;
};
