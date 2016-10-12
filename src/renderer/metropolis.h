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
	bool mBidirectional;//是否使用双向路径追踪
	unsigned int mNumPixelSamples;//每个像素平均的样本点个数
	Camera* mCamera;
	unsigned int mMaxDepth;//最大的路径深度
	unsigned int mLargeStepsPerPixel;//每个像素largeStep的个数
	unsigned int mNumBootstrap;
	unsigned int mNumDirectPixelSamples;//直接光照下的每个像素包含的样本点
	unsigned int mMaxConsecutiveRejects;//最大连续拒绝次数
	atomic_uint mNumTasksFinished;


	//普通的Path Tracing算法下计算radiance
	RGB LPath(const Scene *scene, const PathVertex *path, int pathLength,
			MemoryArena &arena, const vector<LightingSample> &samples,
			Random &rng, const Distribution1D *lightDistribution,
			const RayDifferential &escapedRay, const RGB &escapedAlpha) const;
	RGB LBidir(const Scene* scene, const PathVertex* cameraPath,
			int cameraPathLength, const PathVertex* lightPath,
			int lightPathLength, MemoryArena &arena,
			const vector<LightingSample>& samples, Random& rng,
			const Distribution1D* lightDistribution,
			const RayDifferential &escapedRay, const RGB &escapedAlpha) const;
	//生成路径并计算radiance
	RGB PathL(const MLTSample &sample, const Scene*scene, MemoryArena& arena,
			const Camera* camera, const Distribution1D* lightDistribution,
			PathVertex *cameraPath, PathVertex* lightPath, Random& rng) const;
	friend class MLTTask;
public:

	MetropolisRenderer(int perPixelSamples,int nBootstrap,int directPixelSamples,
			float largeStepProbability,bool doDirectSeparately,int maxConsecutiveRejects,int maxDepth, Camera *camera, bool doBidirectional);
	~MetropolisRenderer();
	//渲染函数
	void render(const Scene *scene);
	RGB Li(const Scene *scene, const RayDifferential &ray,
		const Sample *sample, Random &rng, MemoryArena &arena,
		Intersection *isect, RGB *T) const {
		return RGB(0);
	}
};

#endif /* SRC_RENDERER_METROPOLIS_H_ */
