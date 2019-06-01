#include <numeric>

#include "equalizer_drawer.h"

//--------------------------------------------------------------
void CircularEqualizerDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;
    vertexes_.resize(kPointNum);
    color_list_.resize(kPointNum);

    float hue_step = 1.f / static_cast<float>(kBarNum);
    for (size_t i = 0; i < kBarNum; ++i) {
        ofFloatColor color;
        color.setHsb(hue_step * i, 1.f, 1.f, 0.8f);
        for (size_t j = 0; j < 6; ++j) {
            color_list_.at(6 * i + j).set(color);
        }
    }

    vbo_.setVertexData(vertexes_.data(), kPointNum, GL_DYNAMIC_DRAW);
    vbo_.setColorData(color_list_.data(), kPointNum, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void CircularEqualizerDrawer::update(const std::vector<float>& spectrum) {
    float rad_step = 2.f * PI / 256;

    for (int i = 0; i < 256; ++i) {
        float x1 = kOutDirectRadius * sin(rad_step * i);
        float x2 = kOutDirectRadius * sin(rad_step * (i + 1));
        float y1 = kOutDirectRadius * cos(rad_step * i);
        float y2 = kOutDirectRadius * cos(rad_step * (i+1));
        float z1 = 0.f;
        float z2 = kBarHeight * spectrum.at(i);

        vertexes_.at(6 * i + 0).set(x1, y1, z1);
        vertexes_.at(6 * i + 1).set(x2, y2, z1);
        vertexes_.at(6 * i + 2).set(x2, y2, z2);
        vertexes_.at(6 * i + 3).set(x1, y1, z1);
        vertexes_.at(6 * i + 4).set(x2, y2, z2);
        vertexes_.at(6 * i + 5).set(x1, y1, z2);
    }
    vbo_.updateVertexData(vertexes_.data(), kPointNum);

    time_ += 0.1f;
}

//--------------------------------------------------------------
void CircularEqualizerDrawer::draw() {
    cam_.begin();
    ofRotate(time_, 0.f, 0.f, 1.f);
    vbo_.draw(GL_TRIANGLES, 0, kPointNum);
    cam_.end();
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;

    initializeVertex();
    initializeColor();
}

void BoxEqualizerDrawer::initializeVertex() {
    vertexes_.resize(kPointNum);
    
    float width_step  = win_cache_->getWidth() / kBarColNum + 10;
    float height_step = win_cache_->getHeight() * 0.7 / kBarRowNum;

    for (size_t i = 0; i < kBarColNum; ++i) {
        float x1 = width_step * i + kOffsetXforBottom;
        float x2 = x1 + width_step - 10;
        
        for (size_t j = 0; j < kBarRowNum; ++j) {
            float y1 = win_cache_->getHeight() - (height_step * j);
            float y2 = y1 - height_step + 10;

            vertexes_.at(6 * (kBarRowNum * i + j) + 0).set(x1, y1, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 1).set(x2, y2, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 2).set(x1, y2, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 3).set(x1, y1, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 4).set(x2, y1, 0.f);
            vertexes_.at(6 * (kBarRowNum * i + j) + 5).set(x2, y2, 0.f);
        }
    }

    vbo_.setVertexData(vertexes_.data(), kPointNum, GL_DYNAMIC_DRAW);
}

void BoxEqualizerDrawer::initializeColor() {
    color_list_.resize(kPointNum);

    ofFloatColor initial_color;
    initial_color.set(0.f, 0.f);
    for (auto& c : color_list_) {
        c.set(initial_color);
    }

    vbo_.setColorData(color_list_.data(), kPointNum, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::update(const std::vector<float>& spectrum) {
    size_t spectrum_step = spectrum.size() / kBarColNum;
    size_t spectrum_index = 0;

    ofFloatColor color_non;
    color_non.set(0.f, 0.f);
    float hue_step = 1.f / kBarRowNum;

    for (size_t i = 0; i < kBarColNum; ++i) {
        ofFloatColor color;
        float hue_base = 0.f;

        for (size_t j = 0; j < kBarRowNum; ++j) {
            float val = spectrum.at(spectrum_index);
            if (val > ((j + 1) * 0.025f)) {
                ofFloatColor color_upper;
                color.setHsb(hue_base, .7f, 1.f, 0.8f);
                color_upper.setHsb(hue_base + hue_step, .5f, 1.f, 0.8f);

                color_list_.at(6 * (kBarRowNum * i + j) + 0).set(color);
                color_list_.at(6 * (kBarRowNum * i + j) + 1).set(color_upper);
                color_list_.at(6 * (kBarRowNum * i + j) + 2).set(color_upper);
                color_list_.at(6 * (kBarRowNum * i + j) + 3).set(color);
                color_list_.at(6 * (kBarRowNum * i + j) + 4).set(color);
                color_list_.at(6 * (kBarRowNum * i + j) + 5).set(color_upper);
                hue_base += hue_step;
            } else {
                color_list_.at(6 * (kBarRowNum * i + j) + 0).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 1).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 2).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 3).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 4).set(color_non);
                color_list_.at(6 * (kBarRowNum * i + j) + 5).set(color_non);
            }
        }

        spectrum_index += spectrum_step;
    }

    vbo_.updateColorData(color_list_.data(), kPointNum);
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::draw() {
   vbo_.draw(GL_TRIANGLES, 0, kPointNum);
}

//--------------------------------------------------------------
void BoxEqualizerDrawer::resize() {
    initializeVertex();
}

//--------------------------------------------------------------
void GridEqualizerDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;

    vertexes_.resize(kGridNum);
    for (int i = 0; i < kGridRow; ++i) {
        for (int j = 0; j < kGridCol; ++j) {
            float x = static_cast<float>(i);
            float y = static_cast<float>(j);
            vertexes_.at(kGridCol * i + j).set(x, y);
        }
    }

    float base_hue = 0.0;
    colors_list_.resize(kLayerNum);
    for (auto& colors : colors_list_) {
        colors.resize(kGridNum);
        for (auto& color : colors) {
            color.setHsb(base_hue, 0.8, 1.0, 0.9);
        }
        base_hue += 0.1;
    }

    size_t color_index = 0;
    vbo_list_.resize(kLayerNum);
    for (auto& vbo : vbo_list_) {
        vbo.setVertexData(vertexes_.data(), kGridNum, GL_DYNAMIC_DRAW);
        vbo.setColorData(colors_list_.at(color_index).data(), kGridNum, GL_STATIC_DRAW);
        ++color_index;
    }
}

//--------------------------------------------------------------
void GridEqualizerDrawer::update(const std::vector<float>& spectrum) {
    size_t spectrum_step = 2;

    for (int i = 0; i < kGridRow; ++i) {
        size_t spectrum_index = 3;
        size_t index_update_count = 0;

        for (int j = 0; j < kGridCol; ++j) {
            float x = static_cast<float>(i);
            float y = static_cast<float>(j);
            float z = ofMap(spectrum.at(spectrum_index), 0.f, 0.4f, 0.f, 3.f, true);;
            if (i != update_index_) {
                float e = 0.f;
                if (i > update_index_) {
                    e = i - update_index_;
                } else {
                    e = update_index_ - i;
                }

                z = z - (e * e * 0.3);
                if (z < 0.f) {
                    z = 0.f;
                }
            }
            vertexes_.at(kGridCol * i + j).set(x, y, z);

            ++index_update_count;
            if (index_update_count == 2) {
                spectrum_index += spectrum_step;
                index_update_count = 0;
            }
        }
    }

    for (auto& vbo : vbo_list_) {
        vbo.updateVertexData(vertexes_.data(), kGridNum);
    }

    ++update_index_;
    if (update_index_ == kGridRow) {
        update_index_ = 0;
    }
}

//--------------------------------------------------------------
void GridEqualizerDrawer::draw() {
    cam_.begin();
    cam_.setFov(25.f);
    cam_.lookAt(ofVec3f(32, 16, 0), ofVec3f(0, 0, 1));
    cam_.setPosition(ofVec3f(64, -64, 32));

    glPointSize(2);
    ofPushMatrix();

    for (size_t i = 0; i < 4; ++i) {
        ofPushMatrix();
        ofTranslate(0, 0, 10);

        for (const auto& vbo : vbo_list_) {
            vbo.draw(GL_POINTS, 0, kGridNum);
            ofTranslate(0, 0, -4);
        }

        ofPopMatrix();
        ofTranslate(16, 0, 0);
    }

    ofPopMatrix();
    cam_.end();
}

//--------------------------------------------------------------
void CylinderEqualizerDrawer::setup(std::shared_ptr<ofRectangle> win_cache) {
    win_cache_ = win_cache;

    for (size_t i = 0; i < 255; ++i) {
        while (true) {
            ofVec3f position(ofRandom(-kAreaMax, kAreaMax),
                             0.f,
                             ofRandom(-kAreaMax, kAreaMax));
            float radius = ofRandom(kRadiusMin, kRadiusMax);
            if (!isOverrap(cylinders_, position, radius)) {
                ofCylinderPrimitive cp;
                cp.setPosition(position);
                cp.setRadius(radius);
                cp.setResolutionRadius(32);
                cylinders_.emplace_back(cp);
                break;
            }
        }
    }

    point_light_.setDiffuseColor(ofColor(0.f, 255.f, 0.f));
    point_light_.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    point_light_.setPosition(0.f, 0.f, 0.f);

    light_color_.setBrightness(255.f);
    light_color_.setSaturation(180.f);
    material_color_.setBrightness(255.f);
    material_color_.setSaturation(180.f);
    material_.setShininess(32);

}

bool CylinderEqualizerDrawer::isOverrap(const std::vector<ofCylinderPrimitive>& cylinders,
                                        ofVec3f& pos,
                                        float radius) const {
    for (const auto& cylinder : cylinders) {
        ofVec3f cylinder_pos = cylinder.getPosition();
        float   cylinder_radius = cylinder.getRadius();
        float dist = ofDist(cylinder_pos.x, cylinder_pos.y, cylinder_pos.z,
                            pos.x,          pos.y,          pos.z);
        float threshold = cylinder_radius + radius;

        if (dist < threshold) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------
void CylinderEqualizerDrawer::update(const std::vector<float>& spectrum) {
    float hue = color_ * 255.f;
    light_color_.setHue(hue);
    point_light_.setDiffuseColor(light_color_);

    material_color_.setHue(hue);
    material_.setSpecularColor(material_color_);

    for (size_t i = 0; i < cylinders_.size(); ++i) {
        float height = ofMap(spectrum.at(i), 0.1f, 0.3f, kHeightMin, kHeightMax, true);
        cylinders_.at(i).setHeight(height);

        ofVec3f pos = cylinders_.at(i).getPosition();
        pos.y = height / 2.f;
        cylinders_.at(i).setPosition(pos);
    }
}

//--------------------------------------------------------------
void CylinderEqualizerDrawer::draw() {
    cam_.begin();
    cam_.setFov(45.f);
    cam_.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
    cam_.setPosition(ofVec3f(100, 800, 100));

    ofEnableDepthTest();
    ofEnableLighting();

    point_light_.enable();
    material_.begin();

    for (const auto& cylinder : cylinders_) {
        cylinder.draw();
    }

    point_light_.disable();
    material_.end();

    ofDisableLighting();
    ofDisableDepthTest();
    cam_.end();
}
