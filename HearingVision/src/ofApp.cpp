#include "ofApp.h"

#include "scene/box/box.h"
#include "scene/circle/circle.h"
#include "scene/fraltal/fractal.h"
#include "scene/flash/flash.h"
#include "scene/grid/grid.h"
#include "scene/lissajous/lissajous.h"
#include "scene/mandala/mandala.h"
#include "scene/plsphere/plsphere.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
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
    selecting_scene_no_ = 0;
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
    if ((key >= '0') && (key <= '9')) {
        int scene_no = key - 0x30;
        if (isSceneNoValid(scene_no)) {
            selecting_scene_no_ = scene_no;
        }
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    win_cache_->setSize(w, h);
    for (auto& scene_fbo : scene_fbos_) {
        scene_fbo.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    }
}

//--------------------------------------------------------------
void ofApp::createScenes() {
    scene_list_.push_back(std::make_unique<BoxScene>());
    scene_list_.push_back(std::make_unique<FractalScene>());
    scene_list_.push_back(std::make_unique<FlashScene>());
    scene_list_.push_back(std::make_unique<LissajousScene>());
    scene_list_.push_back(std::make_unique<GridScene>());
    scene_list_.push_back(std::make_unique<CircleScene>());
    scene_list_.push_back(std::make_unique<MandalaScene>());
    scene_list_.push_back(std::make_unique<PLSphereScene>());
}

void ofApp::setupScenes() {
    for (auto& scene : scene_list_) {
        scene->setup(pfft_, win_cache_);
    }
}

bool ofApp::isSceneNoValid(int scene_no) {
    if ((scene_no >= 0) &&
        (scene_no < scene_list_.size())) {
        return true;
    } else {
        return false;
    }
}