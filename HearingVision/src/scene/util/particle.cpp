#include "particle.h"

Particles::Particles(const int _maxParticles) {
    maxParticles = _maxParticles;
    numParticles = 0;
    friction = 0.01;
    mesh.setMode(OF_PRIMITIVE_POINTS);
}

void Particles::resetForce() {
    for (int i = 0; i < positions.size(); ++i) {
        forces.at(i) = ofVec2f(0, 0);
    }
}

void Particles::addForce(const int count, const ofVec2f& force) {
    forces.at(count) += force;
}

void Particles::addDampingForce() {
    for (int i = 0; i < positions.size(); ++i) {
        forces.at(i) -= velocitys.at(i) * friction;
    }
}

void Particles::resetOffWalls() {
    float minx = 0;
    float miny = 0;
    float maxx = ofGetWidth();
    float maxy = ofGetHeight();

    for (int i = 0; i < positions.size(); ++i) {
        if (positions.at(i).x > maxx) {
            positions.at(i).x = minx;
        }
        if (positions.at(i).y > maxy) {
            positions.at(i).y = miny;
        }
        if (positions.at(i).x < minx) {
            positions.at(i).x = maxx;
        }
        if (positions.at(i).y < miny) {
            positions.at(i).y = maxy;
        }
    }
}

void Particles::updatePos() {
    for (int i = 0; i < positions.size(); ++i) {
        velocitys.at(i) += forces.at(i);
        positions.at(i) += velocitys.at(i);
    }
}

void Particles::update() {
    for (int i = 0; i < positions.size(); ++i) {
        forces.at(i) = ofVec2f(0, 0);
        forces.at(i) -= velocitys.at(i) * friction;
        velocitys.at(i) += forces.at(i);
        positions.at(i) += velocitys.at(i);
    }
}

void Particles::draw() {
    mesh.clear();
    for (int i = 0; i < positions.size(); ++i) {
        mesh.addVertex(ofVec3f(positions.at(i).x, positions.at(i).y));
        mesh.addColor(ofFloatColor(1.0, 1.0, 1.0));
    }
    mesh.draw();
}

void Particles::addParticle(const ofVec2f& _position,
                            const ofVec2f& _velocity,
                            const ofColor& _color) {
    positions.push_back(_position);
    velocitys.push_back(_velocity);
    colors.push_back(_color);
    forces.push_back(ofVec2f(0, 0));
    ++numParticles;

    if (positions.size() > maxParticles) {
        positions.pop_front();
        colors.pop_front();
        velocitys.pop_front();
        forces.pop_front();
        --numParticles;
    }
}
