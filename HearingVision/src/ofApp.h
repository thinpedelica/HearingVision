#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "ofxOsc.h"
#include "ofxSpout2Sender.h"

#include "scene/base_scene.h"
#include "scene/scene_param.h"
#include "scene/util/next_scene.h"
#include "colorController/color_controller.h"
#include "paramController/param_controller.h"
#include "sceneController/scene_controller.h"
#include <map>
#include <vector>
#include <memory>

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    ofTexture& getMonitor(const int index);
    NextScene& getNextScene(const int index);

    void keyPressed(int key);
    void windowResized(int w, int h);

private:
    static constexpr size_t kDrawableSceneNum    = 2;
    static constexpr size_t kSystemControlOffset = 6;
    static constexpr float  kColorResolution     = 1.f / 255.f;


    void createScenes();
    void setupScenes();
    void resizeScenes();
    void updateScene();
    void selectScene();

    void updateSceneParam();
    void clearSceneParam();

    void updateTriggerState(const bool button,
                            SceneParam::TriggerState& state);
    void clearTriggerState(SceneParam::TriggerState& state);

    using ScenePtr  = std::unique_ptr<BaseScene>;
    using SceneList = std::vector<ScenePtr>;
    using KeyVsSceneNo = std::map<int, int>;
    SceneList scene_list_;
    KeyVsSceneNo  key_vs_scene_no_;
    int selecting_scene_no_{0};

    std::vector<ofFbo> scene_fbos_;
    std::vector<size_t> active_scene_lsit_;
    std::vector<SceneParam> scenen_params_;

    std::shared_ptr<ProcessFFT> pfft_;
    std::shared_ptr<ofRectangle> win_cache_;
    ColorController color_controller_;
    ParamController param_controller_;
    SceneController scene_controller_;


    std::map<std::string, std::unique_ptr<ofxSpout2::Sender>> name_vs_spout_sender_;
};
