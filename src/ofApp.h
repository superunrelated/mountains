#pragma once

#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxGui.h"

#include "NoiseGenerator.h"
#include "MeshGenerator.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // GUI
    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameter<float> zoom;
    ofParameterGroup debug;
    ofParameter<bool> showHelpers;
    
    // COMPONENTS
    NoiseGenerator noise, distortion;
    MeshGenerator mesh;
    Utils utils;

    ofParameter<ofVec3f> lightPosition;
    ofxLabel fps;

    ofLight pointLight;
    ofEasyCam cam;
    ofMaterial material;
};