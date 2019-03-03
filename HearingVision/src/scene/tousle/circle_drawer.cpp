#include "circle_drawer.h"

//--------------------------------------------------------------
void MovingCircleDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;
}

//--------------------------------------------------------------
void MovingCircleDrawer::update() {
    auto itr = circle_list_.begin();
    while (itr != circle_list_.end()) {
        itr->pos_.x += itr->speed_;
        if (isOutOfWindow(*itr)) {
            itr = circle_list_.erase(itr);
        } else {
            ++itr;
        }
    }
}

//--------------------------------------------------------------
void MovingCircleDrawer::draw() {
    ofSetColor(255, 64);
    ofSetLineWidth(2.f);
    ofNoFill();
    for (auto& circle : circle_list_) {
        ofDrawCircle(circle.pos_, circle.radius_);
    }
}

//--------------------------------------------------------------
void MovingCircleDrawer::addCircle(const std::vector<float>& spectrum,
                                   const float level) {
    float sampling_step = 10.f - (9.f * level);
    ofColor color(255, 64);
    for (size_t i = 0; i < spectrum.size(); i += sampling_step) {
        if (spectrum.at(i) > 0.2) {
            float speed_direct = 1.0;
            if (ofRandomf() > 0.5) speed_direct = -1.0;
            Circle circle(ofVec2f(win_cache_->getWidth() * 0.5,
                                  win_cache_->getHeight() * 0.75 + ofRandomf() * 100.f),
                          ofMap(spectrum.at(i), 0.1f, 0.5f, 50.f, 200.f),
                          10.f * spectrum.at(i) * speed_direct,
                          color);
            circle_list_.emplace_back(circle);
        }
    }
}

//--------------------------------------------------------------
bool MovingCircleDrawer::isOutOfWindow(const Circle& circle) const {
    if (circle.speed_ > 0) {
        float x = circle.pos_.x - circle.radius_;
        if (x > win_cache_->getWidth()) {
            return true;
        }
    } else {
        float x = circle.pos_.x + circle.radius_;
        if (x < 0) {
            return true;
        }
    }

    return false;
}



//--------------------------------------------------------------
void VanishingCircleDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;
}

//--------------------------------------------------------------
void VanishingCircleDrawer::update() {
    auto itr = circle_list_.begin();
    while (itr != circle_list_.end()) {
        if (itr->color_.a < 3) {
            itr = circle_list_.erase(itr);
        } else {
            itr->color_.a -= 3;
            ++itr;
        }
    }
}

//--------------------------------------------------------------
void VanishingCircleDrawer::draw() {
    ofFill();
    for (auto& circle : circle_list_) {
        ofSetColor(circle.color_);
        ofDrawCircle(circle.pos_, circle.radius_);
    }
}

//--------------------------------------------------------------
void VanishingCircleDrawer::addCircle(const std::vector<float>& spectrum,
                                      const float level) {
    std::vector<float>draw_pos_x_list = createDrawPosXList();
    float  draw_pos_y = win_cache_->getHeight() / 2.f;

    size_t draw_pos_x_index = 0;
    size_t next_draw_pos_count = spectrum.size() / 8.f;

    ofColor draw_color;
    float base_color_h = 30.f * ofRandomf();
    draw_color.setHsb(base_color_h, 200.f, 255.f, 128.f);

    float sampling_step = 10.f - (9.f * level);
    for (size_t i = 0; i < spectrum.size(); i += sampling_step) {
        if (i > next_draw_pos_count) {
            next_draw_pos_count += spectrum.size() / 8.f;

            base_color_h = 30.f * ofRandomf();

            ++draw_pos_x_index;

            float y = ofMap(ofRandomf(), 0.f, 1.f, 0.f, win_cache_->getWidth() * 0.3f);
            float coin = ofRandomf();
            if (coin > 0.5) {
                draw_pos_y = win_cache_->getHeight() / 2.f + y;
            } else {
                draw_pos_y = win_cache_->getHeight() / 2.f - y;
            }
        }

        if (spectrum.at(i) > 0.2) {
            draw_color.setHue(base_color_h + spectrum.at(i) * 10.f);
            ofVec2f pos(draw_pos_x_list.at(draw_pos_x_index), draw_pos_y);

            Circle circle(pos,
                          ofMap(spectrum.at(i), 0.1f, 0.5f, 5.f, 80.f),
                          0.f,
                          draw_color);
            circle_list_.emplace_back(circle);
        }
    }
}

std::vector<float> VanishingCircleDrawer::createDrawPosXList() const {
    std::vector<float>draw_pos_x_list(8);
    float  width = win_cache_->getWidth() / 8.f;
    for (auto& x : draw_pos_x_list) {
        x = width * ofRandom(0.5f, 8.5f);
    }

    return draw_pos_x_list;
}

