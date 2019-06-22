#include <numeric>

#include "stripes_drawer.h"


//--------------------------------------------------------------
void BloomLineDrawer::setup() {
    bloom_line_shader_.setGeometryInputType(GL_LINES);
    bloom_line_shader_.setGeometryOutputType(GL_TRIANGLE_STRIP);
    bloom_line_shader_.setGeometryOutputCount(12);
    bloom_line_shader_.load("shader/stripes/bloom_line.vert",
                            "shader/stripes/bloom_line.frag",
                            "shader/stripes/bloom_line.geom");
    bloom_line_shader_.bindDefaults();
    bloom_line_shader_.linkProgram();
}

//--------------------------------------------------------------
void BloomLineDrawer::draw(const ofVbo& vbo,
                           const size_t offset,
                           const size_t line_num,
                           const ofVec3f& cam_position) {
    bloom_line_shader_.begin();
    bloom_line_shader_.setUniform3f("camPosition", cam_position);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    vbo.draw(GL_LINES, offset, line_num);

    bloom_line_shader_.end();
}


//--------------------------------------------------------------
void StripesDrawer::initialize(std::shared_ptr<ofRectangle> win_cache,
                               std::shared_ptr<BloomLineDrawer> bloom_line_drawer) {
    win_cache_ = win_cache;
    bloom_line_drawer_ = bloom_line_drawer;
    resetCam();
}

uint32_t StripesDrawer::getUpdateThreshold() {
    uint32_t result = 60 - static_cast<uint32_t>(50.f * threshold_);  // calc by fps
    return result;
}

void StripesDrawer::reset() {
    update_count_ = 0;
    draw_count_   = 0;
    draw_end_     = false;
}

void StripesDrawer::resize() {
    resetCam();
}

void StripesDrawer::resetCam() {
    cam_pos_ = ofVec3f(0.f, 0.f, kCamPosZ);
    render_cam_.setPosition(cam_pos_);
    render_cam_.lookAt(ofVec3f(0.f, 0.f, 0.f), ofVec3f(0, 1, 0));

    float fov = ofRadToDeg(atan2(win_cache_->getWidth() * 0.5, kCamPosZ));
    render_cam_.setFov(fov * 1.2f);  // TODO why need 1.2
}

void StripesDrawer::updateDrawCount(const uint32_t draw_count_max) {
    ++update_count_;
    uint32_t update_threshold = getUpdateThreshold();
    if (update_count_ > update_threshold) {
        update_count_ = 0;
        draw_count_ += 1;
        if (draw_count_ == draw_count_max) {
            draw_end_ = true;
        }
    }
}

bool StripesDrawer::isDrawEnd() {
    return draw_end_;
}


//--------------------------------------------------------------
void BarStripesDrawer::setup() {
    reset();
}

//--------------------------------------------------------------
void BarStripesDrawer::update(const std::vector<float>& spectrum) {
    updateSceneCondition();
    updateVerts(spectrum);

    if (update_count_ == 0) {
        if (draw_col_num_ < kColNum) {
            draw_col_num_ += 2;
        }
        if (draw_count_ == kMoveFastCount) {
            move_fast_ = true;
        }
    }
}

void BarStripesDrawer::updateSceneCondition() {
    updateDrawCount(kDrawCountMax);
    updateMoveStep();
}

void BarStripesDrawer::updateMoveStep() {
    line_move_step_ = 1.f + 2.f * threshold_;
    if (move_fast_) {
        line_move_step_ += 4.f;
    }
}

void BarStripesDrawer::updateVerts(const std::vector<float>& spectrum) {
    std::vector<float> color_a_list;
    color_a_list.resize(kRowNum);
    for (size_t line_num = 0; line_num < kRowNum; ++line_num) {
        color_a_list.at(line_num) =  ofMap(spectrum.at(spectrum_index_.at(line_num)), 0.05, 0.2, 0.5, 1.0, true);
    }

    for (size_t col_num = 0; col_num < draw_col_num_; ++col_num) {
        size_t blocks_offset     = (kRowNum * kPointsPerLine * kBlocksPerCol) * col_num;
        size_t up_block_offset   = blocks_offset;
        size_t down_block_offset = up_block_offset + kRowNum * kPointsPerLine;

        for (size_t line_num = 0; line_num < kRowNum; ++line_num) {
            // up block
            verts_.at(up_block_offset  + line_num * kPointsPerLine + 0).y += line_move_step_;
            verts_.at(up_block_offset  + line_num * kPointsPerLine + 1).y += line_move_step_;
            colors_.at(up_block_offset + line_num * kPointsPerLine + 0).a  = color_a_list.at(line_num);
            colors_.at(up_block_offset + line_num * kPointsPerLine + 1).a  = color_a_list.at(line_num);

            // down block
            verts_.at(down_block_offset  + line_num * kPointsPerLine + 0).y -= line_move_step_;
            verts_.at(down_block_offset  + line_num * kPointsPerLine + 1).y -= line_move_step_;
            colors_.at(down_block_offset + line_num * kPointsPerLine + 0).a  = color_a_list.at(line_num);
            colors_.at(down_block_offset + line_num * kPointsPerLine + 1).a  = color_a_list.at(line_num);
        }
    }

    vbo_.setVertexData(verts_.data(), verts_.size(), GL_DYNAMIC_DRAW);
    vbo_.setColorData(colors_.data(), colors_.size(), GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void BarStripesDrawer::draw() {
    render_cam_.begin();
    bloom_line_drawer_->draw(vbo_, 0, verts_.size(), render_cam_.getPosition());
    render_cam_.end();
}

//--------------------------------------------------------------
void BarStripesDrawer::reset() {
    StripesDrawer::reset();
    draw_col_num_ = 1;
    move_fast_    = false;
    resetVerts();
    resetSpectrumIndex();
}

void BarStripesDrawer::resetVerts() {
    // pos
    verts_.resize(kRowNum * kColNum * kPointsPerLine * kBlocksPerCol);

    float x_center          = 0.f;
    float x_sign            = 1.f;

    for (size_t col_num = 0; col_num < kColNum; ++col_num) {
        x_center += (col_num * (kLineWidth + kLineWidthSpace) * x_sign);
        x_sign *= -1.f;

        float x_left     = x_center - kLineWidth / 2;
        float x_right    = x_center + kLineWidth / 2;
        float y_bottom_a = -1000.f;
        float y_bottom_b =  -200.f;

        size_t blocks_offset     = (kRowNum * kPointsPerLine * kBlocksPerCol) * col_num;
        size_t up_block_offset   = blocks_offset;
        size_t down_block_offset = up_block_offset + kRowNum * kPointsPerLine;

        for (size_t line_num = 0; line_num < kRowNum; ++line_num) {
            // up block
            verts_.at(up_block_offset + line_num * kPointsPerLine + 0).set(x_left,  y_bottom_a, kLineHeight / 2); // z = line width
            verts_.at(up_block_offset + line_num * kPointsPerLine + 1).set(x_right, y_bottom_a, kLineHeight / 2);
            y_bottom_a += (kLineHeight + kLineHeightSpace);

            // down block
            verts_.at(down_block_offset + line_num * kPointsPerLine + 0).set(x_left,  y_bottom_b, kLineHeight / 2);
            verts_.at(down_block_offset + line_num * kPointsPerLine + 1).set(x_right, y_bottom_b, kLineHeight / 2);
            y_bottom_b += (kLineHeight + kLineHeightSpace);
        }

    }

    // color
    colors_.assign(verts_.size(), ofFloatColor(1.f, 1.f, 1.f, 0.f));

    vbo_.setVertexData(verts_.data(), verts_.size(), GL_DYNAMIC_DRAW);
    vbo_.setColorData(colors_.data(), colors_.size(), GL_DYNAMIC_DRAW);
}

void BarStripesDrawer::resetSpectrumIndex() {
    spectrum_index_.resize(kRowNum);

    for (size_t i = 0; i < kRowNum; ++i) {
        spectrum_index_.at(i) = static_cast<size_t>(i * ofRandom(1, 8));
    }
}









//--------------------------------------------------------------
void PointStripesDrawer::setup() {
    reset();
}

//--------------------------------------------------------------
void PointStripesDrawer::update(const std::vector<float>& spectrum) {
    updateSceneCondition();
    updateStripeVerts(spectrum);
}

void PointStripesDrawer::updateSceneCondition() {
    updateDrawCount(kDrawCountMax);
    if (update_count_ == 0) {
        if (draw_count_ == kDrawCountStripe) {
            draw_down_stripe_ = true;
        }
        if (draw_count_ == kDrawCountNarrow) {
            draw_narrow_ = true;
        }
    }
}

void PointStripesDrawer::updateStripeVerts(const std::vector<float>& spectrum) {
    float update_width   = 0.6f;
    float spectrum_upper = 0.3f;
    if (draw_down_stripe_) {
        update_width   = 0.f;
        spectrum_upper = 0.1f;
    }
    if (draw_narrow_) {
        update_width   = -2.f;
        spectrum_upper = 0.1f;
    }

    for (size_t col_num = 0; col_num < kColNum; ++col_num) {
        size_t row_offset = kRowNum * col_num * kPointsPerLine;

        float spectrum_val = ofMap(spectrum.at(spectrum_index_.at(col_num)), 0.0, spectrum_upper, 0, kRowNum, true);

        for (size_t line_num = 0; line_num < kRowNum; ++line_num) {
            stripe_verts_.at(row_offset + line_num * kPointsPerLine + 0).x -= update_width;
            stripe_verts_.at(row_offset + line_num * kPointsPerLine + 1).x += update_width;

            if (spectrum_val > line_num) {
                stripe_colors_.at(row_offset + line_num * kPointsPerLine + 0).a = 1.f;
                stripe_colors_.at(row_offset + line_num * kPointsPerLine + 1).a = 1.f;
            } else {
                stripe_colors_.at(row_offset + line_num * kPointsPerLine + 0).a = 0.f;
                stripe_colors_.at(row_offset + line_num * kPointsPerLine + 1).a = 0.f;
            }

            if (draw_down_stripe_) {
                down_stripe_verts_.at(row_offset + line_num * kPointsPerLine + 0).x = stripe_verts_.at(row_offset + line_num * kPointsPerLine + 0).x;
                down_stripe_verts_.at(row_offset + line_num * kPointsPerLine + 1).x = stripe_verts_.at(row_offset + line_num * kPointsPerLine + 1).x;

                if (spectrum_val > line_num) {
                    down_stripe_colors_.at(row_offset + line_num * kPointsPerLine + 0).a = 1.f;
                    down_stripe_colors_.at(row_offset + line_num * kPointsPerLine + 1).a = 1.f;
                } else {
                    down_stripe_colors_.at(row_offset + line_num * kPointsPerLine + 0).a = 0.f;
                    down_stripe_colors_.at(row_offset + line_num * kPointsPerLine + 1).a = 0.f;
                }
            }
        }
    }

    stripe_vbo_.setVertexData(stripe_verts_.data(), stripe_verts_.size(), GL_DYNAMIC_DRAW);
    stripe_vbo_.setColorData(stripe_colors_.data(), stripe_colors_.size(), GL_DYNAMIC_DRAW);

    down_stripe_vbo_.setVertexData(down_stripe_verts_.data(), down_stripe_verts_.size(), GL_DYNAMIC_DRAW);
    down_stripe_vbo_.setColorData(down_stripe_colors_.data(), down_stripe_colors_.size(), GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void PointStripesDrawer::draw() {
    render_cam_.begin();
    bloom_line_drawer_->draw(stripe_vbo_,      0, stripe_verts_.size(),      render_cam_.getPosition());
    bloom_line_drawer_->draw(down_stripe_vbo_, 0, down_stripe_verts_.size(), render_cam_.getPosition());
    render_cam_.end();
}

//--------------------------------------------------------------
void PointStripesDrawer::reset() {
    StripesDrawer::reset();
    draw_down_stripe_ = false;
    draw_narrow_      = false;
    resetStripeVerts();
    resetSpectrumIndex();
    resetCam();
}

void PointStripesDrawer::resetStripeVerts() {
    // pos
    stripe_verts_.resize(kRowNum * kColNum * kPointsPerLine);
    down_stripe_verts_.resize(kRowNum * kColNum * kPointsPerLine);

    float x_pos    = -win_cache_->getWidth()  * 0.5 - 300.f;
    float y_bottom = -win_cache_->getHeight() * 0.5 - 100.f;
    float y_top    =  win_cache_->getHeight() * 0.5 + 100.f;
    for (size_t col_num = 0; col_num < kColNum; ++col_num) {
        size_t row_offset = kRowNum * col_num * kPointsPerLine;
        x_pos += kWidthSpace;

        for (size_t line_num = 0; line_num < kRowNum; ++line_num) {
            stripe_verts_.at(row_offset + line_num * kPointsPerLine + 0).set(x_pos, y_bottom + (kHeightSpace * line_num), kLineHeight / 2); // z = line width
            stripe_verts_.at(row_offset + line_num * kPointsPerLine + 1).set(x_pos, y_bottom + (kHeightSpace * line_num), kLineHeight / 2);

            down_stripe_verts_.at(row_offset + line_num * kPointsPerLine + 0).set(x_pos, y_top - (kHeightSpace * line_num), kLineHeight / 2);
            down_stripe_verts_.at(row_offset + line_num * kPointsPerLine + 1).set(x_pos, y_top - (kHeightSpace * line_num), kLineHeight / 2);
        }
    }

    // color
    stripe_colors_.assign(stripe_verts_.size(), ofFloatColor(1.f, 1.f, 1.f, 0.f));
    down_stripe_colors_.assign(down_stripe_verts_.size(), ofFloatColor(1.f, 1.f, 1.f, 0.f));

    stripe_vbo_.setVertexData(stripe_verts_.data(), stripe_verts_.size(), GL_DYNAMIC_DRAW);
    stripe_vbo_.setColorData(stripe_colors_.data(), stripe_colors_.size(), GL_DYNAMIC_DRAW);

    down_stripe_vbo_.setVertexData(down_stripe_verts_.data(), down_stripe_verts_.size(), GL_DYNAMIC_DRAW);
    down_stripe_vbo_.setColorData(down_stripe_colors_.data(), down_stripe_colors_.size(), GL_DYNAMIC_DRAW);
}

void PointStripesDrawer::resetSpectrumIndex() {
    spectrum_index_.resize(kRowNum);

    size_t index_base = 10;
    for (size_t i = 0; i < kColNum; ++i) {
        spectrum_index_.at(i) = index_base;
        index_base += 10;
    }
}





//--------------------------------------------------------------
void FractalStripesDrawer::setup() {
    reset();
}

//--------------------------------------------------------------
void FractalStripesDrawer::update(const std::vector<float>& spectrum) {
    updateSceneCondition();
    updateFractal(spectrum);
}

void FractalStripesDrawer::updateSceneCondition() {
    updateDrawCount(kDrawCountMax);
    if (update_count_ == 0) {
        if (draw_count_ == kDrawAllCount) {
            draw_all_ = true;
        }
        if (draw_count_ == kChangeSizeCount) {
            draw_narrow_ = true;
        }
    }
}

void FractalStripesDrawer::updateFractal(const std::vector<float>& spectrum) {
    spectrum_ = spectrum;
    if (draw_narrow_) {
        size_ratio_ *= 0.95f;
    }

    if ((update_count_ % 10) == 0) {
        for (auto& item : drawable_list_) {
            float coin = ofRandomf();
            if (coin > 0.5) {
                item = true;
            } else {
                if (!draw_all_) {
                    item = false;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void FractalStripesDrawer::draw() {
    render_cam_.begin();
    drawFractal();
    render_cam_.end();
}

void FractalStripesDrawer::drawFractal() {
    float box_w = win_cache_->getWidth() / kCol * size_ratio_;
    float box_h = win_cache_->getHeight() / kRow * size_ratio_;
    size_t step = spectrum_.size() / (kCol * kRow);

    ofPushMatrix();
    ofTranslate(-win_cache_->getWidth() * 0.5 * size_ratio_,
                -win_cache_->getHeight() * 0.5 * size_ratio_);

    for (size_t i = 0; i < kRow; ++i) {
        float top = box_h * i;

        for (size_t j = 0; j < kCol; ++j) {
            size_t draw_index = kCol *  i + j;
            if (drawable_list_.at(draw_index)) {
                size_t spectrum_index = ((kCol * i) + j) * step;
                float left = box_w * j;
                float line_width = ofMap(spectrum_.at(spectrum_index), 0.0, 1.0, kMinLineWidth, kMaxLineWidth);
                divideRect(left, top, box_w, box_h, line_width);

                ofSetLineWidth(line_width);
                ofSetColor(255);
                ofNoFill();
                ofDrawRectangle(left, top, box_w, box_h);
            }
        }
    }

    ofPopMatrix();
}

void FractalStripesDrawer::divideRect(float x, float y, float w, float h, int n) {
    float randw = ofRandom(w);
    float randh = ofRandom(h);
    ofSetLineWidth(n);
    ofSetColor(255);
    ofNoFill();

    if (w > h) {
        ofDrawLine(x + randw, y, x + randw, y + h);    // vertical
        if (n > 1) {
            divideRect(x,         y, randw,     h, n - kNarrowingRatio);    // left box
            divideRect(x + randw, y, w - randw, h, n - kNarrowingRatio);    // right box
        }
    } else {
        ofDrawLine(x, y + randh, x + w, y + randh);    // holizontal
        if (n > 1) {
            divideRect(x, y,         w, randh,     n - kNarrowingRatio);    // top box
            divideRect(x, y + randh, w, h - randh, n - kNarrowingRatio);    // bottom box
        }
    }
}

//--------------------------------------------------------------
void FractalStripesDrawer::reset() {
    StripesDrawer::reset();
    size_ratio_  = 1.f;
    draw_all_    = false;
    draw_narrow_ = false;

    drawable_list_.resize(kRow * kCol, false);
}








//--------------------------------------------------------------
void HorizontalStripesDrawer::setup() {
    reset();
}

//--------------------------------------------------------------
void HorizontalStripesDrawer::update(const std::vector<float>& spectrum) {
    updateSceneCondition();
    updateVerts(spectrum);
    updateColors();
}

void HorizontalStripesDrawer::updateSceneCondition() {
    updateDrawCount(kDrawCountMax);
}

void HorizontalStripesDrawer::updateVerts(const std::vector<float>& spectrum) {
    for (size_t row_num = 0; row_num < kRowNum; ++row_num) {
        verts_.at(row_num * kPointsPerLine + 0).z = ofMap(spectrum.at(row_num), 0.0f, 0.3f, 1.f, 20.f, true);
    }

    vbo_.setVertexData(verts_.data(), verts_.size(), GL_DYNAMIC_DRAW);
}

void HorizontalStripesDrawer::updateColors() {
    std::vector<size_t> up_list;
    std::vector<size_t> down_list;

    size_t current_block = getCurrentUpBlock();

    if (draw_count_ < 2) {
        up_list.push_back(current_block);
        for (size_t i = 0; i < kBlockNum; ++i) {
            if (i != current_block) {
                down_list.push_back(i);
            }
        }
    } else if (draw_count_ < 4) {
        up_list.push_back(current_block);
    } else if (draw_count_ < 7) {
        // keep
    } else {
        for (size_t i = 0; i < kBlockNum; ++i) {
            down_list.push_back(i);
        }
    }

    for (const auto block_no : up_list) {
        if (block_no >= kBlockNum) {
            continue;
        }
        size_t start_index = kBlockPerLine * block_no;
        size_t end_index   = start_index + kBlockPerLine;
        for (size_t i = start_index; i < end_index; ++i) {
            colors_.at(i * kPointsPerLine + 0).a += 0.5f;
            colors_.at(i * kPointsPerLine + 1).a += 0.5f;
            if (colors_.at(i * kPointsPerLine + 0).a > 1.0f) {
                colors_.at(i * kPointsPerLine + 0).a = 1.0f;
                colors_.at(i * kPointsPerLine + 1).a = 1.0f;
            }
        }
    }

    for (const auto block_no : down_list) {
        if (block_no >= kBlockNum) {
            continue;
        }
        size_t start_index = kBlockPerLine * block_no;
        size_t end_index   = start_index + kBlockPerLine;
        for (size_t i = start_index; i < end_index; ++i) {
            colors_.at(i * kPointsPerLine + 0).a -= 0.02f;
            colors_.at(i * kPointsPerLine + 1).a -= 0.02f;
            if (colors_.at(i * kPointsPerLine + 0).a < 0.0f) {
                colors_.at(i * kPointsPerLine + 0).a = 0.0f;
                colors_.at(i * kPointsPerLine + 1).a = 0.0f;
            }
        }
    }

    vbo_.setColorData(colors_.data(), colors_.size(), GL_DYNAMIC_DRAW);
}

size_t HorizontalStripesDrawer::getCurrentUpBlock() {
    float update_count_max = getUpdateThreshold() * 2;
    float draw_count       = draw_count_ % 2;
    float update_count     = update_count_ + (getUpdateThreshold() * draw_count);

    float ratio = update_count / update_count_max;

    size_t offset_block  = 8;
    size_t current_block = (kBlockNum + offset_block) * ratio;

    return current_block;
}

//--------------------------------------------------------------
void HorizontalStripesDrawer::draw() {
    render_cam_.begin();
    bloom_line_drawer_->draw(vbo_, 0, verts_.size(), render_cam_.getPosition());
    render_cam_.end();
}

//--------------------------------------------------------------
void HorizontalStripesDrawer::reset() {
    StripesDrawer::reset();
    resetVerts();
}

void HorizontalStripesDrawer::resetVerts() {
    // pos
    verts_.resize(kRowNum * kPointsPerLine);

    float left   = -win_cache_->getWidth() * 0.5;
    float right  =  win_cache_->getWidth() * 0.5;
    float bottom = -win_cache_->getHeight() * 0.5;

    float line_step  = win_cache_->getHeight() / kRowNum * 0.2;
    float block_step = win_cache_->getHeight() / kRowNum * kBlockPerLine * 0.8;

    for (size_t row_num = 0; row_num < kRowNum; ++row_num) {
        verts_.at(row_num * kPointsPerLine + 0).set(left,  bottom, 0.f);
        verts_.at(row_num * kPointsPerLine + 1).set(right, bottom, 0.f);

        if ((row_num % kBlockPerLine) == 0) {
            bottom += block_step;
        } else {
            bottom += line_step;
        }
    }

    // color
    colors_.assign(verts_.size(), ofFloatColor(1.f, 1.f, 1.f, 0.f));

    vbo_.setVertexData(verts_.data(), verts_.size(), GL_DYNAMIC_DRAW);
    vbo_.setColorData(colors_.data(), colors_.size(), GL_DYNAMIC_DRAW);
}
