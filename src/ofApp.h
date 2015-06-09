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
    
    
    // GUI
    ofxPanel gui;
    ofxFloatSlider scaleSlider;
    ofxFloatSlider noiseXSlider;
    ofxFloatSlider noiseYSlider;
    ofxFloatSlider extrusionSlider;
    ofxToggle showWireframe;
    ofxToggle showNoise;
    ofxLabel fps;
    void vec2ValChanged(ofVec2f &val);
    void floatValChanged(float &val);
    void boolValChanged(bool &val);
    void intValChanged(int &val);
    

    // NOISE
    ofxSimplex *simplex;
    ofImage noiseImage;
    ofImage meshImage;
    void generateNoise(ofImage &img, int offsetX, int offsetY);
    float sumOctave(int num_iterations, float x, float y, float persistence, float scale, int low, int high);
    
    
    // MESH
    ofLight spot;
    ofEasyCam cam; // add mouse controls for camera movement
    ofVboMesh mesh;
    void generateMesh(ofVboMesh &mesh, int width, int height);
    void updateMesh(ofVboMesh &mesh, ofImage &img, float extrusion);
};
