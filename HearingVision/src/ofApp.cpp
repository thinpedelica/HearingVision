#include "ofApp.h"

#include "scene/alphabeat/alphabeat.h"
#include "scene/box/box.h"
#include "scene/circle/circle.h"
#include "scene/debri/debri.h"
#include "scene/equalizer/equalizer.h"
#include "scene/grid/grid.h"
#include "scene/humanpose/humanpose.h"
#include "scene/lissajous/lissajous.h"
#include "scene/mandala/mandala.h"
#include "scene/psychedelica/psychedelica.h"
#include "scene/orb/orb.h"
#include "scene/qsphere/qsphere.h"
#include "scene/river/river.h"
#include "scene/stripes/stripes.h"
#include "scene/tousle/tousle.h"
#include "scene/walking/walking.h"
#include "scene/xflash/xflash.h"
#include "scene/zcoming/zcoming.h"

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

    nano_kon_.setup();

    // setup scene
    createScenes();
    setupScenes();

    scene_fbos_.resize(kDrawableSceneNum);
    for (auto& scene_fbo : scene_fbos_) {
        scene_fbo.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    }

    active_scene_lsit_.resize(kDrawableSceneNum);
    scenen_params_.resize(kDrawableSceneNum);
}

//--------------------------------------------------------------
void ofApp::update() {
    pfft_->update();
    nano_kon_.update();

    changeScene();
    updateSceneParam();
    updateScene();
    clearSceneParam();
}

void ofApp::changeScene() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        if (nano_kon_.buttonsSolo.at(index)) {
            active_scene_lsit_.at(index) = selecting_scene_no_;
        }
    }
}

void ofApp::updateScene() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        size_t scene_index = active_scene_lsit_.at(index);
        scene_list_.at(scene_index)->update(scenen_params_.at(index));
    }
}

void ofApp::updateSceneParam() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        updateTriggerState(nano_kon_.buttonsMute.at(index),
                           scenen_params_.at(index).change_mode_);
        updateTriggerState(nano_kon_.buttonsRec.at(index),
                           scenen_params_.at(index).reset_);
        scenen_params_.at(index).alpha_     = nano_kon_.sliders.at(index) * kControlResolution;
        scenen_params_.at(index).level_     = nano_kon_.knobs.at(index)   * kControlResolution;
        scenen_params_.at(index).color_     = nano_kon_.sliders.at(kSystemControlOffset + index) * kControlResolution;
        scenen_params_.at(index).threshold_ = nano_kon_.knobs.at(kSystemControlOffset + index)   * kControlResolution;
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

        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
        ofSetColor(255, 255, 255, 255 * scenen_params_.at(index).alpha_);
        scene_fbos_.at(index).draw(0, 0);
    }
}

ofTexture& ofApp::getMonitor(const int index) {
    return scene_fbos_.at(index).getTexture();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if ((key >= 'a') && (key <= 'z')) {
        if (key_vs_scene_no_.count(key)) {
            selecting_scene_no_ = key_vs_scene_no_.at(key);
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

    scene_list_.push_back(std::make_unique<RiverScene>());
    key_vs_scene_no_.emplace('r', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<StripesScene>());
    key_vs_scene_no_.emplace('s', scene_list_.size() - 1);

    scene_list_.push_back(std::make_unique<TousleScene>());
    key_vs_scene_no_.emplace('t', scene_list_.size() - 1);

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
