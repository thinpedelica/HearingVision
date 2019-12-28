#pragma once

#include <vector>

#include "ofMain.h"
#include "ofxOsc.h"

class ParamController {

public:
    struct Param {
        float level;
        float speed;
        float gain;
        bool  mode;
        bool  reset;

        Param();
    };

    ParamController();
	virtual ~ParamController();
	void setup();
	void update();
	Param get(const size_t index) const;
	void exit();

private:
    static constexpr int kListenPort = 9200;
    static const std::string kLabelKnob;
    static const std::string kLabelButton;

    void updateOscMessage();
    void getValueKnob(const ofxOscMessage& msg, std::vector<Param>& params);
    void getValueButton(const ofxOscMessage& msg, std::vector<Param>& params);

    ofxOscReceiver receiver;

    std::vector<Param> params_;
};
