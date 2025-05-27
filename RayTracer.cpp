#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Vec3.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"
#include "DistantLight.h"

int main()
{
    std::shared_ptr<Plane> plane = std::make_shared<Plane>(Vec3(0,0,0),Vec3(0,-1,0),color(0.1,0.7,0), Diffuse);

    //TODO: Create class for storing the Objects instead of passing around the raw vector
    std::vector<std::shared_ptr<Object>> SceneObjects;
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(2, 2, 10), 2, color(0, 1, 1), Refractive));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(4, 2, 6), 2, color(), Reflective));
    SceneObjects.push_back(plane);
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(-0.5, 0.5, 4), 0.5, color(0, 1, 1)));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(-2, 0.5, 7), 0.5, color(1, 0.7, 1)));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(2, 0.5, 8), 0.5, color(1, 0.7, 1)));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(0, 0.5, 5), 0.5, color(), Fresnel));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(0, 0.5, 8), 0.5, color(), Reflective));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(3, 0.5, 14), 0.5, color(0.8,0,0.4)));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(1.3, 0.5, 2.5), 0.5, color(1, 0.3, 0)));

    DistantLight Light(Vec3(-2, -2, 1), color(1.0, 1.0, 1.0), 0.7);

    Image image(1280, 960);

    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (float(image.width) / image.height);
    Point3 camera_origin(0, 2.5, -0.5);

    Camera camera(camera_origin, focal_length, viewport_width, image);

    camera.render_scene(SceneObjects, Light);

    camera.print_output();
}