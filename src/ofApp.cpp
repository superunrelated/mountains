#include "ofApp.h"


int noiseW = 100;
int noiseH = 100;
int scale = 6;
int meshW = noiseW * scale;
int meshH = noiseH * scale;

bool dirty = true;
bool guiHide = false;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(66,66,66);
    
    // GUI
    gui.setup();
    guiHide = false;
    gui.add(scaleSlider.setup("Scale", scale, 1, 20 ));
    gui.add(noiseXSlider.setup("NoiseX", 0, 0, 1000 ));
    gui.add(noiseYSlider.setup("NoiseY", 0, 0, 1000 ));
    gui.add(extrusionSlider.setup("Extrusion", 300, 0, 1000 ));
    gui.add(showWireframe.setup("Wireframe", false));
    gui.add(showNoise.setup("Noise", false));
    gui.add(fps.setup("FPS", ""));
    
    scaleSlider.addListener(this, &ofApp::floatValChanged);
    noiseXSlider.addListener(this, &ofApp::floatValChanged);
    noiseYSlider.addListener(this, &ofApp::floatValChanged);
    extrusionSlider.addListener(this, &ofApp::floatValChanged);
    
    
    // NOISE
    simplex = new ofxSimplex();
    noiseImage.allocate(noiseW, noiseH, OF_IMAGE_COLOR);
    
    // MESH
//    meshImage.allocate(meshW, meshH, OF_IMAGE_COLOR);
//    generateMesh(meshW, meshH);
    
//    spot.setDiffuseColor(ofFloatColor(255.0, 0.0, 0.0f));
//    spot.setSpecularColor(ofColor(0, 0, 255));
//    spot.setSpotlight();
//    spot.setSpotConcentration(5);
//    spot.setSpotlightCutOff(10);
    
//    spot_rot = ofVec3f(0, 0, 0);
//    setLightOri(spot, spot_rot);
    
//    spot.setPosition(0, 0, 300);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (dirty){
        generateNoise(noiseImage, noiseXSlider, noiseYSlider);
        meshImage.clone(noiseImage);
        int meshW = noiseW * scaleSlider;
        int meshH = noiseH * scaleSlider;
        meshImage.resize(meshW, meshH);
        generateMesh(mesh, meshW, meshH);
        updateMesh(mesh, meshImage, extrusionSlider);
        dirty = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDisableDepthTest();
    if (showNoise){
        noiseImage.draw(0,0);
        meshImage.draw(noiseW,0);
    }
    
    //but we want to enable it to show the mesh
    ofEnableDepthTest();
    cam.begin();
//    ofEnableLighting();
//    spot.enable();
    
//    ofRotateX(ofRadToDeg(.5));
//    ofRotateY(ofRadToDeg(-.5));
    
    ofTranslate(-meshW/2, -meshH/2, 0);
//    mainMesh.rotate(rotation->x, rotation->y, rotation->z);
    if (showWireframe){
        mesh.drawWireframe();
    } else {
        mesh.drawFaces();
    }
    
//    spot.disable();
//    ofDisableLighting();
    cam.end();
    ofDisableDepthTest();
    
    if(!guiHide){
        gui.draw();
        fps = ofToString(ofGetFrameRate());
    }
}

// -- BEGIN CUSTOM STUFF --

void ofApp::generateNoise(ofImage &img, int offsetX, int offsetY){
    img.setColor(ofColor::white);
    float scale = .007;
    int w = img.getWidth();
    int h = img.getHeight();
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            // float n = (simplex->noise(x * scale, y * scale) + 1) / 2.0 * 255.0;
            float n = sumOctave(16, x+offsetX, y+offsetY, .5, scale, 0, 255);
            ofColor color= ofColor(n,255);
            img.setColor(x % w, y % h, color);
        }
    }
    img.update();
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

void ofApp::generateMesh(ofVboMesh &mesh, int width, int height){
    mesh.clear();
    for (int y = 0; y<height; y++){
        for (int x = 0; x<width; x++){
            mesh.addVertex(ofPoint(x,y,0));	// mesh index = x + y*width
            // this replicates the pixel array within the camera bitmap...
            mesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
        }
    }
    
    for (int y = 0; y<height-1; y++){
        for (int x=0; x<width-1; x++){
            mesh.addIndex(x+y*width);				// 0
            mesh.addIndex((x+1)+y*width);			// 1
            mesh.addIndex(x+(y+1)*width);			// 10
            
            mesh.addIndex((x+1)+y*width);			// 1
            mesh.addIndex((x+1)+(y+1)*width);		// 11
            mesh.addIndex(x+(y+1)*width);			// 10
        }
    }
}

void ofApp::updateMesh(ofVboMesh &mesh, ofImage &img, float extrusion){
    int width = img.getWidth();
    int height = img.getHeight();
    unsigned char * noisePixels = img.getPixels();
    for (int i = 0; i<width*height; i++) {
        ofFloatColor sampleColor(noisePixels[i*3]/255.f,			// r
                                 noisePixels[i*3+1]/255.f,			// g
                                 noisePixels[i*3+2]/255.f);			// b

        ofVec3f tmpVec = mesh.getVertex(i);
        tmpVec.z = (-extrusion/2) + sampleColor.getBrightness()*extrusion;
        mesh.setVertex(i, tmpVec);
        mesh.setColor(i, sampleColor);
    }
}

void ofApp::vec2ValChanged(ofVec2f &val) { dirty = true; }
void ofApp::floatValChanged(float &val) { dirty = true; }
void ofApp::boolValChanged(bool &val) { dirty = true; }
void ofApp::intValChanged(int &val) { dirty = true; }

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
