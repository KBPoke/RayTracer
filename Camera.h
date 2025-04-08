#pragma once
#include "helper_functions.h"
#include "Vec3.h"
#include "Image.h"
#include "Ray.h"
#include "Object.h"

class Camera {
    Point3 camera_origin;
    const float focal_length;
    const float viewport_size;
    Image& output;
    Vec3 pixel_delta_horizontal;
    Vec3 pixel_delta_vertical;
    Point3 pixel00_loc;

public:
    Camera(const Point3& origin, const float& focal_length, const float& size, Image& image);

    void render_scene(const std::vector<std::shared_ptr<Object>>& SceneObjectList);

    void print_output() const;
};