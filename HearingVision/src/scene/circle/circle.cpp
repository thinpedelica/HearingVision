#include "circle.h"

//--------------------------------------------------------------
CircleScene::CircleScene() {
    // nop
}
//--------------------------------------------------------------
CircleScene::~CircleScene() {
    // nop
}
//--------------------------------------------------------------
void CircleScene::setup(std::shared_ptr<ProcessFFT> pfft,
                        std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    update_shader_.load("shader/circle/update");
    draw_shader_.load("shader/circle/draw");

    size_t texture_resolution = ceil(sqrt(kParticleNum));
    setupPingPongBuffer(texture_resolution);
    setupParticleMesh(texture_resolution);
    setupBoardMesh(texture_resolution);
}

void CircleScene::setupPingPongBuffer(const size_t texture_resolution) {
    ping_pong_buffer_.allocate(texture_resolution, texture_resolution, GL_RGBA32F_ARB);

    float* initial_values = new float[texture_resolution * texture_resolution * 4];
    for (size_t i = 0; i < texture_resolution; ++i) {
        for (size_t j = 0; j < texture_resolution; ++j) {
            size_t index = texture_resolution * i + j;
            float  radius = kRadius * static_cast<float>(i) / static_cast<float>(texture_resolution) - kRadius;
            float  angle = 360.f * static_cast<float>(j) / static_cast<float>(texture_resolution);

            initial_values[index * 4 + 0] = radius * sin(angle);
            initial_values[index * 4 + 1] = radius * cos(angle);
            initial_values[index * 4 + 2] = 0.f; // x vec
            initial_values[index * 4 + 3] = 0.f; // y vec
        }
    }
    ping_pong_buffer_.src->getTextureReference(0).loadData(initial_values, 
                                                           texture_resolution,
                                                           texture_resolution,
                                                           GL_RGBA);

    delete[] initial_values;
}

void CircleScene::setupParticleMesh(const size_t texture_resolution) {
    particle_mesh_.setMode(OF_PRIMITIVE_POINTS);
    for (size_t i = 0; i < texture_resolution; ++i) {
        for (size_t j = 0; j < texture_resolution; ++j) {
            particle_mesh_.addVertex(ofVec3f(0, 0, 0));
            particle_mesh_.addTexCoord(ofVec2f(i, j));
        }
    }
}

void CircleScene::setupBoardMesh(const size_t texture_resolution) {
    board_mesh_.setMode(OF_PRIMITIVE_TRIANGLES);
    board_mesh_.addVertex(ofVec3f(-1.f, -1.f, 0.f));
    board_mesh_.addVertex(ofVec3f( 1.f, -1.f, 0.f));
    board_mesh_.addVertex(ofVec3f( 1.f,  1.f, 0.f));
    board_mesh_.addVertex(ofVec3f(-1.f,  1.f, 0.f));
    board_mesh_.addTexCoord(ofVec2f(0.f,                0.f));
    board_mesh_.addTexCoord(ofVec2f(texture_resolution, 0.f));
    board_mesh_.addTexCoord(ofVec2f(texture_resolution, texture_resolution));
    board_mesh_.addTexCoord(ofVec2f(0.f,                texture_resolution));
    board_mesh_.addIndex(0);
    board_mesh_.addIndex(1);
    board_mesh_.addIndex(2);
    board_mesh_.addIndex(0);
    board_mesh_.addIndex(2);
    board_mesh_.addIndex(3);
}

//--------------------------------------------------------------
void CircleScene::update(SceneParam scene_param) {
    // to generate circular motion
    float target_pos_low  = -kBaseTargetPos + kBaseTargetPos * scene_param.level_;
    float target_pos_high =  kBaseTargetPos - kBaseTargetPos * scene_param.level_;
    ofVec2f target_pos{ ofRandom(target_pos_low, target_pos_high), 
                        ofRandom(target_pos_low, target_pos_high) };

    float threshold = 0.2f - scene_param.threshold_ * 0.1f;
    bool is_move_particle;
    if (pfft_->getLowVal() > threshold) {
        is_move_particle = true;
    } else if (pfft_->getMidVal() > threshold) {
        is_move_particle = true;
    }

    if (is_move_particle) {
        particle_velocity_ = kMovingVelocity;
    } else {
        particle_velocity_ *= kDecelerationRatio;
    }

    if (scene_param.color_ < 0.1f) {
        particle_color_.set(255, 128);
    } else {
        float hue_min = scene_param.color_ * 128.f;
        float hue_max = hue_min + 128.f;
        particle_color_.setHsb(ofMap(pfft_->getMidVal(), 0.0, 1.0, hue_min, hue_max), 255, 255, 128);
    }
    updateParticlePos(target_pos, particle_velocity_, is_move_particle);
}

void CircleScene::updateParticlePos(const ofVec2f& target_pos, const float velocity, const bool is_move) {
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_BLEND);

    ping_pong_buffer_.dst->begin();
    ping_pong_buffer_.dst->activateAllDrawBuffers();
    ofClear(0);

    update_shader_.begin();
    update_shader_.setUniformTexture("u_pos_tex", ping_pong_buffer_.src->getTextureReference(0), 0);
    update_shader_.setUniform2fv("u_target_pos",  target_pos.getPtr());
    update_shader_.setUniform1f("u_velocity",     velocity);
    update_shader_.setUniform1i("u_is_move",      is_move);
    board_mesh_.draw();
    update_shader_.end();

    ping_pong_buffer_.dst->end();
    ping_pong_buffer_.swap();

    glPopAttrib();
}

//--------------------------------------------------------------
void CircleScene::draw() {
    cam_.begin();
    drawParticle(particle_color_);
    cam_.end();
}

void CircleScene::drawParticle(const ofColor& color) {
    ofSetColor(color);

    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();

    draw_shader_.begin();
    draw_shader_.setUniformTexture("u_pos_tex", ping_pong_buffer_.src->getTextureReference(0), 0);
    particle_mesh_.draw();
    draw_shader_.end();

    ofDisablePointSprites();
    ofPopStyle();
}

//--------------------------------------------------------------
