#include "debri.h"

//--------------------------------------------------------------
DebriScene::DebriScene() {
    // nop
}

//--------------------------------------------------------------
DebriScene::~DebriScene() {
    // nop
}

//--------------------------------------------------------------
void DebriScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    shader_.setGeometryInputType(GL_TRIANGLES);
    shader_.setGeometryOutputType(GL_TRIANGLE_STRIP);
    shader_.setGeometryOutputCount(6);
    shader_.load("shader/debri/draw.vert", "shader/debri/draw.frag", "shader/debri/draw.geom");

    vbo_mesh_.setMode(OF_PRIMITIVE_TRIANGLES);

    img_.load("image/debri/earth.png");

    cam_.setNearClip(0);
    cam_.setFarClip(1500);

    sats_.resize(kDebriNum);
    color_list_.resize(kDebriNum);
    for (int i = 0; i < sats_.size() / 2; ++i) {
        float r = ofRandom(kGeoStationaryDistMin, kGeoStationaryDistMax);
        float y = ofRandom(-kGeoStationaryBand, kGeoStationaryBand);
        float phi = ofRandom(0, 2 * PI);
        sats_.at(i).set(r * cos(phi), y, r * sin(phi));
    }
    for (int i = sats_.size() / 2; i < sats_.size(); ++i) {
        float r = ofRandom(kLeoDistMin, kLeoDistMax);
        float phi1 = ofRandom(0, 2 * PI);
        float phi2 = ofRandom(0, 2 * PI);

        sats_.at(i).set(r * cos(phi1)*cos(phi2), r * sin(phi1)*cos(phi2), r * sin(phi2));
    }

    color_list_.assign(kDebriNum, ofFloatColor(1.0, 1.0));

    vbo_.setVertexData(sats_.data(), sats_.size(), GL_DYNAMIC_DRAW);
    vbo_.setColorData(color_list_.data(), color_list_.size(), GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void DebriScene::update(SceneParam scene_param) {
    time_ += 0.01f;

    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        normal_mode_ = !normal_mode_;
    }

    if (normal_mode_) {
        point_size_ = ofMap(pfft_->getLowVal(), 0.0, 0.2, 1.f, 2.f, true);
    } else {
        point_size_ = 1.f;
    }

    if (normal_mode_) {
        scale_ = ofMap(pfft_->getLowVal(), 0.0, 0.3, 0.0, kSparkDist * scene_param.level_, true);
    } else {
        scale_ = kFloatingDist;
    }

    float alpha = ofMap(pfft_->getMidVal(), 0.0, 0.2, 0.6, 0.9, true);
    if (normal_mode_ && (pfft_->getLowVal() > 0.15f)) {
        ofFloatColor c;
        c.setHsb(0.95f, 0.8f, 0.7f, alpha);
        color_list_.assign(kDebriNum, c);
    } else {
        color_list_.assign(kDebriNum, ofFloatColor(1.0, alpha));
    }
    vbo_.updateColorData(color_list_.data(), color_list_.size());

    if (normal_mode_ && (scene_param.level_ > 0.3f)) {
        cam_pos_.set(ofRandom(-20.f, 20.f) * scene_param.level_,
                     ofRandom(-20.f, 20.f) * scene_param.level_,
                     -400.f);
    } else {
        cam_pos_.set(0, 0, -400.f);
    }
}

//--------------------------------------------------------------
void DebriScene::draw() {
    ofEnableAlphaBlending();
    ofEnableDepthTest();

    cam_.begin();
    cam_.setFov(90);
    cam_.setPosition(cam_pos_);
    cam_.lookAt(ofVec3f(0, 0, 0));

    // model Matrix
    ofMatrix4x4 modelMatrix;
    modelMatrix.translate(0, 0, 0);
    modelMatrix.rotate(time_ * 10, 0.0, 1.0, 0.0);
    sphere_.setPosition(ofVec3f(0, 0, 0));
    sphere_.set(kEarthRadius, 32);
    sphere_.mapTexCoordsFromTexture(img_.getTexture());

    // view Matrix
    ofMatrix4x4 viewMatrix;
    viewMatrix = ofGetCurrentViewMatrix();

    // projection Matrix
    ofMatrix4x4 projectionMatrix;
    projectionMatrix = cam_.getProjectionMatrix();

    // mvp Matrix
    ofMatrix4x4 mvpMatrix;
    mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;

    shader_.begin();
    shader_.setUniformMatrix4f("model", modelMatrix);
    shader_.setUniformMatrix4f("view", viewMatrix);
    shader_.setUniformMatrix4f("projection", projectionMatrix);
    shader_.setUniformMatrix4f("mvp", mvpMatrix);
    shader_.setUniform1i("scale", scale_);
    shader_.setUniform1i("NormalMode", normal_mode_);
    shader_.setUniformTexture("tex0", img_.getTexture(), 0);

    vbo_mesh_ = sphere_.getMesh();
    for (int i = 0; i<vbo_mesh_.getVertices().size(); ++i) {
        vbo_mesh_.addColor(ofColor(255, 220));
    }
    vbo_mesh_.drawWireframe();

    shader_.end();
    cam_.end();

    cam_.begin();
    ofPushMatrix();
    ofRotate(time_ * 10, 0.0, 1.0, 0.0);

    glPointSize(point_size_);
    vbo_.draw(GL_POINTS, 0, sats_.size());

    ofPopMatrix();

    cam_.end();

    ofDisableAlphaBlending();
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void DebriScene::setupNextScene() {
    NextScene::NextSceneList next_scene_list;
    next_scene_list.push_back("O");
    next_scene_list.push_back("Q");

    next_scene_.setNextSceneList(next_scene_list);
}

//--------------------------------------------------------------
