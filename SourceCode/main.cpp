#include "../Headers/ModelLoader.h"
#include <iostream>
using namespace std;
int main()
{
    
    vector<Vertex*> vertices;
    //string str = "model2/scene1.OBJ";
    string str = "model/pose2.OBJ";
     //string str = "Sample.obj";
    //string str = "objectfile.txt";
    //string str = "surf_curve_test_file.txt";
    auto start = chrono::steady_clock::now();
    Model* model =  ModelLoader::load(str);
    auto end = chrono::steady_clock::now();
    double result = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout<<"time to load file:"<<result<<endl;
    Object *obj = model->getObject(0);
    cout<<"object name:"<<obj->getObjectName();
    for (int i=0; i<obj->getNumberOfElements(); i++)
    {
        Geometry* geo = obj->getElement(i);
         cout<<endl;
        if(geo->getGeomType() == GL_POLYGON)
        {
            PolygonGeometry* pl_geom = (PolygonGeometry*)geo;
            if(pl_geom->getType()==GL_TRIANGLE)
            {
                for(int j=0;j<3;j++)
                {
                    Vertex *vert = pl_geom->getVertexAt(j);
                    cout<<"Position:"<<vert->postion->x<<" "<<vert->postion->y<<" "<<vert->postion->z<<" "<<vert->postion->w<<endl;
                    cout<<"Texture Cord:"<<vert->tex_cord->u<<" "<<vert->tex_cord->v<<" "<<vert->tex_cord->w<<endl;
                    cout<<"Normal:"<<vert->normal->x<<" "<<vert->normal->y<<" "<<vert->normal->z<<endl;
                }
            }
        }
    }
    
    SmoothingGroup *s = obj->getSmoothingGroup(0);
    if(s!=NULL)
    {
    for (int i=0; i<s->element_list.size(); i++)
    {
        Geometry* geo = s->element_list.at(i);
        cout<<endl;
        if(geo->getGeomType() == GL_POLYGON)
        {
            PolygonGeometry* pl_geom = (PolygonGeometry*)geo;
            if(pl_geom->getType()==GL_TRIANGLE)
            {
                for(int j=0;j<3;j++)
                {
                    Vertex *vert = pl_geom->getVertexAt(j);
                    cout<<"Position:"<<vert->postion->x<<" "<<vert->postion->y<<" "<<vert->postion->z<<" "<<vert->postion->w<<endl;
                    cout<<"Texture Cord:"<<vert->tex_cord->u<<" "<<vert->tex_cord->v<<" "<<vert->tex_cord->w<<endl;
                    cout<<"Normal:"<<vert->normal->x<<" "<<vert->normal->y<<" "<<vert->normal->z<<endl;
                }
            }
        }
    }
}
    
    cout<<"file is loaded"<<endl;
    /*loader.load(str, vertices);
    for(int i=0;i<vertices.size();i++)
    {
        Vertex *v = vertices[i];
        std::cout<<"position:"<<v->postion->x<<" "<<v->postion->y<<" "<<v->postion->z<<" "<<v->postion->w<<endl;
        std::cout<<"TexCord:"<<v->tex_cord->u<<" "<<v->tex_cord->v<<" "<<v->tex_cord->w<<endl;
        std::cout<<"normal:"<<v->normal->x<<" "<<v->normal->y<<" "<<v->normal->z<<endl;
    }*/
    return 0;
}



