#include "Camera.h"

extern const float PI;

const int MAX_DEPTH = 7;

color ray_color_background(const Ray& r) {
	auto a = 0.5 * (r.direction.y + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.1, 0.4, 0.7);
}

Camera::Camera(const Point3& origin, const float& focal_length, const float& size, Image& image)
: camera_origin(origin), focal_length(focal_length), viewport_size(size), output(image), rg() {
	const Vec3 viewport_horizontal_vector(size, 0, 0);
	const Vec3 viewport_vertical_vector(0, -size * (float(image.height) / image.width), 0);

	this->pixel_delta_horizontal = viewport_horizontal_vector / image.width;
	this->pixel_delta_vertical = viewport_vertical_vector / image.height;

	Point3 viewport_upper_left_position = camera_origin + Vec3(0, 0, focal_length) - viewport_horizontal_vector / 2 - viewport_vertical_vector / 2;
	pixel00_loc = viewport_upper_left_position + 0.5 * (pixel_delta_horizontal + pixel_delta_vertical);
}

const color Normal_Based_Surface_Color(const Vec3& Normal_Hit,const std::shared_ptr<const Object>& HitObject) {
    return 0.5 * (0.5 * color(Normal_Hit.x + 1, Normal_Hit.y + 1, Normal_Hit.z + 1) + HitObject->get_color());
}

const color Camera::cast_ray(const Ray ray, const std::vector<std::shared_ptr<Object>>& SceneObjectList, const DistantLight& Light, int Depth = 0) {
    float tNearest = MAX_RENDER_DISTANCE, ttemp = MAX_RENDER_DISTANCE;
    std::shared_ptr<const Object> ObjectNearest = nullptr;

    if (Depth >= MAX_DEPTH) {
        return color(0, 0, 0);
    }

    //checking for Object hits
    for (const std::shared_ptr<const Object> object : SceneObjectList) {
        if (object->check_intersection(ray, ttemp) == true) {
            if (ttemp < tNearest && ttemp > 0.001) {
                tNearest = ttemp;
                ObjectNearest = object;
            }
        }
    }

    if (tNearest < MAX_RENDER_DISTANCE) {
        Hit_Data hit_data(ray, tNearest, ray.delta(tNearest), ObjectNearest->get_surface_normal(ray.delta(tNearest)), ObjectNearest);
        color color_of_hit(0, 0, 0);

        switch (ObjectNearest->get_type()) {
        case Diffuse:
            for (int sample = 0; sample < sample_amount; sample++) {
                const Vec3 monte_carlo_direction = hit_data.Hit_Normal + Normalized(Vec3(rg.get_random(), rg.get_random(), rg.get_random()));
                Ray ray(hit_data.Point_Hit, monte_carlo_direction);

                color_of_hit += cast_ray(ray, SceneObjectList, Light, Depth + 2) * Normal_Based_Surface_Color(hit_data.Hit_Normal, hit_data.Object_Hit)
                    * hit_data.Object_Hit->get_albedo();
            }
            color_of_hit *= pixel_sample_scale * 2 * PI ;
            break;
        case Reflective:
        {
            Vec3 Reflected = Reflect(hit_data.ray.direction, hit_data.Hit_Normal);
            color_of_hit += 0.9 * cast_ray(Ray(hit_data.Point_Hit, Reflected), SceneObjectList, Light, Depth + 1);
            break;
        }
        case Refractive:
        {
            Vec3 Refracted = Refract(hit_data.ray.direction, hit_data.Hit_Normal, 2);
            color_of_hit += 0.9 * cast_ray(Ray(hit_data.Point_Hit - 0.001 * hit_data.Hit_Normal, Refracted), SceneObjectList, Light, Depth + 1);
            break;
        }
        case Fresnel:
        {
            float reflected_light_ratio = Fresnel_reflected_ratio(hit_data.ray.direction, hit_data.Hit_Normal, 2);
            Vec3 Reflected = Reflect(hit_data.ray.direction, hit_data.Hit_Normal);
            Vec3 Refracted = Refract(hit_data.ray.direction, hit_data.Hit_Normal, 1.8);
            color_of_hit += 0.9 * reflected_light_ratio * cast_ray(Ray(hit_data.Point_Hit + 0.001 * hit_data.Hit_Normal, Reflected), SceneObjectList, Light, Depth + 1) + 
                0.9 * (1 - reflected_light_ratio) * cast_ray(Ray(hit_data.Point_Hit - 0.001 * hit_data.Hit_Normal, Refracted), SceneObjectList, Light, Depth + 1);
            break;
        }
        }
        return color_of_hit;
    }

    return Light.get_color() * Light.get_intensity() / 2 + 
        ray_color_background(ray) / 2;
}

void Camera::render_scene(const std::vector<std::shared_ptr<Object>>& SceneObjectList, const DistantLight& Light) {
    for (int j = 0; j < output.height; j++) {
        for (int i = 0; i < output.width; i++) {
            const Point3 pixel_center = pixel00_loc + (i * pixel_delta_horizontal) + (j * pixel_delta_vertical);
            const Vec3 ray_direction = pixel_center - camera_origin;

            color pixel_color = color(0,0,0);
            for (int sample = 0; sample < sample_amount; sample++) {
                const Vec3 random_offset = Vec3(rg.get_random() - 0.5, rg.get_random() - 0.5, 0);
                const Vec3 monte_carlo_direction = ray_direction + random_offset.x * pixel_delta_horizontal + random_offset.y * pixel_delta_vertical;
                Ray ray(camera_origin, monte_carlo_direction);
                
                pixel_color += cast_ray(ray, SceneObjectList, Light);
            }

            output.data.push_back(pixel_sample_scale * pixel_color);
        }
        std::cout << j << "\n";
    }
}

void Camera::print_output() const {
	output.print_image();
}