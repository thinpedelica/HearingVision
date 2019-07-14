#include "idol_drawer.h"

//--------------------------------------------------------------
void IdolDrawer::initialize(std::shared_ptr<ofRectangle> win_cache,
                            std::shared_ptr<ProcessFFT> pfft,
                            std::shared_ptr<ofxAssimpModelLoader> model_loader) {
    win_cache_    = win_cache;
    pfft_         = pfft;
    model_loader_ = model_loader;
}

void IdolDrawer::reset() {
    // non
}

void IdolDrawer::resize() {
    // non
}

void IdolDrawer::setCamCenter() {
    cam_.setPosition(0.f, 500.f, 1000.f);
    cam_.lookAt(ofVec3f(0.f, 300.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamCenterFar() {
    cam_.setPosition(0.f, 500.f, 2000.f);
    cam_.lookAt(ofVec3f(0.f, 300.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamCenterNear() {
    cam_.setPosition(0.f, 600.f, 500.f);
    cam_.lookAt(ofVec3f(0.f, 500.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamLookDown() {
    cam_.setPosition(0.f, 1000.f, 500.f);
    cam_.lookAt(ofVec3f(0.f, 300.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamLookDownFace() {
    cam_.setPosition(0.f, 650.f, 150.f);
    cam_.lookAt(ofVec3f(0.f, 600.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamLookUp() {
    cam_.setPosition(0.f, 300.f, 400.f);
    cam_.lookAt(ofVec3f(0.f, 500.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamLookUpSide() {
    cam_.setPosition(-300.f, 300.f, 400.f);
    cam_.lookAt(ofVec3f(200.f, 500.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}
void IdolDrawer::setCamSideUp() {
    cam_.setPosition(-50.f, 650.f, 150.f);
    cam_.lookAt(ofVec3f(-100.f, 650.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamSideBody() {
    cam_.setPosition(-400.f, 400.f, 200.f);
    cam_.lookAt(ofVec3f(-100.f, 600.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::setCamDiagonallyAbove() {
    cam_.setPosition(-200.f, 700.f, 600.f);
    cam_.lookAt(ofVec3f(-200.f, 500.f, 0.f), ofVec3f(0.f, 1.f, 0.f));
}

void IdolDrawer::rotateBody(const float angle_deg) {
    model_loader_->setRotation(kRotateForDrawZ, angle_deg, 0, 0, 1);
}

void IdolDrawer::updatePointSize(const float input_level) {
    float point_size = ofMap(input_level, 0.0f, 0.5f, 0.1f, 4.f, true);
    glPointSize(point_size);
}

void IdolDrawer::lightBegin(const float color) {
    DrawColorMode mode = drawColorMode(color);
    switch(mode) {
        case DrawColorMode::kNoLight:
            // non
            break;
        case DrawColorMode::kWhite:
            ofEnableDepthTest();
            ofEnableLighting();
            material_.setShininess(255);
            material_.setDiffuseColor(ofColor(255));
            material_.setSpecularColor(ofColor(255));
            material_.begin();
            model_loader_->enableMaterials();
            break;
        case DrawColorMode::kShadow:
            ofEnableDepthTest();
            ofEnableLighting();
            point_light_1_.setDiffuseColor(ofColor(64.f));
            point_light_1_.setSpecularColor(ofColor(64.f));
            point_light_1_.setPosition(1500.f, 100.f, 500.f);
            material_.setShininess(128);
            material_.setDiffuseColor(ofColor(128.f));
            material_.setSpecularColor(ofColor(128.f));
            point_light_1_.enable();
            material_.begin();
            model_loader_->disableMaterials();
            break;
        case DrawColorMode::kShadow2:
            ofEnableDepthTest();
            ofEnableLighting();
            point_light_1_.setDiffuseColor(ofColor(255.f));
            point_light_1_.setSpecularColor(ofColor(255.f));
            point_light_1_.setPosition(1500.f, 100.f, 500.f);
            material_.setShininess(0);
            material_.setDiffuseColor(ofColor(255.f));
            material_.setSpecularColor(ofColor(255.f));
            point_light_1_.enable();
            material_.begin();
            model_loader_->disableMaterials();
            break;
        case DrawColorMode::kColor:
            ofEnableDepthTest();
            ofEnableLighting();

            ofSetSmoothLighting(true);
            point_light_1_.setDiffuseColor(ofFloatColor(.85, .85, .55));
            point_light_1_.setSpecularColor(ofFloatColor(1.f, 1.f, 1.f));

            point_light_2_.setDiffuseColor(ofFloatColor(238.f / 255.f, 57.f / 255.f, 135.f / 255.f));
            point_light_2_.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
            point_light_2_.setPosition(cos(ofGetElapsedTimef() * 1.5f) * win_cache_->getWidth() * .5f,
                                       sin(ofGetElapsedTimef() * 1.5f) * win_cache_->getWidth() * .5f,
                                       pfft_->getMidVal() * 500.f);

            point_light_3_.setDiffuseColor(ofFloatColor( 19.f / 255.f,  94.f / 255.f,  77.f / 255.f));
            point_light_3_.setSpecularColor(ofFloatColor(18.f / 255.f, 150.f / 255.f, 135.f / 255.f));
            point_light_3_.setPosition(win_cache_->getWidth()  * .5f + cos(ofGetElapsedTimef() * .15f) * (win_cache_->getWidth()*.3f),
                                       win_cache_->getHeight() * .5f + sin(ofGetElapsedTimef() * .7f)  * (win_cache_->getHeight()),
                                       cos(ofGetElapsedTimef() * .5f) * win_cache_->getWidth());

            material_.setShininess(120);
            material_.setSpecularColor(ofColor(255, 255, 255, 255));

            point_light_1_.enable();
            point_light_2_.enable();
            point_light_3_.enable();
            material_.begin();
            break;
    }
}

void IdolDrawer::lightEnd() {
    point_light_1_.disable();
    point_light_2_.disable();
    point_light_3_.disable();
    material_.end();

    ofDisableLighting();
    ofDisableDepthTest();
}

void IdolDrawer::drawIdol(const float color) {
    DrawColorMode mode = drawColorMode(color);
    switch(mode) {
        case DrawColorMode::kNoLight:
            model_loader_->drawVertices();
            break;
        case DrawColorMode::kWhite:
            model_loader_->drawFaces();
            break;
        case DrawColorMode::kShadow:
            model_loader_->drawFaces();
            break;
        case DrawColorMode::kShadow2:
            model_loader_->drawFaces();
            break;
        case DrawColorMode::kColor:
            model_loader_->drawFaces();
            break;
    }
}

IdolDrawer::DrawColorMode IdolDrawer::drawColorMode(const float color) {
    if (color < 0.1f) {
        return DrawColorMode::kNoLight;
    } else if (color < 0.3f) {
        return DrawColorMode::kWhite;
    } else if (color < 0.6f) {
        return DrawColorMode::kShadow;
    } else if (color < 0.8f) {
        return DrawColorMode::kShadow2;
    } else {
        return DrawColorMode::kColor;
    }
}

//--------------------------------------------------------------
void SoloIdolDrawer::setup() {
    // none
}

void SoloIdolDrawer::update() {
    updatePointSize(pfft_->getMidVal());

    if (level_ > 0.1f) {
        rorate_count_ += ofMap(level_, 0.0f, 1.0f, 0.5, 4.0f);
        rotateBody(rorate_count_);
    } else {
        rotateBody(0.f);
    }
}

void SoloIdolDrawer::draw() {
    cam_.begin();
    setCamCenter();

    lightBegin(color_);

    model_loader_->setPosition(0, 100, 0);
    model_loader_->setScale(1.f, 1.f, 1.f);
    drawIdol(color_);

    lightEnd();
    cam_.end();
}

//--------------------------------------------------------------
void TrioIdolDrawer::setup() {
    // none
}

void TrioIdolDrawer::update() {
    updatePointSize(pfft_->getMidVal());

    if (level_ > 0.1f) {
        rorate_count_ += ofMap(level_, 0.0f, 1.0f, 0.5, 4.0f);
        rotateBody(rorate_count_);
    } else {
        rotateBody(0.f);
    }
}

void TrioIdolDrawer::draw() {
    cam_.begin();
    setCamCenter();

    lightBegin(color_);

    model_loader_->setPosition(0, 100, 0);
    model_loader_->setScale(1.f, 1.f, 1.f);

    ofPushMatrix();
    ofTranslate(-800.f, 0, 0);
    for (int i = 0; i < 3; ++i) {
        drawIdol(color_);
        ofTranslate(800.f, 0, 0);
    }
    ofPopMatrix();

    lightEnd();
    cam_.end();
}

//--------------------------------------------------------------
void MoveIdolDrawer::setup() {
    initializePositionList();
}

void MoveIdolDrawer::update() {
    updatePointSize(pfft_->getMidVal());
    updatePositionList();
}

void MoveIdolDrawer::draw() {
    cam_.begin();
    setCamCenter();

    lightBegin(color_);

    model_loader_->setScale(0.7, 0.7, 0.7);

    for (const auto& position : position_list_) {
        model_loader_->setPosition(position.position_.x, position.position_.y, position.position_.z);
        rotateBody(position.rotation_);
        drawIdol(color_);
    }

    lightEnd();
    cam_.end();
}

void MoveIdolDrawer::initializePositionList() {
    position_list_.resize(8);
    float x_base = -1000.f;
    for (size_t i = 0; i < position_list_.size() / 2; ++i) {
        position_list_.at(i * 2 + 0).position_.set(x_base,         0.f,  500.f);
        position_list_.at(i * 2 + 1).position_.set(x_base + 600.f, 0.f, -300.f);
        x_base += 900.f;

        position_list_.at(i * 2 + 0).rotation_ = ofRandom(360);
        position_list_.at(i * 2 + 1).rotation_ = ofRandom(360);
    }
}

void MoveIdolDrawer::updatePositionList() {
    float x      = ofMap(level_, 0.0f, 1.0f, 5.f, 20.0f);
    float rotate = ofMap(level_, 0.0f, 1.0f, 0.5, 4.0f);

    for (auto& position : position_list_) {
        position.position_.x -= x;
        if (position.position_.x < -1800.f) {
            position.position_.x = 1800.f;
        }

        position.rotation_ += rotate;
    }
}

//--------------------------------------------------------------
void MultiAngleIdolDrawer::setup() {
    initializeAngleList();
}

void MultiAngleIdolDrawer::initializeAngleList() {
    set_angles_.push_back([this]() {setCamCenterFar();});

    set_angles_.push_back([this]() {setCamCenterNear(); });
    set_angles_.push_back([this]() {setCamLookDown(); });
    set_angles_.push_back([this]() {setCamLookDownFace(); });
    set_angles_.push_back([this]() {setCamLookUp(); });
    set_angles_.push_back([this]() {setCamLookUpSide(); });
    set_angles_.push_back([this]() {setCamSideUp(); });
    set_angles_.push_back([this]() {setCamSideBody(); });
    set_angles_.push_back([this]() {setCamDiagonallyAbove(); });
}

void MultiAngleIdolDrawer::update() {
    updatePointSize(pfft_->getMidVal());

    counter_.setThreshold(level_);
    if (counter_.update()) {
        angle_ = ofRandom(1, set_angles_.size() - 1);
    }
}

void MultiAngleIdolDrawer::draw() {
    if (pfft_->getLowVal() > 0.1) {
        drawIdol(angle_, color_);
    }
}

void MultiAngleIdolDrawer::drawIdol(const uint32_t angle_no, const float color) {
    cam_.begin();
    set_angles_.at(angle_no)();
    rotateBody(0.f);

    lightBegin(color);

    model_loader_->setPosition(0, 100, 0);
    model_loader_->setScale(1.f, 1.f, 1.f);
    IdolDrawer::drawIdol(color);

    lightEnd();
    cam_.end();
}
