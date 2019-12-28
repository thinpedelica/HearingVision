#include "ofApp.h"

#include <sstream>

#include "scene/alphabeat/alphabeat.h"
#include "scene/box/box.h"
#include "scene/circle/circle.h"
#include "scene/debri/debri.h"
#include "scene/equalizer/equalizer.h"
#include "scene/grid/grid.h"
#include "scene/humanpose/humanpose.h"
#include "scene/idol/idol.h"
#include "scene/lissajous/lissajous.h"
#include "scene/mandala/mandala.h"
#include "scene/psychedelica/psychedelica.h"
#include "scene/orb/orb.h"
#include "scene/qsphere/qsphere.h"
#include "scene/stripes/stripes.h"
#include "scene/walking/walking.h"
#include "scene/xflash/xflash.h"
#include "scene/zcoming/zcoming.h"

const std::string ofApp::kLabelKeyBoard = "/KeyBoard";

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofSetBackgroundColor(0);
    ofSetCircleResolution(64);
    ofEnableSmoothing();

    pfft_ = std::make_shared<ProcessFFT>();
    pfft_->setup();
    pfft_->setNumFFTBins(256);
    pfft_->setNormalize(true);

    win_cache_ = std::make_shared<ofRectangle>();
    win_cache_->setSize(ofGetWidth(), ofGetHeight());

    color_controller_.setup();
    param_controller_.setup();
    setupOsc();

    createScenes();
    setupScenes();

    scene_fbos_.resize(kDrawableSceneNum);
    for (auto& scene_fbo : scene_fbos_) {
        scene_fbo.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    }

    active_scene_lsit_.resize(kDrawableSceneNum);
    scenen_params_.resize(kDrawableSceneNum);

    for (int index = 0; index < kDrawableSceneNum; ++index) {
        std::stringstream ss;
        ss << "OFSyphonSpoutOut" << index;
        name_vs_spout_sender_[ss.str()] = std::make_unique<ofxSpout2::Sender>();
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    pfft_->update();
    color_controller_.update();
    param_controller_.update();
    updateOsc();

    updateSceneParam();
    updateScene();
    clearSceneParam();
}

void ofApp::updateScene() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        size_t scene_index = active_scene_lsit_.at(index);
        scene_list_.at(scene_index)->update(scenen_params_.at(index));
    }
}

// TODO
void ofApp::updateSceneParam() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        ParamController::Param param = param_controller_.get(index);
        updateTriggerState(param.mode,
                           scenen_params_.at(index).change_mode_);
        updateTriggerState(param.reset,
                           scenen_params_.at(index).reset_);
        // scenen_params_.at(index).alpha_     = nano_kon_.nanokon2_items.sliders.at(index) * kControlResolution;

        scenen_params_.at(index).level_ = param.level;
        scenen_params_.at(index).speed_ = param.speed;
        scenen_params_.at(index).gain_  = param.gain;

        scenen_params_.at(index).color_ = color_controller_.get().getHue() * kColorResolution;
    }
}

void ofApp::clearSceneParam() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        clearTriggerState(scenen_params_.at(index).change_mode_);
        clearTriggerState(scenen_params_.at(index).reset_);
    }
}

void ofApp::updateTriggerState(const bool button,
                               SceneParam::TriggerState& state) {
    if (button) {
        if (state == SceneParam::TriggerState::kOff) {
            state = SceneParam::TriggerState::kOn;
        }
    } else {
        if (state == SceneParam::TriggerState::kWaitForOff) {
            state = SceneParam::TriggerState::kOff;
        }
    }
}

void ofApp::clearTriggerState(SceneParam::TriggerState& state) {
    if (state == SceneParam::TriggerState::kOn) {
        state = SceneParam::TriggerState::kWaitForOff;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        scene_fbos_.at(index).begin();
        ofClear(0);
        scene_list_.at(active_scene_lsit_.at(index))->draw();
        scene_fbos_.at(index).end();

        //glEnable(GL_BLEND);
        //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
        //ofSetColor(255, 255, 255, 255 * scenen_params_.at(index).alpha_);
        //scene_fbos_.at(index).draw(0, 0);

        std::stringstream ss;
        ss << "OFSyphonSpoutOut" << index;
        name_vs_spout_sender_.at(ss.str())->sendTexture(scene_fbos_.at(index).getTexture(), ss.str());

    }
}

ofTexture& ofApp::getMonitor(const int index) {
    return scene_fbos_.at(index).getTexture();
}

NextScene& ofApp::getNextScene(const int index) {
    return scene_list_.at(active_scene_lsit_.at(index))->getNextScene();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if ((key >= 'a') && (key <= 'z')) {
        if (key_vs_scene_no_.count(key)) {
            selecting_scene_no_ = key_vs_scene_no_.at(key);
        }
    }
}

void ofApp::sceneSelect(const int key, const int scene_index) {
    if ((key >= 'a') && (key <= 'z') && (scene_index < kDrawableSceneNum)) {
        if (key_vs_scene_no_.count(key)) {
            active_scene_lsit_.at(scene_index) = key_vs_scene_no_.at(key);
        }
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    win_cache_->setSize(w, h);
    resizeScenes();
    for (auto& scene_fbo : scene_fbos_) {
        scene_fbo.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    }
}

//--------------------------------------------------------------
void ofApp::createScenes() {
    scene_list_.push_back(std::make_unique<AlphabeatScene>());
    key_vs_scene_no_.emplace('a', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<BoxScene>());
    key_vs_scene_no_.emplace('b', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<CircleScene>());
    key_vs_scene_no_.emplace('c', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<DebriScene>());
    key_vs_scene_no_.emplace('d', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<EqualizerScene>());
    key_vs_scene_no_.emplace('e', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<GridScene>());
    key_vs_scene_no_.emplace('g', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<HumanPoseScene>());
    key_vs_scene_no_.emplace('h', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<IdolScene>());
    key_vs_scene_no_.emplace('i', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<LissajousScene>());
    key_vs_scene_no_.emplace('l', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<MandalaScene>());
    key_vs_scene_no_.emplace('m', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<OrbScene>());
    key_vs_scene_no_.emplace('o', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<PsychedelicaScene>());
    key_vs_scene_no_.emplace('p', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<QSphereScene>());
    key_vs_scene_no_.emplace('q', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<StripesScene>());
    key_vs_scene_no_.emplace('s', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<WalkingScene>());
    key_vs_scene_no_.emplace('w', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<XFlashScene>());
    key_vs_scene_no_.emplace('x', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<ZcomingScene>());
    key_vs_scene_no_.emplace('z', scene_list_.size() - 1);

}

void ofApp::setupScenes() {
    for (auto& scene : scene_list_) {
        scene->setup(pfft_, win_cache_);
    }
}

void ofApp::resizeScenes() {
    for (auto& scene : scene_list_) {
        scene->resize();
    }
}

void ofApp::setupOsc() {
    osc_receiver_.setup(kOscListenPort);
    setupKeyMap();
}

void ofApp::setupKeyMap() {
    int key = 'a';
    for (int i = 0; i < 26; ++i) {
        key_map_[i] = key;
        ++key;
    }
}

void ofApp::updateOsc() {
    while (osc_receiver_.hasWaitingMessages()) {
        ofxOscMessage msg;
        osc_receiver_.getNextMessage(msg);
        std::string addr = msg.getAddress();
        if (addr == kLabelKeyBoard) {
            int key_index   = msg.getArgAsInt32(0);
            int scene_index = msg.getArgAsInt32(1);
            sceneSelect(key_map_.at(key_index), scene_index);
        }
    }
}
