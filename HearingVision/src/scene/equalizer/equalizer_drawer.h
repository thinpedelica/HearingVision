#pragma once

#include "ofMain.h"

#include <memory>
#include <vector>

class EqualizerDrawer {
public:
    EqualizerDrawer() = default;
    virtual ~EqualizerDrawer() = default;
    virtual void setup(std::shared_ptr<ofRectangle> win_cache) = 0;
    virtual void update(const std::vector<float>& spectrum) = 0;
    virtual void draw() = 0;
    virtual void resize() {}
};

class CircularEqualizerDrawer : public EqualizerDrawer {
public:
    CircularEqualizerDrawer() = default;
    ~CircularEqualizerDrawer() = default;
    virtual void setup(std::shared_ptr<ofRectangle> win_cache) override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;

private:
    static constexpr size_t kBarNum   = 256;
    static constexpr size_t kPointNum = 3 * 2 * kBarNum;
    static constexpr float kOutDirectRadius = 200.f;
    static constexpr float kBarHeight       = 1000.f;

    ofVbo vbo_;
    std::vector<ofVec3f> vertexes_;
    std::vector<ofFloatColor> color_list_;
    float time_{0.f};

    ofEasyCam cam_;
    std::shared_ptr<ofRectangle> win_cache_;

};

class BoxEqualizerDrawer : public EqualizerDrawer {
public:
    BoxEqualizerDrawer() = default;
    ~BoxEqualizerDrawer() = default;
    virtual void setup(std::shared_ptr<ofRectangle> win_cache) override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;
    virtual void resize() override;

private:
    static constexpr size_t kBarRowNum = 24;
    static constexpr size_t kBarColNum = 36;
    static constexpr size_t kPointNum  = 3 * 2 * kBarRowNum * kBarColNum;

    static constexpr float kOffsetXforBottom = -100.f;

    void initializeVertex();
    void initializeColor();

    ofVbo vbo_;
    std::vector<ofVec3f> vertexes_;
    std::vector<ofFloatColor> color_list_;

    std::shared_ptr<ofRectangle> win_cache_;

};

class GridEqualizerDrawer : public EqualizerDrawer {
public:
    GridEqualizerDrawer() = default;
    ~GridEqualizerDrawer() = default;
    virtual void setup(std::shared_ptr<ofRectangle> win_cache) override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;

private:
    static constexpr size_t kGridRow = 16;
    static constexpr size_t kGridCol = 32;
    static constexpr size_t kGridNum = kGridRow * kGridCol;
    static constexpr size_t kLayerNum = 4;

    std::vector<ofVbo> vbo_list_;
    std::vector<ofVec3f> vertexes_;
    std::vector<std::vector<ofFloatColor>> colors_list_;

    size_t update_index_{0};

    ofEasyCam cam_;
    std::shared_ptr<ofRectangle> win_cache_;

};
