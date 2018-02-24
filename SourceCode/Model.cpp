//
//  Model.cpp
//  ModelLoaderLib
//
//  Created by Gaurav Sharma on 20/11/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//

#include "../Headers/Model.hpp"
#define EXPORT __attribute__((visibility("default")))

EXPORT
size_t Model::getChildModelListSize()
{
    return child_model_list.size();
}
EXPORT
size_t Model::getObjectListSize()
{
    return object_list.size();
}
EXPORT
Model* Model::getChild(int index)
{
    if(!child_model_list.empty())
    {
        return child_model_list.at(index);
        
    }
    return NULL;
}

Model* Model::addModel()
{
    Model* new_model = new Model();
    child_model_list.push_back(new_model);
    return new_model;
}
Object* Model::addObject()
{
    Object *new_object = new Object();
    object_list.push_back(new_object);
    return new_object;
}
EXPORT
Object* Model::getObject(int i)
{
    if(!object_list.empty())
    {
        return object_list.at(i);
    }
    return NULL;
}

void Model::setMapLibraryList(vector<char*> map_lib_list)
{
    map_library_list = map_lib_list;
}
void Model::setMatrialLibList(vector<const char*> mat_lib_list)
{
    material_lib_list = mat_lib_list;
}
void Model::setModelName(const char* name)
{
    model_name = name;
}
Model::Model()
{
    trace_object_filename = NULL;
    shadow_object_filename = NULL;
    
}
EXPORT
const char* Model::getModelName()
{
    return model_name;
}
EXPORT
Model::~Model()
{
    delete trace_object_filename;
    delete shadow_object_filename;
}
