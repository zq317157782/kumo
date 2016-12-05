#include "Model.h"
#include <cstdlib>
Model::Model(){};

Model::Model(char * filename){
    load(filename);
}

void Model::addVertex(_POINT point){
    vertices.push_back(point);
}

void Model::addTriangle(_TRIANGLE triangle){
     triangles.push_back(triangle);
}

void Model::readByLine(FILE * fp, char * s){
    char c;
    int len = 0;
    _POINT p;
    while((c = fgetc(fp)) != EOF){
        if(c != '\n')s[len++] = c;
        else{

            s[len] = '\0';

            if(s[0] == 'v' && s[1] == ' '){ // vertices information

                sscanf(s+2, "%f %f %f", &p.x, &p.y, &p.z);
               // printf("%f/%f/%f\n",p.x,p.y,p.z);
                addVertex(p);

            }else if(s[0] == 'f' && s[1] == ' '){ // triangles information

                int a, b, c,t;
                sscanf(s+2, "%d/%d/%d %d/%d/%d %d/%d/%d", &a, &t, &t, &b, &t, &t, &c, &t, &t);
               // printf("%d/%d/%d\n",a,b,c);
                addTriangle(_TRIANGLE(a-1, b-1, c-1));

            }
            len = 0;
        }
    }
}

int Model::load(char * filename) {
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) return 0 * printf("Open file error.\n");

    char s[200];
    readByLine(fp, s);

    printf("Model loaded successfully.\n");
    printf("Number of vertices: %d.\n", vertices.size());
    printf("Number of triangles: %d.\n", triangles.size());
	return 0;
}

int Model::numberOfVertices(){
    return vertices.size();
}

int Model::numberOfTriangles(){
    return triangles.size();
}

_POINT Model::getVertex(int _index){
    return vertices[_index];
}
_TRIANGLE Model::getTriangle(int _index){
    return triangles[_index];
}

_POINT Model::getNormal(int _index){
    _POINT p[3], U, V, N;
    for(int i = 0; i < 3; i ++) p[i] = getVertex(triangles[_index].index[i]);
    U = p[1] - p[0];
    V = p[2] - p[0];
    N = U ^ V;
    return N.normalize();
}

_POINT Model::getRandomPointInTriangle(int _index){
    _POINT p[3], U, V;
    for(int i = 0; i < 3; i ++) p[i] = getVertex(triangles[_index].index[i]);
    U = p[1] - p[0];
    V = p[2] - p[0];
    float x =  1.0 * rand()/ RAND_MAX;
    float y =  1.0 * rand() / RAND_MAX;

    if(x + y > 1.0){
        x = 1.0 - x;
        y = 1.0 - y;
    }
    return p[0] + U * x + V * y;
}

void Model::show(){
    for(std::vector<_POINT>::iterator it = vertices.begin(); it != vertices.end(); it ++)
        printf("%f %f %f\n", it->x, it->y, it->z);
}
