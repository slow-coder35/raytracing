#pragma once

#include "hittable.h"
#include"hittable_list.h"




class ellipsoid:public hittable{

    public:
        ellipsoid(point3 center,double x,double y,double z,shared_ptr<material> mat):center(center),x(x),y(y),z(z),mat(mat){
            bbox=aabb(center-vec3(x,y,z),center+vec3(x,y,z));
        }


        bool hit(const ray& r, interval ray_t,hit_record& rec)const override{
            vec3 oc=r.origin()-center;

            auto A =( r.direction().x()*r.direction().x()/ (x*x))+(r.direction().y()*r.direction().y()/(y*y))+(r.direction().z()*r.direction().z()/(z*z));
            auto B=2*( r.origin().x()*r.direction().x()/ (x*x))+(r.origin().y()*r.direction().y()/(y*y))+(r.origin().z()*r.direction().z()/(z*z));
            auto C =( r.origin().x()*r.origin().x()/ (x*x))+(r.origin().y()*r.origin().y()/(y*y))+(r.origin().z()*r.origin().z()/(z*z))-1;

            auto discriminant=B*B-4*A*C;
            if(discriminant<0) return false;

            auto sqrtd=std::sqrt(discriminant);

        auto root =(-B-sqrtd)/2*A;                 //solution of the length of the ray from centre to the sphere
        if(!ray_t.surrounds(root)){
            root=(-B+sqrtd)/2*A;
            if(!ray_t.surrounds(root))return false;
        }            
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 p = rec.p - center;

        vec3 outward_normal(
            p.x()/(x*x),
            p.y()/(y*y),
            p.z()/(z*z)
        );

        outward_normal = unit_vector(outward_normal);

        
        rec.set_face_normal(r, outward_normal);
       
        rec.mat = mat;
    return true;
    }
    aabb bounding_box()const override {return bbox;}           





    private:
        point3 center;
        double x;
        double y;
        double z;
        aabb bbox;
        shared_ptr<material> mat;
    




};