#pragma once

#include "ofMain.h"
#include "nanokon2_base.h"
#include "nanokon2_items.h"
#include "ofxOsc.h"

class Nanokon2osc : public Nanokon2Base {

public:
    Nanokon2osc() = default;
	virtual ~Nanokon2osc();
	void setup() override;
	void update(Nanokon2Items& items) override;
	void exit() override;

private:
    static constexpr int kListenPort = 8888;

    static const std::string kLabelNanokon2;
    static const std::string kLabelKnobs;
    static const std::string kLabelSliders;
    static const std::string kLabelButttonsSolo;
    static const std::string kLabelButttonsMute;
    static const std::string kLabelButttonsRec;
    static const std::string kLabelButtonTrackLeft;
    static const std::string kLabelButtonTrackRight;
    static const std::string kLabelButtonCycle;
    static const std::string kLabelButtonMarkerSet;
    static const std::string kLabelButtonMarkerLeft;
    static const std::string kLabelButtonMarkerRight;
    static const std::string kLabelButtonRewind;
    static const std::string kLabelButtonFastForward;
    static const std::string kLabelButtonStop;
    static const std::string kLabelButtonPlay;
    static const std::string kLabelButtonRecord;

    void updateOscMessage(Nanokon2Items& items);
    void getValue(const ofxOscMessage& msg, Nanokon2Items& items);
    void getValue(const ofxOscMessage& msg, std::vector<int>& item);
    void getValue(const ofxOscMessage& msg, std::vector<bool>& item);
    void getValue(const ofxOscMessage& msg, bool& item);

    bool getBoolValue(const int32_t value);

    ofxOscReceiver receiver;
};
