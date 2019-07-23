#pragma once

#include "ofMain.h"

#include <memory>
#include <vector>

#include "../util/next_scene.h"

class EqualizerDrawer {
public:
    EqualizerDrawer() = default;
    virtual ~EqualizerDrawer() = default;
    virtual void setup(std::shared_ptr<ofRectangle> win_cache) = 0;
    virtual void update(const std::vector<float>& spectrum) = 0;
    virtual void draw() = 0;
    virtual void resize() {}

    virtual void setColor(const float color) { color_ = color; }
    virtual void setLevel(const float level) { level_ = level; }
    virtual NextScene& getNextScene();

protected:
    std::shared_ptr<ofRectangle> win_cache_;
    NextScene next_scene_;

    virtual void setupNextScene() = 0;

    float color_{0.f};
    float level_{0.f};

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

    virtual void setupNextScene() override;

    ofVbo vbo_;
    std::vector<ofVec3f> vertexes_;
    std::vector<ofFloatColor> color_list_;
    float time_{0.f};

    ofEasyCam cam_;

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

    virtual void setupNextScene() override;

    ofVbo vbo_;
    std::vector<ofVec3f> vertexes_;
    std::vector<ofFloatColor> color_list_;

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

    virtual void setupNextScene() override;

    std::vector<ofVbo> vbo_list_;
    std::vector<ofVec3f> vertexes_;
    std::vector<std::vector<ofFloatColor>> colors_list_;

    size_t update_index_{0};

    ofEasyCam cam_;

};

class CylinderEqualizerDrawer : public EqualizerDrawer {
public:
    CylinderEqualizerDrawer() = default;
    ~CylinderEqualizerDrawer() = default;
    virtual void setup(std::shared_ptr<ofRectangle> win_cache) override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;

private:
    static constexpr float kAreaMax   = 500.f;
    static constexpr float kRadiusMax =  30.f;
    static constexpr float kRadiusMin =   5.f;
    static constexpr float kHeightMax = 500.f;
    static constexpr float kHeightMin = 200.f;

    std::vector<ofCylinderPrimitive> cylinders_;

    bool isOverrap(const std::vector<ofCylinderPrimitive>& cylinders, ofVec3f& pos, float radius) const;

    virtual void setupNextScene() override;

    ofLight point_light_;
    ofMaterial material_;

    ofColor light_color_;
    ofColor material_color_;

    ofEasyCam cam_;

};
