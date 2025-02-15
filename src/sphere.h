#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

inline void get_sphere_uv(const vec3& p, float& u, float& v) {
  float phi = atan2(p.z(), p.x());
  float theta = asin(p.y());
  u = 1 - (phi + M_PI) / (2 * M_PI);
  v = (theta + M_PI / 2) / M_PI;
}

class sphere : public Hitable {
 public:
  sphere() {}
  sphere(vec3 cen, float r, material* mat)
      : center(cen), radius(r), mat_ptr(mat){};
  virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
  virtual bool bounding_box(float t0, float t1, aabb& box) const;
  virtual float pdf_value(const vec3& origin, const vec3& direction) const;
  virtual vec3 random(const vec3& origin) const;

  vec3 center;
  float radius;
  material* mat_ptr;
};

class moving_sphere : public Hitable {
 public:
  moving_sphere() {}
  moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material* m)
      : center0(cen0),
        center1(cen1),
        time0(t0),
        time1(t1),
        radius(r),
        mat_ptr(m){};
  virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
  virtual bool bounding_box(float t0, float t1, aabb& box) const;
  vec3 center(float time) const;
  vec3 center0, center1;
  float time0, time1;
  float radius;
  material* mat_ptr;
};

#endif
