/*
 * mesh.h
 *
 *  Created on: 2016年9月13日
 *      Author: zhuqian
 */

#ifndef SRC_TOOL_MESH_H_
#define SRC_TOOL_MESH_H_
//mesh相关的工具函数
#include "global.h"


struct MeshData{
	int numTriangle;
	int numVertex;
	Point * vertexs;
	int * vertex_indexs;
};


struct MeshGroup{
	int numVertex;
	Point * vertexs;
	vector<MeshData> data;
};

MeshGroup LoadObjMesh(string dir,string inputfile);


#endif /* SRC_TOOL_MESH_H_ */
