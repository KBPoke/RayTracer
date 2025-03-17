#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "helper_functions.cpp"
#include "Vec3.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"


color ray_color(const Ray& r) {
    Vec3 unit_direction = Normalized(r.get_direction());
    auto a = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.5, 0.3);
}

int main()
{
    Image image(640, 480);

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
        std::cout << "\rScanlines remaining: " << (image.height - j) << ' ' << std::flush;
        for (int i = 0; i < image.width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_horizontal) + (j * pixel_delta_vertical);
            auto ray_direction = pixel_center - camera_origin;
            Ray r(camera_origin, ray_direction);

            image.data.push_back(ray_color(r));
        }
    }

    image.print_image();
}