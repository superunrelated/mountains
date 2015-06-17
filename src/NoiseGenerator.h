
#include "ofMain.h"

class NoiseGenerator;

template<typename ParameterType>
class ofNoiseGeneratorParam: public ofReadOnlyParameter<ParameterType, NoiseGenerator>{
    friend class NoiseGenerator;
};

class NoiseGenerator {
private:
    bool dirty = true;
    
public:
    NoiseGenerator();
    
    void setup(string name);
    void draw();
    void generate();
    
    ofFloatImage noiseImage;
    ofFloatImage scaleImage;
    
    // GUI
    ofParameterGroup parameters;
    ofParameter<ofVec2f> size, offset;
    ofParameter<float> scale, alpha, beta;
    ofParameter<bool> showNoise;
    //ofParameter<bool> normalize;
    void vec2ValChanged(ofVec2f &val);
    void floatValChanged(float &val);
    bool getDirty();
};


