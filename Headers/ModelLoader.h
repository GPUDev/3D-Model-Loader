//  ModelLoader.h
//  OpenGL3DEngine
//
//  Created by Gaurav Sharma on 23/10/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//

#ifndef ModelLoader_h
#define ModelLoader_h
#include"DataType.h"
#include "Model.hpp"
class ModelLoader
{
    public:
        void load(std::string &file_path,std::vector<Vertex*> &vertices);
        void  loadMaterial(std::string &image_file_path,unsigned char** data);
        static Model* load(std::string &file_path);
    private:
    /*this will return either triangle,quadrilateral,pentagon etc.*/
    static PolygonGeometry* getPolygonGeometry(vector<Normal*> *vn_list,vector<TexCord*> *vt_list,vector<Position*> *v_list,ifstream &file_obj);
    /*this will return surface,curve2 and curve object*/
    static Geometry* getFreeFormGeometry();
    static char skipSpaces(ifstream& file_object);
    /*this function will read n number of values from a line and return those values in float_val_list.this function
     is useful when we don't want to read line till \n.if line has not float values greater than n then it returns as many
     values as line has.*/
    static void readFloatValuesLine(ifstream& file_object,vector<float> *float_val_list,int n);
    static void readFloatValuesLine(ifstream& file_object,vector<float> *float_val_list);
    static string getVisibleString(ifstream& file_object);
    /*this will read the vertices given in format "v/vt/vn" or v/vt or v*/
    static void getVertexList(vector<Normal*> *vn_list,vector<TexCord*> *vt_list,vector<Position*> *v_list,ifstream &file_obj,vector<Vertex*> *vertex_list);
    /*this method is used to get all space seperated strings in a line of given file*/
    static vector<const char*> getStringsInLine(ifstream& file_object);
    
    
};

#endif /* ModelLoader_h */
