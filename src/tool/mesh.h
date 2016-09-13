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
#include "thrid/tinyobjloader/tiny_obj_loader.h"

//todo 销毁数据
struct RawMeshData{
	int numTriangle;
	int numVertex;
	Point * vertexs;
	int * vertex_indexs;
	Normal* normals;
	int * normal_indexs;
	float* UVs;
	int *tex_indexs;
	unsigned int materialID;
};


struct RawMeshGroup{
	int numVertex;
	Point * vertexs;
	Normal* normals;
	float*  UVs;
	vector<RawMeshData> data;
	vector<tinyobj::material_t> materials;
};

RawMeshGroup LoadObjMesh(string dir,string inputfile);


#endif /* SRC_TOOL_MESH_H_ */
