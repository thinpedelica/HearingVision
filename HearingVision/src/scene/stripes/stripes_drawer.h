#pragma once

#include "ofMain.h"

#include "../util/counter.h"

#include <memory>
#include <vector>

class BloomLineDrawer {
public:
    BloomLineDrawer() = default;
    ~BloomLineDrawer() = default;
    void setup();
    void draw(const ofVbo& vbo,
              const size_t offset,
              const size_t line_num,
              const ofVec3f& cam_position);
private:
    ofShader bloom_line_shader_;
};

class StripesDrawer {
public:
    StripesDrawer() = default;
    virtual ~StripesDrawer() = default;
    virtual void initialize(std::shared_ptr<ofRectangle> win_cache,
                            std::shared_ptr<BloomLineDrawer> bloom_line_drawer);
    virtual void setup() = 0;
    virtual void update(const std::vector<float>& spectrum) = 0;
    virtual void draw() = 0;
    bool isDrawEnd();

    virtual void reset();
    virtual void resize();
    void resetCam();

    virtual void setColor(const float color) { color_ = color; }
    virtual void setLevel(const float level) { level_ = level; }
    virtual void setThreshold(const float threshold) { threshold_ = threshold; }

protected:
    static constexpr float kCamPosZ = 1000.f;

    void updateDrawCount(const uint32_t draw_count_max);
    uint32_t getUpdateThreshold();

    std::shared_ptr<ofRectangle> win_cache_;
    std::shared_ptr<BloomLineDrawer> bloom_line_drawer_;
    float color_{0.f};
    float level_{0.f};
    float threshold_{0.f};

    ofCamera render_cam_;
    ofVec3f cam_pos_;

    uint32_t update_count_;
    uint32_t draw_count_;
    bool draw_end_;
};

class BarStripesDrawer : public StripesDrawer {
public:
    BarStripesDrawer() = default;
    ~BarStripesDrawer() = default;
    virtual void setup() override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;

    virtual void reset() override;

private:
    static constexpr size_t kRowNum = 32;
    static constexpr size_t kColNum =  9;
    static constexpr size_t kPointsPerLine = 2;
    static constexpr size_t kBlocksPerCol  = 2;

    static constexpr float  kLineHeight      = 48;
    static constexpr float  kLineHeightSpace = 32;
    static constexpr float  kLineWidth       = 220;
    static constexpr float  kLineWidthSpace  =  20;

    static constexpr uint32_t kDrawCountMax = 8;
    static constexpr uint32_t kMoveFastCount = 7;

    void updateSceneCondition();
    void updateMoveStep();

    void updateVerts(const std::vector<float>& spectrum);

    void resetVerts();
    void resetSpectrumIndex();

    std::vector<ofVec3f> verts_;
    std::vector<ofFloatColor> colors_;
    ofVbo vbo_;

    std::vector<size_t> spectrum_index_;

    float line_move_step_;

    uint32_t draw_col_num_;

    bool move_fast_;
};

class PointStripesDrawer : public StripesDrawer {
public:
    PointStripesDrawer() = default;
    ~PointStripesDrawer() = default;
    virtual void setup() override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;

    virtual void reset() override;
    virtual void resize() { reset(); }
private:
    static constexpr size_t kRowNum = 80;
    static constexpr size_t kColNum = 16;

    static constexpr size_t kPointsPerLine = 2;

    static constexpr float  kHeightSpace = 20;
    static constexpr float  kWidthSpace  = 150;
    static constexpr float  kLineHeight  = 32;

    static constexpr uint32_t kDrawCountMax = 8;
    static constexpr uint32_t kDrawCountStripe = 4;
    static constexpr uint32_t kDrawCountNarrow = 7;

    void updateSceneCondition();

    void updateStripeVerts(const std::vector<float>& spectrum);
    void resetStripeVerts();

    void resetSpectrumIndex();

    std::vector<ofVec3f> stripe_verts_;
    std::vector<ofFloatColor> stripe_colors_;
    ofVbo stripe_vbo_;

    std::vector<ofVec3f> down_stripe_verts_;
    std::vector<ofFloatColor> down_stripe_colors_;
    ofVbo down_stripe_vbo_;

    std::vector<size_t> spectrum_index_;

    bool draw_down_stripe_;
    bool draw_narrow_;
};



class FractalStripesDrawer : public StripesDrawer {
public:
    FractalStripesDrawer() = default;
    ~FractalStripesDrawer() = default;
    virtual void setup() override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;

    virtual void reset() override;
private:
    static constexpr uint32_t kDrawCountMax    = 8;
    static constexpr uint32_t kDrawAllCount    = 3;
    static constexpr uint32_t kChangeSizeCount = 6;

    static constexpr size_t kRow = 4;
    static constexpr size_t kCol = 8;
    static constexpr float kMinLineWidth = 2.0f;
    static constexpr float kMaxLineWidth = 10.0f;
    static constexpr float kNarrowingRatio = 0.2f;

    void updateSceneCondition();

    void updateFractal(const std::vector<float>& spectrum);
    void drawFractal();

    void divideRect(float x, float y, float w, float h, int n);

    std::vector<float> spectrum_;
    std::vector<bool> drawable_list_;
    float size_ratio_;

    bool draw_all_;
    bool draw_narrow_;
};



class HorizontalStripesDrawer : public StripesDrawer {
public:
    HorizontalStripesDrawer() = default;
    ~HorizontalStripesDrawer() = default;
    virtual void setup() override;
    virtual void update(const std::vector<float>& spectrum) override;
    virtual void draw() override;

    virtual void reset() override;
private:
    static constexpr uint32_t kDrawCountMax    = 8;
    static constexpr size_t kPointsPerLine     = 2;
    static constexpr size_t kRowNum            = 248;

    static constexpr size_t kBlockPerLine = 8;
    static constexpr size_t kBlockNum = kRowNum / kBlockPerLine;

    void updateSceneCondition();

    void updateVerts(const std::vector<float>& spectrum);
    void updateColors();
    size_t getCurrentUpBlock();
    void resetVerts();

    std::vector<ofVec3f> verts_;
    std::vector<ofFloatColor> colors_;
    ofVbo vbo_;
};

