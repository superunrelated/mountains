#include "ofApp.h"


int noiseW = 12;
int noiseH = 12;
ofVec3f offset = ofVec3f(0, 0, 0);
int zoom = 200;

int scale = 1;
int scaleMin = 1;
int scaleMax = 20;

int space = 6;
int spaceMin = 1;
int spaceMax = 20;

int extrusion = 100;
int extrusionMin = 0;
int extrusionMax = 1000;

int meshW = noiseW * scaleMin;
int meshH = noiseH * scaleMin;

bool dirty = true;
bool guiHide = false;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //glShadeModel(GL_FLAT);
    
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(66,66,66);
    
    // UIGUI
    uigui = new ofxUICanvas();
    uigui->addSlider("BACKGROUND",0.0,255.0,100.0);
    uigui->autoSizeToFitWidgets();
    ofAddListener(uigui->newGUIEvent, this, &ofApp::guiEvent);
    uigui->loadSettings("settings.xml");
    
    
    // GUI
    gui.setup();
    guiHide = false;
    gui.add(zoomSlider.setup("Zoom", zoom, 100, 1500 ));
    
    gui.add(noiseWSlider.setup("noiseW", noiseW, 2, 128 ));
    gui.add(noiseHSlider.setup("noiseH", noiseH, 2, 128 ));
    gui.add(scaleSlider.setup("Scale", scale, scaleMin, scaleMax ));
    gui.add(spaceSlider.setup("Space", space, spaceMin, spaceMax ));
    gui.add(alpha.setup("alpha", 2.0, 0.5, 10.0));
    gui.add(beta.setup("beta", 2.0, 0.5, 5.0));
    
    gui.add(normalize.setup("Normalize", true));
    gui.add(noiseXSlider.setup("NoiseX", 0, 0, 10 ));
    gui.add(noiseYSlider.setup("NoiseY", 0, 0, 10 ));
    gui.add(extrusionSlider.setup("Extrusion", extrusion, extrusionMin, extrusionMax));
    gui.add(fps.setup("FPS", ""));
    
    debugGroup.setName("Debug");
    debugGroup.add(showWireframe.set("Wireframe", true));
    debugGroup.add(showNormals.set("Normals", true));
    debugGroup.add(showNoise.set("Noise", true));
    gui.add(debugGroup);
    
    gui.add(lightPosition.set("Light Position",
                                     ofVec3f(0,     0,      500),
                                     ofVec3f(-500, -500,    0),
                                     ofVec3f(500,   500,    500)
                                     )
                   );
    
    gui.loadFromFile("settings.xml");
    
    
    noiseWSlider.addListener(this, &ofApp::floatValChanged);
    noiseHSlider.addListener(this, &ofApp::floatValChanged);
    scaleSlider.addListener(this, &ofApp::floatValChanged);
    spaceSlider.addListener(this, &ofApp::floatValChanged);
    alpha.addListener(this, &ofApp::floatValChanged);
    beta.addListener(this, &ofApp::floatValChanged);
    noiseXSlider.addListener(this, &ofApp::floatValChanged);
    noiseYSlider.addListener(this, &ofApp::floatValChanged);
    extrusionSlider.addListener(this, &ofApp::floatValChanged);
    
    
    // NOISE
//    simplex = new ofxSimplex();

    
    // MESH
//    meshImage.allocate(meshW, meshH, OF_IMAGE_COLOR);
//    generateMesh(meshW, meshH);
    
    pointLight.setDiffuseColor(ofColor(255.f, 255.f, 255.f));       // The Color
    pointLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));      // highlights
    pointLight.setAmbientColor(ofColor(0, 0, 0));                   // Shadow
//    pointLight.setDirectional();
//    pointLight.setOrientation( ofVec3f(0, 90, 0) );
    
    material.setAmbientColor(ofColor(0, 0, 0));                     // Shadow
    material.setDiffuseColor(ofColor(0, 0, 0));                     // The Color
    material.setEmissiveColor(ofColor(0, 5, 0));                    // Self lit
    material.setSpecularColor(ofColor(0, 0, 0));                    // highlights
    material.setShininess(150);
}

void ofApp::exit(){
    uigui->saveSettings("settings.xml");
    delete uigui;
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    pointLight.setPosition(lightPosition);
    cam.setDistance(zoomSlider);
    
    if (dirty){
        noiseW = noiseWSlider;
        noiseH = noiseHSlider;
        meshW = noiseW * scaleSlider;
        meshH = noiseH * scaleSlider ;
        offset = ofVec3f(-meshW/2 * spaceSlider, -meshH/2 * spaceSlider, extrusionSlider/2);
        
        
        generateNoise(noiseImage, noiseXSlider, noiseYSlider);
        
        // SCALE UP NOISE
        meshImage.clone(noiseImage);
        meshImage.resize(meshW, meshH);
        
        generateMesh(mesh, meshW, meshH, spaceSlider);
        updateMesh(mesh, meshImage, extrusionSlider);
        dirty = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(50, 50, 50), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);
    
    ofDisableDepthTest();
    if (showNoise){
        noiseImage.draw(0,0);
        meshImage.draw(noiseW,0);
    }
    
    ofEnableDepthTest();
    cam.begin();
    ofEnableLighting();
    material.begin();
    ofSetColor(255, 255, 255);

    pointLight.enable();
    
    ofPushMatrix();
    ofTranslate(offset);
    if (showWireframe){
        mesh.drawWireframe();
    } else {
//        mesh.drawFaces();
        mesh.draw();
    }
    ofPopMatrix();
    
    material.end();
    
    // WORLD CENTER
    ofSetColor(255, 255, 255);
    ofDrawSphere(0, 0, 0, 15);
    
    // LIGHT
    ofSetColor(255, 255, 0);
    ofDrawSphere(pointLight.getPosition() + ofVec3f(0, 0, 20), 15);
    
    ofSetColor(70, 00, 70);
    ofDrawPlane(0, 0, 0, 1000, 1000);
    
    
    pointLight.disable();
    ofDisableLighting();
    
    // NORMALS
    if (showNormals){
        ofPushMatrix();
        ofTranslate(offset);
        drawNormals();
        ofPopMatrix();
    }

    cam.end();
    ofDisableDepthTest();
    
    


    
    if(!guiHide){
        gui.draw();
        fps = ofToString(ofGetFrameRate());
    }
}

// -- BEGIN CUSTOM STUFF --

void ofApp::drawNormals(){
    // draw our normals, and show that they are perpendicular to the vector from the center to the vertex
    vector<ofVec3f> n = mesh.getNormals();
    vector<ofVec3f> v = mesh.getVertices();
    float normalLength = space/2;
    
    if(!ofGetKeyPressed()){
        ofDisableLighting();
        ofSetColor(255,0,0,255);
        for(unsigned int i=0; i < n.size(); i++){
            ofLine(v[i].x,
                   v[i].y,
                   v[i].z,
                   v[i].x + n[i].x * normalLength,
                   v[i].y + n[i].y * normalLength,
                   v[i].z + n[i].z * normalLength
            );
        }
    }
}


void ofApp::generateNoise(ofFloatImage &img, float offsetX, float offsetY){
    double maxval = 0.0;
    double minval = 10.0;
    int octaves = 5;
    
    noiseImage.allocate(noiseW, noiseH, OF_IMAGE_GRAYSCALE);
    noiseImage.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    
    for (int i = 0; i < img.getWidth(); i++) {
        for (int j = 0; j < img.getHeight(); j++) {
            double sum = 0;
            double scale = 1.0;
            double p[2];
            p[0] = i /(float)img.getWidth() + offsetX;
            p[1] = j /(float)img.getHeight() + offsetY;
            
            for (int k = 0; k < octaves; k++) {
                double val = ofNoise(p[0], p[1]);
//                images[k].setColor(i, j, val);
                sum += val / scale;
                scale *= alpha;
                p[0] *= beta;
                p[1] *= beta;
            }
            img.setColor(i, j, sum);
            if (sum > maxval)
                maxval = sum;
            if (sum < minval)
                minval = sum;
        }
    }
    
    if (normalize) {
        for (int i = 0; i < img.getWidth(); i++) {
            for (int j = 0; j < img.getHeight(); j++) {
                float v = img.getColor(i, j).r;
                img.setColor(i, j, v/maxval);
                //                img.setColor(i, j, ofMap(v, 0.0, maxval, 0.0, 1.0));
                //                img.setColor(i, j, ofMap(v, minval, maxval, 0.0, 1.0));
            }
        }
    }
    
    img.update();
    
//    for (ofFloatImage &i : images)
//        i.update();
    
//    dirty = false;
}


/*
void ofApp::generateNoise(ofFloatImage &img, int offsetX, int offsetY){
    img.setColor(ofColor::white);
    float scale = .007;
    int w = img.getWidth();
    int h = img.getHeight();
    float maxValue = 0;
    float minValue = 1;
    
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            float n = sumOctave(16, x+offsetX, y+offsetY, .5, scale, 0, 1);
            maxValue = max(maxValue, n);
            minValue = min(minValue, n);
            img.setColor(x % w, y % h, ofFloatColor(n));
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
*/

void ofApp::generateMesh(ofVboMesh &mesh, int width, int height, int space){
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.enableIndices();
    
    for (int y = 0; y<height; y++){
        for (int x = 0; x<width; x++){
            mesh.addVertex(ofVec3f(x * space, y * space, 0));	// mesh index = x + y*width
            // this replicates the pixel array within the camera bitmap...
            mesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
//            
//            mesh.addNormal(ofVec3f(0, 0, 1));
//            mesh.addTexCoord(ofVec2f(x * space, y * space));
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

void ofApp::updateMesh(ofVboMesh &mesh, ofFloatImage &img, float extrusion){
    int width = img.getWidth();
    int height = img.getHeight();
//    float * noisePixels = img.getPixels();
    for (int i = 0; i<width*height; i++) {
        ofFloatColor sampleColor(img.getPixels()[i]);
//        ofFloatColor->alpha = 0.0f;
        ofVec3f tmpVec = mesh.getVertex(i);
        tmpVec.z = (-extrusion/2) + sampleColor.getBrightness()*extrusion;
        mesh.setVertex(i, tmpVec);
        mesh.setColor(i, sampleColor);
    }
    ofxMeshUtils::calcNormals(mesh, true);
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
