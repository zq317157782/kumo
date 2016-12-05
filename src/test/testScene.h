/*
 * testScene.h
 *
 *  Created on: 2016年12月5日
 *      Author: zhuqian
 */

#ifndef SRC_TEST_TESTSCENE_H_
#define SRC_TEST_TESTSCENE_H_
#include "shape/sphere.h"
#include "texture/constant.h"
#include "RGB.h"
#include "primitive.h"
#include "material/matte.h"
#include "camera/perspective.h"
#include "renderer/simpleRenderer.h"
#include "film/png.h"
#include "filter/triangle.h"
#include "sampler/randomSampler.h"
#include "integrator/path.h"
#include "scene.h"
#include "light/distant.h"
#include "accelerator/bvh.h"
#include "integrator/directlight.h"
void TestSceneOne() {
	Transform m = Translate(Vector(0, 0, 10));
	Transform invM = Inverse(m);
	Sphere sphere(&m, &invM, false, 4, -4, 4, 360);
	ConstantTexture<RGB> *white = new ConstantTexture<RGB>(RGB(1, 1, 1));
	Matte matte(white);
	GeomPrimitive p(&sphere, &matte);
	vector<Reference<Primitive>> primtives;
	primtives.push_back(&p);
	BVHAccel grid(primtives, 128, BVHAccel::SPLIT_MIDDLE);

	vector<Light*> lights;
	Transform localToWorld3 = Translate(Vector(0, 0, 2));
	Transform worldToLocal3 = Translate(Vector(0, 0, -2));
	DistantLight* lig = new DistantLight(localToWorld3, RGB(1, 1, 1),
			Vector(0, 0, -1));
	lights.push_back(lig);

	Scene scene(&grid, lights);
	//scene.background = RGB(121.0 / 255, 121.0 / 255, 121.0 / 255);

	Transform cameraTransform = Translate(Vector(0, 0, 0));
	float w[4] = { -1.2, 1.2, -1.2, 1.2 };
	PerspectiveCamera camera(cameraTransform, w, 0, 0, 60,
			new PNGFilm(600, 600, new TriangleFilter(0.5, 0.5),
					"result/TestSceneOne.png"));

	SimpleRenderer renderer(&camera, new RandomSampler(0, 600, 0, 600, 4),
			new DirectLightingIntegrator());	//new PathIntegrator(5)

	renderer.render(&scene);
}

#endif /* SRC_TEST_TESTSCENE_H_ */
