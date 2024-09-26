#include <iostream>

int main() {

	int width = 256;
	int height = 256;

	std::cout << "P3\n" << width << ' ' << height << "\n255\n";

	for (int i = 0; i < height; i++) {
		std::clog << "\rScanlines remaining: " << (height - i) << ' '
		          << std::flush;
		for (int j = 0; j < width; j++) {
			double r = double(i) / height;
			double g = 0;
			double b = double(j) / width;

			int ir = int(255.0f * r);
			int ig = int(255.0f * g);
			int ib = int(255.0f * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	std::clog << "\rDone.                 \n";
	return 0;
}