#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

class Material;


//Structure pour stoquer le point d'impact, valeur de normale, et valeur t 
struct Hit_record 
{
    Vec3 p; //Stoque le point d'impact
    Vec3 normal; //Stoque la normale du point d'impact
    shared_ptr<Material> mat_ptr; //Material de l'objet hittable
    double t; //Stoque la value t

    bool front_face;//Stoque si ce qu'on touche est de l'intérieur ou de l'extérieur

    ///Set la normale de la face
    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) 
    {
        front_face = dot(r.direction(), outward_normal) < 0; //Check si c'est une front face
        normal = front_face ? outward_normal :-outward_normal; //Set la normal si c'est une front face
    }

};


//Interface qu'on va mettre 
class Hittable 
{
    public:
        virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const = 0;
};

#endif