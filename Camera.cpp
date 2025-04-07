#include "Camera.h"

Camera::Camera(const Point3& origin, const float& focal_length, const float& size, Image& image)
: camera_origin(origin), focal_length(focal_length), viewport_size(size), output(image) {
	const Vec3 viewport_horizontal_vector(size, 0, 0);
	const Vec3 viewport_vertical_vector(0, -size * (float(image.height) / image.width), 0);

	this->pixel_delta_horizontal = viewport_horizontal_vector / image.width;
	this->pixel_delta_vertical = viewport_vertical_vector / image.height;

	Point3 viewport_upper_left_position = camera_origin + Vec3(0, 0, focal_length) - viewport_horizontal_vector / 2 - viewport_vertical_vector / 2;
	pixel00_loc = viewport_upper_left_position + 0.5 * (pixel_delta_horizontal + pixel_delta_vertical);
}

void Camera::print_output() const {
	output.print_image();
}