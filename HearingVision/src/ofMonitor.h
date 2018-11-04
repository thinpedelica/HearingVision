#pragma once

#include "ofMain.h"
#include "ofApp.h"
#include <memory>

class ofMonitor : public ofBaseApp{

public:
    void setMonitorIndex(const int index);
    void setMainApp(const shared_ptr<ofApp> app);
    void setup();
    void update();
    void draw();


private:
    shared_ptr<ofApp> main_app_;
    int monitor_index_;
    ofRectangle win_cache_;
};
