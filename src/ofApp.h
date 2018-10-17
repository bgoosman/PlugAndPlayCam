#pragma once

#include "ofMain.h"
#include "ofxBenG.h"
#include <functional>

class ofApp : public ofBaseApp {

public:
    ofApp(std::shared_ptr<ofAppBaseWindow> mainWindow);
    ~ofApp();

    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void onVideoStreamAdded(ofxBenG::video_stream &stream);
    void onVideoStreamRemoved(ofxBenG::video_stream &stream);
    void onMonitorAdded(ofxBenG::monitor &monitor);
    void onMonitorRemoved(ofxBenG::monitor &monitor);

    std::shared_ptr<ofAppBaseWindow> myWindow;
    ofxBenG::playmodes *playModes;
    ofxBenG::monitor_manager *monitorManager;
};