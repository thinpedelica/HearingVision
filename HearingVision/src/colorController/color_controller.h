#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ColorController {

public:
    ColorController() = default;
	virtual ~ColorController();
	void setup();
	void update();
	ofColor get() const;
	void exit();

private:
    static constexpr int kListenPort = 8900;
    static const std::string kLabelColorR;
    static const std::string kLabelColorG;
    static const std::string kLabelColorB;

    void updateOscMessage();

    ofxOscReceiver receiver;
    ofColor color_{0};
};
