#pragma once

#include "../base_scene.h"
#include <memory>

#include "equalizer_drawer.h"

class EqualizerScene : public BaseScene {
public:
    EqualizerScene();
    ~EqualizerScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void resize() override;

    virtual NextScene& getNextScene() override;

private:
    std::vector<std::unique_ptr<EqualizerDrawer>> drawer_prt_list_;
    size_t draw_list_index_{0};
};
