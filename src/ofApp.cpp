#include "ofApp.h"

ofApp::ofApp(std::shared_ptr<ofAppBaseWindow> myWindow) :
        myWindow(myWindow) {
}

ofApp::~ofApp() {
    delete playModes;
    delete ableton;
    delete timeline;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_WARNING);
    int const desiredWidth = 800;
    int const desiredHeight = 600;
    int const desiredFps = 30;
    int const defaultBufferSize = 1;
    double const beatsPerMinute = 60;
    playModes = new ofxBenG::playmodes(desiredWidth, desiredHeight, desiredFps, defaultBufferSize);
    ofAddListener(playModes->onVideoStreamAdded, this, &ofApp::onVideoStreamAdded);
    ofAddListener(playModes->onVideoStreamRemoved, this, &ofApp::onVideoStreamRemoved);
    monitorManager = new ofxBenG::monitor_manager();
    ofAddListener(monitorManager->onMonitorAdded, this, &ofApp::onMonitorAdded);
    ofAddListener(monitorManager->onMonitorRemoved, this, &ofApp::onMonitorRemoved);
    ableton = new ofxBenG::ableton();
    ableton->setupLink(beatsPerMinute, 8.0);
    timeline = new ofxBenG::timeline();
}

//--------------------------------------------------------------
void ofApp::update() {
    float const currentBeat = ableton->getBeat();
    playModes->update();
    monitorManager->update();
    timeline->update(currentBeat);
}

//--------------------------------------------------------------
void ofApp::draw() {
}

void ofApp::onVideoStreamAdded(ofxBenG::video_stream &stream) {
    std::cout << "onVideoStreamAdded stream: " << stream.getDeviceName() << std::endl;
    auto anyMonitor = monitorManager->getUnusedMonitor();
    if (anyMonitor != nullptr) {
        stream.makeScreen(myWindow, anyMonitor);
    }
}

void ofApp::onVideoStreamRemoved(ofxBenG::video_stream &stream) {
    std::cout << "onVideoStreamRemoved stream: " << stream.getDeviceName() << std::endl;
}

void ofApp::onMonitorAdded(ofxBenG::monitor &monitor) {
    std::cout << "Monitor added: " << monitor.toString() << std::endl;
    auto stream = playModes->getUnusedStream();
    if (stream != nullptr) {
        stream->makeScreen(myWindow, &monitor);
    }
}

void ofApp::onMonitorRemoved(ofxBenG::monitor &monitor) {
    std::cout << "Monitor removed: " << monitor.toString() << std::endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == 'f') {
        auto screenZero = playModes->getStream(0)->getScreen();
        float const blackoutLengthBeats = 0.25;
        float const videoLengthBeats = 5.0;
        timeline->schedule(new ofxBenG::flicker(screenZero, blackoutLengthBeats, videoLengthBeats), ableton->getBeat(), 0);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
