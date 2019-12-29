#pragma once

#include <map>
#include <vector>

#include "ofMain.h"
#include "ofxOsc.h"

class SceneController {

public:
    SceneController();
	virtual ~SceneController();
	void setup();
	void update();
	int  get(const size_t index) const;
	void exit();

private:
    static constexpr int kListenPort = 9300;
    static const std::string kLabelScene;

    void updateOscMessage();
    void setupKeyMap();
    void setScene(const int scene_index, const int key_index);

    ofxOscReceiver receiver_;

    std::map<int, int> key_map_;
    std::vector<int> scene_ids_;
};
