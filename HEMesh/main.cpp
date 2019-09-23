#include <iostream>

#include "MHalfEdge.h"

using namespace std;

int main()
{
    char* path = "myObjs/human.obj";
    HalfEdgeM<ToolBox> miMalla;
    miMalla.loadOBJ(path);
    miMalla.print();

    return 0;
}
