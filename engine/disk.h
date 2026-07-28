#ifndef DISK_H
#define DISK_H


#include "aabb.h"
#include "hittable.h"


class disk :public hittable{
    public:
    disk(){};
    disk(const point3& center,const vec3& normal,double radius,shared_ptr<material> mat) : center(center),normal(unit_vector(normal)),radius(radius),mat(mat){
        auto rvec=vec3(radius,radius,radius);
        bbox=aabb(center-rvec,center+rvec);
    }

    aabb bounding_box() const override {return bbox;}

    bool hit(const ray& r,interval ray_t,hit_record& rec) const override {
        double denom=dot(normal,r.direction());//denominator

        if(std::fabs(denom)<1e-8) return false;   //is parallel to the plane
        
        auto t=(dot(center-r.origin(),normal))/denom;  //parameter t
        if(!ray_t.contain(t)) return false;            //intersection is behind the camera

        point3 p=r.at(t);

        if((p-center).length_squared()>radius*radius)return false;
            
        
        rec.t=t;
        rec.p=r.at(t);
        rec.mat=mat;
        rec.set_face_normal(r,normal);
            
        return true;
        
    }  
    
    protected:

    point3 center;
    vec3 normal;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;

};

class ring:public disk{

    public:
        ring(const point3& center,const vec3& normal,double inner_radius,double outer_radius,shared_ptr<material> mat) : disk(center,normal,outer_radius,mat),inner_radius(inner_radius){}

    aabb bounding_box()const override {return bbox;}

    bool hit(const ray& r,interval ray_t,hit_record& rec)const override{
    
        double denom=dot(normal,r.direction());//denominator

        if(std::fabs(denom)<1e-8) return false;   //is parallel to the plane
        
        auto t=(dot(center-r.origin(),normal))/denom;  //parameter t
        if(!ray_t.contain(t)) return false;            //intersection is behind the camera

        point3 p=r.at(t);

        if((p-center).length_squared()>radius*radius || (p-center).length_squared()>inner_radius*inner_radius)return false;
            
        
        rec.t=t;
        rec.p=r.at(t);
        rec.mat=mat;
        rec.set_face_normal(r,normal);
            
        return true;
        

    }





    private:
        double inner_radius;
        

};





#endif