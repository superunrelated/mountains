//
//  NoiseGenerator.h
//  Mountains
//
//  Created by Skåla, Knut Emil (OSL-SDG) on 17.06.15.
//
//

#ifndef __Mountains__NoiseGenerator__
#define __Mountains__NoiseGenerator__

#include "ofMain.h"

class NoiseGenerator;

template<typename ParameterType>
class ofNoiseGeneratorParam: public ofReadOnlyParameter<ParameterType, NoiseGenerator>{
    friend class NoiseGenerator;
};

class NoiseGenerator {
    bool dirty = true;
    
public:
    NoiseGenerator();
    
    void setup(string name);
    void draw();
    void generate();
    float sumOctave(int num_iterations, float x, float y, float persistence, float scale, int low, int high);
    
    void vec2ValChanged(ofVec2f &val);
    void floatValChanged(float &val);
    bool getDirty();
    
    ofParameterGroup parameters;
    ofParameter<ofVec2f> size, offset;
    ofParameter<float> scale, alpha, beta;
    //ofParameter<bool> normalize;
    
    ofFloatImage noiseImage;
    ofFloatImage scaleImage;
};



#endif /* defined(__Mountains__NoiseGenerator__) */

