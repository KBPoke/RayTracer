#include <iostream>
#include <vector>
#include <cmath>
#include "Vec3.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"

color ray_color_background(const Ray& r) {
    auto a = 0.5 * (r.direction.y + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.1, 0.4, 0.7);
}

int main()
{
    std::vector<Sphere> spheres;

    spheres.push_back(Sphere(Point3(0, 0, -4), 1.0));
    spheres.push_back(Sphere(Point3(1, 0, 5), 2.0));

    Image image(1280, 960);

    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (float(image.width) / image.height);
    Point3 camera_origin(0, 0, 0);

    Vec3 viewport_horizontal_vector(viewport_width, 0, 0);
    Vec3 viewport_vertical_vector(0, -viewport_height, 0);

    Vec3 pixel_delta_horizontal = viewport_horizontal_vector / image.width;
    Vec3 pixel_delta_vertical = viewport_vertical_vector / image.height;

    Point3 viewport_upper_left_position = camera_origin - Vec3(0, 0, focal_length) - viewport_horizontal_vector / 2 - viewport_vertical_vector / 2;
    Point3 pixel00_loc = viewport_upper_left_position + 0.5 * (pixel_delta_horizontal + pixel_delta_vertical);


    for (int j = 0; j < image.height; j++) {
        for (int i = 0; i < image.width; i++) {
            Point3 pixel_center = pixel00_loc + (i * pixel_delta_horizontal) + (j * pixel_delta_vertical);
            Vec3 ray_direction = pixel_center - camera_origin;
            Ray r(camera_origin, ray_direction);

            float tNearest = MAX_RENDER_DISTANCE, ttemp = MAX_RENDER_DISTANCE;
            Sphere* ObjectNearest = nullptr;

            for (Sphere& object : spheres) {
                if (object.check_intersection(r, ttemp) == true) {
                    if (ttemp < tNearest) {
                        tNearest = ttemp;
                        ObjectNearest = &object;
                    }
                }
            }

            if (tNearest >= MAX_RENDER_DISTANCE) {
                image.data.push_back(ray_color_background(r));
            }
            else {
                image.data.push_back(ObjectNearest->return_color());
            }
        }
    }

    image.print_image();
}