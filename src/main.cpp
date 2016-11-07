//#define UNIT_TEST
#ifdef UNIT_TEST
#else
#include <iostream>
#include "shape/sphere.h"
#include "camera/PinholeCamera.h"
#include "transform.h"
#include "global.h"
#include "renderer/simpleRenderer.h"
#include "reflection.h"
#include "primitive.h"
#include "material/matte.h"
#include "integrator/SimpleIntegrator.h"
#include "random.h"
#include "sampler/randomSampler.h"
#include "film/ppm.h"
#include "filter/box.h"
#include "filter/triangle.h"
#include "shape/trianglemesh.h"
#include "Scene.h"
#include "thrid/objloader/Model.h"
#include "light/point.h"
#include "light/spot.h"
#include "light/distant.h"
#include "material/metal.h"
#include "texture/constant.h"
#include "texture/scale.h"
#include "texture/checkerboard.h"
#include "texture.h"
#include "light/diffuse.h"
#include "montecarlo.h"
#include "integrator/directlight.h"
#include "material/mirror.h"
#include "material/translucent.h"
#include "material/grass.h"
#include "integrator/path.h"
#include "sampler/stratified.h"
#include "parallel.h"
#include "film/png.h"
#include "integrator/igi.h"
#include "accelerator/grid.h"
#include "accelerator/normal.h"
#include "texture/image.h"
#include "accelerator/bvh.h"
#include "integrator/ic.h"
#include "camera/orthographic.h"
#include "camera/perspective.h"
#include "sampler/lowdiscrepancy.h"
#include "tool/mesh.h"
#include "renderer/metropolis.h"
//#include "SDL2/SDL.h"
using namespace std;
//#define UNIT_TEST
#ifdef UNIT_TEST
#include "gtest/gtest.h"
#include "test/test.h"
#endif

GeomPrimitive * CreatePanel(Transform* l2w, Transform*w2l, const Point& p1,
		const Point& p2, const Point& p3, const Point& p4, Material* mat) {
	Point* points = new Point[4];
	points[0] = p1;
	points[1] = p2;
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

int main(int argc, char** argv) {
#ifdef UNIT_TEST
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
#endif

//	MeshGroup group=LoadObjMesh("res/","ironman_Scene.obj");
//	cout<<"顶点个数:"<<group.numVertex<<endl;
//	cout<<"mesh个数:"<<group.data.size();
//	return 0;
//	for(float i=0.0000000000001;i<1;i+=0.001){
//		cout<<i<<" "<<Log2(i)<<endl;
//	}
//	exit(1);

	ConstantTexture<RGB> *white = new ConstantTexture<RGB>(RGB(1, 1, 1));
	ConstantTexture<RGB> *red = new ConstantTexture<RGB>(RGB(1, 0, 0));
	ConstantTexture<RGB> *black = new ConstantTexture<RGB>(RGB(0.1, 0.1, 0.1));
	ConstantTexture<RGB> *eta = new ConstantTexture<RGB>(RGB(1.2, 1.2, 1.2));
	ConstantTexture<RGB> *kk = new ConstantTexture<RGB>(RGB(1.2, 1.2, 1.2));
	ConstantTexture<RGB> *half_white = new ConstantTexture<RGB>(
			RGB(0.5, 0.5, 0.5));
	ConstantTexture<float> *roughess = new ConstantTexture<float>(0.9);
	Checkerboard2DTexture<RGB> *checker = new Checkerboard2DTexture<RGB>(
			new UVMapping2D(3, 3), black, white);

	SimpleImageTexture *tex = new SimpleImageTexture(new UVMapping2D(1, 1),
			"res/lala.png", true, 0, TEXTURE_CLAMP, 0, 0);
	Matte * m = new Matte(tex);

	Metal * metal = new Metal(white, eta, kk, new Blinn(25));
	Metal * metal2 = new Metal(checker, eta, kk, new Anisotropic(1000, 1000));
	Translucent *trans = new Translucent(black, white, roughess, black, white);
	Mirror * mirror = new Mirror(white);
	ConstantTexture<float> *ior = new ConstantTexture<float>(1.5);
	Grass * grass = new Grass(white, white, ior);

//	ConstantTexture<float> *ior = new ConstantTexture<float>(1.5);
//	Grass * grass = new Grass(white, white, ior);

	Transform localToWorld = Translate(Vector(-1, 2, 5));
	Transform worldToLocal = Translate(Vector(1, -2, -5));
	//第一个sphere
	Sphere* sphere = new Sphere(&localToWorld, &worldToLocal, false, 0.5, -0.5,
			0.5, 360);
	DiffuseAreaLight *diffuse = new DiffuseAreaLight(localToWorld,
			RGB(10, 10, 10), 16, sphere);
	GeomPrimitive * primit = new GeomPrimitive(Reference<Shape>(sphere),
			Reference<Material>(m), diffuse);
	Transform localToWorld2 = Translate(Vector(0, -1, 6));
	Transform worldToLocal2 = Translate(Vector(-0, 1, -6));
	Sphere* sphere2 = new Sphere(&localToWorld2, &worldToLocal2, false, 0.5,
			-0.5, 0.5, 360);
	GeomPrimitive * primit2 = new GeomPrimitive(Reference<Shape>(sphere2),
			Reference<Material>(m));
	Transform localToWorld2_2 = Translate(Vector(1, 0, 6));
	Transform worldToLocal2_2 = Translate(Vector(-1, 0, -6));
	Sphere* sphere3 = new Sphere(&localToWorld2_2, &worldToLocal2_2, false, 0.5,
			-0.5, 0.5, 360);
	GeomPrimitive * primit3 = new GeomPrimitive(Reference<Shape>(sphere3),
			Reference<Material>(mirror));

	Transform localToWorld2_3 = Translate(Vector(-1, 0, 6));
	Transform worldToLocal2_3 = Translate(Vector(1, 0, -6));
	Sphere* sphere4 = new Sphere(&localToWorld2_3, &worldToLocal2_3, false, 0.5,
			-0.5, 0.5, 360);
	GeomPrimitive * primit4 = new GeomPrimitive(Reference<Shape>(sphere4),
			Reference<Material>(grass));

	Transform l2w_panel1 = Translate(Vector(0, 0, 0));
	Transform w2l_panel1 = Translate(Vector(0, 0, -0));
	ConstantTexture<RGB> *half_red = new ConstantTexture<RGB>(RGB(1, 0, 0));
	Matte * m1 = new Matte(white);
	GeomPrimitive * panel1 = CreatePanel(&l2w_panel1, &w2l_panel1,
			Point(-2, 2, 8), Point(-2, -2, 8), Point(2, -2, 8), Point(2, 2, 8),
			m1);

	Transform l2w_panel2 = Translate(Vector(0, 0, 0));
	Transform w2l_panel2 = Translate(Vector(0, 0, -0));
	ConstantTexture<RGB> *half_blue = new ConstantTexture<RGB>(RGB(0, 0, 1));
	Matte * m2 = new Matte(half_red);
	GeomPrimitive * panel2 = CreatePanel(&l2w_panel2, &w2l_panel2,
			Point(-2, 2, -3 + 8), Point(-2, -2, -3 + 8), Point(-2, -2, 2 + 8),
			Point(-2, 2, 2 + 8), m2);

	Transform l2w_panel3 = Translate(Vector(0, 0, 0));
	Transform w2l_panel3 = Translate(Vector(0, 0, -0));
	ConstantTexture<RGB> *half_green = new ConstantTexture<RGB>(RGB(0, 1, 0));
	Matte * m3 = new Matte(half_green);
	GeomPrimitive * panel3 = CreatePanel(&l2w_panel3, &w2l_panel3,
			Point(2, 2, 2 + 8), Point(2, -2, 2 + 8), Point(2, -2, -3 + 8),
			Point(2, 2, -3 + 8), m3);

	Transform l2w_panel4 = Translate(Vector(0, 0, 0));
	Transform w2l_panel4 = Translate(Vector(0, 0, -0));
	Matte * m4 = new Matte(white);
	GeomPrimitive * panel4 = CreatePanel(&l2w_panel4, &w2l_panel4,
			Point(-100, -2, 100 + 8), Point(-100, -2, -100 + 8),
			Point(100, -2, -100 + 8), Point(100, -2, 100 + 8), m4);

	Transform l2w_panel5 = Translate(Vector(0, 0, 0));
	Transform w2l_panel5 = Translate(Vector(0, 0, -0));
	Matte * m5 = new Matte(white);
	GeomPrimitive * panel5 = CreatePanel(&l2w_panel5, &w2l_panel5,
			Point(-2, 2, 2 + 8), Point(2, 2, 2 + 8), Point(2, 2, -3 + 8),
			Point(-2, 2, -3 + 8), m4);

	Transform l2w_panel6 = Translate(Vector(0, 0, 0));
	Transform w2l_panel6 = Translate(Vector(0, 0, -0));
	ConstantTexture<RGB> *yellow = new ConstantTexture<RGB>(RGB(1, 1, 0));
	Matte * m6 = new Matte(yellow);
	GeomPrimitive * panel6 = CreatePanel(&l2w_panel6, &w2l_panel6,
			Point(10, 10, -2), Point(10, -10, -2), Point(-10, -10, -2),
			Point(-10, 10, -2), m6);

	Transform cameraTransform = Translate(Vector(0,0.5,3));
	//PinholeCamera camera(cameraTransform,
	//		new PNGFilm(600, 600, new TriangleFilter(0.5, 0.5),
	//				"result/Renderer.png")); //int xres,int yres,Filter* f,const char* file
	//camera.setDistanceToView(700);

	float w[4] = { -1.2, 1.2, -1.2, 1.2 };
	/*OrthoCamera camera(cameraTransform, w, 0, 0, new PNGFilm(600, 600, new TriangleFilter(0.5, 0.5),
	 "result/Renderer_ortho.png"));*/

	PerspectiveCamera camera(cameraTransform, w, 0, 0, 60,
			new PNGFilm(600, 600, new TriangleFilter(0.5, 0.5),
					"result/Renderer_persp.png"));
	//场景初始化
	vector<Reference<Primitive>> primtives;
	primtives.push_back(primit);
	//primtives.push_back(primit2);
//	primtives.push_back(primit3);
//	primtives.push_back(primit4);

	primtives.push_back(panel1);
	/* primtives.push_back(panel2);
	 primtives.push_back(panel3);*/
	primtives.push_back(panel4);
	//primtives.push_back(panel5);
	//scene.addPrimitive(panel6);

	RawMeshGroup group = LoadObjMesh("res/", "ironman_Scene.obj");
	Transform localToWorld_tri = Translate(Vector(0, -1.8, 6)) * RotateY(180)
			* RotateX(90) * Scale(0.3, 0.3, 0.3);
	Transform worldToLocal_tri = Translate(Vector(0, 1.8, -6)) * RotateY(180)
			* RotateX(-90) * Scale(0.3, 0.3, 0.3);
	for (int i = 0; i < group.data.size(); ++i) {
		TriangleMesh* mesh = new TriangleMesh(&localToWorld_tri,
				&worldToLocal_tri, false, group.data[i].numTriangle,
				group.numVertex, group.data[i].vertex_indexs, group.vertexs,
				group.normals, nullptr, group.UVs, group.data[i].normal_indexs,
				group.data[i].tex_indexs);
//		string texName="res/";
//		string albedoName=group.materials[group.data[i].materialID].;
//		texName.append(albedoName);
//		cout<<texName<<endl;
//		SimpleImageTexture *tex = new SimpleImageTexture(new UVMapping2D(1, 1),
//				texName, true, 0, TEXTURE_BLACK, 0, 0);
//		float* kd=group.materials[group.data[i].materialID].diffuse;
//		ConstantTexture<RGB> *duffuse = new ConstantTexture<RGB>(RGB(kd[0], kd[1], kd[2]));
		GeomPrimitive * primit_tri = new GeomPrimitive(mesh,
				Reference<Material>((grass)));
		primtives.push_back(primit_tri);
	}

	BVHAccel grid(primtives, 128, BVHAccel::SPLIT_MIDDLE);
	//GridAccel grid(primtives, true);
	//NormalAggregate na(primtives);
	vector<Light*> lights;
	Transform localToWorld3 = Translate(Vector(0, 0, 5.3));
	Transform worldToLocal3 = Translate(Vector(0, 0, -5.3));
	DistantLight* p2 = new DistantLight(localToWorld3, RGB(1, 1, 1),
			Vector(0, 0, -1));
	DistantLight* p = new DistantLight(localToWorld3, RGB(1, 1, 1),
			Vector(1, 0, 0));
	lights.push_back(diffuse);
	//lights.push_back(p);
	//scene.addLight(p2);

	Scene scene(&grid, lights);
	scene.background = RGB(121.0 / 255, 121.0 / 255, 121.0 / 255);

//	SimpleRenderer renderer(&camera, new RandomSampler(0, 800, 0, 600, 64),
//			new PathIntegrator(5));	//new PathIntegrator(5)

//	SimpleRenderer renderer(&camera,
//	new StratifiedSampler(0, 600, 0, 600, 4, 4, true),
//	new PathIntegrator(5));	//new PathIntegrator(5)

	SimpleRenderer renderer(&camera, new LDSampler(0, 600, 0, 600, 32),
			new IrradianceCacheIntegrator(0.5f, 2.5, 15, 10, 5, 3, 1024));//new PathIntegrator(5)

	// MetropolisRenderer renderer(100,100000,4,0.25f,true,512,7,&camera,true);
	//MetropolisRenderer renderer(100,100000,4,0.25f,true,1,7,&camera,true);
//	SimpleRenderer renderer(&camera,
//			new StratifiedSampler(0, 800, 0, 600, 1, 1, true),
//			new IGIIntegrator(4, 3, 0.01f, 2, 0.1f, 1));

	renderer.render(&scene);
	cout << "----" << endl;

}

//	//	//测试三角面片
//		Model model;
//		model.load("WALL_E.obj");
//		int triCount = model.numberOfTriangles();
//		int vertexCount = model.numberOfVertices();
//		Point* points = new Point[vertexCount];
//		for (int i = 0; i < vertexCount; ++i) {
//			_POINT p = model.getVertex(i);
//			points[i] = Point(p.x, p.y, p.z);
//		}
//
//		int * indexs = new int[3 * triCount];
//		for (int i = 0, j = 0; i < triCount; ++i) {
//			_TRIANGLE t = model.getTriangle(i);
//			indexs[j++] = t.index[0];
//			indexs[j++] = t.index[1];
//			indexs[j++] = t.index[2];
//		}
//
//
////		Transform localToWorld_tri = Scale(10,10,10);
////		Transform worldToLocal_tri = Scale(-10,-10,-10);
//		Transform localToWorld_tri = Translate(Vector( 0,0, 6));
//		Transform worldToLocal_tri = Translate(Vector(0,0, -6));
//
//		TriangleMesh* mesh = new TriangleMesh(&localToWorld_tri, &worldToLocal_tri, false,
//				triCount, vertexCount, indexs, points, nullptr, nullptr, nullptr);
//
//		GeomPrimitive * primit_tri = new GeomPrimitive(mesh, Reference<Material>(metal));
//
#endif
