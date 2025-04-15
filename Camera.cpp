#include "Camera.h"

color ray_color_background(const Ray& r) {
	auto a = 0.5 * (r.direction.y + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.1, 0.4, 0.7);
}

Camera::Camera(const Point3& origin, const float& focal_length, const float& size, Image& image)
: camera_origin(origin), focal_length(focal_length), viewport_size(size), output(image) {
	const Vec3 viewport_horizontal_vector(size, 0, 0);
	const Vec3 viewport_vertical_vector(0, -size * (float(image.height) / image.width), 0);

	this->pixel_delta_horizontal = viewport_horizontal_vector / image.width;
	this->pixel_delta_vertical = viewport_vertical_vector / image.height;

	Point3 viewport_upper_left_position = camera_origin + Vec3(0, 0, focal_length) - viewport_horizontal_vector / 2 - viewport_vertical_vector / 2;
	pixel00_loc = viewport_upper_left_position + 0.5 * (pixel_delta_horizontal + pixel_delta_vertical);
}

const color& Normal_Based_Surface_Color(const Vec3& Normal_Hit,const std::shared_ptr<const Object>& HitObject) {
    return 0.5 * (0.5 * color(Normal_Hit.x + 1, Normal_Hit.y + 1, Normal_Hit.z + 1) + HitObject->get_color());
}

const color& determine_hit_color(Hit_Data hit_data, const std::vector<std::shared_ptr<Object>>& SceneObjectList, const DistantLight& Light) {
    float visibility_mult = 1.0;

    if (Light.check_direct_Lighting(hit_data.Point_Hit, hit_data.Hit_Normal, SceneObjectList) == false) {
        visibility_mult = 0.05;
    }

    const color ret = 5 * visibility_mult * hit_data.Object_Hit->get_albedo() * Light.get_color() 
        * std::max(0.f, Dot_Product(-Light.get_direction(), hit_data.Hit_Normal)) * Normal_Based_Surface_Color(hit_data.Hit_Normal, hit_data.Object_Hit);

    return ret;
}

void Camera::render_scene(const std::vector<std::shared_ptr<Object>>& SceneObjectList, const DistantLight& Light) {
    for (int j = 0; j < output.height; j++) {
        for (int i = 0; i < output.width; i++) {
            const Point3 pixel_center = pixel00_loc + (i * pixel_delta_horizontal) + (j * pixel_delta_vertical);
            const Vec3 ray_direction = pixel_center - camera_origin;
            Ray ray(camera_origin, ray_direction);

            float tNearest = MAX_RENDER_DISTANCE, ttemp = MAX_RENDER_DISTANCE;
            std::shared_ptr<const Object> ObjectNearest = nullptr;


            //checking for Object hits
            for (const std::shared_ptr<const Object> object : SceneObjectList) {
                if (object->check_intersection(ray, ttemp) == true) {
                    if (ttemp < tNearest) {
                        tNearest = ttemp;
                        ObjectNearest = object;
                    }
                }
            }

            if (tNearest < MAX_RENDER_DISTANCE) {
                Hit_Data hit_data(tNearest, ray.delta(tNearest), ObjectNearest->get_surface_normal(ray.delta(tNearest)), ObjectNearest);
               
                color color_of_hit = determine_hit_color(hit_data, SceneObjectList, Light);
                output.data.push_back(color_of_hit);
            }
            
            else {
                output.data.push_back(ray_color_background(ray));
            }
            
        }
    }
}

void Camera::print_output() const {
	output.print_image();
}