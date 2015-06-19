#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup(){
    
    glShadeModel(GL_SMOOTH);
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(66,66,66);
    
    // DOF
    setupDepthOfField(ofGetWidth(), ofGetHeight());
    
    
    // LIGHT  ------------------------------
    pointLight.setDiffuseColor(ofColor(255.f, 255.f, 255.f));       // The Color
    pointLight.setSpecularColor(ofColor(0, 0, 0));            // highlights
    pointLight.setAmbientColor(ofColor(0, 0, 0));                   // Shadow
    //    pointLight.setDirectional();
    //    pointLight.setOrientation( ofVec3f(0, 90, 0) );
    
    
    
    // GUI --------------------------------
    
    parameters.setName("settings");
    parameters.add(zoom.set("Zoom", zoom, 100, 1500));
    parameters.add(size.set("Size",
                            ofVec2f(64, 64),
                            ofVec2f(1,1),
                            ofVec2f(128,128)
                            )
                   );
    parameters.add(scale.set("Scale", 1,   1,   10   ));
    
    noise.setup("Noise");
    parameters.add(noise.parameters);
    
    distortion.setup("Distortion");
    parameters.add(distortion.parameters);
    
    mesh.setup("Mesh");
    parameters.add(mesh.parameters);
    gui.setup(parameters);
    
    gui.add(lightPosition.set("Light Position",
                                     ofVec3f(0,     0,      500),
                                     ofVec3f(-500, -500,    0),
                                     ofVec3f(500,   500,    500)
                                     )
                   );
    
    gui.add(showHelpers.set("Helpers", false, false, true));
    gui.add(fps.setup("FPS", ""));
    
    
    gui.add(shader.passes.setup("dof.passes", 1, 0, 5));
    gui.add(shader.blur.setup("dof.blur", 2.6, 0, 10));
    gui.add(shader.focus.setup("dof.focus", 0.45, 0, 1));
    
    
    gui.loadFromFile("settings.xml");
    
    size.addListener(this, &ofApp::vec2ValChanged);
    scale.addListener(this, &ofApp::floatValChanged);
}

void ofApp::setupDepthOfField(int width, int height){
    ofFbo::Settings fboSettings;
    fboSettings.width = width;
    fboSettings.height = height;
    fboSettings.useDepth = true;
    fboSettings.depthStencilAsTexture = true;
    scene.allocate(fboSettings);
    
    ofFbo::Settings sdof;
    sdof.width  = width;
    sdof.height = height;
    dof.allocate(sdof);
    shader.setup();
}

void ofApp::vec2ValChanged(ofVec2f &val) {
    dirty = true;
}

void ofApp::floatValChanged(float &val) {
    dirty = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    pointLight.setPosition(lightPosition);
    cam.setDistance(zoom);
    if (dirty || noise.getDirty()){
        noise.generate(size, scale);
        distortion.generate(size, scale);
        mesh.generate(noise.scaleImage, distortion.scaleImage);
    } else if (mesh.getDirty()){
        mesh.generate(noise.scaleImage, distortion.scaleImage);
    }
   
}





//--------------------------------------------------------------
void ofApp::draw(){
    
   
   // DOF SCENE
    scene.begin();
    ofClear(0, 0, 0, 0);
    //glEnable(GL_DEPTH_TEST);
    ofBackgroundGradient(ofColor(50, 50, 50), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
    
    
    ofEnableLighting();
    ofEnableDepthTest();
    cam.begin();
    pointLight.enable();
    ofSetColor(255, 255, 255);
    mesh.draw();
    
    pointLight.disable();
    cam.end();
    ofDisableLighting();
    ofDisableDepthTest();
    
    if (showHelpers){
        // CENTER
        ofSetColor(255, 255, 255);
        ofDrawSphere(0, 0, 0, 10);
        // LIGHT
        ofSetColor(255, 255, 0);
        ofDrawSphere(pointLight.getPosition() + ofVec3f(0, 0, 20), 10);
        // PLANE
//        ofSetColor(70, 00, 70);
//        ofDrawPlane(0, 0, 0, 1000, 1000);
    }
    
    // DOF SCENE
    //glDisable(GL_DEPTH_TEST);
    scene.end();
    
    // DOF RENDER
    dof.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(255, 255, 255);
    shader.render(scene);
    dof.end();
    ofSetColor(255, 255, 255);
    dof.draw(0, 0);
    

    
    noise.draw(0, 0);
    distortion.draw(0, noise.scaleImage.height);
    
    if(!guiHide){
        gui.draw();
        fps = ofToString(ofGetFrameRate());
    }
}



//--------------------------------------------------------------
//void ofApp::draw(){
//    ofBackgroundGradient(ofColor(50, 50, 50), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
//    
//    ofEnableLighting();
//    ofEnableDepthTest();
//    cam.begin();
//    
//    pointLight.enable();
//    mesh.draw();
//    
//    if (false){
//        // CENTER
//        ofSetColor(255, 255, 255);
//        ofDrawSphere(0, 0, 0, 10);
//        // LIGHT
//        ofSetColor(255, 255, 0);
//        ofDrawSphere(pointLight.getPosition() + ofVec3f(0, 0, 20), 10);
//        // PLANE
//        ofSetColor(70, 00, 70);
//        ofDrawPlane(0, 0, 0, 1000, 1000);
//    }
//    
//    pointLight.disable();
//    cam.end();
//    ofDisableLighting();
//    ofDisableDepthTest();
//    
//    
//    noise.draw(0, 0);
//    distortion.draw(0, noise.scaleImage.height);
//    
//    if(!guiHide){
//        gui.draw();
//        fps = ofToString(ofGetFrameRate());
//    }
//}


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
    setupDepthOfField(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
