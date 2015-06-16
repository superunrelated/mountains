#pragma once

#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxGui.h"
#include "ofxMeshUtils.h"
#include "ofxUI.h"


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
    
    
    // UTILS
    void drawNormals();
    
    // OTHER GUI
    ofxUICanvas *uigui;
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    
    
    // GUI
    ofxPanel gui;
    ofxFloatSlider zoomSlider;
    ofxFloatSlider noiseWSlider;
    ofxFloatSlider noiseHSlider;
    ofxFloatSlider scaleSlider;
    ofxFloatSlider spaceSlider;
    
    ofxToggle normalize;
    ofxFloatSlider alpha;
    ofxFloatSlider beta;
    ofxFloatSlider noiseXSlider;
    ofxFloatSlider noiseYSlider;
    ofxFloatSlider extrusionSlider;
    ofxLabel fps;
    
    ofParameter<bool> showWireframe;
    ofParameter<bool> showNoise;
    ofParameter<bool> showNormals;
    ofParameterGroup debugGroup;
    
    ofParameter<ofVec3f> lightPosition;
    
    void vec2ValChanged(ofVec2f &val);
    void floatValChanged(float &val);
    void boolValChanged(bool &val);
    void intValChanged(int &val);
    

    // NOISE
    ofxSimplex *simplex;
    ofFloatImage noiseImage;
    ofFloatImage meshImage;
    void generateNoise(ofFloatImage &img, float offsetX, float offsetY);
    float sumOctave(int num_iterations, float x, float y, float persistence, float scale, int low, int high);
    
    
    // MESH
    ofLight pointLight;
    ofEasyCam cam; // add mouse controls for camera movement
    ofVboMesh mesh;
    void generateMesh(ofVboMesh &mesh, int width, int height, int space);
    void updateMesh(ofVboMesh &mesh, ofFloatImage &img, float extrusion);
    ofMaterial material;
};
