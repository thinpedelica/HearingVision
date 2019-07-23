#pragma once

#include <vector>
#include <memory>

#include "ofxAssimpModelLoader.h"
#include "../base_scene.h"

#include "idol_drawer.h"

class IdolScene : public BaseScene {
public:
    IdolScene();
    ~IdolScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void resize() override;

    virtual NextScene& getNextScene() override;

private:
    bool setupModel();

    std::shared_ptr<ofxAssimpModelLoader> model_loader_;
    std::vector<std::unique_ptr<IdolDrawer>> drawer_prt_list_;
    size_t draw_list_index_{0};
};
