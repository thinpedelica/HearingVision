#include "ofMain.h"
#include "ofApp.h"
#include "ofMonitor.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 3);

    settings.width = 1280;
    settings.height = 720;
    settings.setPosition(ofVec2f(200, 0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    settings.width = 800;
    settings.height = 600;
    settings.resizable = false;
    settings.shareContextWith = mainWindow;
    settings.setPosition(ofVec2f(100, 500));
    shared_ptr<ofAppBaseWindow> monitorWindow1 = ofCreateWindow(settings);
    settings.setPosition(ofVec2f(900, 500));
    shared_ptr<ofAppBaseWindow> monitorWindow2 = ofCreateWindow(settings);

    shared_ptr<ofApp> mainApp(new ofApp);
    shared_ptr<ofMonitor> monitorApp1(new ofMonitor);
    shared_ptr<ofMonitor> monitorApp2(new ofMonitor);
    monitorApp1->setMainApp(mainApp);
    monitorApp2->setMainApp(mainApp);

    monitorApp1->setMonitorIndex(0);
    monitorApp2->setMonitorIndex(1);

    ofRunApp(monitorWindow1, monitorApp1);
    ofRunApp(monitorWindow2, monitorApp2);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
