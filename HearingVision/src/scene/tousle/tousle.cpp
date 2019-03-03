#include "tousle.h"


//--------------------------------------------------------------
TousleScene::TousleScene() {
    // nop
}

//--------------------------------------------------------------
TousleScene::~TousleScene() {
    // nop
}

//--------------------------------------------------------------
void TousleScene::setup(std::shared_ptr<ProcessFFT> pfft,
                        std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    circle_drawer_ptr_ = std::make_unique<MovingCircleDrawer>();
    circle_drawer_ptr_->setup(win_cache_);
}

//--------------------------------------------------------------
void TousleScene::update(SceneParam scene_param) {
    changeMode(scene_param.change_mode_);

    counter_.setThreshold(scene_param.threshold_);
    bool ret = counter_.update();
    if (ret) {
        std::vector<float> spectrum = pfft_->getSpectrum();
        circle_drawer_ptr_->addCircle(spectrum, scene_param.level_);
    }

    circle_drawer_ptr_->update();
}

//--------------------------------------------------------------
void TousleScene::draw() {
    circle_drawer_ptr_->draw();
}

//--------------------------------------------------------------
void TousleScene::reset() {
    // nop
}

//--------------------------------------------------------------
void TousleScene::changeMode(const SceneParam::TriggerState is_change) {
    if (is_change == SceneParam::TriggerState::kOn) {
        if (mode_ == TousleMode::kMove) {
            mode_ = TousleMode::kVanish;
            circle_drawer_ptr_ = std::make_unique<VanishingCircleDrawer>();
            circle_drawer_ptr_->setup(win_cache_);
        } else {
            mode_ = TousleMode::kMove;
            circle_drawer_ptr_ = std::make_unique<MovingCircleDrawer>();
            circle_drawer_ptr_->setup(win_cache_);
        }
    }
}
