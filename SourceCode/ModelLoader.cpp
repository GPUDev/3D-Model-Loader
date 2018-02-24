//
//  ModelLoader.cpp
//  Created by Gaurav Sharma on 23/10/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//
//to do list for model loader
/*
 1.we need not to take vertex object when it is referred by index in face tag because so many triangles may refer to same vertex object in .obj file so we only need to return a list of vertices
 and a list of indices.this approach will save time and memory of getting vertex object through index and even sending data to rendering engine will be efficient.
 2.implement smart pointer to prevent memory leak
 */
/*this is the path relative to current directory*/
#include "../Headers/ModelLoader.h"
#include <fstream>
#include <iostream>
#include "../Headers/Utility.hpp"

using namespace std;
#define EXPORT __attribute__((visibility("default")))
/*this function will skip all the spaces and will return the character where function terminates*/
char ModelLoader::skipSpaces(ifstream& file_object)
{
    char charc = NULL;
    if(file_object.eof())
      return EOF;
   
    file_object.get(charc);
    if(charc == ' ')
       charc = skipSpaces(file_object);
    return  charc;
}
void ModelLoader::readFloatValuesLine(ifstream& file_object,vector<float> *float_val_list,int n)
{
    char c = skipSpaces(file_object);
    if(c=='\n')
    {
      return;
        
    }
    else if(c=='\r')
    {
        file_object.get(c);
        return;
    }
    else if(c=='\\')//it means we need to go next line and read values of next line also.
    {
        file_object.get(c);//going to next line
        readFloatValuesLine(file_object, float_val_list);
        
    }
    /*may be we need to read double size value because there may be chance that object file may have values greater than float
     size here file pointer is at second byte of float value so read bytes equal to float value*/
    else
    {
        //here c will have starting byte of each floating value because of skipSpaces function
        //file_object.seekg(-1,ios::cur);/*we can avoid this line of code */
        string float_bytes;
        //char temp_char=c;
        //file_object.get(temp_char);
        /*here we are reading float value*/
        while(c!=' ' && c!='\r' && c!='\n')
        {   float_bytes.push_back(c);
            
            file_object.get(c);
        }
        float val_in_float = atof(float_bytes.c_str());
        //cout<<val_in_float<<" ";
        float_val_list->push_back(val_in_float);
        /*this is because when control reaches to next line after reading last float value then this function must be
         terminated otherwise it will get into infinite loop.*/
        if(float_val_list->size()<n)
            readFloatValuesLine(file_object, float_val_list,n);
        
        
    }
}
/*Always terminating at new line and this function is only for reading float values line where values are seperated from
 space*/
void ModelLoader::readFloatValuesLine(ifstream& file_object,vector<float> *float_val_list)
{
    char c = skipSpaces(file_object);
    if(c=='\n')
    {
        
        return;
        
    }
    else if(c=='\r')
    {
        file_object.get(c);
        return;
    }
    else if(c=='\\')//it means we need to go next line and read values of next line also.
    {
        file_object.get(c);//going to next line
        readFloatValuesLine(file_object, float_val_list);
        
    }
    /*may be we need to read double size value because there may be chance that object file may have values greater than float
     size here file pointer is at second byte of float value so read bytes equal to float value*/
     else
    {
        //here c will have starting byte of each floating value because of skipSpaces function
        //file_object.seekg(-1,ios::cur);/*we can avoid this line of code */
        string float_bytes;
        //char temp_char=c;
        //file_object.get(temp_char);
        /*here we are reading float value*/
        while(c!=' ' && c!='\r' && c!='\n')
        {   float_bytes.push_back(c);
            
            file_object.get(c);
        }
        float val_in_float = atof(float_bytes.c_str());
        //cout<<val_in_float<<" ";
        float_val_list->push_back(val_in_float);
        /*this is because when control reaches to next line after reading last float value then this function must be
         terminated otherwise it will get into infinite loop.*/
        if(c!='\n')
            readFloatValuesLine(file_object, float_val_list);
    }
}
/*1.here visible string means that string which can be printed on console and can be seen.
 this string will have numeric and alphanumeric and special character excluding space,newline,carriage return,tab
 (characters which can't be seen when they are printed on console.)
 2.This function is used here to get line starting strings which indicate that what type data,a particular line has.
 for example:it will give us "v","vn","vt",etc to identify that what data are we going to read.
 */
string ModelLoader::getVisibleString(ifstream& file_object)
{
    char curr_char;
    string str="";
    file_object.get(curr_char);
    
    /*if these type of more characters would be found then we have add those charaters also in this while condition
     or we can store hash table of such type of characters then we will check curr_char against to this hash table*/
    while(!file_object.eof() && curr_char!='\n' && curr_char!='\r' && curr_char!=' ')
    {
        str.push_back(curr_char);
        file_object.get(curr_char);
    }
    /*because this newline makes skipped next line not the current line in which newline is found.this is the case when
     we found visible continous series of character terminating at newline for eg:#############\n,#\n,ndhdhh\n etc*/
    //if(str!="" && curr_char == '\n')
        //file_object.seekg(-1, ios::cur);
    if(str=="" && curr_char == '\n')//it will keep skipping new line untill it finds a valid visible string
        str = getVisibleString(file_object);
    /*this is the case when we encounter a string just followed by new line and that string is to be
     skipped because it has no meaning in our dictionary then we take back file pointer to current line by seekg function
     otherwise \n line character takes file pointer to next line that is not read yet and that line gets skipped by default
     case which is given in below load method*/
    else if(curr_char=='\n')
        file_object.seekg(-1,ios::cur);
    
    return str;
}
EXPORT
Model* ModelLoader::load(std::string &path)
{
    
    
    ifstream file_obj;
    file_obj.open(path,ios::binary);
    if(!file_obj.is_open())
    {
        cout<<"file can't be opended"<<endl;
        return NULL;
    }
    Utility util;
    Model *model = new Model();
    model->setModelName("Human_Model");
    
    /*these are the temporary lists for reading data from .obj file as we find coorresponding elements as per list then keep
     copying that list data in corresponding elements and once all elements are copied then make list empty and again start
     filling lists for next corresponding elements*/
    vector<Position*> v_list;
    //vp point has 3 coordinate as textCord has that's why we will use position object's 3 coordinate only for curve2
    vector<Position*> vp_list;
    vector<Curve2*> curv2_list;
    vector<TexCord*> vt_list;
    vector<Normal*> vn_list;
    Object *current_object = NULL;
    SmoothingGroup *current_smooth_group = NULL;
    MergingGroup* current_merging_group = NULL;
    Curve2* current_curve2 = NULL;
    Curve* current_curve = NULL;
    Surface* current_surf = NULL;
    int current_deg_u=0;
    int current_deg_v=0;
    int current_step_u=0;
    int current_step_v = 0;
    float *current_bmat_u = NULL;
    float *current_bmat_v = NULL;
    
    bool is_rational = false;
    CurveSurfTypeEnum current_curv_surf_type = GL_INVALID_CURVE_SURF;
    
    
    
    
    vector<float> float_val_list;
    string str ="";//this is multi-purpose variable for keeping data
    char cur_char = ' ';
    
    //cout<<"tag line value:"<<tag_value<<endl;
    while(!file_obj.eof())
    {
        str = getVisibleString(file_obj);
        //cout<<endl<<"str:"<<str<<endl;
        //cout<<endl;
        int tag_value = util.getTagValue(str);
        switch (tag_value) {
            case 1://"v"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    Position *pos = NULL;
                    if(float_val_list.size()==4)
                    {
                        pos = new Position(float_val_list[0],float_val_list[1],float_val_list[2],float_val_list[3]);
                    }
                    else//here may be we can get 3-coordinates only for position
                    {
                        pos = new Position(float_val_list[0],float_val_list[1],float_val_list[2],1.0);
                    }
                    float_val_list.clear();
                    v_list.push_back(pos);
                    break;
                    
                }
                
            case 2://"vt"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    TexCord *tex_cord = NULL;
                    if(float_val_list.size() == 3)
                    {
                        tex_cord = new TexCord(float_val_list[0],float_val_list[1],float_val_list[2],0);
                    }
                    else//here may be we can get only 2-coordinates for texture coordinate
                    {
                       tex_cord = new TexCord(float_val_list[0],float_val_list[1],0,0);
                    }
                    float_val_list.clear();
                    vt_list.push_back(tex_cord);
                    break;
                
                }
                
            case 3://"vn"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    Normal *norm = new Normal(float_val_list[0],float_val_list[1],float_val_list[2],0);
                    float_val_list.clear();
                    vn_list.push_back(norm);
                    break;
                
                }
                
            case 4://"vp"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    Position *control_point = NULL;
                    if(float_val_list.size()==2)
                    {
                        control_point  = new Position(float_val_list[0],float_val_list[1],0.0,1.0);
                    }
                    else//here 3
                    {
                        control_point = new Position(float_val_list[0],float_val_list[1],float_val_list[2],1.0);
                    }
                    vp_list.push_back(control_point);
                
                }
                break;
                
            case 5://"cstype"
                {
                    string str;
                    char c = skipSpaces(file_obj);
                    while(c!='\n')
                    {
                        if(c==' ')
                        {
                          if(str=="rat")
                              is_rational = true;
                          str.clear();
                        }
                        else
                        {
                            str.push_back(c);
                        }
                        file_obj.get(c);
                    }
                    current_curv_surf_type = util.getCStype(str.c_str());
                    
                }
                
                break;
            case 6://"bmat"
                {
                    skipSpaces(file_obj);
                    file_obj.seekg(-1,ios::cur);
                    getline(file_obj, str, ' ');
                    readFloatValuesLine(file_obj, &float_val_list);
                    int mat_size = (current_deg_u+1)*(current_deg_u+1);
                    if(mat_size == float_val_list.size())
                    {
                        if(str == "u")
                        {
                            current_bmat_u = new float[mat_size];
                            float *temp = current_bmat_u;
                            for(int i=0;i<mat_size;i++)
                            {
                                *current_bmat_u=float_val_list.at(i);
                                *current_bmat_u = *current_bmat_u+1;
                            }
                            current_bmat_u = temp;
                        }
                        else if(str == "v")
                        {
                            current_bmat_v = new float[mat_size];
                            float *temp = current_bmat_v;
                            for(int i=0;i<mat_size;i++)
                            {
                                *current_bmat_v=float_val_list.at(i);
                                *current_bmat_v = *current_bmat_v+1;
                            }
                            current_bmat_v = temp;
                        }
                        else
                        {
                            cout<<"not matched with either u or v";
                        }
                        
                    }
                    else
                    {
                        cout<<"matrix's number of values and deg value are not consistent";
                    }
                }
                
                break;
            case 7://"step"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    if(float_val_list.size()==2)
                    {
                        current_step_u = float_val_list[0];
                        current_step_v = float_val_list[1];
                    }
                    else
                    {
                       current_step_u = float_val_list[0];
                    }
                    float_val_list.clear();
                }
                
                break;
            case 8://"f"
                {
                    PolygonGeometry *face_geom = getPolygonGeometry(&vn_list, &vt_list, &v_list, file_obj);
                    face_geom->setGeometryType(GL_POLYGON);
                    if(face_geom->getVertexListSize() == 3)
                    {
                        face_geom->setFaceType(GL_TRIANGLE);
                    }
                    else if(face_geom->getVertexListSize() == 4)
                    {
                        face_geom->setFaceType(GL_QUADRILATERAL);
                    }
                    if(current_smooth_group!=NULL)
                    {
                        current_smooth_group->element_list.push_back(face_geom);
                    }
                    else//if smoothing group is not specified for given face element
                    {
                        current_object->addElement(face_geom);
                    }
                    
                    break;
                }
            case 9://"p"
                
                break;
            case 10://"l"
                
                break;
                
            case 11://"curv"
                    {
                        readFloatValuesLine(file_obj, &float_val_list);
                        current_curve = new Curve();
                        current_curve->setStartParamU0(float_val_list[0]);
                        current_curve->setLastParamU1(float_val_list[1]);
                        for(int i =0;i<float_val_list.size();i++)
                        {
                            int v_index = float_val_list.at(i)-1;
                            current_curve->addControlPoint(v_list.at(v_index));
                        }
                        float_val_list.clear();
                        //set all the properties of curve,we found till this point of time.
                        current_curve->setType(current_curv_surf_type);
                        current_curve->setDegU(current_deg_u);
                        current_curve->setStepU(current_step_u);
                        current_curve->setBasisMatrixU(current_bmat_u);
                        current_curve->setRationalFlag(is_rational);
                        
                        //reset temporary variables
                        current_curv_surf_type = GL_INVALID_CURVE_SURF;
                        current_deg_u = 0;
                        current_step_u = 0;
                        current_bmat_u = NULL;
                        is_rational =  false;
                        
                    }
                
                break;
            case 12://"curv2"
                    {
                        readFloatValuesLine(file_obj, &float_val_list);
                        current_curve2 = new Curve2();
                        for(int i =0;i<float_val_list.size();i++)
                        {
                            int vp_index = float_val_list.at(i)-1;
                            current_curve2->addControlPoint(vp_list.at(vp_index));
                        }
                        float_val_list.clear();
                        
                        //set all the properties of curve2,we found till this point of time.
                        current_curve2->setType(current_curv_surf_type);
                        current_curve2->setDegU(current_deg_u);
                        current_curve2->setStepU(current_step_u);
                        current_curve2->setBasisMatrixU(current_bmat_u);
                        current_curve2->setRationalFlag(is_rational);
                        
                        //reset temporary variables so that we can use them for next curve/curve2/surface object otherise
                        //wrong value can be set.
                        current_curv_surf_type = GL_INVALID_CURVE_SURF;
                        current_deg_u = 0;
                        current_step_u = 0;
                        current_bmat_u = NULL;
                        is_rational =  false;
                    }
                
                break;
            case 13://"surf"
                {
                    readFloatValuesLine(file_obj, &float_val_list,4);
                    current_surf = new Surface();
                    current_surf->setStartParamU0(float_val_list[0]);
                    current_surf->setLastParamU1(float_val_list[1]);
                    current_surf->setStartParamV0(float_val_list[2]);
                    current_surf->setLastParamV1(float_val_list[3]);
                    float_val_list.clear();
                    vector<Vertex*> *list = new vector<Vertex*>();
                    getVertexList(&vn_list, &vt_list, &v_list, file_obj, list);
                    current_surf->setControlPointList(list);
                    
                    //set all the properties of curve2,we found till this point of time.
                    current_surf->setType(current_curv_surf_type);
                    current_surf->setDegU(current_deg_u);
                    current_surf->setDegV(current_deg_v);
                    current_surf->setStepU(current_step_u);
                    current_surf->setStepV(current_step_v);
                    current_surf->setBasisMatrixU(current_bmat_u);
                    current_surf->setBasisMatrixV(current_bmat_v);
                    current_surf->setRationalFlag(is_rational);
                    
                    //reset temporary variables so that we can use them for next curve/curve2/surface object otherise
                    //wrong value can be set.
                    current_curv_surf_type = GL_INVALID_CURVE_SURF;
                    current_deg_u = 0;
                    current_deg_v = 0;
                    current_step_u = 0;
                    current_step_v = 0;
                    current_bmat_u = NULL;
                    current_bmat_v = NULL;
                    is_rational =  false;
                }
                
                break;
            case 14://"param"
                {
                    vector<float> *knot_vec = new vector<float>();
                    skipSpaces(file_obj);
                    file_obj.seekg(-1,ios::cur);
                    getline(file_obj, str, ' ');
                    readFloatValuesLine(file_obj, knot_vec);
                   
                    if(str=="u")
                    {
                        if(current_curve!=NULL)
                        {
                            current_curve->setUKnotVector(knot_vec);
                        }
                        else if(current_curve2!=NULL)
                        {
                            current_curve2->setUKnotVector(knot_vec);
                        }
                        else if(current_surf!=NULL)
                        {
                            current_surf->setUKnotVector(knot_vec);
                        }
                    }
                    else if(str=="v")
                    {
                        /*if we have found v parameter then it is sure that we already created surface object and v parameter is only for surface.*/
                        current_surf->setVKnotVector(knot_vec);
                    }
                    else
                    {
                        cout<<"wrong parameter specification";
                    }
                    
                    
                }
                
                break;
                
            case 15://"trim"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    Curve2Sequence *curve_to_seq = current_surf->addCurve2Sequence();
                    curve_to_seq->type = GL_OUTER_TRIM_LOOP;
                    for(int i=0;i<float_val_list.size();i++)
                    {
                        if((i+1)%3==0)
                        {
                            int curve2_index = float_val_list.at(i)-1;
                            curve_to_seq->curve2_list.push_back(curv2_list.at(curve2_index));
                        }
                        else if(i%3==0)
                        {
                            curve_to_seq->u0_list.push_back(float_val_list.at(i));
                        }
                        else//when (i-1)%3 ==0
                        {
                             curve_to_seq->u1_list.push_back(float_val_list.at(i));
                        }
                    }
                }
                
                break;
            case 16://"hole"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    Curve2Sequence *curve_to_seq = current_surf->addCurve2Sequence();
                    curve_to_seq->type = GL_INNER_TRIM_LOOP;
                    for(int i=0;i<float_val_list.size();i++)
                    {
                        if((i+1)%3==0)
                        {
                            int curve2_index = float_val_list.at(i)-1;
                            curve_to_seq->curve2_list.push_back(curv2_list.at(curve2_index));
                        }
                        else if(i%3==0)
                        {
                            curve_to_seq->u0_list.push_back(float_val_list.at(i));
                        }
                        else//when (i-1)%3 ==0
                        {
                            curve_to_seq->u1_list.push_back(float_val_list.at(i));
                        }
                    }
                }
                
                break;
            case 17://"sp"
                
                break;
            case 18://"scrv"
                {
                    readFloatValuesLine(file_obj, &float_val_list);
                    Curve2Sequence *curve_to_seq = current_surf->addCurve2Sequence();
                    curve_to_seq->type = GL_SPECIAL_CURVE_ON_SURF;
                    for(int i=0;i<float_val_list.size();i++)
                    {
                        if((i+1)%3==0)
                        {
                            int curve2_index = float_val_list.at(i);
                            curve_to_seq->curve2_list.push_back(curv2_list.at(curve2_index));
                        }
                        else if(i%3==0)
                        {
                            curve_to_seq->u0_list.push_back(float_val_list.at(i));
                        }
                        else//when (i-1)%3 ==0
                        {
                            curve_to_seq->u1_list.push_back(float_val_list.at(i));
                        }
                    }
                }
                
                break;
            case 19://"con"
                
                break;
            case 20://"g"
                {
                    
                    
                    /* here we will get a new empty"object" and we will keep hold the pointer of this object untill we get
                     next object in file.
                     this object is filled by the elements following this object in file.
                     */
                    current_object = model->addObject();
                    //we can avoid this if we take return character of below function in str and then read remaining
                    //character using getline function.
                    skipSpaces(file_obj);
                    file_obj.seekg(-1, ios::cur);
                    str.clear();
                    getline(file_obj, str, '\n');
                    size_t l = str.size();
                    char *n = new char[l];
                    for(int i=0;i<l;i++)
                    {
                        n[i]=str[i];
                    }
                    n[l]='\0';
                    /*int l = str.size();
                    if(str[l-1] == '\r')
                    {
                        str.pop_back();
                    }*/
                    //cout<<"group name:"<<str;
                    current_object->setObjectName(n);
                
                }
                break;
            case 21://"s"
                {
                    
                    skipSpaces(file_obj);
                    file_obj.seekg(-1, ios::cur);
                    str.clear();
                    getline(file_obj, str, '\n');
                    if(str!="off"||str!="OFF"||str!="0")
                    {
                        
                        /*we are not checking here that current_object is null or not because smooth_group can't exist
                         without group object(group object is "Object" object)*/
                        current_smooth_group = current_object->addSmoothingGroup();
                        /*these below three lines are same in most of the cases.we could have created a small function for them
                         and then that function would have been called at the place wherever we require but this implementation
                         would have made program control switching frequently so this can increase execution time of loader.
                     */
                     current_smooth_group->group_no = atoi(str.c_str());
                        //cout<<"smooth group:"<<str;
                    }

                }
                break;
            case 22://"mg"
                {
                    /*we are not checking here that current_object is null or not because merging_group can't exist
                     without group object(group object is "Object" object)*/
                    current_merging_group = current_object->addMergingGroup();
                    skipSpaces(file_obj);
                    file_obj.seekg(-1, ios::cur);
                    str.clear();
                    getline(file_obj, str, '\n');
                    current_merging_group->group_no = atoi(str.c_str());
                }
                
                break;
            case 23://"o"
                
                break;
            case 24://"bevel"
                
                break;
                
            case 25://"c_interp"
                
                break;
            case 26://"d_interp"
                
                break;
            
            case 27://"lod"
                {
                
                    break;
                }
            case 28://"usemtl"
                {
                    skipSpaces(file_obj);
                    file_obj.seekg(-1,ios::cur);
                    getline(file_obj, str, '\n');
                    current_object->setMatrialName(str.c_str());
                }
                break;
            case 29://"mtlib"
                {
                    model->setMatrialLibList(getStringsInLine(file_obj));
                }
                break;
            case 30://"shadow_obj"
                
                break;
            case 31://"trace_obj"
                
                break;
            case 32://"c_tech"
                
                break;
            case 33://"stech"
                
                break;
            case 34://"call"
                
                break;
                
            case 35://"deg"
            {
                readFloatValuesLine(file_obj, &float_val_list);
                /*we got two degree values that means these are specified for surface which we will encounter further */
                if(float_val_list.size()==2)
                {
                    current_deg_u = float_val_list[0];
                    current_deg_v = float_val_list[1];
                }
                else//only one deg value it means either curve or curve2 will be given as free form geometry.
                {
                    current_deg_u = float_val_list[0];
                }
                float_val_list.clear();
            }
            
                break;
                /*this end is for surface,curve2,curve when we will encounter this then we will check here
                 which type of object is created surface,curve or curve2 by checking which one does not have null
                 value and then that object will be added to current object or current smoothig group and after
                 adding it.it will be set to null so that we can check other types(curve2,curve,surface) also*/
            case 36://"end"
            {
                
                if(current_curve!=NULL)
                {
                    current_object->addElement(current_curve);
                    current_curve = NULL;
                }
                else if(current_curve2!=NULL)
                {
                    //temporarily added to list because we need them at the time of trim,holes,scrv object specification
                    //trim,holes,scrv reference curv2s by the index number.
                    curv2_list.push_back(current_curve2);
                    current_curve2 = NULL;
                }
                else if(current_surf!=NULL)
                {
                    current_object->addElement(current_surf);
                    current_surf = NULL;
                    
                }
                else
                {
                    cout<<"error:no free form geometry specified yet";
                }
            }
                
                break;
            default:
                //cout<<"line skipped";
                   file_obj.ignore(numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
       
        if(cur_char =='\r')
        {
            cout<<"carriage return found";
            file_obj.get(cur_char);
            
        }
        
    }
    file_obj.close();
    return model;
}
void ModelLoader::getVertexList(vector<Normal*> *vn_list,vector<TexCord*> *vt_list,vector<Position*> *v_list,
                                ifstream &file_obj,vector<Vertex*> *vertex_list)
{
    char curr_char;
    file_obj.get(curr_char);
    string value_in_bytes;
    int slash_count = 0;
    Vertex *vert = NULL;
    while(curr_char!='\n' && curr_char!='\r')
    {
        if(curr_char=='/' || curr_char==' ')
        {
            /*it is for the cases which may be "v//vn" and "v/vt/vn    v/vt/vn".in first case when we encounter
             double slashes continously that value_in_bytes will be empty and in second case also,we will get empty
             value_in_bytes string after encountering space after space*/
            if(!value_in_bytes.empty())
            {
                int index = atoi(value_in_bytes.c_str())-1;
                //cout<<index+1;
                value_in_bytes.clear();
                if(slash_count==0)
                {
                    vert = new Vertex();
                    vert->postion = v_list->at(index);
                }
                else if(slash_count == 1)
                {
                    vert->tex_cord = vt_list->at(index);
                }
                else if(slash_count==2)
                {
                    vert->normal = vn_list->at(index);
                }
            }
            if(curr_char=='/')
            {
                //cout<<"/";
                slash_count++;
                
            }
            
            else//it is space
            {
                //cout<<" ";
                slash_count = 0;
                vertex_list->push_back(vert);
            }
            
        
        }
        //this character is indication that values for a particular line identifier tag are not limited to this line
        //only.need to read next line also.
        else if(curr_char=='\\')//it means remaining values are in second line so go to next line
        {
            //here it will give us '\n' character because just after '\' character,file has '\n' character.
            //in this way,control will come to next line and value reading will be continued untill we are sure that
            //values for a particular line identifier tag are not left.
            file_obj.get(curr_char);
        }
        else
        {
            value_in_bytes.push_back(curr_char);
        }
        file_obj.get(curr_char);
    }
    /*this code is here because when we encounter new line or carriage return then we will not enter while loop.at that time
     we would have read bytes in value_in_bytes string.these bytes may be index of position,texure coordinate or normal.
     so here we need to take that value also otherwise face line would not be read correctly*/
    if(!value_in_bytes.empty())
    {
        int index = atoi(value_in_bytes.c_str())-1;
        //cout<<index+1;
        value_in_bytes.clear();
        if(slash_count==0)
        {
            vert = new Vertex();
            vert->postion = v_list->at(index);
        }
        else if(slash_count == 1)
        {
            vert->tex_cord = vt_list->at(index);
        }
        else if(slash_count==2)
        {
            vert->normal = vn_list->at(index);
        }
    }
}

PolygonGeometry* ModelLoader::getPolygonGeometry(vector<Normal*> *vn_list, vector<TexCord*> *vt_list, vector<Position*> *v_list,ifstream &file_obj)
{
    //here we will check vertex list,normal list and texture list to know the format of faces and then we
    //can print error message indicating that faces are not in correct format and parsing not possible
    
    PolygonGeometry* new_geom = new PolygonGeometry();
    char curr_char;
    file_obj.get(curr_char);
    string value_in_bytes;
    int slash_count = 0;
    Vertex *vert = NULL;
    while(curr_char!='\n' && curr_char!='\r')
    {
        if(curr_char=='/' || curr_char==' ')
        {
            /*it is for the cases which may be "v//vn" and "v/vt/vn    v/vt/vn".in first case when we encounter
             double slashes continously that value_in_bytes will be empty and in second case also,we will get empty
             value_in_bytes string after encountering space after space*/
            if(!value_in_bytes.empty())
            {
                int index = atoi(value_in_bytes.c_str())-1;
                //cout<<index+1;
                value_in_bytes.clear();
                if(slash_count==0)
                {
                    vert = new Vertex();
                    vert->postion = v_list->at(index);
                }
                else if(slash_count == 1)
                {
                    vert->tex_cord = vt_list->at(index);
                }
                else if(slash_count==2)
                {
                    vert->normal = vn_list->at(index);
                }
            }
            if(curr_char=='/')
            {
                //cout<<"/";
                slash_count++;
                
            }
            else//it is space
            {
                //cout<<" ";
                slash_count = 0;
                new_geom->addVertex(vert);
            }
                
        }
        //this character is indication that values for a particular line identifier tag are not limited to this line
        //only.need to read next line also.
        else if(curr_char=='\\')//it means remaining values are in second line so go to next line
        {
            //here it will give us '\n' character because just after '\' character,file has '\n' character.
            //in this way,control will come to next line and value reading will be continued untill we are sure that
            //values for a particular line identifier tag are not left.
            file_obj.get(curr_char);
        }
        else
        {
            value_in_bytes.push_back(curr_char);
        }
        file_obj.get(curr_char);
    }
    /*this code is here because when we encounter new line or carriage return then we will not enter while loop.at that time
     we would have read bytes in value_in_bytes string.these bytes may be index of position,texure coordinate or normal.
     so here we need to take that value also otherwise face line would not be read correctly*/
    if(!value_in_bytes.empty())
    {
        int index = atoi(value_in_bytes.c_str())-1;
        //cout<<index+1;
        value_in_bytes.clear();
        if(slash_count==0)
        {
            vert = new Vertex();
            vert->postion = v_list->at(index);
        }
        else if(slash_count == 1)
        {
            vert->tex_cord = vt_list->at(index);
        }
        else if(slash_count==2)
        {
            vert->normal = vn_list->at(index);
        }
        new_geom->addVertex(vert);
    }
    return  new_geom;

}
vector<const char*>  ModelLoader::getStringsInLine(ifstream &file_obj)
{
    vector<const char*> string_list;
    string str;
    char c;
    file_obj.get(c);
    while(c!='\n')
    {
        if(c!=' '&&c!='\r')
        {
            str.push_back(c);
        }
        else
        {
            string_list.push_back(str.c_str());
            str.clear();
        }
       file_obj.get(c);
    }
    string_list.push_back(str.c_str());
    return string_list;
}















void ModelLoader::load(std::string &path, std::vector<Vertex*> &vertices)
{
    vector<Position*> positions;
    vector<TexCord*> tex_cords;
    vector<Normal*> normals;
    vector<float> cord_vals;
    Vertex *v = NULL;
    int slash_count=0;
    ifstream fin;
    fin.open(path,ios::binary);
   
    if(!fin.is_open())
    {
        //cout<<"can't be opened file\n";
        exit(0);
    }
    
    string line="";
    char prev_char='\n',curr_char=' ';
    fin.get(curr_char);
    char c1 = curr_char,c2=' ';
    while(!fin.eof())
    {
        //cout<<"entered in loop\n";
        //cout<<"curr_char:"<<curr_char<<endl;
        if(curr_char != '\n')
        {
            if(prev_char == '\n')
            {
            
                c1 = curr_char;
                fin.get(curr_char);
                c2 = curr_char;
                /*it means c2 has either n or t then we have to shift file pointer one more character so that
                 file pointer could start reading floting values of normal and texture otherwise we will end up with
                 reading char n or t as floting value in below conditions of normal reading and texture reading and then
                 this leads us to wrong understanding of x coordinate which will be 0 all the time because atof() returns
                 zero for string which are not numeric string*/
            if(c2 =='n' || c2 == 't')
                fin.get(curr_char);
               
           
            }
            
        }
        if(c1=='v' && c2 == ' ')
        {
            
            if(curr_char != ' ' && curr_char != '\n'  && curr_char!='\r' )
            {

                line.push_back(curr_char);
            }
            else if(!line.empty())//because in the case when curr_char having either space or newline then line will be empty
            {
                float val = atof(line.c_str());
                if(curr_char==' ')
                {
                    cord_vals.push_back(val);
                    //cout<<val<<" ";
                }
                else//here we got all 3 position values so make position vector here
                {
                    Position *p  = new Position(cord_vals[0],cord_vals[1],val,1.0);
                    positions.push_back(p);
                    //cout<<val<<endl;
                    cord_vals.clear();
                }
                line.clear();
            }
            
        }
        else if(c1=='v' && c2 == 'n' )//reading normals
        {
            if(curr_char != ' ' && curr_char != '\n' && curr_char!='\r')
            {
                
                line.push_back(curr_char);
            }
            else if(!line.empty())//because in the case when curr_char having either space or newline then line will be empty
            {
                
                float val = atof(line.c_str());
                //cout<<val<<" ";
                if(curr_char==' ')
                    cord_vals.push_back(val);
                else//here we got all 3 position values so make position vector here
                {
                    Normal *n = new Normal(cord_vals[0],cord_vals[1],val,0);
                    normals.push_back(n);
                    cord_vals.clear();
                    //cout<<val<<endl;
                }
                line.clear();
            }
        }
        else if(c1=='v' && c2 == 't')//reading texture coordinates
        {
            if(curr_char != ' ' && curr_char != '\n' && curr_char!='\r')
            {
                
                line.push_back(curr_char);
            }
            else if(!line.empty())//because in the case when curr_char having either space or newline then line will be empty
            {
                float val = atof(line.c_str());
                //cout<<val<<" ";
                if(curr_char==' ')
                    cord_vals.push_back(val);
                else//here we got all 2 texuture coordinate values so make TextCord vector here
                {
                    TexCord *tc = new TexCord(cord_vals[0],cord_vals[1],val,0);
                    tex_cords.push_back(tc);
                    //cout<<val<<endl;
                    cord_vals.clear();
                }
                line.clear();
            }
        }
        else if(c1=='f' && c2 == ' ')//reading faces
        {
            
            if(curr_char != '\n' && curr_char != '/' && curr_char != ' ' && curr_char != '\r')
            {
               
                line.push_back(curr_char);//here we are getting numeric character only
                if(prev_char == ' ')
                {
                    slash_count = 2;
                    v = new Vertex();
                    //cout<<"new vertex is created\n";
                }
               
                
            }
            else if(curr_char == ' ')
            {
                
                
                if(!line.empty())//here we have read a number string after second / character
                {
                    int norm_index = atoi(line.c_str())-1;
                    v->normal = normals[norm_index];
                    //cout<<norm_index+1<<" ";
                    line.clear();
                    vertices.push_back(v);
                }
            }
            else if(curr_char == '/')
            {
                int index = atoi(line.c_str())-1;
                if(slash_count == 2)//now line has position index
                {
                    
                    v->postion = positions[index];
                    //cout<<index+1<<"/";
                    slash_count--;
                   
                    
                }
                else if(slash_count == 1)//now line has tex_cord index
                {
                    v->tex_cord = tex_cords[index];
                    //cout<<index+1<<"/";
                   
                }
                //cout<<line<<endl;
                line.clear();
            }
        }
        /*in the case of single line character,we  would not be able to ignore the line by using this ignore function because
         newline character takes file pointer to next line so till this point,we are not sure that next line should be ignored
         or not.*/
        else if(curr_char !='\n')
        {
            
            fin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            curr_char = '\n';
            
        }
        
        prev_char = curr_char;
        fin.get(curr_char);
        
        }
    fin.close();
    cout<<"size of position vector:"<<positions.size()<<endl;
    cout<<"size of tex_cords vector:"<<tex_cords.size()<<endl;
    cout<<"size of normals vector:"<<normals.size()<<endl;
    cout<<"size of cord_values:"<<cord_vals.size()<<endl;
    cout<<"size of line:"<<line.size();
    
}
