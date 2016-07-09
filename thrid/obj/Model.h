#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
class _POINT{
public:
    float x, y, z;

    _POINT(){};
    _POINT(float _x, float _y, float _z):x(_x),y(_y),z(_z){};


	_POINT operator+(const _POINT a)const{
		return _POINT(x + a.x, y + a.y, z + a.z);
	}
	_POINT operator-(const _POINT a)const{
		return _POINT(x - a.x, y - a.y, z - a.z);
	}
	_POINT operator*(const float a)const{ // Scalar multiplication
		return _POINT(x * a, y * a, z * a);
	}
	int operator*(const _POINT a)const{ // dot product
		return x * a.x + y * a.y + z * a.z;
	}
	_POINT operator^(const _POINT a)const{ // cross product
		return _POINT(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
	}
	_POINT normalize(){
		float D = sqrt(x * x + y * y +z * z);
		x /= D;
		y /= D;
		z /= D;
		return _POINT(x, y, z);
	}
};

// A triangle includes 3 point indices.
class _TRIANGLE{
public:
    int index[3];
    _TRIANGLE(){};
    _TRIANGLE(int _a, int _b, int _c){
        index[0] = _a;
        index[1] = _b;
        index[2] = _c;
    };

};

class Model{

    std::vector<_POINT> vertices;
    std::vector<_TRIANGLE> triangles;

public:
    Model();
    Model(char * filename);
    void readByLine(FILE * fp, char * s);
    int load(char *filename);
    void addVertex(_POINT point);
    void addTriangle(_TRIANGLE triangle);
	int numberOfVertices();
    int numberOfTriangles();
	_POINT getVertex(int _index);
	_TRIANGLE getTriangle(int _index);
	_POINT getNormal(int _index);
	_POINT getRandomPointInTriangle(int _index);
    void show();
};

#endif // MODEL_H_INCLUDED
