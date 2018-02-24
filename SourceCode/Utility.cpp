//
//  Utility.cpp
//  OpenGL3DEngine
//
//  Created by Gaurav Sharma on 15/11/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//

#include "../Headers/Utility.hpp"
#define LINE_IDENTIFIER_COUNT 36
string Utility::lineIdentifierTags[] = {"v","vt","vn","vp","cstype","bmat","step","f","p","l","curv","curv2","surf",
                                        "parm","trim","hole","sp","scrv","con","g","s","mg","o","bevel","c_interp",
                                        "d_interp","lod","usemtl","mtllib","shadow_obj","trace_obj","ctech","stech","call","deg","end"
                                       }; 


Utility::Utility()
{
    for(int i=0;i<LINE_IDENTIFIER_COUNT;i++)
    {
        const pair<string, int> tagStringToInt(lineIdentifierTags[i],i+1);
        lineIdentifierTagMap.insert(tagStringToInt);
    }
    curveSurfTypeMap["bmatrix"] = GL_BASIS_MATRIX;
    curveSurfTypeMap["bezier"] = GL_BEZIER;
    curveSurfTypeMap["bspline"] = GL_BSPLINE;
    curveSurfTypeMap["cardinal"]= GL_CARDINAL;
    curveSurfTypeMap["taylor"] = GL_TAYLOR;
    

}
int Utility::getTagValue(string &key)
{
    return lineIdentifierTagMap[key];
}
CurveSurfTypeEnum Utility::getCStype(const char* type)
{
    return curveSurfTypeMap[type];
}
