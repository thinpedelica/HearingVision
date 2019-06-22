#pragma once

#include "../base_scene.h"
#include "../util/counter.h"
#include <memory>

#include "stripes_drawer.h"

class StripesScene : public BaseScene {
public:
    StripesScene();
    ~StripesScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void resize() override;

private:
    std::vector<std::unique_ptr<StripesDrawer>> drawer_prt_list_;
    size_t draw_list_index_{0};
};
