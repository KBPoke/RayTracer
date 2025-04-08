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


void Camera::render_scene(const std::vector<std::shared_ptr<Object>>& SceneObjectList) {
    for (int j = 0; j < output.height; j++) {
        for (int i = 0; i < output.width; i++) {
            const Point3 pixel_center = pixel00_loc + (i * pixel_delta_horizontal) + (j * pixel_delta_vertical);
            const Vec3 ray_direction = pixel_center - camera_origin;
            Ray r(camera_origin, ray_direction);

            float tNearest = MAX_RENDER_DISTANCE, ttemp = MAX_RENDER_DISTANCE;
            std::shared_ptr<const Object> ObjectNearest = nullptr;

            for (const std::shared_ptr<Object> object : SceneObjectList) {
                if (object->check_intersection(r, ttemp) == true) {
                    if (ttemp < tNearest) {
                        tNearest = ttemp;
                        ObjectNearest = object;
                    }
                }
            }

            if (tNearest >= MAX_RENDER_DISTANCE) {
                output.data.push_back(ray_color_background(r));
            }
            else {
                const Vec3 Normal_Hit = ObjectNearest->get_surface_normal(r.delta(tNearest));
                color color_of_hit = 0.5 * (0.5 * color(Normal_Hit.x + 1, Normal_Hit.y + 1, Normal_Hit.z + 1) + ObjectNearest->get_color());
                output.data.push_back(color_of_hit);
            }
        }
    }
}

void Camera::print_output() const {
	output.print_image();
}