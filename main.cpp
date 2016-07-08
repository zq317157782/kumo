
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
#include <Scene.h>

TEST_CASE( "Vector3 are computed", "[Vector3]" ){
    Vector3 v1;
    REQUIRE(v1.x==0);
    REQUIRE(v1.y==0);
    REQUIRE(v1.z==0);
    Vector3 v2(1);
    REQUIRE(v2.x==1);
    REQUIRE(v2.y==1);
    REQUIRE(v2.z==1);

    SECTION("Vector3 operater+"){
        Vector3 temp=v1+v2;
        REQUIRE(temp.x==1);
        temp+=v2;
        REQUIRE(temp.x==2);
        temp=temp+1;
        REQUIRE(temp.x==3);
        temp=1.0+temp;
        REQUIRE(temp.x==4);
    }

    SECTION("Vector3 operater-"){
        Vector3 temp=v1-v2;
        REQUIRE(temp.x==-1);
        temp-=v2;
        REQUIRE(temp.x==-2);
        temp=temp-1;
        REQUIRE(temp.x==-3);
    }

    SECTION("Vector3 operater*"){
        Vector3 temp=v2*v2;
        REQUIRE(temp.x==1);
        temp*=v2;
        REQUIRE(temp.x==1);
        temp=temp*3;
        REQUIRE(temp.x==3);
        temp=3*temp;
        REQUIRE(temp.x==9);
    }

    REQUIRE(v1!=v2);
    Vector3 v3(1,1,1);
    Vector3 v4(1,1,1);
    REQUIRE(v3==v4);
    REQUIRE(v3.dot(v4)==3);
    //测试叉乘
    Vector3 v5(1,0,0);
    Vector3 v6(0,1,0);
    REQUIRE(v5.cross(v6)==Vector3(0,0,1));

}


TEST_CASE( "Vector2 are computed", "[Vector2]" ){
    Vector2 v1;
    REQUIRE(v1.x==0);
    REQUIRE(v1.y==0);
    Vector2 v2(1);
    REQUIRE(v2.x==1);
    REQUIRE(v2.y==1);

    SECTION("Vector2 operater+"){
        Vector2 temp=v1+v2;
        REQUIRE(temp.x==1);
        temp+=v2;
        REQUIRE(temp.x==2);
        temp=temp+1;
        REQUIRE(temp.x==3);
        temp=1.0+temp;
        REQUIRE(temp.x==4);
    }

    SECTION("Vector2 operater-"){
        Vector2 temp=v1-v2;
        REQUIRE(temp.x==-1);
        temp-=v2;
        REQUIRE(temp.x==-2);
        temp=temp-1;
        REQUIRE(temp.x==-3);
    }

    SECTION("Vector2 operater*"){
        Vector2 temp=v2*v2;
        REQUIRE(temp.x==1);
        temp*=v2;
        REQUIRE(temp.x==1);
        temp=temp*3;
        REQUIRE(temp.x==3);
        temp=3*temp;
        REQUIRE(temp.x==9);
    }

    REQUIRE(v1!=v2);
    Vector2 v3(1,1);
    Vector2 v4(1,1);
    REQUIRE(v3==v4);
    REQUIRE(v3.dot(v4)==2);
}

TEST_CASE( "RGB are computed", "[RGB]" ){
    RGB c1(0.5,0.5,0.5);
    RGB c2(0.5,0.5,0.5);
    REQUIRE(c1==c2);
    SECTION("operater+"){
        REQUIRE((c1+c2)==RGB(1,1,1));
        REQUIRE((0.5+c2)==RGB(1,1,1));
        REQUIRE((c1+=0.5)==RGB(1,1,1));
    }

    SECTION("operater-"){
        REQUIRE((c1-c2)==RGB(0,0,0));
        REQUIRE((c2-0.5)==RGB(0,0,0));
        REQUIRE((c1-=0.5)==RGB(0,0,0));
    }

    SECTION("operater*"){
        REQUIRE((c1*c2)==RGB(0.25,0.25,0.25));
        REQUIRE((c2*0.5)==RGB(0.25,0.25,0.25));
        REQUIRE((c1*=0.5)==RGB(0.25,0.25,0.25));
    }
}

TEST_CASE( "JitteredSampler are computed", "[JitteredSampler]" ){
    JitteredSampler jitteredSampler(10);
    REQUIRE(jitteredSampler.getSampleNum()==10);
    REQUIRE(jitteredSampler.getSetNum()==JitteredSampler::DEFAULT_SET_NUM);
    Vector2 sample=jitteredSampler.sampleUnitSquare();
    REQUIRE(sample.x<=1);
    REQUIRE(sample.x>=0);
    jitteredSampler.mapSamples2UnitDisk();
    Vector2 sampleDisk=jitteredSampler.sampleUnitDisk();
    REQUIRE(sampleDisk.x<=1);
    REQUIRE(sampleDisk.x>=-1);
    jitteredSampler.mapSamples2Hemisphere(1);
    Vector3 sampleHemi=jitteredSampler.sampleHemi();
    REQUIRE(sampleHemi.x<=1);
    REQUIRE(sampleHemi.x>=-1);
}

TEST_CASE( "Ray are computed", "[Ray]" ){
    Ray ray(Vector3(0,0,0),Vector3(1,0,0));
    Vector3 p=ray.getPointByDistance(1);
    REQUIRE(p.x==1);
}


TEST_CASE( "Picture are computed", "[Picture]" ){
   Picture p(2,2);
    int k=0;
    for(int i=0;i<2;++i)
        for(int j=0;j<2;++j){
            p[i][j]=k;
            ++k;
        }
    REQUIRE(p[0][0]==0);
    REQUIRE(p[0][1]==1);
    REQUIRE(p[1][0]==2);
    REQUIRE(p[1][1]==3);

}


TEST_CASE( "scene are computed", "[scene]" ){
    Scene scene;
    REQUIRE(scene.ambient());
}
#else
#include <iostream>
#include "shape/Sphere.h"
#include "camera/PinholeCamera.h"
#include "oldSampler/MultiJitteredOldSampler.h"
#include "light/Directional.h"
#include "material/TorranceSparrow.h"
#include "material/CookTorranceMaterial.h"
#include "material/BlinnPhongMaterial.h"
#include "material/PhongMaterial.h"
#include "transform.h"
#include "global.h"
#include "renderer/simpleRenderer.h"
#include "reflection.h"
#include "primitive.h"
#include "material/Matte.h"
#include "integrator/SimpleIntegrator.h"
#include "random.h"
#include "sampler/randomSampler.h"
#include "film/ppm.h"
#include "filter/box.h"
#include "shape/trianglemesh.h"
using namespace std;
//#define UNIT_TEST
#ifdef UNIT_TEST
#include "gtest/gtest.h"
#include "test/test.h"
#endif

int main(int argc,char** argv) {
#ifdef UNIT_TEST
    ::testing::InitGoogleTest(&argc,argv);
    return  RUN_ALL_TESTS();
#endif


    Matte * m=new Matte();

    Transform localToWorld=Translate(Vector(0,0,400));
    Transform worldToLocal=Translate(Vector(0,0,-400));
    //第一个sphere
    Sphere* sphere=new Sphere(&localToWorld,&worldToLocal,false, 100,-100,100,360);


    Transform localToWorld2=Translate(Vector(0,0,800));
    Transform worldToLocal2=Translate(Vector(0,0,-800));
    Sphere* sphere2=new Sphere(&localToWorld2,&worldToLocal2,false, 300,-300,300,360);
   // TriangleMesh* mesh=new TriangleMesh(&localToWorld2,&worldToLocal2,false);

   // sphere->setMaterial(cookTorranceMaterial);

    GeomPrimitive * primit=new GeomPrimitive(Reference<Shape>(sphere),Reference<Material>(m));
    GeomPrimitive * primit2=new GeomPrimitive(Reference<Shape>(sphere2),Reference<Material>(m));

    //Film picture(800, 600, 1);

    Transform cameraTransform= RotateY(0);
    PinholeCamera camera(new PPMFilm(800,600,new BoxFilter(0.5,0.5),"Renderer.ppm"),&cameraTransform);//int xres,int yres,Filter* f,const char* file
    //camera.setSampler(new MultiJitteredSampler(25));
    camera.setDistanceToView(500);

    //场景初始化
    Scene scene;
    scene.background=RGB(1,1,0);
    scene.addPrimitive(Reference<Primitive>(primit));
    scene.addPrimitive(Reference<Primitive>(primit2));


    Directional* directional=new Directional(RGB(1,0,0),RotateX(30)(Vector(0,0,1)));


    scene.addLight(directional);




    //camera.film=&picture;

    SimpleRenderer renderer(&camera,new RandomSampler(0,800,0,600,32),new SimpleIntegrator());
    renderer.render(&scene);
    //camera.renderScene(scene,picture);
   // camera.film->saveToLocal("Renderer.ppm");
   // picture.saveToLocal("AAA.png");



}
#endif
