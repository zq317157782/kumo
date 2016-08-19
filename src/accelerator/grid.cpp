/*
 * grid.cpp
 *
 *  Created on: 2016年8月19日
 *      Author: zhuqian
 */

#include "grid.h"

GridAccel::GridAccel(const vector<Reference<Primitive>>& prims,
		bool refineImmediately) {
	if (refineImmediately) {
		for(unsigned int i=0;i<prims.size();++i){
			prims[i]->FullyRefine(mPrimitives);
		}
	} else {
		mPrimitives=prims;
	}

	for(unsigned int i=0;i<mPrimitives.size();++i){
		mBounds=Union(mBounds,mPrimitives[i]->WorldBound());//做并联
	}
	Vector delta=mBounds.pMax-mBounds.pMin;

	int maxAxis=mBounds.MaximumExtent();
	float invMaxWidth=1/delta[maxAxis];
	float cubeRoot=powf(float(mPrimitives.size()),1.0f/3.0f)*3.0f;
	float voxelPerUnit=cubeRoot*invMaxWidth;

	//计算获取每个轴体素的个数
	for(int axis=0;axis<3;++axis){
		mNumVoxels[axis]=Round2Int(delta[axis]*voxelPerUnit);
		mNumVoxels[axis]=Clamp(mNumVoxels[axis],1,64);//裁剪在1~64的范围内
	}

	for(int axis=0;axis<3;++axis){
		mWidth[axis]=delta[axis]/mNumVoxels[axis];
		mInvWidth[axis]=(mWidth[axis]==0)?0.0f:1.0f/mWidth[axis];
	}
	int numVoxels=mNumVoxels[0]*mNumVoxels[1]*mNumVoxels[2];
	mVoxels=AllocAligned<Voxel*>(numVoxels);
	memset(mVoxels, 0, numVoxels * sizeof(Voxel *));//分配体素的内存空间
	//开始填充图元
	for(unsigned int i=0;i<mPrimitives.size();++i){
		 BBox pb=mPrimitives[i]->WorldBound();
		 int vMax[3],vMin[3];
		 for(int axis=0;axis<3;++axis){
			 vMin[axis]=posToVoxel(pb.pMin,axis);
			 vMax[axis]=posToVoxel(pb.pMax,axis);
		 }
		 //向需要填充的VOXEL中填充图元
		 for(int z=vMin[2];z<vMax[2];++z){
			 for(int y=vMin[1];y<vMax[1];++y){
				 for(int x=vMin[0];x<vMax[0];++x){
					 int o=offset(x,y,z);
					 if(mVoxels[o]){
						 mVoxels[o]->AddPrimitive(mPrimitives[i]);
					 }
					 else{
						 mVoxels[o]=mVoxelArena.Alloc<Voxel>();
						 *mVoxels[o]=Voxel(mPrimitives[i]);
					 }
				 }
			 }
		 }
	}
}

