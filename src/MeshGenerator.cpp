//
//  MeshGenerator.cpp
//  Mountains
//
//  Created by Skåla, Knut Emil (OSL-SDG) on 17.06.15.
//
//

#include "MeshGenerator.h"

MeshGenerator::MeshGenerator() {
    // TODO Auto-generated constructor stub
}

void MeshGenerator::setup(string name){
    parameters.setName(name);
    parameters.add(space.set("Space",         1,   1,   50   ));
    parameters.add(extrusion.set("Extrusion", 2.0, 0.5, 1000.0 ));
    parameters.add(distortion.set("Distortion",         1.0,   1.0,   1000.0   ));

    parameters.add(wireframe.set("Wireframe", false));
    parameters.add(normals.set("Normals", false));
    
    space.addListener(this, &MeshGenerator::floatValChanged);
    extrusion.addListener(this, &MeshGenerator::floatValChanged);
    distortion.addListener(this, &MeshGenerator::floatValChanged);
    
    // MATERIAL
    material.setAmbientColor(ofColor(0, 0, 0));                     // Shadow
    material.setDiffuseColor(ofColor(0, 0, 0));                     // The Color
    material.setEmissiveColor(ofColor(0, 0, 0));                    // Self lit
    material.setSpecularColor(ofColor(0, 0, 0));                    // highlights
    material.setShininess(150);
}

void MeshGenerator::floatValChanged(float &val) {
    dirty = true;
}

void MeshGenerator::boolValChanged(bool &val) {
    dirty = true;
}

bool MeshGenerator::getDirty(){
    return dirty;
}

void MeshGenerator::draw(){
    ofPushMatrix();
    ofTranslate(offset);
    if (wireframe){
        mesh.drawWireframe();
    } else {
        material.begin();
        mesh.draw();
        material.end();
    }
    
    if (normals){
        ofSetColor(255, 0, 0);
        Utils::drawNormals(mesh, 1);
    }
    
    ofPopMatrix();
}

void MeshGenerator::generate(ofFloatImage &noiseMap, ofFloatImage &distortionMap){
    int width = noiseMap.getWidth();
    int height = noiseMap.getHeight();
    
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.enableIndices();
    
    for (int y = 0; y<height; y++){
        for (int x = 0; x<width; x++){
            int index = (y * width) + x;
            ofFloatColor extrude(noiseMap.getPixels()[index]);
            
            float xPos = x * space;
            float yPos = y * space;
            float zPos = (-extrusion/2) + extrude.getBrightness() * extrusion;
            
            
            ofFloatColor distort(distortionMap.getPixels()[index]);
            float offsetX = cos(distort.getBrightness()*PI*2) * distortion;
            float offsetY = sin(distort.getBrightness()*PI*2) * distortion;
            float offsetZ = -cos(distort.getBrightness()*PI*2) * distortion;
            
            mesh.addVertex(ofVec3f(xPos + offsetX,
                                   yPos + offsetY,
                                   zPos + offsetZ
                                   )
                           );
//            mesh.addColor(sampleColor);
            mesh.addColor(ofColor(255, 255, 255));
            
            // mesh.addVertex(ofVec3f(xPos, yPos, zPos));
            // mesh.addColor(ofFloatColor(0,0,0));
            // mesh.addNormal(ofVec3f(0, 0, 1));
            // mesh.addTexCoord(ofVec2f(x * space, y * space));
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
    
    ofxMeshUtils::calcNormals(mesh, true);
    
    offset = ofVec3f(-width/2 * space, -height/2 * space, -extrusion/2);
    dirty = false;
}
