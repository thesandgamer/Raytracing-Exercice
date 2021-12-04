#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
    public:
        Sphere() {};
        Sphere(Vec3 cen, double r, shared_ptr<Material> m): center(cen), radius(r), mat_ptr(m) {}; //Constructeur de la sphere

        //virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const ; //Ovverride la fonction Hit
        bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override ; //Ovverride la fonction Hit

    public:
        Vec3 center; //Centre de la sphere
        double radius;  //Radius de la sphere
        shared_ptr<Material> mat_ptr; //Material de la sphere
};

bool Sphere::Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const { //Function de check de hit de la sphere
    Vec3 oc = r.origin() - center;
    double a = r.direction().squaredLength();
    double half_b = dot(oc, r.direction());
    double c = oc.squaredLength() - radius*radius;

    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr; //Set le material de la sphere dans le record

    return true;
}

#endif