#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"
#include "ofxNanokon2.h"

#include "scene/base_scene.h"
#include "scene/scene_param.h"
#include "scene/util/next_scene.h"
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
    static constexpr float  kControlResolution   = 1.f / 127.f;

    void createScenes();
    void setupScenes();
    void resizeScenes();
    void changeScene();
    void updateScene();

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
    ofxNanokon2 nano_kon_;
};
