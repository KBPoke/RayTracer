#pragma once
#include <algorithm>
#include "helper_functions.h"
#include "Vec3.h"
#include "Image.h"
#include "Ray.h"
#include "Object.h"
#include "DistantLight.h"
#include "Hit_Data.h"
#include "PRNG.h"
#include "ThreadPool.h"

class Camera {
    Point3 camera_origin;
    const float focal_length;
    const float viewport_size;
    Image& output;
    Vec3 pixel_delta_horizontal;
    Vec3 pixel_delta_vertical;
    Point3 pixel00_loc;
    mutable PRNG rg;
    const int sample_amount = 12;
    const float pixel_sample_scale = 1.0 / sample_amount;
    std::vector<std::shared_ptr<Object>> SceneObjectList; 
    DistantLight Light;


    const color render_pixel(int x, int y) const;

    const color cast_ray(const Ray ray, int Depth = 0) const;

    class Renderer {
    private:
        Camera& camera;
        
        void render_scanline(int y, int width) {
            for (int x = 0; x < width; ++x) {
                color color = camera.render_pixel(x, y);
                camera.set_pixel(x, y, color);
            }
        }

    public:
        Renderer(Camera& cam) : camera(cam) {}

        void render_image() {
            const int height = camera.get_height();
            const int width = camera.get_width();

            ThreadPool pool(std::thread::hardware_concurrency());

            std::vector<std::future<void>> futures;
            for (int y = 0; y < height; ++y) {
                futures.push_back(pool.add_task([this, y, width]() {
                    render_scanline(y, width);
                    }));
            }

            for (auto& future : futures) {
                future.get();
            }
        }
    };

public:
    Camera(const Point3& origin, const float& focal_length, const float& size, Image& image);

    void render_scene_single_thread();

    void render_scene();

    void move_scene(std::vector<std::shared_ptr<Object>>& SceneObjectList, DistantLight& Light) {
        this->SceneObjectList = std::move(SceneObjectList);
        this->Light = std::move(Light);
    }

    void print_output() const;

    int get_height() const {
        return output.height;
    }
    
    int get_width() const {
        return output.width;
    }

    void set_pixel(int x, int y, const color& col) {
        output.data[output.width * y + x] = col;
    }
};