#pragma once

#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxGui.h"

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
    
    void generateNoise(int w, int h);
    float sumOctave(int num_iterations, float x, float y, float persistence, float scale, int low, int high);
    void updateMesh();
        
    int width;
    int height;
    
    // GUI
    bool guiHide;
    ofxPanel gui;
    ofxVec3Slider rotation;
    ofxVec3Slider position;
    ofxVec2Slider noiseOffset;
    ofxToggle showWireframe;
    ofxToggle showNoise;
    ofxLabel fps;
    
    // NOISE
    ofxSimplex *simplex;
    ofImage simplexImage;
    ofVec2f currentOffset;
    
    // MESH
    ofLight spot;
    ofEasyCam cam; // add mouse controls for camera movement
    float extrusionAmount;
    ofVboMesh mainMesh;
    
};
