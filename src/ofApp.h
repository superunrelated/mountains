#pragma once

#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxGui.h"
#include "ofxDofShader.h"

#include "NoiseGenerator.h"
#include "MeshGenerator.h"

class ofApp : public ofBaseApp{
private:
    bool dirty = true;
    bool guiHide = false;
    
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
    ofParameter<float> zoom, scale;
    ofParameter<ofVec2f> size;
    ofParameter<bool> showHelpers;
    void vec2ValChanged(ofVec2f &val);
    void floatValChanged(float &val);
    
    // COMPONENTS
    NoiseGenerator noise, distortion;
    MeshGenerator mesh;
    Utils utils;

    ofParameter<ofVec3f> lightPosition;
    ofxLabel fps;

    ofLight pointLight;
    ofEasyCam cam;
    
    // DOF
    ofFbo scene;
    ofFbo dof;
    ofxDofShader shader;
    void setupDepthOfField(int width, int height);
};
