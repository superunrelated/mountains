#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(66,66,66);
    
    width = 1000;
    height = 1000;
    
    // GUI
    gui.setup();
    guiHide = false;
    gui.add(noiseOffset.setup("center", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(500,500) ));
    gui.add(rotation.setup("Rotation", ofVec3f(0,0,0), ofVec3f(0,0,0), ofVec3f(1,1,1) ));
    gui.add(position.setup("Position", ofVec3f(0,0,0), ofVec3f(0,0,0), ofVec3f(500, 500, 500) ));
    gui.add(showWireframe.setup("Wireframe", false));
    gui.add(showNoise.setup("Noise", false));
    gui.add(fps.setup("FPS", ""));
    
    
    // NOISE
    simplex = new ofxSimplex();
    simplexImage.allocate(width, height, OF_IMAGE_COLOR);
    currentOffset = ofVec2f(-1,-1);
    generateNoise(width, height);
    
    // MESH
    for (int y = 0; y<height; y++){
        for (int x = 0; x<width; x++){
            mainMesh.addVertex(ofPoint(x,y,0));	// mesh index = x + y*width
												// this replicates the pixel array within the camera bitmap...
            mainMesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
        }
    }
    
    for (int y = 0; y<height-1; y++){
        for (int x=0; x<width-1; x++){
            mainMesh.addIndex(x+y*width);				// 0
            mainMesh.addIndex((x+1)+y*width);			// 1
            mainMesh.addIndex(x+(y+1)*width);			// 10
            
            mainMesh.addIndex((x+1)+y*width);			// 1
            mainMesh.addIndex((x+1)+(y+1)*width);		// 11
            mainMesh.addIndex(x+(y+1)*width);			// 10
        }
    }
    
    extrusionAmount = 200.0;
    updateMesh();
    
    spot.setDiffuseColor(ofFloatColor(255.0, 0.0, 0.0f));
    spot.setSpecularColor(ofColor(0, 0, 255));
    spot.setSpotlight();
    spot.setSpotConcentration(5);
    spot.setSpotlightCutOff(10);
//    spot_rot = ofVec3f(0, 0, 0);
//    setLightOri(spot, spot_rot);
    
    spot.setPosition(0, 0, 300);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofVec2f newOffset = noiseOffset;
    bool of = currentOffset == newOffset;
    if (currentOffset != newOffset){
        generateNoise(width, height);
        updateMesh();
    }
    
    //let's move the camera when you move the mouse
//    float rotateAmount = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 360);
//    
//    //move the camera around the mesh
//    ofVec3f camDirection(0,0,1);
//    ofVec3f centre(width/2, height/2, 255/2.f);
//    ofVec3f camDirectionRotated = camDirection.rotated(rotateAmount, ofVec3f(1,0,0));
//    ofVec3f camPosition = centre + camDirectionRotated * 300;
    
//    mainMesh.setPosition(1, 1, 1);
//    mainMesh.rotate(1, 1, 1);
    
//    cam.setPosition(camPosition);
//    cam.lookAt(centre);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDisableDepthTest();
    if (showNoise){
        simplexImage.draw(0,0);
    }
    
    //but we want to enable it to show the mesh
    ofEnableDepthTest();
    cam.begin();
    ofEnableLighting();
    spot.enable();
    
    ofRotateX(ofRadToDeg(.5));
    ofRotateY(ofRadToDeg(-.5));
    
    ofTranslate(-width/2, -height/2, 0);
//    mainMesh.rotate(rotation->x, rotation->y, rotation->z);
    if (showWireframe){
        mainMesh.drawWireframe();
    } else {
        mainMesh.drawFaces();
    }
    
    spot.disable();
    ofDisableLighting();
    cam.end();
    ofDisableDepthTest();
    
    if(!guiHide){
        gui.draw();
        fps = ofToString(ofGetFrameRate());
    }
}

// -- BEGIN CUSTOM STUFF --

void ofApp::generateNoise(int w, int h){
    currentOffset = (ofVec2f)noiseOffset;
    simplexImage.setColor(ofColor::white);
    float scale = .007;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            
            // float n = (simplex->noise(x * scale, y * scale) + 1) / 2.0 * 255.0;
            float n = sumOctave(16, x + currentOffset.x, y + currentOffset.y, .5, scale, 0, 255);
            ofColor color= ofColor(n,255);
            simplexImage.setColor(x % w, y % h, color);
        }
    }
    simplexImage.update();
}
    
float ofApp::sumOctave(int num_iterations, float x, float y, float persistence, float scale, int low, int high){
    float maxAmp = 0;
    float amp = 1;
    float freq = scale;
    float noise = 0;
    // add successively smaller, higher-frequency terms
    for(int i = 0; i < num_iterations; ++i){
        noise += simplex->noise(x * freq, y * freq) * amp;
        maxAmp += amp;
        amp *= persistence;
        freq *= 2;
    }
    // take the average value of the iterations
    noise /= maxAmp;
    // normalize the result
    noise = noise * (high - low) / 2 + (high + low) / 2;
    return noise;
}

void ofApp::updateMesh(){
    unsigned char * noisePixels = simplexImage.getPixels();
    for (int i = 0; i<width*height; i++) {
        ofFloatColor sampleColor(noisePixels[i*3]/255.f,			// r
                                 noisePixels[i*3+1]/255.f,			// g
                                 noisePixels[i*3+2]/255.f);			// b

        ofVec3f tmpVec = mainMesh.getVertex(i);
        tmpVec.z = (-extrusionAmount/2) + sampleColor.getBrightness()*extrusionAmount;
        mainMesh.setVertex(i, tmpVec);
        mainMesh.setColor(i, sampleColor);
    }
}

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
        case 'n':
            generateNoise(width, height);
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
