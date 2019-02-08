#include "alphabeat.h"

//--------------------------------------------------------------
AlphabeatScene::AlphabeatScene() {
    // nop
}

//--------------------------------------------------------------
AlphabeatScene::~AlphabeatScene() {
    // nop
}

//--------------------------------------------------------------
void AlphabeatScene::setup(std::shared_ptr<ProcessFFT> pfft,
                     std::shared_ptr<ofRectangle> win_cache) {
    pfft_      = pfft;
    win_cache_ = win_cache;

    setupFont();
    setupStrings();

    fbo_.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    glitch_.setup(&fbo_);
}

void AlphabeatScene::setupFont() {
    ofTrueTypeFont font;

    ///// narrow
    font.loadFont("font/RBNo2/RBNo2Light-Light.otf", 255);
    font_list_.push_back(font);

    font.loadFont("font/Rafale/Rafale-BG.otf", 255);
    font_list_.push_back(font);

    font.loadFont("font/Alpha/Alpha-Outline.otf", 255);
    font_list_.push_back(font);

    //// mid
    font.loadFont("font/dosis/Dosis-SemiBold.ttf", 255);
    font_list_.push_back(font);

    font.loadFont("font/Phenomena/Phenomena-Bold.ttf", 255);
    font_list_.push_back(font);

    font.loadFont("font/Timeburner/timeburnerbold.ttf", 255);
    font_list_.push_back(font);

    font.loadFont("font/maw/MAW.ttf", 255);
    font_list_.push_back(font);

    //// wide
    font.loadFont("font/Navia-Type/Navia.ttf", 255);
    font_list_.push_back(font);

    font.loadFont("font/Muller/MullerNarrowDemo-ExtraBold.otf", 255);
    font_list_.push_back(font);

    font.loadFont("font/apex/ApexMk2-LightCondensed.otf", 255);
    font_list_.push_back(font);

    font.loadFont("font/blackout-master/BlackoutSunrise.ttf", 255);
    font_list_.push_back(font);


    font.loadFont("font/Origram/Origram.otf", 255);
    font_list_.push_back(font);

}

void AlphabeatScene::setupStrings() {
    alphabet_list_.resize(26);
    char c_str = 'A';
    for (auto& str : alphabet_list_) {
        str = c_str;
        c_str += 1;
    }

    message_list_.push_back("Alpha");
    message_list_.push_back("Beat");
    message_list_.push_back("Coming");
    message_list_.push_back("Dance");
    message_list_.push_back("Enjoy");
    message_list_.push_back("Flying");
    message_list_.push_back("Girls");
    message_list_.push_back("High");
    message_list_.push_back("Image");
    message_list_.push_back("Jump");
    message_list_.push_back("Keep");
    message_list_.push_back("Love");
    message_list_.push_back("Magic");
    message_list_.push_back("Night");
    message_list_.push_back("Over");
    message_list_.push_back("Play");
    message_list_.push_back("Quiet?");
    message_list_.push_back("Rock");
    message_list_.push_back("Shake");
    message_list_.push_back("Touch");
    message_list_.push_back("Unit");
    message_list_.push_back("Vive");
    message_list_.push_back("World");
    message_list_.push_back("XXX");
    message_list_.push_back("Yeah");
    message_list_.push_back("Zeal");
}

//--------------------------------------------------------------
void AlphabeatScene::update(SceneParam scene_param) {
    if (scene_param.reset_ == SceneParam::TriggerState::kOn) {
        reset();
    }

    level_ = scene_param.level_;
    counter_.setThreshold(scene_param.threshold_);

    updateFont();
    updateStrings();

    fbo_.begin();
    ofClear(0, 0, 0, 255);

    if (level_ < 0.6) {
        drawString(font_list_.at(current_font_), alphabet_list_.at(current_string_));
    } else {
        drawString(font_list_.at(current_font_), message_list_.at(current_string_));
    }

    fbo_.end();

    glitch_.setFx(OFXPOSTGLITCH_GLOW, true);
    glitch_.generateFx();
}

void AlphabeatScene::updateFont() {
    if (level_ > 0.2) {
        current_font_ = static_cast<size_t>(ofMap(pfft_->getLowVal(), 0.f, 0.2f, 0.f, font_list_.size() - 1, true));
    }
}

void AlphabeatScene::updateStrings() {
    bool ret = counter_.update();
    if (ret) {
        if (level_ < 0.4) {
            current_string_ += 1;
            if (current_string_ == alphabet_list_.size()) {
                current_string_ = 0;
            }
        } else {
            current_string_ = static_cast<size_t>(ofRandom(alphabet_list_.size() - 1));
            updateFx();
        }
    } else if (counter_.isOver(0.3f)) {
        resetFx();
    }
}

void AlphabeatScene::updateFx() {
    float coin = ofRandomf();
    if (coin < 0.2f) {
        glitch_.setFx(OFXPOSTGLITCH_INVERT, true);
    } else if (coin < 0.4f) {
        glitch_.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        glitch_.setFx(OFXPOSTGLITCH_SHAKER, true);
    } else if (coin < 0.6f) {
        glitch_.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        glitch_.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    } else if (coin < 0.8f) {
        glitch_.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        glitch_.setFx(OFXPOSTGLITCH_OUTLINE, true);
    } else {
        glitch_.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        glitch_.setFx(OFXPOSTGLITCH_SHAKER, true);
        glitch_.setFx(OFXPOSTGLITCH_GLOW, true);
    }
}

void AlphabeatScene::resetFx() {
    for (size_t i = 0; i < GLITCH_NUM; ++i) {
        glitch_.setFx(static_cast<ofxPostGlitchType>(i), false);
    }
}

void AlphabeatScene::drawString(const ofTrueTypeFont& font, const std::string& str) const {
    ofSetColor(255);
    ofRectangle r = font.getStringBoundingBox(str, 0, 0);
    ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
    font.drawString(str, win_cache_->getWidth() * 0.5f + offset.x, win_cache_->getHeight() * 0.5f + offset.y);
}

//--------------------------------------------------------------
void AlphabeatScene::draw() {
    ofSetColor(255);
    fbo_.draw(0, 0);
}

//--------------------------------------------------------------
void AlphabeatScene::reset() {
    fbo_.allocate(win_cache_->getWidth(), win_cache_->getHeight());
    glitch_.setup(&fbo_);
}
