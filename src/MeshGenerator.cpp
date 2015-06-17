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
    parameters.add(space.set("Space",         1,   1,   10   ));
    parameters.add(extrusion.set("Extrusion", 2.0, 0.5, 300.0 ));
    parameters.add(wireframe.set("Wireframe", false));
    parameters.add(normals.set("Normals", false));
    
    space.addListener(this, &MeshGenerator::floatValChanged);
    extrusion.addListener(this, &MeshGenerator::floatValChanged);
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
        mesh.draw();
    }
    
    if (normals){
        Utils::drawNormals(mesh, 1);
    }
    
    ofPopMatrix();
}

void MeshGenerator::generate(ofFloatImage &image){
    if (!dirty) return;
    
    int width = image.getWidth();
    int height = image.getHeight();
    
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

    // AND SHAKE IT ALL ABOUT!
    for (int i = 0; i<width*height; i++) {
        ofFloatColor sampleColor(image.getPixels()[i]);
        //        ofFloatColor->alpha = 0.0f;
        ofVec3f tmpVec = mesh.getVertex(i);
        tmpVec.z = (-extrusion/2) + sampleColor.getBrightness()*extrusion;
        mesh.setVertex(i, tmpVec);
        mesh.setColor(i, sampleColor);
    }
    ofxMeshUtils::calcNormals(mesh, true);
    
    offset = ofVec3f(-width/2 * space, -height/2 * space, -extrusion/2);
}
