#pragma once

#include "../base_scene.h"
#include "ofxPostGlitch.h"

class AlphabeatScene : public BaseScene {
public:
    AlphabeatScene();
    ~AlphabeatScene();
    virtual void setup(std::shared_ptr<ProcessFFT> pfft,
                       std::shared_ptr<ofRectangle> win_cache);
    virtual void update(SceneParam scene_param);
    virtual void draw();
    virtual void reset();

private:
    static constexpr float kStringChangeThreshold = 1.f;
    static constexpr float kStringUpdateCount     = 0.01f;
    void setupFont();
    void updateFont();
    void setupStrings();
    void updateStrings();
    void updateFx();
    void resetFx();
    void drawString(const ofTrueTypeFont& font, const std::string& str) const;

    std::vector<ofTrueTypeFont> font_list_;
    std::vector<std::string> alphabet_list_;
    std::vector<std::string> message_list_;
    size_t current_string_{ 0 };
    size_t current_font_{ 0 };

    float string_change_count_{0.f};
    float string_change_threshold_{0.1f};

    ofFbo fbo_;
    ofxPostGlitch glitch_;

    float level_{0.f};
    bool is_fx_{false};
};
