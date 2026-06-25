#ifndef CYLINDER_H
#define CYLINDER_H

#include "hittable.h"
#include "disk.h"
#include "aabb.h"

class cylinder : public hittable
{
public:
    cylinder(const point3 &center, const vec3 &axis, double radius, double height, shared_ptr<material> mat) : center(center), axis(unit_vector(axis)), height(height), radius(radius), mat(mat)
    {
        d1 = disk(center + axis * height / 2, axis, radius, mat);
        d2 = disk(center - axis * height / 2, -axis, radius, mat);
        make_bounding_box();
    }

    aabb bounding_box() const override
    {

        return bbox;
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        hit_record side_rec, best_rec, rec1, rec2;
        bool hit(false);

        bool d1_hit = d1.hit(r, ray_t, rec1);
        if (d1_hit)
        {
            best_rec = rec1;
            hit = true;
        }

        bool d2_hit = d2.hit(r, ray_t, rec2);
        if (d2_hit && (!hit || rec2.t < best_rec.t))
        {
            best_rec = rec2;
            hit = true;
        }

        vec3 m, D, M;
        double a, b, c;
        m = r.origin() - center;
        D = r.direction() - dot(r.direction(), axis) * axis;
        M = m - dot(m, axis) * axis;

        a = dot(D, D);
        b = 2 * dot(M, D);
        c = dot(M, M) - radius * radius;

        bool surface_hit(false);
        double discriminant;
        if (std::fabs(a) < 1e-8)
            surface_hit = false;
        else
        {
             discriminant = b * b - 4 * a * c;
            if (discriminant >= 0)
                surface_hit = true;
        }

        double root;
        if (surface_hit)
        {
            auto sqrtd = std::sqrt(discriminant);

            root = (-b - sqrtd) / (2 * a); // solution of the length of the ray from centre to the sphere
            if (!ray_t.surrounds(root))
            {
                root = (-b + sqrtd) / (2 * a);
                if (!ray_t.surrounds(root))
                    surface_hit = false;
                ;
            }
        }

        if (surface_hit)
        {
            side_rec.t = root;
            side_rec.p = r.at(side_rec.t);
            side_rec.mat = mat;
            double h = dot(side_rec.p - center, axis);
            if (h > height / 2 || h < -height / 2)
            {
                surface_hit = false;
            }
            else
            {
                point3 foot = center + dot(side_rec.p - center, axis) * axis;
                side_rec.set_face_normal(r, side_rec.p - foot);
                
            }
        }

        if (surface_hit && (!hit || side_rec.t < best_rec.t))
        {
            best_rec = side_rec;
            hit = true;
        }

        if (hit)
        {
            rec = best_rec;
        }
        return hit;
    }

private:
    disk d1;
    disk d2;
    vec3 axis;
    point3 center;
    double radius;
    aabb bbox;
    double height;
    shared_ptr<material> mat;

    void make_bounding_box()
    {

        auto top = center + axis * (height / 2);

        auto bottom = center - axis * (height / 2);

        double extent_x = radius * sqrt(1 - axis.x() * axis.x());

        double extent_y = radius * sqrt(1 - axis.y() * axis.y());

        double extent_z = radius * sqrt(1 - axis.z() * axis.z());

        point3 min(std::min(top.x(), bottom.x()) - extent_x,

                   std::min(top.y(), bottom.y()) - extent_y,

                   std::min(top.z(), bottom.z()) - extent_z

        );

        point3 max(std::max(top.x(), bottom.x()) + extent_x,

                   std::max(top.y(), bottom.y()) + extent_y,

                   std::max(top.z(), bottom.z()) + extent_z

        );

        bbox = aabb(min, max);
    }
};

#endif