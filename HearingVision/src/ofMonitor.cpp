#include "ofMonitor.h"

//--------------------------------------------------------------
void ofMonitor::setMonitorIndex(const int index) {
    monitor_index_ = index;
}

void ofMonitor::setMainApp(const shared_ptr<ofApp> app) {
    main_app_ = app;
}

void ofMonitor::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetBackgroundColor(0);

    win_cache_.setSize(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofMonitor::update() {
    // nop
}

//--------------------------------------------------------------
void ofMonitor::draw() {
    ofSetColor(255);
    main_app_->getMonitor(monitor_index_).draw(0, 0,
                                              win_cache_.getWidth(),
                                              win_cache_.getHeight());
    main_app_->getNextScene(monitor_index_).draw();
}
