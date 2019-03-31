#include "humanpose.h"

#include <fstream>

//--------------------------------------------------------------
HumanPoseScene::HumanPoseScene() {
    // nop
}

//--------------------------------------------------------------
HumanPoseScene::~HumanPoseScene() {
    // nop
}

//--------------------------------------------------------------
void HumanPoseScene::setup(std::shared_ptr<ProcessFFT> pfft,
                           std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    initializeHumanPoseList();
}

bool HumanPoseScene::initializeHumanPoseList() {
    std::string file_path = "born/born_usa.bin";
    std::string abs_path  = ofToDataPath(file_path, true);
    std::ifstream ifs(abs_path, std::ios::binary);
    if (!ifs.is_open()) {
        std::cout << "### failed to open file: " << file_path << std::endl;
        return false;
    }

    // frame num
    uint32_t frame_num = 0.;
    ifs.read(reinterpret_cast<char*>(&frame_num), sizeof(frame_num));
    // std::cout << "### frame_num: " << frame_num << std::endl;

    // born list
    human_pose_list_.resize(frame_num);
    for (uint32_t i = 0; i < frame_num; ++i) {
        ifs.read(reinterpret_cast<char*>(&human_pose_list_.at(i).frame_no),
                 sizeof(human_pose_list_.at(i).frame_no));
        for (uint32_t j = 0; j < kBornNum; ++j) {
            ifs.read(reinterpret_cast<char*>(&human_pose_list_.at(i).born_list.at(j)),
                     sizeof(human_pose_list_.at(i).born_list.at(j)));
        }
        // std::cout << "### frame_no: " << human_pose_list_.at(i).frame_no << std::endl;
    }

    return true;
}

//--------------------------------------------------------------
void HumanPoseScene::update(SceneParam scene_param) {
    level_ = scene_param.level_;
    if (scene_param.change_mode_ == SceneParam::TriggerState::kOn) {
        changeMode();
    }
    if (isUpdateTiming()) {
        ++current_pose_;
        if (current_pose_ == human_pose_list_.size()) {
            current_pose_ = 0;
        }
        updateShadow();
    }
}

void HumanPoseScene::updateShadow() {
    shadow_info_list_.push_back(std::make_pair(current_pose_, kShadowLifeTime));
    auto itr = shadow_info_list_.begin();
    while (itr != shadow_info_list_.end()) {
        itr->second -= 1.f;
        if (itr->second < 0.f) {
            itr = shadow_info_list_.erase(itr);
        } else {
            ++itr;
        }
    }
}

bool HumanPoseScene::isValidBorn(const ofVec2f& born) {
    if (born.x < kInvalidValue) {
        return false;
    }
    if (born.y < kInvalidValue) {
        return false;
    }
    return true;
}

//--------------------------------------------------------------
void HumanPoseScene::draw() {
    drawHumanPose(0.f, -150.f);

    if (mode_ == DrawMode::kHumanMany) {
        drawHumanMany();
    }
    if (mode_ == DrawMode::kShadows) {
        drawShadow(0.f, -150.f);
    }
}

void HumanPoseScene::drawHumanPose(const float offset_x, const float offest_y) {
    cam_.begin();
    ofPushMatrix();
    ofTranslate(offset_x, offest_y);

    drawHumanLines(current_pose_);
    drawHumanPoints(current_pose_);

    ofPopMatrix();
    cam_.end();
}

void HumanPoseScene::drawHumanMany() {
    if (level_ > 0.5) {
        drawHumanPose(200.f, -150.f);
        drawHumanPose(-200.f, -150.f);
    }
    if (level_ > 0.7) {
        drawHumanPose(400.f, -150.f);
        drawHumanPose(-400.f, -150.f);
    }
}

void HumanPoseScene::drawShadow(const float offset_x, const float offest_y) {
    cam_.begin();
    ofPushMatrix();
    ofTranslate(offset_x, offest_y);

    float alpha = 180.f;
    for (auto ritr = shadow_info_list_.rbegin(); ritr != shadow_info_list_.rend(); ++ritr) {
        ofTranslate(kShadowSlideX, 0.f);
        drawHumanLines(ritr->first, alpha);
        drawHand(ritr->first, true, alpha);
        drawHand(ritr->first, false, alpha);
        alpha -= 1.f;
    }

    ofPopMatrix();
    cam_.end();
}

void HumanPoseScene::drawHumanPoints(const size_t index) {
    ofFill();

    for (size_t i = 0; i < human_pose_list_.at(index).born_list.size(); ++i) {
        if (isValidBorn(human_pose_list_.at(index).born_list.at(i))) {
            if (i == 4) {
                drawHand(index, true);
            } else if (i == 7) {
                drawHand(index, false);
            } else {
                ofSetColor(255, 255);
                ofDrawCircle(human_pose_list_.at(index).born_list.at(i), 3.f);
            }
        }
    }
}

void HumanPoseScene::drawHand(const size_t index, const bool is_right, const float alpha) {
    size_t born_index = 0;

    if (is_right) {
        born_index = 4;
        ofSetColor(ofColor::fromHsb(0, 120, alpha));
    } else {
        born_index = 7;
        ofSetColor(ofColor::fromHsb(160, 120, alpha));
    }

    if (isValidBorn(human_pose_list_.at(index).born_list.at(born_index))) {
        ofDrawCircle(human_pose_list_.at(index).born_list.at(born_index), 3.f);
    }
}

void HumanPoseScene::drawHumanLines(const size_t index, const float alpha) {
    ofSetColor(255, alpha);
    ofSetLineWidth(8.f);
    // Sholder - Elbow - Wrist (Right)
    for (uint32_t i = 2; i < 4; ++i) {
        if ((isValidBorn(human_pose_list_.at(index).born_list.at(i))) &&
            (isValidBorn(human_pose_list_.at(index).born_list.at(i + 1)))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(i),
                       human_pose_list_.at(index).born_list.at(i + 1));
        }
    }
    // Sholder - Elbow - Wrist (Left)
    for (uint32_t i = 5; i < 7; ++i) {
        if ((isValidBorn(human_pose_list_.at(index).born_list.at(i))) &&
            (isValidBorn(human_pose_list_.at(index).born_list.at(i + 1)))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(i),
                       human_pose_list_.at(index).born_list.at(i + 1));
        }
    }
    // Hip - Knee - Ankle (Right)
    for (uint32_t i = 8; i < 10; ++i) {
        if ((isValidBorn(human_pose_list_.at(index).born_list.at(i))) &&
            (isValidBorn(human_pose_list_.at(index).born_list.at(i + 1)))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(i),
                       human_pose_list_.at(index).born_list.at(i + 1));
        }
    }
    // Hip - Knee - Ankle (Left)
    for (uint32_t i = 11; i < 13; ++i) {
        if ((isValidBorn(human_pose_list_.at(index).born_list.at(i))) &&
            (isValidBorn(human_pose_list_.at(index).born_list.at(i + 1)))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(i),
                       human_pose_list_.at(index).born_list.at(i + 1));
        }
    }

    if (isValidBorn(human_pose_list_.at(index).born_list.at(1))) {
        // Neck - Head
        if (isValidBorn(human_pose_list_.at(index).born_list.at(0))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(1),
                       human_pose_list_.at(index).born_list.at(0));
        }
        // Neck - Sholder(Right)
        if (isValidBorn(human_pose_list_.at(index).born_list.at(2))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(1),
                       human_pose_list_.at(index).born_list.at(2));
        }
        // Neck - Sholder(Left)
        if (isValidBorn(human_pose_list_.at(index).born_list.at(5))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(1),
                       human_pose_list_.at(index).born_list.at(5));
        }
        // Neck - Hip(Right)
        if (isValidBorn(human_pose_list_.at(index).born_list.at(8))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(1),
                       human_pose_list_.at(index).born_list.at(8));
        }
        // Neck - Hip(Left)
        if (isValidBorn(human_pose_list_.at(index).born_list.at(11))) {
            ofDrawLine(human_pose_list_.at(index).born_list.at(1),
                       human_pose_list_.at(index).born_list.at(11));
        }
    }
}

//--------------------------------------------------------------
void HumanPoseScene::changeMode() {
    if (mode_ == DrawMode::kHumanOnly) {
        mode_ = DrawMode::kShadows;
        shadow_info_list_.clear();
    } else if (mode_ == DrawMode::kShadows) {
        mode_ = DrawMode::kHumanMany;
    } else if (mode_ == DrawMode::kHumanMany) {
        mode_ = DrawMode::kHumanOnly;
    }
}

bool HumanPoseScene::isUpdateTiming() {
    return true;
}