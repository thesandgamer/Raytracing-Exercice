#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

#include "Ray.h"


struct Hit_record;

class Material {
    public:
        virtual bool scatter(const Ray& r_in, const Hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0; //Facçon de renvoyer le ray 
};


//Material mat
class Lambertian : public Material {
    public:
        Lambertian(const Vec3& a) : albedo(a) {}

        virtual bool scatter(const Ray& r_in, const Hit_record& rec, Vec3& attenuation, Ray& scattered) const override //Ovverid le scatter du material
        {
            auto scatter_direction = rec.normal + random_unit_vector(); //On renvoi le rayon dans la direction de la normale mais avec un random
            
            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())//Si la direction du rayon renvoyé est presque 0
                scatter_direction = rec.normal; //On dit que son renvoi est egal à celui de la normale
                
            scattered = Ray(rec.p, scatter_direction); //Où est dispercé le rayon
            attenuation = albedo; //De combien est atténué le rayon(sa couleur)
            return true;
        }

    public:
        Vec3 albedo;
};

//Material metalic
class Metal : public Material {
    public:
        Metal(const Vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {} //Le fuzzy va donner un apsect un peut flou(random un peut le vecteur de renvoi)

        virtual bool scatter(
            const Ray& r_in, const Hit_record& rec, Vec3& attenuation, Ray& scattered
        ) const override {
            Vec3 reflected = reflect(unitVector(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        Vec3 albedo;
        double fuzz;
};

//Material de verre qui refracte
class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(const Ray& r_in, const Hit_record& rec, Vec3& attenuation, Ray& scattered) const override 
        {
            attenuation = Vec3(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            Vec3 unit_direction = unitVector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = Ray(rec.p, direction);

            return true;
        }

    public:
        double ir; // Index of Refraction

    private:
        static double reflectance(double cosine, double ref_idx) 
        {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }

};

#endif