#include "ofApp.h"


int zoom = 200;

bool dirty = true;
bool guiHide = false;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //glShadeModel(GL_FLAT);
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(66,66,66);
    
    // GUI
    parameters.setName("settings");
    parameters.add(zoom.set("Zoom", zoom, 100, 1500));
    
    noise.setup("Noise");
    parameters.add(noise.parameters);
    
    //    distortion.setup("Distortion");
    //    parameters.add(distortion.parameters);
    
    mesh.setup("Mesh");
    parameters.add(mesh.parameters);
    gui.setup(parameters);
    
    ofParameterGroup debug;
    debug.setName("Debug");
    debug.add(showHelpers.set("Helpers", false));
    gui.add(debug);
    
    gui.add(lightPosition.set("Light Position",
                                     ofVec3f(0,     0,      500),
                                     ofVec3f(-500, -500,    0),
                                     ofVec3f(500,   500,    500)
                                     )
                   );
    
    gui.add(fps.setup("FPS", ""));
    gui.loadFromFile("settings.xml");
    
    
    // LIGHT
    pointLight.setDiffuseColor(ofColor(255.f, 255.f, 255.f));       // The Color
    pointLight.setSpecularColor(ofColor(0, 0, 0));            // highlights
    pointLight.setAmbientColor(ofColor(0, 0, 0));                   // Shadow
//    pointLight.setDirectional();
//    pointLight.setOrientation( ofVec3f(0, 90, 0) );
    
    // MATERIAL
    material.setAmbientColor(ofColor(0, 0, 0));                     // Shadow
    material.setDiffuseColor(ofColor(0, 0, 0));                     // The Color
    material.setEmissiveColor(ofColor(0, 0, 0));                    // Self lit
    material.setSpecularColor(ofColor(0, 0, 0));                    // highlights
    material.setShininess(150);
}

//--------------------------------------------------------------
void ofApp::update(){
    pointLight.setPosition(lightPosition);
    cam.setDistance(zoom);
    noise.generate();
    mesh.generate(noise.scaleImage);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(50, 50, 50), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
    
    ofEnableDepthTest();
    ofEnableLighting();
    cam.begin();
    
    pointLight.enable();
    material.begin();
    mesh.draw();
    material.end();
    
    if (showHelpers){
        // WORLD CENTER
        ofSetColor(255, 255, 255);
        ofDrawSphere(0, 0, 0, 15);
        // LIGHT
        ofSetColor(255, 255, 0);
        ofDrawSphere(pointLight.getPosition() + ofVec3f(0, 0, 20), 15);
        // PLANE
        ofSetColor(70, 00, 70);
        ofDrawPlane(0, 0, 0, 1000, 1000);
    }
    
    pointLight.disable();
    ofDisableLighting();

    cam.end();
    ofDisableLighting();
    ofDisableDepthTest();
    
    noise.draw();
    
    if(!guiHide){
        gui.draw();
        fps = ofToString(ofGetFrameRate());
    }
}

// -- BEGIN CUSTOM STUFF --





// -- END CUSTOM STUFF --

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'h':
            guiHide = !guiHide;
            break;
        case 's':
            gui.saveToFile("settings.xml");
            break;
        case 'l':
            gui.loadFromFile("settings.xml");
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
