#include "zcoming.h"

//--------------------------------------------------------------
ZcomingScene::ZcomingScene() {
    // nop
}

//--------------------------------------------------------------
ZcomingScene::~ZcomingScene() {
    // nop
}

//--------------------------------------------------------------
void ZcomingScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    cur_color_.set(1.0, 0.6);
}

//--------------------------------------------------------------
void ZcomingScene::update(SceneParam scene_param) {
    z_step_    = kBaseZStep + kBaseZStep * scene_param.level_;
    gen_cnt_   = kBaseGenCnt - kBaseGenCnt * scene_param.level_ * 0.7;
    threshold_ = kBaseThreshold + scene_param.threshold_;

    cur_cnt_ += 1;
    if (cur_cnt_ > gen_cnt_) {
        cur_cnt_ = 0;
        createDepthLine(1.f, 1.f);
        createDepthCircle(1.f, 1.f);
    }

    if (pfft_->getLowVal() > threshold_) {
        if (toggle_low_ == false) {
            createTriangle(1.f, 1.f);
            toggle_low_ = true;
        }
    } else {
        toggle_low_ = false;
    }
    if (pfft_->getMidVal() > threshold_) {
        if (toggle_mid_ == false) {
            createSquare(1.f, 1.f);
            toggle_mid_ = true;
        }
    } else {
        toggle_mid_ = false;
    }
    if (pfft_->getHighVal() > threshold_) {
        if (toggle_high_ == false) {
            createDepthScrew(1.f, 1.f);
            toggle_high_ = true;
        }
    } else {
        toggle_high_ = false;
    }

    updateZpos();
    eraseOvers();
    line_colors_.assign(line_points_.size(), cur_color_);
    vbo_line_.setVertexData(line_points_.data(), line_points_.size(), GL_DYNAMIC_DRAW);
    vbo_line_.setColorData(line_colors_.data(), line_colors_.size(), GL_DYNAMIC_DRAW);

    depth_line_colors_.assign(depth_line_points_.size(), cur_color_);
    vbo_depth_line_.setVertexData(depth_line_points_.data(), depth_line_points_.size(), GL_DYNAMIC_DRAW);
    vbo_depth_line_.setColorData(depth_line_colors_.data(), depth_line_colors_.size(), GL_DYNAMIC_DRAW);

}

void ZcomingScene::updateZpos() {
    for (auto& point : line_points_) {
        point.z -= z_step_;
    }

    for (auto& point : depth_line_points_) {
        point.z -= z_step_;
    }
}

void ZcomingScene::eraseOvers() {
    auto itr = line_points_.begin();
    while (itr != line_points_.end()) {
        if (itr->z < z_end_) {
            itr = line_points_.erase(itr);
        } else {
            ++itr;
        }
    }

    auto dl_itr = depth_line_points_.begin();
    while ((dl_itr != depth_line_points_.end()) &&
           ((dl_itr + 1) != depth_line_points_.end())) {
        if ((dl_itr+1)->z < z_end_) {
            dl_itr = depth_line_points_.erase(dl_itr);
            dl_itr = depth_line_points_.erase(dl_itr);
        } else {
            dl_itr += 2;
        }
    }
}

void ZcomingScene::createTriangle(const float exp_ratio, const float angle) {
    // TODO use ratio and angle
    for (int i = 0; i < 30; ++i) {
        line_points_.push_back(ofVec3f(   0.f,  100.f, 500.f + kDepthInterval * i));
        line_points_.push_back(ofVec3f(-100.f, -100.f, 500.f + kDepthInterval * i));

        line_points_.push_back(ofVec3f(-100.f, -100.f, 500.f + kDepthInterval * i));
        line_points_.push_back(ofVec3f( 100.f, -100.f, 500.f + kDepthInterval * i));

        line_points_.push_back(ofVec3f(100.f,  -100.f, 500.f + kDepthInterval * i));
        line_points_.push_back(ofVec3f(   0.f,  100.f, 500.f + kDepthInterval * i));
    }
}

void ZcomingScene::createSquare(const float exp_ratio, const float angle) {
    // TODO use ratio and angle
    for (int i = 0; i < 30; ++i) {
        line_points_.push_back(ofVec3f(-50.f,  50.f, 500.f + kDepthInterval * i));
        line_points_.push_back(ofVec3f(-50.f, -50.f, 500.f + kDepthInterval * i));

        line_points_.push_back(ofVec3f(-50.f, -50.f, 500.f + kDepthInterval * i));
        line_points_.push_back(ofVec3f( 50.f, -50.f, 500.f + kDepthInterval * i));

        line_points_.push_back(ofVec3f( 50.f, -50.f, 500.f + kDepthInterval * i));
        line_points_.push_back(ofVec3f( 50.f,  50.f, 500.f + kDepthInterval * i));

        line_points_.push_back(ofVec3f( 50.f,  50.f, 500.f + kDepthInterval * i));
        line_points_.push_back(ofVec3f(-50.f,  50.f, 500.f + kDepthInterval * i));
    }
}

void ZcomingScene::createDepthLine(const float exp_ratio, const float angle) {
    // TODO use ratio and angle
    for (int i = 0; i < 50; ++i) {
        float offset = ofRandom(100.f);
        depth_line_points_.push_back(ofVec3f(-100.f - i,  100.f - i,  500.f + offset));
        depth_line_points_.push_back(ofVec3f(-100.f - i,  100.f - i, 1000.f + offset));

        depth_line_points_.push_back(ofVec3f(-100.f - i, -100.f + i,  500.f + offset));
        depth_line_points_.push_back(ofVec3f(-100.f - i, -100.f + i, 1000.f + offset));

        depth_line_points_.push_back(ofVec3f( 100.f + i, -100.f + i,  500.f + offset));
        depth_line_points_.push_back(ofVec3f( 100.f + i, -100.f + i, 1000.f + offset));

        depth_line_points_.push_back(ofVec3f( 100.f + i,  100.f - i,  500.f + offset));
        depth_line_points_.push_back(ofVec3f( 100.f + i,  100.f - i, 1000.f + offset));
    }
}

void ZcomingScene::createDepthCircle(const float exp_ratio, const float angle) {
    for (int i = 0; i < 300; ++i) {
        float x = 100 * cos(i * 2 * PI / 100.f);
        float y = 100 * sin(i * 2 * PI / 100.f);

        depth_line_points_.push_back(ofVec3f(x, y, 500.f));
        depth_line_points_.push_back(ofVec3f(x, y, 510.f));
        depth_line_points_.push_back(ofVec3f(x, y, 520.f));
        depth_line_points_.push_back(ofVec3f(x, y, 530.f));
    }
}

void ZcomingScene::createDepthScrew(const float exp_ratio, const float angle) {
    for (int i = 0; i < 300; ++i) {
        float x1 = i * 0.2 * cos(i * 2 * PI / 200.f);
        float y1 = i * 0.2 * sin(i * 2 * PI / 200.f);
        float x2 = i * 0.2 * cos((i + 50) * 2 * PI / 200.f);
        float y2 = i * 0.2 * sin((i + 50) * 2 * PI / 200.f);

        depth_line_points_.push_back(ofVec3f(x1, y1, 300.f + kDepthInterval * i));
        depth_line_points_.push_back(ofVec3f(x2, y2, 500.f + kDepthInterval * i));
    }
}

//--------------------------------------------------------------
void ZcomingScene::draw() {
    cam_.begin();
    cam_.setPosition(ofVec3f(0.f, 10.f, -200.f));
    cam_.lookAt(ofVec3f(0.f, 0.f, 0.f));
    cam_.setFov(20.f);

    vbo_line_.draw(GL_LINES, 0, line_points_.size());
    vbo_depth_line_.draw(GL_LINES, 0, depth_line_points_.size());

    cam_.end();
}

