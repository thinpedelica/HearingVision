#pragma once

#include "../base_scene.h"
#include "../util/pingPongBuffer.h"
#include "ofxProcessFFT.h"

#define _PROGRAMMABLE_RENDERER

class CircleScene : public BaseScene {
public:
    CircleScene();
    ~CircleScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr size_t kParticleNum = 1000000;
    static constexpr float  kRadius = 400.f;
    static constexpr float  kMovingVelocity = 7.f;
    static constexpr float  kDecelerationRatio = 0.98f;
    static constexpr float  kBaseTargetPos = 3.0f;

    void setupPingPongBuffer(const size_t texture_resolution);
    void setupParticleMesh(const size_t texture_resolution);
    void setupBoardMesh(const size_t texture_resolution);

    void updateParticlePos(const ofVec2f& target_pos, const float velocity, const bool is_move);
    void drawParticle(const ofColor& color);

    void setupNextScene();

    ofEasyCam cam_;

    PingPongBuffer ping_pong_buffer_;
    ofShader update_shader_;
    ofShader draw_shader_;

    ofVboMesh particle_mesh_;
    ofVboMesh board_mesh_;

    float   particle_velocity_;
    ofColor particle_color_;
};
