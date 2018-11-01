#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "nanoKon2.h"

#include "scene/base_scene.h"
#include <vector>
#include <memory>

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void windowResized(int w, int h);

private:
    static constexpr size_t kDrawableSceneNum = 2;

    void createScenes();
    void setupScenes();
    void updateSceneParam();
    bool isSceneNoValid(int scene_no);

    using ScenePtr  = std::unique_ptr<BaseScene>;
    using SceneList = std::vector<ScenePtr>;
    SceneList scene_list_;
    int selecting_scene_no_;

    std::vector<ofFbo> scene_fbos_;
    std::vector<size_t> active_scene_lsit_;

    enum class TriggerState {
        kOff,
        kOn,
        kWaitForOff,
    };

    struct SceneParam {
        uint8_t alpha;
        TriggerState reset;

        SceneParam() {
            alpha = 0;
            reset = TriggerState::kOff;
        }
    };
    std::vector<SceneParam> scenen_params_;


    std::shared_ptr<ProcessFFT> pfft_;
    std::shared_ptr<ofRectangle> win_cache_;
    NanoKon2 nano_kon_;
};
