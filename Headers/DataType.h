//
//  DataType.h
//  Created by Gaurav Sharma on 23/10/17.
//  Copyright © 2017 Godrej Innovation Center. All rights reserved.
//
//need to write destructor of every class otherwise memory leake will be in code
#ifndef DataType_h
#define DataType_h
#include <vector>
#include <string>
using namespace std;
struct Position
{
    float x;
    float y;
    float z;
    float w;
    Position(float val1,float val2,float val3,float val4)
    {
        x = val1;
        y = val2;
        z = val3;
        w = val4;
    }
};
struct TexCord
{
    
    float u;
    float v;
    float w;
    float t;
    TexCord(float v1,float v2,float v3,float v4)
    {
        u=v1;
        v=v2;
        w=v3;
        t=v4;
    }
};

struct Normal
{
    float x;
    float y;
    float z;
    float w;
    Normal(float v1,float v2,float v3,float v4)
    {
        x=v1;
        y=v2;
        z=v3;
        w = v4;
    }
};
struct Vertex
{
    
    Position *postion;
    Normal *normal;
    TexCord *tex_cord;
    
    Vertex()
    {
        postion = NULL;
        tex_cord  = NULL;
        normal = NULL;
    }
};
enum FreeFormGeomEnum
{
    GL_CURVE2,//GL means gaurav loader
    GL_CURVE,
    GL_SURFACE,
    GL_INVALID_FREE_FORM_GEOM
    
};
enum FaceTypeEnum
{
    GL_TRIANGLE,
    GL_QUADRILATERAL,
    GL_PENTAGON,
    GL_INVALID_FACE
};
enum GeometryTypeEnum
{
   GL_POLYGON,
   GL_FREE_FORM,
   GL_INVALID_GEOM
};
class Geometry//renderable geometry
{
    protected:
        GeometryTypeEnum geom_type;
        int level_detail;
        Geometry()
        {
            level_detail = 0;
            geom_type = GL_INVALID_GEOM;
        }
    public:
    void setGeometryType(GeometryTypeEnum type){geom_type = type;};
    void setLod(int lod){level_detail = lod;};
    int getLevelOfDetail(){return level_detail;};
    GeometryTypeEnum getGeomType(){return geom_type;};
    
};
class PolygonGeometry:public Geometry
{
    
    protected:
        FaceTypeEnum type;
        vector<Vertex*> vertex_list;
        bool bevel;
        bool c_interp;//color interpolation
        bool d_interp;//dissolve interpolation
    public:
        PolygonGeometry()
        {
            type = GL_INVALID_FACE;
            bevel = false;
            c_interp = false;
            d_interp = false;
        }
      ~PolygonGeometry()
        {
            for(int i=0;i<vertex_list.size();i++)
            {
                delete vertex_list.at(i);
            }
            vertex_list.clear();
            
        }
        void setBevel(bool flag){bevel = flag;};
        void setCInterp(bool flag){c_interp = flag;};
        void setDInterp(bool flag){d_interp = flag;};
        void setFaceType(FaceTypeEnum ft){type = ft;};
        void addVertex(Vertex* vt){vertex_list.push_back(vt);};
        size_t getVertexListSize(){return vertex_list.size();};
        bool getBevel(){return bevel;};
        FaceTypeEnum getType(){return type;};
        Vertex* getVertexAt(int index)
        {
            if(!vertex_list.empty())
            {
                return vertex_list.at(index);
                
            }
            return  NULL;
        }
    
};
enum CurveSurfTypeEnum
{
    GL_BEZIER,
    GL_BASIS_MATRIX,
    GL_BSPLINE,
    GL_CARDINAL,
    GL_TAYLOR,
    GL_INVALID_CURVE_SURF//means type is not specified
    
    
};
enum Curve2SequenceTypeEnum
{
    GL_OUTER_TRIM_LOOP,//sequence of curve2 for outer trim loop
    GL_INNER_TRIM_LOOP,//sequence of curve2 for inner trim loop and identified by hole tag
    GL_SPECIAL_CURVE_ON_SURF,//sequence of curve2 which lie on given surface to build a special curve
    GL_NO_SEQ//means no sequence is specified
    
};
class Curve2;//forward declaration

//struct Curve2Sequence;

//this structure is designed to support trim,hole,scrv
struct Curve2Sequence
{
    Curve2SequenceTypeEnum type;
    //for ith curve2,ith u0 and ith u1 are start and last parameter values respectively
    vector<float> u0_list;//curve2 corresponding starting parameter value
    vector<float> u1_list;//curve2 corresponding last parameter value
    vector<Curve2*> curve2_list;
    Curve2Sequence()
    {
        type = GL_NO_SEQ;
    }
    ~Curve2Sequence()
    {
        for(int i=0; i<curve2_list.size();i++)
        {
            Curve2* curve2 = curve2_list[i];
            delete curve2;
            
        }
    }
};
//this curve is defined on given surface geometry
class Curve2:public Geometry
{
    
   protected:
        int deg_u;
        /*this value is set for curve by looking symbol"ctech cparm res" and for surface by looking "stech cparmb uvres" in .obj file*/
        float tech_cparam_res;
        /*this value is set for curve by looking symbol "ctech cspace maxlength" and for surface by looking symbol "stech cspace maxlength"
         */
       float tech_cspace_maxlength;
        /*the below two variables are set for curve by looking symbol "ctech curv maxdist maxangle" and for surface by looking symbol "stech curv maxdist maxangle"*/
        float tech_curv_maxdist;
        float tech_curv_maxangle;
        CurveSurfTypeEnum type;
        FreeFormGeomEnum free_Form_geom;
        vector<float> *u_knot_vec;//identified by param u in .obj file
        int step_u;//identified by step symbol in .obj file and only applicable for basisMatrix type curve
        float *basis_matrix_u;//identified bmat u in .obj file and applicable only for if type specified as basisMatrix
      /*identified by vp and v symbol,only two or three co-ordinates of position object are used for curv2 type of curve and
       4-coordinates are used for curve type of curve*/
       vector<Position*> control_point_list;
    
        vector<float> special_point_list;//identified by sp symbol in .obj file
        bool is_rational;
       //Curve2Sequence *curve2_Sequence;//it may be trim,holes or special curve idenfied as trim,hole,scrv symbols in .obj file
    public:
    Curve2()
    {
        type = GL_INVALID_CURVE_SURF;
        free_Form_geom = GL_INVALID_FREE_FORM_GEOM;
        step_u = 0;
        deg_u = 0;
        basis_matrix_u = NULL;
        is_rational = false;
        //curve2_Sequence = NULL;//it will point a structure
        tech_cparam_res = 0.0;
        tech_curv_maxangle = 0.0;
        tech_curv_maxdist = 0.0;
        tech_curv_maxangle = 0.0;
        
    }
    ~Curve2()
    {
        delete basis_matrix_u;
        basis_matrix_u = NULL;
        //delete curve2_Sequence;
        //curve2_Sequence = NULL;
        for(int i=0;i<control_point_list.size();i++)
        {
            delete control_point_list[i];
        }
    }
    //setter methods
     void setDegU(int deg){deg_u = deg;};
    void addControlPoint(Position *vp)
    {
        control_point_list.push_back(vp);
    }
    void setCParamTechnique(float resolution){tech_cparam_res = resolution;};
    void setCSpaceTechniqueMaxLength(float length){tech_cspace_maxlength = length;};
    void setTechnique(float max_dis,float max_angle){tech_curv_maxdist = max_dis;tech_curv_maxangle = max_angle;};
    void setStepU(int step){step = step_u;};
    void setBasisMatrixU(float *mat){basis_matrix_u = mat;};
    void setType(CurveSurfTypeEnum curve_type){type = curve_type;};
    //void setCurveSequene(Curve2Sequence* crv_seq){curve2_Sequence = crv_seq;};
    void setRationalFlag(bool val){is_rational = val;};
    void addSpecialPoint(float p){special_point_list.push_back(p);};
    /*need  to error checking that number of knot vector values is consistent or not.this number must be
     equal to (degree+N-1) where deg_u or deg_v ,N is number of control points*/
    void setUKnotVector(vector<float> *knot_vec){u_knot_vec = knot_vec;};
    
    //getter methods
    int getDegU(){return deg_u;};
    CurveSurfTypeEnum getType(){return type; };
    int getStepU(){return step_u;};
    float* getBasisMatU(){return basis_matrix_u;};
    //Curve2Sequence* getCurve2Sequence(){return curve2_Sequence;};
    bool isRational(){return is_rational;};
    Position* getControlPointAt(int index){return control_point_list.at(index);};
    float getSpecialPointAt(int index){return special_point_list.at(index);};
    float getKnotVectorUPointAt(int index){return u_knot_vec->at(index);};
    float getCParamTechnique(){return tech_cparam_res;};
    float getCSpaceTechLength(){return tech_cspace_maxlength;};
    
    
};
//this is 2d curve
class Curve:public Curve2
{
    
    //in this class,control_points are identified by v symbol
    protected:
    
    
        float u0;//starting parameter value
        float u1;//last parameter value
    public:
    
        //setter methods
        void setStartParamU0(float u){u0 = u;};
        void setLastParamU1(float u){u1 = u;}
        //getter methods
    
        float getStartParam(){return u0;};
        float getLastParam(){return u1;};
};

//this is the 3d curve
class Surface:public Curve
{
    private:
        /*these two variables of surface are set by looking symbol "stech cparma ures vres" in .obj file*/
        float tech_cparma_ures;
        float tech_cparam_vres;
        float v0;//starting parameter in v direction
        float v1;//last parameter in v direction
        /*this list will keep control points of vertex type because surface may refer v/vt/vn type control points */
        vector<Vertex*> *control_point_list;
    //protected:
        vector<float> *v_knot_vec;//identified by symbol param v in .obj file
        int step_v;//identified by step symbol in .obj file and only applicable for basisMatrix type curve
        int deg_v;
        float *basis_matrix_v;//identified bmat u in .obj file and applicable only for if type specified as basisMatrix
        vector<Curve2Sequence*> curve2_seq_list;
    public:
        void setVKnotVector(vector<float> *knot_vec){v_knot_vec = knot_vec;};
        void setStepV(int v){step_v = v;};
        void setDegV(int deg){deg_v = deg;}
        void setStartParamV0(float v){v0 = v;};
        void setLastParamV1(float v){v1 = v;};
        void setControlPointList( vector<Vertex*> *list){control_point_list=list;};
        void setBasisMatrixV(float* bmat_v){basis_matrix_v = bmat_v;};
        Curve2Sequence* addCurve2Sequence()
        {
            Curve2Sequence *curveToSeq = new Curve2Sequence();
            curve2_seq_list.push_back(curveToSeq);
            return curveToSeq;
        }
        Curve2Sequence* getCurveToSequenceAtIndex(int index){return curve2_seq_list.at(index);};
    
};

struct ConnectedSurface//identified by "con" tag
{
    Surface *surf1;
    Surface *surf2;
    Curve2  *curve2d_1;//this is the curve2 on first surface
    Curve2  *curve2d_2;//this is the curve2 on second surface
    float q0_1;//starting parameter of curve2d_1
    float q1_1;//last parameter of curve2d_1
    float q0_2;//starting parameter of curve2d_2
    float q1_2;//last parameter of curve2d_2
    
};


struct SmoothingGroup//this represents a list of elements on which smoothing
{
    vector<Geometry*> element_list;
    int group_no;
};

struct MergingGroup//identified by "mg" symbol in .obj file
{
    vector<Surface*> surface_list;//these are the surfaces which are to be merged
    float resolution_value;
    int group_no;
};


#endif /* DataType_h */
