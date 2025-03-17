#pragma once
#include <vector>
#include <algorithm>
#include <fstream>

struct Image {
	const int width, height;
	std::vector<color> data;

    Image(const int width_, const int height_) : width(width_), height(height_) {
        data.reserve(width * height);
    }

    void print_image() const{
        std::ofstream file("output.ppm", std::ios::out | std::ios::binary);

        file << "P6\n" << width << " " << height << "\n255\n";

        for (int i = 0; i < width * height; i++) {
            file << (char)(std::min(1.0f, data[i].x) * 255) <<
                (char)(std::min(1.0f, data[i].y) * 255) <<
                (char)(std::min(1.0f, data[i].z) * 255);
        }
        file.close();
    }
};