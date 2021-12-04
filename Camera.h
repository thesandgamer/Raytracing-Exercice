#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

//Setup de la camera
class Camera {
    public:
            //Origine de la cam//Où regarde la cam//Vertical Fov //
        Camera(Vec3 lookfrom,Vec3 lookat,Vec3 vup,double vfov, double aspect_ratio,double aperture,double focus_dist)
        {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);

            //Taille du rendu de la camera
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            //Coordonées de la camera
            w = unitVector(lookfrom - lookat);
            u = unitVector(cross(vup, w));
            v = cross(w, u);

            //Set l'origine de la camera
            origin = lookfrom;

            //Setup les valeurs du rendu: la taille du rendu et le coin bas gauche
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2; //Définit le rayon de la lentille(ouverture/2)
        }

        Ray get_ray(double s, double t) const 
        {
            Vec3 rd = lens_radius * random_in_unit_disk();//Définir le flou de focus(qui va dépendre du rayon de la lentille)
            Vec3 offset = u * rd.x() + v * rd.y();//Offset du rayon

            return Ray( origin + offset,lower_left_corner + s*horizontal + t*vertical - origin - offset);
            //Retourne un rayon qui part de l'origine de la camera, qui va pointer en haut à gauche +un Vec2*la taille de vue de la cam (on rend pas en dehors de la camera)
        }

    private:
        Vec3 origin;
        Vec3 lower_left_corner;
        Vec3 horizontal;
        Vec3 vertical;
        Vec3 u, v, w;
        double lens_radius;
};

#endif