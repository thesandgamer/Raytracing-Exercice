#include <fstream>
#include <iostream>

#include "rtweekend.h"


#include "Color.h"
#include "Hittable_list.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"


Vec3 ray_color(const Ray& r, const Hittable& world, int depth) {
    Hit_record rec;
        // Si le rayon à trop rebondit, renvoie du noir
    if (depth <= 0)
        return Vec3(0,0,0);
    if (world.Hit(r, 0.001, infinity, rec)) { //0.001 pour la toléreance 
        Ray scattered; 
        Vec3 attenuation; //L'aténuation de la couleur
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) //Si le material est scattered(dispercé)
            return attenuation * ray_color(scattered, world, depth-1); //Retourne la couleur atténué
        return Vec3(0,0,0);//Retourne de la couleur noire
    }

    //Dessine le fond
    Vec3 unit_direction = unitVector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0); //blend entre couleur du haut et du bas en fonction de la hateur du pixel scanné
                   //Couleur en bas       //Couleur en haut
}

//Créer une map avec des boules random
Hittable_list random_scene() {
    Hittable_list world;

    auto ground_material = make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5)); //Material du sol
    world.add(make_shared<Sphere>(Vec3(0,-1000,0), 1000, ground_material)); //Rajoute une énorme sphere qui fait le sol

    //Boucler pour que ça pose des sphères partout au sol de -11 à 11 et x et de -11 à 11 en y
    for (int a = -11; a < 11; a++) { 
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double(); //Prend un nombre au hazard pour définir le material plus tard
            Vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double()); //Définit les centres des spheres au sol 

            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) { //Pour éviter que les sphères se collident
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random() * random(); //définit au hazard l'albedo(couleur) du material
                    sphere_material = make_shared<Lambertian>(albedo);//le material sera un material matte
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));//Ajoute une sphere
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random(0.5, 1); //définit au hazard l'albedo(couleur) du material
                    auto fuzz = random_double(0, 0.5); //définit au hazard la rougness (de combien sa disperce les rayons renvoyés)
                    sphere_material = make_shared<Metal>(albedo, fuzz); //definit le material de la sphere
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5); //Definit un material de verre avec un indice de réfraction de 1.5
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    //Définit les 3 grosses sphères
    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));

    return world;
}


int main()
{
    // Image
    const auto aspect_ratio = 3.0 / 2.0;//Définit le ratio de l'image

    //Taille de l'image rendue
    const int image_width = 500; 
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    const int samples_per_pixel = 500;//Nombre de rayons envoyé 
    const int max_depth = 50; //Nombre de fois qu'un rayon peut rebondir(être renvoyé)

    // Camera
    Vec3 lookfrom(13,2,3);//Origine de la camera
    Vec3 lookat(0,0,0); //La où il regarde
    Vec3 vup(0,1,0); //Définit qui est le vecteur up
    auto dist_to_focus = 10.0; //Définit le focus de la camera
    auto aperture = 0.1; //Définit son ouverture(combien de rayons sont lu(fait rentrer plus ou moins de rayons))

    Camera cam(lookfrom, lookat, vup, 30, aspect_ratio, aperture, dist_to_focus);
    
    // World
    /*
    Hittable_list world;

    auto material_ground = make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));// Metrial du sol mat
    auto material_center = make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5));
    auto material_left   = make_shared<Dielectric>(1.5);
    auto material_right  = make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Vec3( 0.0, -100.5, -1.0), 100.0, material_ground)); //Sphere du sol
    world.add(make_shared<Sphere>(Vec3( 0.0,    0.0, -1.0),   0.5, material_center)); 
    world.add(make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.5, material_left)); 
    world.add(make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),  -0.4, material_left)); // met une sphere négative pour avoir un meilleur verre
    world.add(make_shared<Sphere>(Vec3( 1.0,    0.0, -1.0),   0.5, material_right)); 
*/
    auto world = random_scene();


    //------------------Render
    std::ofstream output;   //Concoit un fichier de type ofstream
    output.open("output.ppm");  //Créer le fichier si il existe pas, on l'ouvre si il existe, le fichier output.ppm

    //Ecrit dans le fichier
    output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //Pour chaque pixel de l'écran
    for (int j = image_height-1; j >= 0; --j) {     //En hauteur
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {     //En largeur
        
            Vec3 pixel_color(0, 0, 0); //Setup la couleur du pixel en pixel de base
            for (int s = 0; s < samples_per_pixel; ++s) { //Pour chaque sample de pixel 
                auto u = (i + random_double()) / (image_width-1); //
                auto v = (j + random_double()) / (image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth); //Couleur du pixel scanné
            }
            write_color(output, pixel_color, samples_per_pixel);//Set la couleur du pixel(écrit dans le output.ppm) en fonction de pixel color
        }
    }
    std::cerr << "\nDone.\n";

    output.close(); //Ferme le fichier

    
    



    return 0;
}

//https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage