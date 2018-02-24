//
//  Model.hpp
//  ModelLoaderLib
//
//  Created by Gaurav Sharma on 20/11/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp
#include <vector>
#include "Object.hpp"
using namespace std;
class Model//this is corresponding to an object file and object of this class will be created as we found "call" tag
{
        /*this is the list of material library list which is used by corresponding mat_name.correspondence will be decided such
         that first material_name will use first material library,second will use second material library in list and so on*/
        vector<const char*> material_lib_list;
        /*this list is also used as material libarary list.only differrence is that here texture_map will use map library*/
        vector<char*> map_library_list;
        /*this is for handlling the case when a model has sub models which are specified such that an object file refers to other
     object file*/
        vector<Model*> child_model_list;
        vector<Object*> object_list; //this is the list of object
        char* trace_object_filename;//this filename is for definin
        char* shadow_object_filename;
        const char* model_name;
    public:
        Model();
       ~Model();
        void setMatrialLibList(vector<const char*> lib_name_list);
        void setMapLibraryList(vector<char*> map_lib_list);
        void readTraceObject(char* object_file_name);
        void readShadowObject(char* object_file_name);
        Model* addModel();//this method will create a model object ,add in model list and will return the model object
        Object* addObject();//this method will create an "Object" object ,add in object list and will return the "Object" object
        Object* getObject(int at_index);
        Model* getChild(int at_index);
        char* getMatrialLib(int at_index);
        char* getMapLib(int at_index);
        size_t getObjectListSize();
        size_t getChildModelListSize();
        const char* getModelName();
        void setModelName(const char* name);
    
    
    
    
    
};
#endif /* Model_hpp */
