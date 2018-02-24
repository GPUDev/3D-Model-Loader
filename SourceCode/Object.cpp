//
//  Object.cpp
//  ModelLoaderLib
//
//  Created by Gaurav Sharma on 20/11/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//

#include "../Headers/Object.hpp"
#define EXPORT __attribute__((visibility("default")))
Object::Object()
{
    name = NULL;
    conn_surf = NULL;
    mat_name = NULL;
    texture_map_name = NULL;
}
EXPORT
Object::~Object()
{
    delete name;
    delete conn_surf;
    delete mat_name;
    delete texture_map_name;
}
void Object::setObjectName(const char *name)
{
    this->name = name;
}
void Object::addElement(Geometry *geom)
{
    element_list.push_back(geom);
}
SmoothingGroup* Object::addSmoothingGroup()
{
    SmoothingGroup* smooth_g = new SmoothingGroup();
    smoothing_group_list.push_back(smooth_g);
    return smooth_g;
}
MergingGroup* Object::addMergingGroup()
{
    MergingGroup* merg_g = new MergingGroup();
    merging_group_list.push_back(merg_g);
    return merg_g;
}
EXPORT
SmoothingGroup* Object::getSmoothingGroup(int i)
{
    if(!smoothing_group_list.empty())
    {
        return smoothing_group_list.at(i);
    }
    
    return NULL;
}
EXPORT
Geometry* Object::getElement(int i)
{
    if(!element_list.empty())
        return element_list.at(i);
    return NULL;
}
EXPORT
size_t Object::getNumberOfElements()
{
    return element_list.size();
}
EXPORT
size_t Object::getNumberOfMergingGroups()
{
    return merging_group_list.size();
}
EXPORT
size_t Object::getNumberOfSmoothingGroups()
{
    return smoothing_group_list.size();
}
void Object::setMatrialName(const char* material_name)
{
    
    mat_name = material_name;
}
EXPORT
const char* Object::getObjectName()
{
    return name;
}
