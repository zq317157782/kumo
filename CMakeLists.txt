cmake_minimum_required(VERSION 3.3)
project(RayTracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp base/Vector3.cpp base/Vector3.h test/catch.hpp base/RGB.cpp base/RGB.h base/Vector2.cpp base/Vector2.h sampler/Sampler.cpp sampler/Sampler.h sampler/JitteredSampler.cpp sampler/JitteredSampler.h sampler/RandomSampler.cpp sampler/RandomSampler.h sampler/NRookSampler.cpp sampler/NRookSampler.h sampler/MultiJitteredSampler.cpp sampler/MultiJitteredSampler.h brdf/BRDF.cpp brdf/BRDF.h ShadeRec.cpp ShadeRec.h brdf/Lambertian.cpp brdf/Lambertian.h)
add_executable(RayTracer ${SOURCE_FILES})