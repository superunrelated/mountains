//
//  NoiseGenerator.cpp
//  Mountains
//
//  Created by Skåla, Knut Emil (OSL-SDG) on 17.06.15.
//
//

#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator() {
    // TODO Auto-generated constructor stub
}

void NoiseGenerator::setup(string name){
    parameters.setName(name);
    parameters.add(size.set("Size",
                            ofVec2f(64, 64),
                            ofVec2f(1,1),
                            ofVec2f(128,128)
                            )
                   );
    parameters.add(offset.set("Offset",
                            ofVec2f(0,  0),
                            ofVec2f(0,  0),
                            ofVec2f(10, 10)
                            )
                   );
    parameters.add(scale.set("Scale", 1,   1,   10   ));
    parameters.add(alpha.set("Alpha", 2.0, 0.5, 10.0 ));
    parameters.add(beta.set("Beta",   2.0, 0.5, 5.0  ));
    parameters.add(showNoise.set("Draw", false       ));
    
    size.addListener(this, &NoiseGenerator::vec2ValChanged);
    offset.addListener(this, &NoiseGenerator::vec2ValChanged);
    scale.addListener(this, &NoiseGenerator::floatValChanged);
    alpha.addListener(this, &NoiseGenerator::floatValChanged);
    beta.addListener(this, &NoiseGenerator::floatValChanged);
}

void NoiseGenerator::vec2ValChanged(ofVec2f &val) {
    dirty = true;
}

void NoiseGenerator::floatValChanged(float &val) {
    dirty = true;
}

bool NoiseGenerator::getDirty(){
    return dirty;
}

void NoiseGenerator::draw(){
    if (showNoise){
        noiseImage.draw(0, 0);
        scaleImage.draw(noiseImage.width, 0);
    }
}

void NoiseGenerator::generate(){
    if (!dirty) return;
    
    double maxval = 0.0;
    double minval = 10.0;
    int octaves = 5;
    
    noiseImage.allocate(size->x, size->y, OF_IMAGE_GRAYSCALE);
    noiseImage.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    
    for (int i = 0; i < noiseImage.getWidth(); i++) {
        for (int j = 0; j < noiseImage.getHeight(); j++) {
            double sum = 0;
            double scale = 1.0;
            double p[2];
            p[0] = i /(float)noiseImage.getWidth() + offset->x;
            p[1] = j /(float)noiseImage.getHeight() + offset->y;
            
            for (int k = 0; k < octaves; k++) {
                double val = ofNoise(p[0], p[1]);
                //                images[k].setColor(i, j, val);
                sum += val / scale;
                scale *= alpha;
                p[0] *= beta;
                p[1] *= beta;
            }
            noiseImage.setColor(i, j, sum);
            if (sum > maxval)
                maxval = sum;
            if (sum < minval)
                minval = sum;
        }
    }
    
//    if (normalize) {
//        for (int i = 0; i < noiseImage.getWidth(); i++) {
//            for (int j = 0; j < noiseImage.getHeight(); j++) {
//                float v = noiseImage.getColor(i, j).r;
//                noiseImage.setColor(i, j, v/maxval);
//                //                img.setColor(i, j, ofMap(v, 0.0, maxval, 0.0, 1.0));
//                //                img.setColor(i, j, ofMap(v, minval, maxval, 0.0, 1.0));
//            }
//        }
//    }
    
    noiseImage.update();
    
    scaleImage.clone(noiseImage);
    scaleImage.resize(size->x * scale, size->y * scale);
    
    dirty = false;
}


// SIMPLEX STUFF:

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

