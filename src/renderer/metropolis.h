/*
 * metropolis.h
 *
 *  Created on: 2016年10月10日
 *      Author: zhuqian
 */

#ifndef SRC_RENDERER_METROPOLIS_H_
#define SRC_RENDERER_METROPOLIS_H_
#include "global.h"
#include "renderer.h"
#include "integrator/directlight.h"

struct MLTSample;
struct PathVertex;
struct LightingSample;

//MLT算法的实现
class MetropolisRenderer: public Renderer {
private:
	DirectLightingIntegrator * mDirectLighting;

	//普通的Path Tracing算法下计算radiance
	RGB LPath(const Scene *scene, const PathVertex *path, int pathLength,
			MemoryArena &arena, const vector<LightingSample> &samples,
			Random &rng,const Distribution1D *lightDistribution,
			const RayDifferential &escapedRay, const RGB &escapedAlpha) const;
public:

};

#endif /* SRC_RENDERER_METROPOLIS_H_ */
