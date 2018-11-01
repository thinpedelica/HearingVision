#pragma once
#include "ofMain.h"

class Particles {
public:
    Particles(const int maxParticles);
    void update();
    void draw();
    void addParticle(const ofVec2f& position,
                     const ofVec2f& velocity,
                     const ofColor& color);
    void resetForce();
    void addForce(const int count, const ofVec2f& force);
    void addDampingForce();
    void resetOffWalls();
    void updatePos();

    int maxParticles;
    int numParticles;
    float pointSize;
    float friction;

    deque<ofVec2f> positions;
    deque<ofVec2f> velocitys;
    deque<ofVec2f> forces;
    deque<ofColor> colors;

    ofVboMesh mesh;
};