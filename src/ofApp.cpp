#include "ofApp.h"

ofApp::ofApp(std::shared_ptr<ofAppBaseWindow> myWindow) :
        myWindow(myWindow) {
}

ofApp::~ofApp() {
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_WARNING);
    int const desiredWidth = 800;
    int const desiredHeight = 600;
    int const desiredFps = 30;
    int const defaultBufferSize = 1;
    double const beatsPerMinute = 60;
    streamManager = new ofxBenG::stream_manager(desiredWidth, desiredHeight, desiredFps, defaultBufferSize);
    ofAddListener(streamManager->onVideoStreamAdded, this, &ofApp::onVideoStreamAdded);
    ofAddListener(streamManager->onVideoStreamRemoved, this, &ofApp::onVideoStreamRemoved);
    monitorManager = new ofxBenG::monitor_manager();
    ofAddListener(monitorManager->onMonitorAdded, this, &ofApp::onMonitorAdded);
    ofAddListener(monitorManager->onMonitorRemoved, this, &ofApp::onMonitorRemoved);
    windowManager = new ofxBenG::window_manager();
    ofxBenG::ableton()->setupLink(beatsPerMinute, 8.0);
    timeline = new ofxBenG::timeline();
    ofSoundStreamSetup(2, 0, this, ofxBenG::audio::getInstance()->getSampleRate(), ofxBenG::audio::getInstance()->getBufferSize(), 4);
    gasClick = new ofxMaxiSample();
    gasClick->load("/Users/admin/Dropbox/Audio/other samples/251814__zabuhailo__gasstovelektropod_click1_short.wav");
    metronome1 = new ofxMaxiSample();
    metronome1->load("/Users/admin/Dropbox/Audio/other samples/250552__druminfected__metronome.wav");
    metronome2 = new ofxMaxiSample();
    metronome2->load("/Users/admin/Dropbox/Audio/other samples/250551__druminfected__metronomeup.wav");
    forgetYou = new ofxMaxiSample();
    forgetYou->load("/Users/admin/Dropbox/Audio/other samples/Panda Bear - Bros/forget-you.wav");
    forgetYou->setVolume(0.5);
    youMay = new ofxMaxiSample();
    youMay->load("/Users/admin/Dropbox/Audio/other samples/Panda Bear - Bros/you-may.wav");
    growUp = new ofxMaxiSample();
    growUp->load("/Users/admin/Dropbox/Audio/other samples/Panda Bear - Bros/grow-up.wav");
}

void ofApp::update() {
    streamManager->update();
    monitorManager->update();
    timeline->update();
}

void ofApp::draw() {
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; i++) {
        float mix = ofxBenG::audio::getInstance()->getMix();
        output[nChannels * i] = mix;
        output[nChannels * i + 1] = mix;
    }
}

void ofApp::onVideoStreamAdded(ofxBenG::video_stream &stream) {
    std::cout << "onVideoStreamAdded stream: " << stream.getDeviceName() << std::endl;
    auto window = windowManager->getWindowWithNoStream();
    if (window != nullptr) {
        window->setStream(&stream);
    }
}

void ofApp::onVideoStreamRemoved(ofxBenG::video_stream &stream) {
    std::cout << "onVideoStreamRemoved stream: " << stream.getDeviceName() << std::endl;
}

void ofApp::onMonitorAdded(ofxBenG::monitor &monitor) {
    std::cout << "Monitor added: " << monitor.toString() << std::endl;
    auto window = windowManager->makeWindow(myWindow);
    window->setStream(streamManager->getUnusedStream());
    window->setMonitor(&monitor);
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
        auto stream = streamManager->getStream(0);
        float const blackoutLengthBeats = 0.05;
        float const videoLengthBeats = ofxBenG::utilities::secondsToBeats(5, ofxBenG::ableton()->getTempo());
        timeline->scheduleNextWholeBeat(new ofxBenG::flicker(stream, blackoutLengthBeats, videoLengthBeats));
    }

    if (key == 's') {
        timeline->scheduleNextWholeMeasure(new ofxBenG::generic_action([this]() {
            ofxBenG::ableton()->setClockToZero();
            scheduleNextMeasure();
        }));
    }
}

void ofApp::scheduleNextMeasure() {
    int const measure = ((int) floor(ofxBenG::ableton()->getBeat())) / 4;

    if (measure % 2 == 0) {
        auto stream = streamManager->getStream(0);
        float const blackoutLengthBeats = 0.05;
        float const videoLengthBeats = 4;
        timeline->schedule(0, new ofxBenG::flicker(stream, blackoutLengthBeats, videoLengthBeats));
        timeline->schedule(0, new ofxBenG::generic_action([this]() {
            ofxBenG::audio::getInstance()->playSample(forgetYou);
        }));
    }

//    if (measure % 3 == 0) {
//        std::cout << ofxBenG::ableton()->getBeat() << ": setTempo(30)" << std::endl;
//        ofxBenG::ableton()->setTempo(30);
//    } else {
//        std::cout << ofxBenG::ableton()->getBeat() << ": setTempo(60)" << std::endl;
//        ofxBenG::ableton()->setTempo(60);
//    }

    timeline->schedule(1, new ofxBenG::generic_action([this]() {
        std::cout << ofxBenG::ableton()->getBeat() << ": play metronome1" << std::endl;
        ofxBenG::audio::getInstance()->playSample(metronome1);
    }));
    timeline->schedule(2, new ofxBenG::generic_action([this]() {
        std::cout << ofxBenG::ableton()->getBeat() << ": play metronome1" << std::endl;
        ofxBenG::audio::getInstance()->playSample(metronome1);
    }));
    timeline->schedule(3, new ofxBenG::generic_action([this]() {
        std::cout << ofxBenG::ableton()->getBeat() << ": play metronome2" << std::endl;
        ofxBenG::audio::getInstance()->playSample(metronome2);
    }));
    timeline->schedule(4, new ofxBenG::generic_action([this]() {
        std::cout << ofxBenG::ableton()->getBeat() << ": play gasClick" << std::endl;
        ofxBenG::audio::getInstance()->playSample(gasClick);
        scheduleNextMeasure();
    }));
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
