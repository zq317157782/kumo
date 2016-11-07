/*
 * mesh.cpp
 *
 *  Created on: 2016年9月13日
 *      Author: zhuqian
 */
#include "mesh.h"
#include "shape/trianglemesh.h"
#include "thrid/tinyobjloader/tiny_obj_loader.h"

RawMeshGroup LoadObjMesh(string dir, string inputfile) {
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	std::string err;
	char* path = new char[100];
	path[0] = 0;
	strcat(path, dir.c_str());
	strcat(path, inputfile.c_str());
	//cout << "载入OBJ模型:" << path << endl;
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
	Normal* normals = nullptr;
	if (attrib.normals.size() > 0) {
		normals = new Normal[numVertex];
		for (unsigned int i = 0; i < attrib.normals.size()/3; ++i) {
			float x = attrib.normals[3 * i + 0];
			float y = attrib.normals[3 * i + 1];
			float z = attrib.normals[3 * i + 2];
			normals[i] = Normal(x, y, z);
		}
	}

	float* UVs = nullptr;
	if (attrib.texcoords.size() > 0) {
		UVs = new float[2 * numVertex];
		for (unsigned int i = 0; i < attrib.texcoords.size()/2; ++i) {
			float u = attrib.texcoords[2 * i + 0];
			float v = attrib.texcoords[2 * i + 1];
			UVs[2 * i + 0] = u;
			UVs[2 * i + 1] = v;
		}

	}

	RawMeshGroup group;
	group.numVertex = numVertex;
	group.vertexs = vertexs;
	group.normals = normals;
	group.UVs = UVs;
	group.materials = materials;
	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0; //一个mesh下的索偏移

		RawMeshData mesh;
		mesh.numVertex = numVertex;
		mesh.vertexs = vertexs; //初始化整个顶点
		mesh.normals = normals;
		size_t triangle_num = shapes[s].mesh.num_face_vertices.size();
		mesh.numTriangle = triangle_num; //三角面片个数
		mesh.vertex_indexs = new int[mesh.numTriangle * 3];
		mesh.normal_indexs = new int[mesh.numTriangle * 3];
		mesh.tex_indexs = new int[mesh.numTriangle * 3];
		//遍历mesh下的所有面
		for (size_t face = 0; face < triangle_num; face++) {
			int face_vertex_num = shapes[s].mesh.num_face_vertices[face]; //获得当前面的顶点个数
			//遍历每个顶点
			for (size_t v = 0; v < face_vertex_num; ++v) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				mesh.vertex_indexs[index_offset + v] = idx.vertex_index;//赋值vertex索引
				mesh.normal_indexs[index_offset + v] = idx.normal_index;
				mesh.tex_indexs[index_offset + v] = idx.texcoord_index;
			}
			index_offset += face_vertex_num;
		}
		mesh.materialID = shapes[s].mesh.material_ids[0];
		group.data.push_back(mesh);
	}
	return group;
}

GeomPrimitive * CreatePanel(Transform* l2w, Transform*w2l, const Point& p1,
		const Point& p2, const Point& p3, const Point& p4, Material* mat) {
	Point* points = new Point[4];
	points[0] = p1;
//	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
	int triCount = 2;
	int vertexCount = 4;
	int * indexs = new int[3 * triCount];
	indexs[0] = 0;
	indexs[1] = 1;
	indexs[2] = 2;
	indexs[3] = 0;
	indexs[4] = 2;
	indexs[5] = 3;

	TriangleMesh* mesh = new TriangleMesh(l2w, w2l, false, triCount,
			vertexCount, indexs, points, nullptr, nullptr, nullptr);

	GeomPrimitive * primit_tri = new GeomPrimitive(mesh,
			Reference<Material>(mat));
	return primit_tri;
}

