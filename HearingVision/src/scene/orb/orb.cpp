#include "orb.h"

//--------------------------------------------------------------
OrbScene::OrbScene() {
    // nop
}

//--------------------------------------------------------------
OrbScene::~OrbScene() {
    // nop
}

//--------------------------------------------------------------
void OrbScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;
    setupNextScene();

    ofSetSmoothLighting(true);
    ofSetSphereResolution(64);

    point_light_.setDiffuseColor(ofColor(0.f, 255.f, 0.f));
    point_light_.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    point_light_.setPosition(0.f, 0.f, 0.f);

    light_color_.setBrightness(255.f);
    material_color_.setBrightness(255.f);
    material_.setShininess(128);

    for (size_t i = 0; i < kSphereNumMax; ++i) {
        Orb orb;
        while (true) {
            orb.position.x = ofRandom(-kAreaMax, kAreaMax);
            orb.position.y = ofRandom(-kAreaMax, kAreaMax);
            orb.position.z = ofRandom(-kAreaMax, kAreaMax);
            orb.radius = ofRandom(kSphereRadiusMax);
            if (!isOverrap(orbs_, orb)) {
                break;
            }
        }

        orb.hue             = ofRandom(255);
        orb.satuation       = ofRandom(255);
        orbs_.emplace_back(orb);
    }

    cam_distance_         = kAreaMax * 2;
    cam_holizontal_angle_ = 0.f;
    cam_vertical_angle_   = 0.f;
}

//--------------------------------------------------------------
void OrbScene::update(SceneParam scene_param) {
    level_ = scene_param.level_;

    updateColor();
    updatePosition();
    updateCamPosition();
}

void OrbScene::updateCamPosition() {
    cam_holizontal_angle_ += 0.001f;
    cam_vertical_angle_   += 0.0005f;

    cam_pos_.x = cos(cam_holizontal_angle_) * cam_distance_;
    cam_pos_.y = sin(cam_holizontal_angle_) * cam_distance_;
    cam_pos_.z = sin(cam_vertical_angle_)   * cam_distance_;
    cam_.setPosition(cam_pos_);
    cam_.setTarget(ofVec3f(0.f));
}

void OrbScene::updateColor() {
    std::vector<float> spectrums = pfft_->getSpectrum();

    for (size_t i = 0; i < orbs_.size(); ++i) {
        if (spectrums.at(i) > kSatuationThreshold) {
            orbs_.at(i).satuation += kSatuationUp;
        } else {
            orbs_.at(i).satuation += kSatuationDown;
        }

        if (orbs_.at(i).satuation > kSatuationMax) {
            orbs_.at(i).satuation = kSatuationMax;
        } else if (orbs_.at(i).satuation < kSatuationMin) {
            orbs_.at(i).satuation = kSatuationMin;
        }
    }
}

void OrbScene::updatePosition() {
    for (auto& orb : orbs_) {
        updateVelocity(orb);
        updatePosition(orb);
    }

    std::vector<float> spectrums = pfft_->getSpectrum();

    for (size_t i = 0; i < orbs_.size(); ++i) {
        updateReflection(orbs_, i);
        updateRepulsive(spectrums.at(i), orbs_.at(i));
    }
}

void OrbScene::updateVelocity(Orb& my_orb) {
    my_orb.velocity = (my_orb.velocity * kVelocityReduce * level_) +
                      (getGravity(my_orb.position) / (my_orb.radius * kMathEffection)) +
                      my_orb.reflection + 
                      my_orb.repulsive;
}

void OrbScene::updatePosition(Orb& my_orb) {
    my_orb.position = my_orb.position + my_orb.velocity;
}

void OrbScene::updateReflection(std::vector<Orb>& orbs, const size_t my_index) {
    ofVec3f reflection(0.f);
    Orb my_orb = orbs.at(my_index);
    for (size_t i = 0; i < orbs.size(); ++i) {
        if (i == my_index) {
            continue;
        }

        if (isOverrap(orbs.at(i), my_orb)) {
            float dist = ofDist(orbs.at(i).position.x, orbs.at(i).position.y, orbs.at(i).position.z,
                                my_orb.position.x, my_orb.position.y, my_orb.position.z);
            ofVec3f direction  = (my_orb.position - orbs.at(i).position).normalized();
            float overrap_dist = orbs.at(i).radius + my_orb.radius - dist;

            reflection += direction * (overrap_dist * kReflection * level_ / (my_orb.radius * kMathEffection));
        }
    }

    orbs.at(my_index).reflection = orbs.at(my_index).reflection * kFriction
                                   + reflection;
}

void OrbScene::updateRepulsive(const float spectrum, Orb& my_orb) {
    float dist = ofDist(my_orb.position.x, my_orb.position.y, my_orb.position.z,
                        0.f, 0.f, 0.f);
    if ((dist < kAreaMax) && (spectrum > kRepulsiveThreshold)) {
        ofVec3f direction = my_orb.position - ofVec3f(0.f);
        direction.normalize();
        my_orb.repulsive = direction * kRepulsiveForce * level_ * my_orb.radius;
    } else {
        my_orb.repulsive *= kRepulsiveReduce;
    }
}

ofVec3f OrbScene::getGravity(const ofVec3f my_position) const {
    ofVec3f gravity = ofVec3f(0.f) - my_position;
    gravity = gravity.normalized() * kGravity;

    return gravity;
}

bool OrbScene::isOverrap(const std::vector<Orb>& orbs, const Orb& my_orb) const {
    for (const auto& orb : orbs) {
        if (isOverrap(orb, my_orb)) {
            return true;
        }
    }

    return false;
}

bool OrbScene::isOverrap(const std::vector<Orb>& orbs, const size_t my_index) const {
    Orb my_orb = orbs.at(my_index);
    for (size_t i = 0; i < orbs.size(); ++i) {
        if (i == my_index) {
            continue;
        }

        if (isOverrap(orbs.at(i), my_orb)) {
            return true;
        }
    }

    return false;
}

bool OrbScene::isOverrap(const Orb& orb1, const Orb& orb2) const {
    float dist = ofDist(orb1.position.x, orb1.position.y, orb1.position.z,
                        orb2.position.x, orb2.position.y, orb2.position.z);
    float threshold = orb1.radius + orb2.radius - 3.f;  // allow bit overrap

    if (dist < threshold) {
        return true;
    }

    return false;
}

//--------------------------------------------------------------
void OrbScene::draw() {
    cam_.begin();
    ofEnableDepthTest();

    ofEnableLighting();

    ofFill();
    for (const auto& orb : orbs_) {
        light_color_.setHue(orb.hue);
        light_color_.setSaturation(orb.satuation);
        point_light_.setDiffuseColor(light_color_);

        material_color_.setHue(orb.hue);
        material_color_.setSaturation(orb.satuation);
        material_.setSpecularColor(material_color_);

        point_light_.enable();
        material_.begin();

        ofDrawSphere(orb.position.x, orb.position.y, orb.position.z, orb.radius);

        point_light_.disable();
        material_.end();
    }


    ofDisableLighting();
    ofDisableDepthTest();

    cam_.end();
}

//--------------------------------------------------------------
void OrbScene::setupNextScene() {
    NextScene::NextSceneList next_scene_list;
    next_scene_list.push_back("D");
    next_scene_list.push_back("E3");
    next_scene_list.push_back("X");

    next_scene_.setNextSceneList(next_scene_list);
}

//--------------------------------------------------------------
