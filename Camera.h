#pragma once
#include "Vec3.h"
#include "Image.h"

struct Camera {
    Point3 camera_origin;
    const float focal_length;
    const float viewport_size;
    Image& output;
    Vec3 pixel_delta_horizontal;
    Vec3 pixel_delta_vertical;
    Point3 pixel00_loc;

//public:
    Camera(const Point3& origin, const float& focal_length, const float& size, Image& image);

    void print_output() const;
};

//TODO: Decide how to handle rendering, whether to change this to a class or keep as struct that's a member of a renderer class
//Should probably construct the Image itself instead of storing a reference