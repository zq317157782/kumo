/*
 * metal.h
 *
 *  Created on: 2016年7月15日
 *      Author: Administrator
 */

#ifndef MATERIAL_METAL_H_
#define MATERIAL_METAL_H_

#include "global.h"
#include "Material.h"

class Metal : public Material{
private:
   Microfacet* mBrdf;
   FresnelConductor* mFresnel;
public:
   Metal(const RGB& reflectance,const RGB& e, const RGB& kk,MicrofacetDistribution* dis);
    virtual RGB shade(const Intersection& sr);
    virtual RGB areaLightShade(Intersection& sr);
};

#endif /* MATERIAL_METAL_H_ */
