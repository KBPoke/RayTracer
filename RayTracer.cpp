#include <iostream>
#include <vector>
#include <cmath>
#include "Vec3.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"

color ray_color_background(const Ray& r) {
    auto a = 0.5 * (r.direction.y + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.1, 0.4, 0.7);
}

int main()
{
    std::vector<Sphere> spheres;
    Plane plane(Vec3(0,0,0),Vec3(0,-1,0),color(0.5,0.5,0));

    spheres.push_back(Sphere(Point3(1, 0, 1), 0.5, color(0,1,1)));
    spheres.push_back(Sphere(Point3(-0.1, 0.1, 2), 1));

    Image image(1280, 960);

    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (float(image.width) / image.height);
    Point3 camera_origin(0, 1, 0);

    Camera camera(camera_origin, focal_length, viewport_width, image);

    std::cout << camera.pixel00_loc - camera_origin;


    for (int j = 0; j < camera.output.height; j++) {
        for (int i = 0; i < camera.output.width; i++) {
            const Point3 pixel_center = camera.pixel00_loc + (i * camera.pixel_delta_horizontal) + (j * camera.pixel_delta_vertical);
            const Vec3 ray_direction = pixel_center - camera.camera_origin;
            Ray r(camera_origin, ray_direction);

            float tNearest = MAX_RENDER_DISTANCE, ttemp = MAX_RENDER_DISTANCE;
            Object* ObjectNearest = nullptr;

            for (Sphere& object : spheres) {
                if (object.check_intersection(r, ttemp) == true) {
                    if (ttemp < tNearest) {
                        tNearest = ttemp;
                        ObjectNearest = &object;
                    }
                }
            }

            if (plane.check_intersection(r, ttemp) == true) {
                if (ttemp < tNearest) {
                    tNearest = ttemp;
                    ObjectNearest = &plane;
                }
            }

            if (tNearest >= MAX_RENDER_DISTANCE) {
                camera.output.data.push_back(ray_color_background(r));
            }
            else {
                const Vec3 Normal_Hit = ObjectNearest->get_surface_normal(r.delta(tNearest));
                color color_of_hit = 0.5 * (0.5 * color(Normal_Hit.x + 1, Normal_Hit.y + 1, Normal_Hit.z + 1) + ObjectNearest->get_color());
                camera.output.data.push_back(color_of_hit);
            }
        }
    }

    camera.print_output();
}