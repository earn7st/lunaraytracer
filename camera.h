#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "rtweekend.h"

class camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 1000;
    int samples_per_pixel = 100;
    int max_depth = 10;

    double yfov = 90;
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat   = point3(0, 0, -1);
    point3 up       = vec3(0, 1, 0);

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
                    pixel_color += ray_color(r, world, max_depth);
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
    vec3 u, v, w;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        pixel_sample_scale = 1.0 / samples_per_pixel;

        // Determine viewport dimensions.
        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(yfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width =
            viewport_height * (double(image_width) / image_height);

        // viewport dimensions
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(up, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical
        // viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to
        // pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left =
            center - focal_length * w - viewport_u / 2 - viewport_v / 2;
        pixel00_pos =
            viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray &r, const hittable &world, int depth) const {

        if (depth < 0)
            return color(0, 0, 0);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, world, --depth);
            }
            return color(0, 0, 0);
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