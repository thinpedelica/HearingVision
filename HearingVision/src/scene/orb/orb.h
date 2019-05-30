#pragma once

#include "../base_scene.h"

class OrbScene : public BaseScene {
public:
    OrbScene();
    ~OrbScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();

private:
    static constexpr uint32_t kSphereNumMax = 255;
    static constexpr float kSphereRadiusMax = 150.f;
    static constexpr float kAreaMax         = 1000.f;

    static constexpr float kGravity    = 6.f;
    static constexpr float kFriction   =  .1f;
    static constexpr float kReflection = 7.f;

    static constexpr float kVelocityReduce = 0.8f;
    static constexpr float kMathEffection  = 0.1f;

    static constexpr float kRepulsiveForce = 2.f;
    static constexpr float kRepulsiveReduce = 0.7f;
    static constexpr float kRepulsiveThreshold = 0.2f;

    static constexpr float kSatuationMin  = 10.f;
    static constexpr float kSatuationMax  = 250.f;
    static constexpr float kSatuationUp   = 5.f;
    static constexpr float kSatuationDown = -1.f;
    static constexpr float kSatuationThreshold = 0.1f;

    struct Orb {
        ofVec3f position;
        ofVec3f velocity;
        ofVec3f reflection;
        ofVec3f repulsive;
        float   radius;
        float   hue;
        float   satuation;
    };

    std::vector<Orb> orbs_;

    void updateColor();
    void updatePosition();

    void updateVelocity(Orb& my_orb);
    void updatePosition(Orb& my_orb);
    void updateReflection(std::vector<Orb>& orbs, const size_t my_index);
    void updateRepulsive(const float spectrum, Orb& my_orb);

    ofVec3f getGravity(const ofVec3f my_position) const;

    bool isOverrap(const std::vector<Orb>& orbs, const Orb& my_orb) const;
    bool isOverrap(const std::vector<Orb>& orbs, const size_t my_index) const;
    bool isOverrap(const Orb& orb1, const Orb& orb2) const;

    void updateCamPosition();

    ofLight point_light_;
    ofMaterial material_;

    ofColor light_color_;
    ofColor material_color_;
    float hue_;

    float level_;

    float cam_distance_;
    float cam_holizontal_angle_;
    float cam_vertical_angle_;
    ofVec3f cam_pos_;
    ofEasyCam cam_;
};
