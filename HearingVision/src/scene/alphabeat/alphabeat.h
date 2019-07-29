#pragma once

#include "../base_scene.h"
#include "../util/counter.h"
#include "ofxPostGlitch.h"

class AlphabeatScene : public BaseScene {
public:
    AlphabeatScene();
    ~AlphabeatScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void resize() override;
private:
    void setupFont();
    void updateMode();
    void updateFont();
    void setupStrings();
    void updateStrings();
    void updateFbo();
    void drawString(const ofTrueTypeFont& font, const std::string& str) const;

    void updateFx();
    void setFx();
    void resetFx();

    void setupNextScene();

    std::vector<ofTrueTypeFont> font_list_;
    std::vector<std::string> alphabet_list_;
    std::vector<std::string> message_list_;
    std::string title_;
    size_t current_string_{ 0 };
    size_t current_font_{ 0 };

    Counter string_counter_;
    Counter fx_counter_;

    ofFbo fbo_;
    ofxPostGlitch glitch_;

    float level_{0.f};
    bool is_fx_{false};
    bool is_title_only_{false};
};
