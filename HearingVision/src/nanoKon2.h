#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include <vector>

class NanoKon2 : public ofxMidiListener {
	
public:
	NanoKon2();
	~NanoKon2();
	void setup();
	void update();
	void exit();
	void newMidiMessage(ofxMidiMessage& eventArgs);
	
	static constexpr int kToggleNum = 8;
	
	std::vector<int>  knobs;
	std::vector<int>  sliders;
	std::vector<bool> buttonsSolo;
    std::vector<bool> buttonsMute;
	std::vector<bool> buttonsRec;
    
    bool buttonTrackLeft, buttonTrackRight;
    bool buttonCycle;
    bool buttonMarkerSet, buttonMarkerLeft, buttonMarkerRight;
    bool buttonRewind, buttonFastForward;
    bool buttonStop, buttonPlay, buttonRecord;

private:
	static constexpr int kKnobsStartIndex 			= 0;
	static constexpr int kSlidersStartIndex 		= 16;
	static constexpr int kButtonnsSoloStartIndex 	= 32;
	static constexpr int kButtonsMuteStartIndex 	= 48;
	static constexpr int kButtonsRecStartIndex 		= 64;
	static constexpr int kButtonTrackLeftIndex 		= 58;
	static constexpr int kButtonTrackRightIndex 	= 59;
	static constexpr int kButtonCycleIndex 			= 46;
	static constexpr int kButtonMarkerSetIndex 		= 60;
	static constexpr int kButtonMarkerLeftIndex 	= 61;
	static constexpr int kButtonMarkerRightIndex 	= 62;
	static constexpr int kButtonRewindIndex 		= 43;
	static constexpr int kButtonFastForwardIndex 	= 44;
	static constexpr int kButtonStopIndex 			= 42;
	static constexpr int kButtonPlayIndex 			= 41;
	static constexpr int kButtonRecordIndex 		= 45;

	bool isMyMessage(const int message_control,
					const int my_control,
					const int range) const;

	bool isMyMessage(const int message_control,
					const int my_control) const;

	bool isMessageBool(const int message_value,
						bool &value) const;

	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
};
