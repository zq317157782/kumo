//
// Created by Administrator on 2016/6/28 0028.
//

#include <rgb.h>
#include "reflection.h"
#include "sampler.h"

RGB BRDFToBTDF::f(const Vector &wo, const Vector &wi) const {
	return mBrdf->f(wo, otherHemisphere(wi));
}

RGB BRDFToBTDF::Sample_f(const Vector& wo, Vector* wi, Float u1, Float u2,
		Float *pdf) const {
	RGB ret = mBrdf->Sample_f(wo, wi, u1, u2, pdf);
	*wi = otherHemisphere(*wi); //改变采样后的入射光的方向
	return ret;
}

RGB ScaledBxDF::f(const Vector &wo, const Vector &wi) const {
	return mScale * mBxdf->f(wo, wi);
}

RGB ScaledBxDF::Sample_f(const Vector& wo, Vector* wi, Float u1, Float u2,
		Float *pdf) const {
	return mScale * mBxdf->Sample_f(wo, wi, u1, u2, pdf);
}

RGB EvaluateFresnelDiel(Float cosi, Float cost, const RGB &etai,
		const RGB &etat) {
	//以下是计算绝缘体Fresnel系数的公式实现
	RGB Rparl = ((etat * cosi) - (etai * cost))
			/ ((etat * cosi) + (etai * cost));
	RGB Rperp = ((etai * cosi) - (etat * cost))
			/ ((etai * cosi) + (etat * cost));
	return (Rparl * Rparl + Rperp * Rperp) / 2.f;
}

RGB EvaluateFresnelCond(Float cosi, const RGB &eta, const RGB &k) {
	//导电体的Fresnel反射系数计算
	RGB tmp = (eta * eta + k * k) * cosi * cosi;
	RGB Rparl2 = (tmp - (2.f * eta * cosi) + 1)
			/ (tmp + (2.f * eta * cosi) + 1);
	RGB tmp_f = eta * eta + k * k;
	RGB Rperp2 = (tmp_f - (2.f * eta * cosi) + cosi * cosi)
			/ (tmp_f + (2.f * eta * cosi) + cosi * cosi);
	return (Rparl2 + Rperp2) / 2.f;
}

RGB FresnelConductor::Evaluate(Float cosi) const {
	//计算入射角度与法线之间的角度的余弦是cosi的导电体Fresnel反射系数
	return EvaluateFresnelCond(cosi, mEta, mK);
}

RGB FresnelDielectric::Evaluate(Float cosi) const {
	cosi = Clamp(cosi, -1.f, 1.f); //保护cosi的值
	bool entering = cosi > 0.; //用来判断射线是入射的还是出射的
	Float ei = mEtaI, et = mEtaT;
	if (!entering)
		std::swap(ei, et); //如果是出射的话，交换折射系数的顺序
	//Snell's law
	Float sint = ei / et * sqrtf(std::max(0.f, 1.f - cosi * cosi));
	if (sint >= 1) {
		return 1;	//完全反射，没有折射
	} else {
		Float cost = sqrtf(std::max(0.f, 1.f - sint * sint));
		return EvaluateFresnelDiel(fabsf(cosi), cost, ei, et);	//返回反射系数
	}
}

RGB SpecularReflection::Sample_f(const Vector& wo, Vector* wi, Float u1,
		Float u2, Float *pdf) const {
	*wi = Vector(-wo.x, -wo.y, wo.z);	//反射向量
	*pdf = 1.f;	//概率分布为1
	return mFresnel->Evaluate(CosTheta(wo)) * mScale / AbsCosTheta(*wi); //镜面反射的brdf公式
}

RGB SpecularTransmission::Sample_f(const Vector& wo, Vector* wi, Float u1,
		Float u2, Float *pdf) const {
	bool entering = CosTheta(wo) > 0.;
	Float ei = mEtaI, et = mEtaT;
	if (!entering) //判断wo是从外面射入还是从内部射出
		std::swap(ei, et);
	//根据Snell's law 计算折射方向
	Float sini2 = SinTheta2(wo);
	Float eta = ei / et;
	Float sint2 = eta * eta * sini2;

	if (sint2 >= 1.)
		return 0.; //所有的光线全部反射，所以没有折射
	Float cost = sqrtf(std::max(0.f, 1.f - sint2));
	if (entering)
		cost = -cost; //设置符号
	Float sintOverSini = eta;
	*wi = Vector(sintOverSini * -wo.x, sintOverSini * -wo.y, cost);
	*pdf = 1.f;
	RGB F = mFresnel.Evaluate(CosTheta(wo)); //计算反射系数
	return (ei * ei) / (et * et) * (RGB(1.0f) - F) * mScale / AbsCosTheta(*wi);
}

Microfacet::Microfacet(const RGB& reflectance, Fresnel* fresnel,
		MicrofacetDistribution* distribution) :
		BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)), mR(reflectance), mFresnel(
				fresnel), mDistribution(distribution) {

}

Float Microfacet::G(const Vector &wo, const Vector &wi,
		const Vector &wh) const {
	Float NdotWh = AbsCosTheta(wh); //半角和法线之间的cos值  相当于是点乘
	Float NdotWo = AbsCosTheta(wo); //出射方向与法线
	Float NdotWi = AbsCosTheta(wi); //入射方向与法线
	Float WodotWh = AbsDot(wh, wo); //出射和半角向量之间的点乘
	return std::min(1.0f,
		std::min((2.f * NdotWh * NdotWo / WodotWh),
					(2.f * NdotWh * NdotWi / WodotWh)));
}

//这里使用的就是Torrance-Sparrow Modle的公式
RGB Microfacet::f(const Vector &wo, const Vector &wi) const {
	Float cosO = AbsCosTheta(wo);
	if (cosO == 0)
		return RGB(0);
	Float cosI = AbsCosTheta(wi);
	if (cosI == 0)
		return RGB(0);
	Vector wh = wi + wo;
	if (wh.x == 0 && wh.y == 0 && wh.z == 0)
		return RGB(0);
	wh = Normalize(wh);
	Float cosH = Dot(wi, wh);
	RGB F = mFresnel->Evaluate(cosH);
	return mR * F * mDistribution->D(wh) * G(wo, wi, wh) / (4.0f * cosO * cosI);
}

//复制自PBRT
void Anisotropic::Sample_f(const Vector &wo, Vector *wi, Float u1, Float u2,
		Float *pdf) const {
	Float phi, costheta;
	if (u1 < .25f) {
		sampleFirstQuadrant(4.f * u1, u2, &phi, &costheta);
	} else if (u1 < .5f) {
		u1 = 4.f * (.5f - u1);
		sampleFirstQuadrant(u1, u2, &phi, &costheta);
		phi = Pi - phi;
	} else if (u1 < .75f) {
		u1 = 4.f * (u1 - .5f);
		sampleFirstQuadrant(u1, u2, &phi, &costheta);
		phi += Pi;
	} else {
		u1 = 4.f * (1.f - u1);
		sampleFirstQuadrant(u1, u2, &phi, &costheta);
		phi = 2.f * Pi - phi;
	}
	Float sintheta = sqrtf(std::max(0.f, 1.f - costheta * costheta));
	Vector wh = SphericalDirection(sintheta, costheta, phi);
	if (!SameHemisphere(wo, wh))
		wh = -wh;

	// Compute incident direction by reflecting about $\wh$
	*wi = -wo + 2.f * Dot(wo, wh) * wh;

	// Compute PDF for $\wi$ from anisotropic distribution
	Float costhetah = AbsCosTheta(wh);
	Float ds = 1.f - costhetah * costhetah;
	Float anisotropic_pdf = 0.f;
	if (ds > 0.f && Dot(wo, wh) > 0.f) {
		Float e = (ex * wh.x * wh.x + ey * wh.y * wh.y) / ds;
		Float d = sqrtf((ex + 1.f) * (ey + 1.f)) * InvTwoPi
				* powf(costhetah, e);
		anisotropic_pdf = d / (4.f * Dot(wo, wh));
	}
	*pdf = anisotropic_pdf;
}

//复制自PBRT
void Anisotropic::sampleFirstQuadrant(Float u1, Float u2, Float *phi,
		Float *costheta) const {
	if (ex == ey)
		*phi = Pi * u1 * 0.5f;
	else
		*phi = atanf(sqrtf((ex + 1.f) / (ey + 1.f)) * tanf(Pi * u1 * 0.5f));
	Float cosphi = cosf(*phi), sinphi = sinf(*phi);
	*costheta = powf(u2,
			1.f / (ex * cosphi * cosphi + ey * sinphi * sinphi + 1));
}

//复制自PBRT
Float Anisotropic::Pdf(const Vector &wo, const Vector &wi) const {
	Vector wh = Normalize(wo + wi);
	Float costhetah = AbsCosTheta(wh);
	Float ds = 1.f - costhetah * costhetah;
	Float anisotropic_pdf = 0.f;
	if (ds > 0.f && Dot(wo, wh) > 0.f) {
		Float e = (ex * wh.x * wh.x + ey * wh.y * wh.y) / ds;
		Float d = sqrtf((ex + 1.f) * (ey + 1.f)) * InvTwoPi
				* powf(costhetah, e);
		anisotropic_pdf = d / (4.f * Dot(wo, wh));
	}
	return anisotropic_pdf;
}

BSDFSampleOffsets::BSDFSampleOffsets(int count, Sample *sample) {
	nSamples = count;
	componentOffset = sample->Add1D(nSamples);
	dirOffset = sample->Add2D(nSamples);
}

BSDFSample::BSDFSample(const Sample *sample, const BSDFSampleOffsets &offsets,
		uint32_t num) {
	uDir[0] = sample->twoD[offsets.dirOffset][2 * num];
	uDir[1] = sample->twoD[offsets.dirOffset][2 * num + 1];
	uComponent = sample->oneD[offsets.componentOffset][num];
}

BSDF::BSDF(const DifferentialGeometry& dg, const Normal& ng, Float e) :
		dgShading(dg), mNG(ng), eta(e) {
	mNN = dgShading.nn;
	mSN = Normalize(dgShading.dpdu);
	mTN = Cross(mNN, mSN); //法向量和切向量叉乘
	mNumBxdf = 0;
}

void BSDF::Add(BxDF *bxdf) {
	//cout<<mNumBxdf<<endl;
	assert(mNumBxdf<MAX_BxDFS);
	mBxdfs[mNumBxdf++] = bxdf;
}

Vector BSDF::WorldToLocal(const Vector& w) const {
	return Vector(Dot(mSN, w), Dot(mTN, w), Dot(mNN, w));
}

Vector BSDF::LocalToWorld(const Vector& w) const {
	return Vector(mSN.x * w.x + mTN.x * w.y + mNN.x * w.z,
			mSN.y * w.x + mTN.y * w.y + mNN.y * w.z,
			mSN.z * w.x + mTN.z * w.y + mNN.z * w.z);
}

RGB BSDF::f(const Vector &woWorld, const Vector &wiWorld,
		BxDFType flags) const {
	Vector wo = WorldToLocal(woWorld);
	Vector wi = WorldToLocal(wiWorld);
	if (Dot(woWorld, mNG) * Dot(wiWorld, mNG) > 0) { //相乘大于零说明在同一半球
		flags = BxDFType(flags & ~BSDF_TRANSMISSION); //去除折射
	} else {
		flags = BxDFType(flags & ~BSDF_REFLECTION); //去除反射
	}
	RGB f = 0;
	for (int i = 0; i < mNumBxdf; ++i) {
		if (mBxdfs[i]->MatchesFlag(flags)) {
			f += mBxdfs[i]->f(wo, wi);
		}
	}
	return f;
}

int BSDF::NumComponents() const {
	return mNumBxdf;
}

int BSDF::NumComponents(BxDFType flags) const {
	int num = 0;
	for (int i = 0; i < mNumBxdf; ++i) {
		if (mBxdfs[i]->MatchesFlag(flags)) {
			num++;
		}
	}
	return num;
}

RGB BSDF::Sample_f(const Vector &woWorld, Vector *wiWorld,
		const BSDFSample &bsdfSample, Float *pdf, BxDFType flags,
		BxDFType *sampledType) const {
	int numComp = NumComponents(flags);
	if (numComp == 0) {
		*pdf = 0;
		if (sampledType)
			*sampledType = BxDFType(0);
		return 0;
	}

	int whichComp = std::min(Floor2Int(bsdfSample.uComponent * numComp),
			numComp - 1);
	int count = whichComp;
	BxDF *bxdf = nullptr; //被选中的Bxdf
	for (int i = 0; i < mNumBxdf; ++i) {
		if (mBxdfs[i]->MatchesFlag(flags) && count-- == 0) { //这技巧 GOOD
			bxdf = mBxdfs[i];
			break;
		}
	}

	Vector wo = WorldToLocal(woWorld); //获得局部坐标下的wo
	Vector wi;
	*pdf = 0.f;
	RGB f = bxdf->Sample_f(wo, &wi, bsdfSample.uDir[0], bsdfSample.uDir[1],
			pdf); //这里的主要作用是采样出射方向
	if (*pdf == 0) {
		if (sampledType)
			*sampledType = BxDFType(0);
		return 0;
	}
	if (sampledType)
		*sampledType = bxdf->type;
	*wiWorld = LocalToWorld(wi); //获得世界坐标系下的wi

	//计算pdf
	if (!(bxdf->type & BSDF_SPECULAR) && numComp > 1) {
		for (int i = 0; i < mNumBxdf; ++i) {
			if (bxdf != mBxdfs[i] && mBxdfs[i]->MatchesFlag(flags)) {
				*pdf += mBxdfs[i]->Pdf(wo, wi);
			}
		}
	}

	if (numComp > 1) {
		*pdf /= numComp;
	}

	//开始计算BRDF系数
	if (!(bxdf->type & BSDF_SPECULAR)) {
		f = 0; //因为不是镜面反射 所以重新计算BRDF
		if (Dot(*wiWorld, mNG) * Dot(woWorld, mNG) > 0) //反射
			flags = BxDFType(flags & ~BSDF_TRANSMISSION);
		else
			//折射
			flags = BxDFType(flags & ~BSDF_REFLECTION);
		for (int i = 0; i < mNumBxdf; ++i)
			if (mBxdfs[i]->MatchesFlag(flags))
				f += mBxdfs[i]->f(wo, wi);
	}

	return f;
}

Float BSDF::Pdf(const Vector &woWorld, const Vector &wiWorld,
		BxDFType flags) const {
	if (mNumBxdf == 0)
		return 0;
	Vector wo = WorldToLocal(woWorld), wi = WorldToLocal(wiWorld);
	Float pdf = 0.0f;
	int matchingComps = 0;
	for (int i = 0; i < mNumBxdf; ++i)
		if (mBxdfs[i]->MatchesFlag(flags)) {
			++matchingComps;
			pdf += mBxdfs[i]->Pdf(wo, wi);
		}
	Float v = matchingComps > 0 ? pdf / matchingComps : 0.f;
	return v;
}

RGB BSDF::rho(const Vector &wo, Random &rng, BxDFType flags,
		int sqrtSamples) const {
	int nSamples = sqrtSamples * sqrtSamples;
	Float *s1 = ALLOCA(Float, 2 * nSamples);
	StratifiedSample2D(s1, sqrtSamples, sqrtSamples, rng);
	RGB ret(0);
	for (int i = 0; i < mNumBxdf; ++i)
		if (mBxdfs[i]->MatchesFlag(flags)){
			ret += mBxdfs[i]->rho(wo, nSamples, s1);
		}
	return ret;
}
