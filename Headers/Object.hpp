//
//  Object.hpp
//  ModelLoaderLib
//
//  Created by Gaurav Sharma on 20/11/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "DataType.h"
using namespace std;

class Object//this reperesents the group of elements and operations applied on those elements and identified by "g"
{
    const char *name;//this is object name identified by o tag in .obj file.
    //these are the lists of elements which has neither smoothing nor merging
    vector<Geometry*> element_list;//may be triangle(face),quadrilateral,curve2,curve,suface
    vector<MergingGroup*> merging_group_list;//
    vector<SmoothingGroup*> smoothing_group_list;
    ConnectedSurface *conn_surf;//this is the surfaces connected to each other
    //render attributes will come here
    /*matrial name for an element which may be a triangle,quadrilateral,curve and surface.identified by symbol"usemtl"*/
    const char *mat_name;
    char *texture_map_name;//taxture map for an element
public:
    Object();
   ~Object();
    void setObjectName(const char* name);
    void setMatrialName(const char* material_name);
    void addElement(Geometry *geom);
    MergingGroup* addMergingGroup();
    SmoothingGroup* addSmoothingGroup();
    void setConnectivity(ConnectedSurface*);
    void readMaterial(char* mat_name);
    void readTextureMapName(char* map_name);
    const char* getObjectName();
    MergingGroup* getMergingGroup(int at_index);
    SmoothingGroup* getSmoothingGroup(int at_index);
    Geometry* getElement(int at_index);
    ConnectedSurface* getConnectedSurface();
    const char* getMatrialName();
    const char* getTextureMapName();
    size_t getNumberOfElements();
    size_t getNumberOfSmoothingGroups();
    size_t getNumberOfMergingGroups();
    
};

#endif /* Object_hpp */
