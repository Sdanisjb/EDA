#ifndef MHALFEDGE_H_INCLUDED
#define MHALFEDGE_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

template <class G>
struct Vertex{
    typedef typename G::edge E;
    int id;
    float x,y,z;
    E* edge;

    bool operator==(Vertex v){return id == v.id;}
    void print(){cout<<x<<" "<<y<<" "<<z<<endl;}
};

template <class G>
struct Edge{
    typedef typename G::face F;
    typedef typename G::vertex V;
    int id;
    Edge* twin;
    Edge* next;
    V* head;
    V* tail;
    F* face;

    Edge(V* _t, V* _h): tail(_t), head(_h){};
    bool operator==(Edge e){return id==e.id;}
};

template <class G>
struct Face{
    typedef typename G::edge E;

    int id;
    E* edge;

    Face(E* _e):edge(_e){};
    bool operator==(Face f){id == f.id;}
};

template <class G>
class HalfEdgeM{
    typedef typename G::vertex v;
    typedef typename G::edge e;
    typedef typename G::face f;
    vector<v> verteces;
    vector<int> vIndex;
    vector<f> faces;
public:
    bool loadOBJ(char* path){
        FILE* file = fopen(path, "r");
        if(file == NULL){
            cout<<"No es posible abrir el archivo. Revisar path."<<endl;
            return false;
        }
        while(1){
            char line_header[128];

            int res = fscanf(file,"%s",line_header);
            if (res == EOF) break;
            if( strcmp(line_header, "v") == 0){
                Vertex<G> v;
                fscanf(file, "%f %f %f\n", &v.x,&v.y,&v.z);
                verteces.push_back(v);
            }/*else if( strcmp(line_header,"vt") == 0 || strcmp(line_header,"vn") == 0){
                continue;
            }*/else if( strcmp(line_header, "f")== 0){
                string v1, v2, v3;
                unsigned int verIndex[3], uvIndex[3], nIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &verIndex[0], &uvIndex[0], &nIndex[0], &verIndex[1], &uvIndex[1], &nIndex[1], &verIndex[2], &uvIndex[2], &nIndex[2]);
                if (matches != 9){
                    cout<< "El archivo no puede ser leído. Revisar sección caras f"<<endl;
                    return false;
                }
                vIndex.push_back(verIndex[0]);
                vIndex.push_back(verIndex[1]);
                vIndex.push_back(verIndex[2]);
            }
        }

        //Start building HE Mesh
        for(unsigned i=0;i<vIndex.size();i+=3){ //Start iterating over the vIndex
            //We point to the three triangle verteces
            v* v1 = &verteces[vIndex[i]-1];
            v* v2 = &verteces[vIndex[i+1]-1];
            v* v3 = &verteces[vIndex[i+2]-1];
            //Now we attach them with edges
            v1->edge = new e(v1,v2);
            v1->edge->next = new e(v2,v3);
            v1->edge->next->next = new e(v3, v1);
            //Now we attach remaining verteces to their edges (in case is the only face they belong)
            v2->edge = v1->edge->next;
            v3->edge = v1->edge->next->next;
            //Now we attach the face to 1 of the edges
            faces.push_back(f(v1->edge));
            cout<<"Contruyó cara"<<endl;
        }
    }
    void print(){
        /*cout<<"vertices"<<endl;
        for(unsigned i=0;i<verteces.size();++i) verteces[i].print();
        cout<<"caras"<<endl;
        for(unsigned i=0;i<vIndex.size();i+=3) cout<<vIndex[i]<<" "<<vIndex[i+1]<<" "<<vIndex[i+2]<<endl;]*/
    }
};



class ToolBox{
public:
    typedef  Edge<ToolBox> edge;
    typedef  Face<ToolBox> face;
    typedef  Vertex<ToolBox> vertex;
};






#endif // MHALFEDGE_H_INCLUDED
