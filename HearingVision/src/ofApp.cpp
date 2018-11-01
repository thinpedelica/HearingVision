#include "ofApp.h"

#include "scene/box/box.h"
#include "scene/fraltal/fractal.h"
#include "scene/flash/flash.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetBackgroundColor(0);

    pfft_ = std::make_shared<ProcessFFT>();
    pfft_->setup();
    pfft_->setNumFFTBins(128);
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
    updateSceneParam();

    // update active scene
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        size_t scene_index = active_scene_lsit_.at(index);

        if (scenen_params_.at(index).reset == TriggerState::kOn) {
            scene_list_.at(scene_index)->reset();
            scenen_params_.at(index).reset = TriggerState::kWaitForOff;
        }
        scene_list_.at(scene_index)->update();
    }
}

void ofApp::updateSceneParam() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        if (nano_kon_.buttonsSolo.at(index)) {
            active_scene_lsit_.at(index) = selecting_scene_no_;
        }

        if (nano_kon_.buttonsRec.at(index)) {
            if (scenen_params_.at(index).reset == TriggerState::kOff) {
                scenen_params_.at(index).reset = TriggerState::kOn;
            }
        } else {
            if (scenen_params_.at(index).reset == TriggerState::kWaitForOff) {
                scenen_params_.at(index).reset = TriggerState::kOff;
            }
        }

        scenen_params_.at(index).alpha = nano_kon_.sliders.at(index) * 2;

    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    for (size_t index = 0; index < kDrawableSceneNum; ++index) {
        if (scenen_params_.at(index).alpha > 0) {
            scene_fbos_.at(index).begin();
            ofClear(0);
            scene_list_.at(active_scene_lsit_.at(index))->draw();
            scene_fbos_.at(index).end();

            ofSetColor(255, 255, 255, scenen_params_.at(index).alpha);
            scene_fbos_.at(index).draw(0, 0);
        }
    }
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