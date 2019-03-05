#include "xflash.h"

//--------------------------------------------------------------
XFlashScene::XFlashScene() : 
    speed_(0.0f),
    friction_(0.1f) {
    // nop
}

//--------------------------------------------------------------
XFlashScene::~XFlashScene() {
    // nop
}

//--------------------------------------------------------------
void XFlashScene::setup(std::shared_ptr<ProcessFFT> pfft,
                        std::shared_ptr<ofRectangle> win_cache) {

    pfft_ = pfft;
    win_cache_ = win_cache;

    createBasePoint(kBasePointNum);
    createParticle(kParticleNum);
}

void XFlashScene::createBasePoint(const int num) {
    base_points_.resize(num);
    for (auto& point : base_points_) {
        point.x = ofRandom(win_cache_->getWidth());
        point.y = ofRandom(win_cache_->getHeight());
    }
}

void XFlashScene::createDotPoint(const int num) {
    dot_points_.resize(num);
    for (int i = 0; i < num; ++i) {
        int base_index = static_cast<int>(ofRandom(base_points_.size()));
        dot_points_.at(i).x = base_points_.at(base_index).x + speed_ * ofRandom(-1.0, 1.0);
        dot_points_.at(i).y = base_points_.at(base_index).y + speed_ * ofRandom(-1.0, 1.0);
    }
}

void XFlashScene::createParticle(const int num) {
    particles_ptr_.reset(new Particles(num));
    ofVec2f velocity(0.f, 0.f);
    ofColor color(255);
    for (int i = 0; i < num; ++i) {
        particles_ptr_->addParticle(ofVec3f(base_points_.at(static_cast<int>(ofRandom(base_points_.size()))).x,
                                            base_points_.at(static_cast<int>(ofRandom(base_points_.size()))).y),
                                    velocity,
                                    color);
    }
}

//--------------------------------------------------------------
void XFlashScene::update(SceneParam scene_param) {
    if (scene_param.reset_ == SceneParam::TriggerState::kOn) {
        reset();
    }

    speed_    = pfft_->getLowVal()  * 70.0;
    friction_ = pfft_->getHighVal() * 3.0;

    if (pfft_->getLowVal() > 0.05) {
        createDotPoint(static_cast<int>(ofRandom(base_points_.size())));
    }

    particles_ptr_->resetForce();
    particles_ptr_->friction = friction_;
    
    int base_index = 0;
    for (int particle_index = 0; particle_index < kParticleNum; ++particle_index) {
        ofVec2f pPoint   = ofVec2f(particles_ptr_->positions.at(particle_index).x,
                                   particles_ptr_->positions.at(particle_index).y);
        ofVec2f tgtPoint = ofVec2f(base_points_.at(base_index).x,
                                   base_points_.at(base_index).y);

        int deg = atan2((pPoint.y - tgtPoint.y), (pPoint.x - tgtPoint.x)) * 180 / PI;
        deg += 180;

        ofVec3f force;
        force.x = cos(deg * PI / 180) *  speed_ / ofRandom(100);
        force.y = sin(deg * PI / 180) *  speed_ / ofRandom(100);

        particles_ptr_->addForce(particle_index, force);
        base_index += 10;
        if (base_index >= base_points_.size()) {
            base_index = 0;
        }
    }

    particles_ptr_->addDampingForce();
    particles_ptr_->updatePos();
    particles_ptr_->resetOffWalls();
}

//--------------------------------------------------------------
void XFlashScene::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0, 0, 0, 70);
    ofRect(0, 0, win_cache_->getWidth(), win_cache_->getHeight());
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    ofSetColor(255, ofMap(pfft_->getMidVal(), 0.0, 0.5, 128, 255));

    drawLineBetweenDots(pfft_->getMidVal());

    if (pfft_->getLowVal() < 0.3) {
        particles_ptr_->draw();
    } else {
        drawCircleOnDots(pfft_->getLowVal());
   }
}

void XFlashScene::drawLineBetweenDots(const float level_val) {
    int dot_index_max = static_cast<int>(ofMap(level_val,
                                               0.0f, 1.0f,
                                               dot_points_.size() * 0.1f, dot_points_.size() * 0.3));
    for (int dot_index = 1; dot_index < dot_index_max; ++dot_index) {
        ofSetLineWidth(ofRandom(0.1, 2.0));
        ofDrawLine(dot_points_.at(dot_index),
                       dot_points_.at(dot_index - 1));
    }
}

void XFlashScene::drawCircleOnDots(const float level_val) {
    int dot_size_max = static_cast<int>(ofMap(level_val,
                                              0.0f, 1.0f,
                                              1.0f, 50.0f));
    int dot_index_max = static_cast<int>(ofMap(level_val,
                                               0.0f, 1.0f,
                                               0.0f, dot_points_.size() * 0.5));
    for (int dot_index = 0; dot_index < dot_index_max; ++dot_index) {
        ofDrawCircle(dot_points_.at(dot_index).x + speed_ * ofRandom(-1.0, 1.0),
                     dot_points_.at(dot_index).y + speed_ * ofRandom(-1.0, 1.0),
                     ofRandom(dot_size_max * 0.1, dot_size_max));
    }
}

//--------------------------------------------------------------
void XFlashScene::resize(){
    createBasePoint(kBasePointNum);
    createParticle(kParticleNum);
}

