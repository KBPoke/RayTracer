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
    std::shared_ptr<Plane> plane = std::make_shared<Plane>(Vec3(0,0,0),Vec3(0,-1,0),color(0.1,1,0), Fresnel);

    //TODO: Create class for storing the Objects instead of passing around the raw vector
    std::vector<std::shared_ptr<Object>> SceneObjects;
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(1, 0, 1), 0.5, color(0, 1, 1)));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(-0.1, 0.1, 2), 1));
    SceneObjects.push_back(plane);
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(-0.1, 1.9, 2), 0.5, color(0, 0, 0), Refractive));
    SceneObjects.push_back(std::make_shared<Sphere>(Point3(-10, 5, 10), 4, color(1, 0.7, 1)));

    DistantLight Light(Vec3(-2, -2, 1), color(1.0,1.0,1.0), 10);

    Image image(400, 300);

    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (float(image.width) / image.height);
    Point3 camera_origin(0, 1, -0.5);

    Camera camera(camera_origin, focal_length, viewport_width, image);

    camera.render_scene(SceneObjects, Light);

    camera.print_output();
}