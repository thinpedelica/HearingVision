#pragma once

#include <vector>

struct Nanokon2Items {
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

    Nanokon2Items();
    ~Nanokon2Items() = default;
};
