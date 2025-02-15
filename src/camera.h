#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(), drand48(), 0); - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
    public:
        camera(vec3 lookfrom, vec3 lookat, vec3 vup,
               float vfov, float aspect, float aperture, float focus_dist,
               float t0, float t1) : time0(t0) , time1(t1) {
            // vfov is top to bottom in degrees
            lens_radius = aperture / 2;
            float theta = vfov * M_PI / 180;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            horizontal = 2 * focus_dist * half_width * u;
            vertical = 2 * focus_dist * half_height * v;
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
        }
        Ray get_ray(float s, float t) {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            float time = time0 + drand48() * (time1 - time0);
            return Ray(origin + offset,
                       lower_left_corner + s*horizontal + t*vertical - origin - offset,
                       time);
        }
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float time0, time1; // shutter open/close time
        float lens_radius;
};
#endif
