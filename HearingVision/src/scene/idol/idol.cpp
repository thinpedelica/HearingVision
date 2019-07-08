#include "idol.h"

//--------------------------------------------------------------
IdolScene::IdolScene() {
    // nop
}

//--------------------------------------------------------------
IdolScene::~IdolScene() {
    // nop
}

//--------------------------------------------------------------
void IdolScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    setupModel();
    drawer_prt_list_.push_back(std::make_unique<SoloIdolDrawer>());
    drawer_prt_list_.push_back(std::make_unique<TrioIdolDrawer>());
    drawer_prt_list_.push_back(std::make_unique<MoveIdolDrawer>());
    drawer_prt_list_.push_back(std::make_unique<MultiAngleIdolDrawer>());
    for (auto& drawer : drawer_prt_list_) {
        drawer->initialize(win_cache, pfft, model_loader_);
        drawer->setup();
    }
}

bool IdolScene::setupModel() {
    model_loader_ = std::make_shared<ofxAssimpModelLoader>();
    if (!model_loader_->loadModel("model/idol/12337_Statue_v1_l1.obj")) {
        std::cout << "##### load fail." << std::endl;
        return false;
    }

    std::vector<ofFloatColor> colors;
    colors.assign(model_loader_->getMesh(0).getIndices().size(), ofFloatColor(1.0f));

    ofxAssimpMeshHelper helper = model_loader_->getMeshHelper(0);
    helper.vbo.setColorData(colors.data(), colors.size(), GL_DYNAMIC_DRAW);

    model_loader_->disableTextures();
    model_loader_->setRotation(0, 90, 1, 0, 0);
    model_loader_->setRotation(1, 180, 0, 0, 1);

    //std::cout << mesh_.getIndices().size() << std::endl;
    //std::cout << mesh_.getColors().size() << std::endl;

    return true;
}

//--------------------------------------------------------------
void IdolScene::update(SceneParam scene_param) {
    if (scene_param.reset_ == SceneParam::TriggerState::kOn) {
        drawer_prt_list_.at(draw_list_index_)->reset();
    }

    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        ++draw_list_index_;
        if (draw_list_index_ == drawer_prt_list_.size()) {
            draw_list_index_ = 0;
        }
    }

    drawer_prt_list_.at(draw_list_index_)->setColor(scene_param.color_);
    drawer_prt_list_.at(draw_list_index_)->setLevel(scene_param.level_);
    drawer_prt_list_.at(draw_list_index_)->setThreshold(scene_param.threshold_);
    drawer_prt_list_.at(draw_list_index_)->update();
}

//--------------------------------------------------------------
void IdolScene::draw() {
    drawer_prt_list_.at(draw_list_index_)->draw();
}

//--------------------------------------------------------------
void IdolScene::resize() {
    for (auto& drawer : drawer_prt_list_) {
        drawer->resize();
    }
}
//--------------------------------------------------------------
