//
//  Utils.cpp
//  Mountains
//
//  Created by Skåla, Knut Emil (OSL-SDG) on 17.06.15.
//
//

#include "Utils.h"

void Utils::drawNormals(ofMesh &mesh, float length){
    // draw our normals, and show that they are perpendicular to the vector from the center to the vertex
    vector<ofVec3f> n = mesh.getNormals();
    vector<ofVec3f> v = mesh.getVertices();
    
    if(!ofGetKeyPressed()){
        ofDisableLighting();
        ofSetColor(255,0,0,255);
        for(unsigned int i=0; i < n.size(); i++){
            ofLine(v[i].x,
                   v[i].y,
                   v[i].z,
                   v[i].x + n[i].x * length,
                   v[i].y + n[i].y * length,
                   v[i].z + n[i].z * length
                   );
        }
    }
}