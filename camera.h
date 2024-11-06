#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "rtweekend.h"

class camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 1000;
    int samples_per_pixel = 100;

    void render(const hittable &world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int i = 0; i < image_height; i++) {
            std::clog << "\rScanlines remaining: " << (image_height - i) << ' '
                      << std::flush;
            for (int j = 0; j < image_width; j++) {
                vec3 pixel_center =
                    pixel00_pos + (j * pixel_delta_u) + (i * pixel_delta_v);

                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(j, i);
                    pixel_color += ray_color(r, world);
                }
                pixel_color /= samples_per_pixel;
                write_color(std::cout, pixel_color);
            }
        }
        std::clog << "\rDone.                 \n";
    }

  private:
    int image_height;
    point3 center;
    point3 pixel00_pos;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_sample_scale;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        pixel_sample_scale = 1.0 / samples_per_pixel;

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width =
            viewport_height * (double(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical
        // viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to
        // pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_pos =
            viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray &r, const hittable &world) const {
        hit_record rec;

        if (world.hit(r, interval(0, infinity), rec)) {
            vec3 direction = random_on_hemisphere(rec.normal);
            return 0.5 * ray_color(ray(rec.p, direction), world);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    ray get_ray(int w, int h) {
        vec3 offset = sample_square();
        vec3 pixel_sample = pixel00_pos + (w + offset.x()) * pixel_delta_u +
                            (h + offset.y()) * pixel_delta_v;
        point3 ray_origin = center;
        point3 ray_direction = pixel_sample - ray_origin;
        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
    }
};

#endif