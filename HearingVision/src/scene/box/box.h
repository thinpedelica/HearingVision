#pragma once

#include <memory>

#include "../base_scene.h"
#include "../util/next_scene.h"
#include "box_drawer.h"

class BoxScene : public BaseScene {
public:
    BoxScene();
    ~BoxScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

    virtual NextScene& getNextScene() override;

private:
    std::vector<std::unique_ptr<BoxDrawer>> drawer_prt_list_;
    size_t draw_list_index_{0};
};
