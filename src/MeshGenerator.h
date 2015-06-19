
#include "ofMain.h"
#include "ofxMeshUtils.h"
#include "Utils.h"

class MeshGenerator;

template<typename ParameterType>
class ofMeshGeneratorParam: public ofReadOnlyParameter<ParameterType, MeshGenerator>{
    friend class MeshGenerator;
};

class MeshGenerator {
private:
    ofVec3f offset = ofVec3f(0, 0, 0);
    bool dirty = true;
    
public:
    MeshGenerator();
    
    void setup(string name);
    void draw();
    void generate(ofFloatImage &noiseMap, ofFloatImage &distortionMap);
    ofVboMesh mesh;
    ofMaterial material;
    
    // GUI
    ofParameterGroup parameters;
    ofParameter<float> space, extrusion, distortion;
    ofParameter<bool> wireframe;
    ofParameter<bool> normals;
    void floatValChanged(float &val);
    void boolValChanged(bool &val);
    bool getDirty();
};



