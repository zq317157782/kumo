//#define UNIT_TEST
#ifdef UNIT_TEST

#define CATCH_CONFIG_MAIN
#include "test/catch.hpp"
#include "base/Vector3.h"
#include "base/RGB.h"
#include "base/Vector2.h"
#include "oldSampler/JitteredOldSampler.h"
#include "common/Ray.h"
#include "common/Picture.h"

#else
#include <iostream>
#include "shape/sphere.h"
#include "camera/PinholeCamera.h"
#include "oldSampler/MultiJitteredOldSampler.h"
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
#include "shape/trianglemesh.h"
#include "Scene.h"
#include "thrid/obj/Model.h"
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
#include "integrator/path.h"
#include "sampler/stratified.h"
#include "parallel.h"
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
	//InitTasks();
	//WaitForAllTasks();

	Vector dir = CosSampleHemisphere(0.1, 0.1);

	ConstantTexture<RGB> *white = new ConstantTexture<RGB>(RGB(1, 1, 1));
	ConstantTexture<RGB> *red = new ConstantTexture<RGB>(RGB(1, 0, 0));
	ConstantTexture<RGB> *black = new ConstantTexture<RGB>(RGB(0, 0, 0));
	ConstantTexture<RGB> *eta = new ConstantTexture<RGB>(RGB(1.2, 1.2, 1.2));
	ConstantTexture<RGB> *kk = new ConstantTexture<RGB>(RGB(1.2, 1.2, 1.2));
	ConstantTexture<RGB> *half_white = new ConstantTexture<RGB>(
			RGB(0.5, 0.5, 0.5));
	ConstantTexture<float> *roughess = new ConstantTexture<float>(1);
	Checkerboard2DTexture<RGB> *checker = new Checkerboard2DTexture<RGB>(
			new UVMapping2D(10, 10), black, white);
	Matte * m = new Matte(checker);
	Metal * metal = new Metal(checker, eta, kk, new Blinn(25));
	Metal * metal2 = new Metal(checker, eta, kk, new Anisotropic(1000, 1000));
	Translucent *trans = new Translucent(black, white, roughess, black, white);
	Mirror * mirror = new Mirror(checker);

	Transform localToWorld = Translate(Vector(0, -1, 6));
	Transform worldToLocal = Translate(Vector(0, 1, -6));
	//第一个sphere
	Sphere* sphere = new Sphere(&localToWorld, &worldToLocal, false, 0.5, -0.5,
			0.5, 360);
	DiffuseAreaLight *diffuse = new DiffuseAreaLight(localToWorld, RGB(10, 10, 10),
			32, sphere);
	GeomPrimitive * primit = new GeomPrimitive(Reference<Shape>(sphere),
			Reference<Material>(m), diffuse);

	Transform localToWorld2 = Translate(Vector(0, 1, 6));
	Transform worldToLocal2 = Translate(Vector(-0,-1, -6));
	Sphere* sphere2 = new Sphere(&localToWorld2, &worldToLocal2, false, 0.5, -0.5,
			0.5, 360);
	GeomPrimitive * primit2 = new GeomPrimitive(Reference<Shape>(sphere2),
			Reference<Material>(m));

	Transform localToWorld2_2 = Translate(Vector(1.5, 0, 6));
	Transform worldToLocal2_2 = Translate(Vector(-1.5, 0, -6));
	Sphere* sphere3 = new Sphere(&localToWorld2_2, &worldToLocal2_2, false, 1,
			-1, 1, 360);
	GeomPrimitive * primit3 = new GeomPrimitive(Reference<Shape>(sphere3),
			Reference<Material>(metal));

	Transform localToWorld2_3 = Translate(Vector(0, -1.5, 6));
	Transform worldToLocal2_3 = Translate(Vector(0, 1.5, -6));
	Sphere* sphere4 = new Sphere(&localToWorld2_3, &worldToLocal2_3, false, 1,
			-1, 1, 360);
	GeomPrimitive * primit4 = new GeomPrimitive(Reference<Shape>(sphere4),
			Reference<Material>(metal));

	Transform l2w_panel1 = Translate(Vector(0, 0, 0));
	Transform w2l_panel1 = Translate(Vector(0, 0, -0));
	ConstantTexture<RGB> *half_red = new ConstantTexture<RGB>(RGB(1, 0, 0));
	Matte * m1 = new Matte(half_red);
	GeomPrimitive * panel1 = CreatePanel(&l2w_panel1, &w2l_panel1,
			Point(-2, 2, 8), Point(-2, -2, 8), Point(2, -2, 8), Point(2, 2, 8),
			m1);

	Transform l2w_panel2 = Translate(Vector(0, 0, 0));
	Transform w2l_panel2 = Translate(Vector(0, 0, -0));
	ConstantTexture<RGB> *half_blue = new ConstantTexture<RGB>(RGB(0, 0, 1));
	Matte * m2 = new Matte(half_blue);
	GeomPrimitive * panel2 = CreatePanel(&l2w_panel2, &w2l_panel2,
			Point(-2, 2, -2 + 8), Point(-2, -2, -2 + 8), Point(-2, -2, 2 + 8),
			Point(-2, 2, 2 + 8), m2);

	Transform l2w_panel3 = Translate(Vector(0, 0, 0));
	Transform w2l_panel3 = Translate(Vector(0, 0, -0));
	ConstantTexture<RGB> *half_green = new ConstantTexture<RGB>(RGB(0, 1, 0));
	Matte * m3 = new Matte(half_green);
	GeomPrimitive * panel3 = CreatePanel(&l2w_panel3, &w2l_panel3,
			Point(2, 2, 2 + 8), Point(2, -2, 2 + 8), Point(2, -2, -2 + 8),
			Point(2, 2, -2 + 8), m3);

	Transform l2w_panel4 = Translate(Vector(0, 0, 0));
	Transform w2l_panel4 = Translate(Vector(0, 0, -0));
	Matte * m4 = new Matte(checker);
	GeomPrimitive * panel4 = CreatePanel(&l2w_panel4, &w2l_panel4,
			Point(-2, -2, 2 + 8), Point(-2, -2, -2 + 8), Point(2, -2, -2 + 8),
			Point(2, -2, 2 + 8), m4);

	Transform l2w_panel5 = Translate(Vector(0, 0, 0));
		Transform w2l_panel5 = Translate(Vector(0, 0, -0));
		Matte * m5 = new Matte(checker);
		GeomPrimitive * panel5 = CreatePanel(&l2w_panel5, &w2l_panel5,
				Point(-2, 2, 2 + 8), Point(2, 2, 2 + 8), Point(2, 2, -2 + 8),
				Point(-2,  2, -2 + 8), metal);

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

	Transform cameraTransform = RotateY(0);
	PinholeCamera camera(
			new PPMFilm(800, 600, new BoxFilter(0.5, 0.5), "Renderer.ppm"),
			&cameraTransform);    //int xres,int yres,Filter* f,const char* file
	camera.setDistanceToView(700);
	//场景初始化
	Scene scene;
	scene.background = RGB(121.0 / 255, 121.0 / 255, 121.0 / 255);
	scene.addPrimitive(primit);
	scene.addPrimitive(primit2);
//	scene.addPrimitive(primit3);
//	scene.addPrimitive(primit4);
	//scene.addPrimitive(primit_tri);
	scene.addPrimitive(panel1);
	scene.addPrimitive(panel2);
	scene.addPrimitive(panel3);
	scene.addPrimitive(panel4);
	scene.addPrimitive(panel5);

	Transform localToWorld3 = Translate(Vector(0, 0, 5.3));
	Transform worldToLocal3 = Translate(Vector(0, 0, -5.3));
	DistantLight* p2 = new DistantLight(localToWorld3, RGB(1, 1, 1),
			Vector(0, 0, -1));
	DistantLight* p = new DistantLight(localToWorld3, RGB(1, 1, 1),
			Vector(1, 0, 0));
	scene.addLight(diffuse);
	//scene.addLight(p);
	//scene.addLight(p2);

//	SimpleRenderer renderer(&camera, new RandomSampler(0, 800, 0, 600, 64),
//			new PathIntegrator(5));	//new PathIntegrator(5)

	SimpleRenderer renderer(&camera, new StratifiedSampler(0, 800, 0, 600, 32,32,true),
				new PathIntegrator(5));	//new PathIntegrator(5)

	renderer.render(&scene);
	cout << "----" << endl;

}

////	//测试三角面片
//	Model model;
//	model.load("../t2.obj");
//
//
//	int triCount = model.numberOfTriangles();
//	int vertexCount = model.numberOfVertices();
//	Point* points = new Point[vertexCount];
//	for (int i = 0; i < vertexCount; ++i) {
//		_POINT p = model.getVertex(i);
//		points[i] = Point(p.x, p.y, p.z);
//	}
//
//	int * indexs = new int[3 * triCount];
//	for (int i = 0, j = 0; i < triCount; ++i) {
//		_TRIANGLE t = model.getTriangle(i);
//		indexs[j++] = t.index[0];
//		indexs[j++] = t.index[1];
//		indexs[j++] = t.index[2];
//	}
//
//
//	Transform localToWorld_tri = Scale(100,100,100);
//	Transform worldToLocal_tri = Scale(-100,-100,-100);
//
//	TriangleMesh* mesh = new TriangleMesh(&localToWorld_tri, &worldToLocal_tri, false,
//			triCount, vertexCount, indexs, points, nullptr, nullptr, nullptr);
//
//	GeomPrimitive * primit_tri = new GeomPrimitive(mesh, Reference<Material>(m));
#endif
