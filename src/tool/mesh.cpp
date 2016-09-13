/*
 * mesh.cpp
 *
 *  Created on: 2016年9月13日
 *      Author: zhuqian
 */
#include "mesh.h"
#include "shape/trianglemesh.h"
#include "thrid/tinyobjloader/tiny_obj_loader.h"

MeshGroup LoadObjMesh(string dir, string inputfile) {
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	std::string err;
	char* path = new char[100];
	strcat(path, dir.c_str());
	strcat(path, inputfile.c_str());
	cout << "载入OBJ模型:" << path << endl;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path,
			dir.c_str(), true);

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}
	if (!ret) {
		exit(1);
	}

	int numVertex = attrib.vertices.size() / 3;

	//初始化整个顶点数组
	Point* vertexs = new Point[numVertex];
	for (unsigned int i = 0; i < numVertex; ++i) {
		float x = attrib.vertices[3 * i + 0];
		float y = attrib.vertices[3 * i + 1];
		float z = attrib.vertices[3 * i + 2];
		vertexs[i] = Point(x, y, z);
	}

	MeshGroup group;
	group.numVertex = numVertex;
	group.vertexs = vertexs;

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0; //一个mesh下的索偏移

		MeshData mesh;
		mesh.numVertex = numVertex;
		mesh.vertexs = vertexs; //初始化整个顶点
		size_t triangle_num = shapes[s].mesh.num_face_vertices.size();
		mesh.numTriangle = triangle_num; //三角面片个数
		mesh.vertex_indexs = new int[mesh.numTriangle * 3];
		//遍历mesh下的所有面
		for (size_t face = 0; face < triangle_num; face++) {
			int face_vertex_num = shapes[s].mesh.num_face_vertices[face]; //获得当前面的顶点个数
			//遍历每个顶点
			for (size_t v = 0; v < face_vertex_num; ++v) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				mesh.vertex_indexs[index_offset + v] = idx.vertex_index;//赋值vertex索引
			}
			index_offset += face_vertex_num;
		}

		group.data.push_back(mesh);
	}
	return group;
}

