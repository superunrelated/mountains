
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
    void draw(int x, int y);
    void generate(ofVec2f size, float scale);
    
    ofFloatImage noiseImage;
    ofFloatImage scaleImage;
    
    // GUI
    ofParameterGroup parameters;
    ofParameter<ofVec2f> offset;
    ofParameter<float> alpha, beta;
    ofParameter<bool> showNoise;
    //ofParameter<bool> normalize;
    
    void vec2ValChanged(ofVec2f &val);
    void floatValChanged(float &val);
    bool getDirty();
};


