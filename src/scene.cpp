#include "scene.h"

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "smartpointerhelp.h"
#include "stb_image.h"

void final_scene(Scene *scene) {
  int l = 0;
  Hitable **list = new Hitable *[30];
  // Ground
  int b = 0;
  int nb = 20;
  std::vector<sPtr<Hitable>> boxlist(nb * nb);

  material *ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < nb; j++) {
      float w = 100;
      float x0 = -1000 + i * w;
      float z0 = -1000 + j * w;
      float y0 = 0;
      float x1 = x0 + w;
      float y1 = 100 * (drand48() + 0.01);
      float z1 = z0 + w;
      boxlist[i * nb + j] = mkS<box>(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
    }
  }
  list[l++] = new BVH(boxlist, 0, 1, SplitMethod::EqualCounts);

  // Top light
  material *light = new diffuse_light(new constant_texture(vec3(7.f)));
  scene->light = new xz_rect(123, 423, 147, 412, 554, light);
  list[l++] = new flip_normals(scene->light);

  // Foam Box
  int ns = 1000;
  std::vector<sPtr<Hitable>> boxlist2(ns);
  material *white = new lambertian(new constant_texture(vec3(0.73f)));
  for (int i = 0; i < ns; i++) {
    boxlist2[i] = mkS<sphere>(vec3(165 * drand48(), 165 * drand48(), 165 * drand48()), 10, white);
  }
  list[l++] =
      new translate(new rotate_y(new BVH(boxlist2, 0.0, 1.0, SplitMethod::EqualCounts), 15), vec3(-100, 270, 395));

  // Moving sphere
  vec3 center(400, 400, 200);
  list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50,
                                new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));

  // Glass
  list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));

  // Metal
  list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));

  // Constant medium
  Hitable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
  list[l++] = boundary;
  list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));
  boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
  list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));

  // Noise
  texture *pertext = new noise_texture(0.1);
  list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));
  scene->world = new hitable_list(list, l);
}

Hitable *cornell_smoke() {
  Hitable **list = new Hitable *[8];
  int i = 0;
  material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
  material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
  material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
  material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
  list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
  list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
  list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
  list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
  list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
  list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
  Hitable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
                              vec3(130, 0, 65));
  Hitable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
                              vec3(265, 0, 295));
  list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
  list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));
  return new hitable_list(list, i);
}

Hitable *simple_light() {
  texture *pertext = new noise_texture(4);
  Hitable **list = new Hitable *[3];
  list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
  list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
  // list[2] = new sphere(vec3(0, 7, 0), 2,
  // new diffuse_light( new constant_texture(vec3(4, 4, 4))));
  list[2] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
  return new hitable_list(list, 3);
}

void cornell_box(Scene *scene) {
  Hitable **list = new Hitable *[8];
  int i = 0;
  material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
  material *white = new lambertian(new constant_texture(vec3(0.73f)));
  material *green = new lambertian(new constant_texture(vec3(0.12, 0.40, 0.15)));
  material *lightMat = new diffuse_light(new constant_texture(vec3(7.f)));
  list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
  list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
  Hitable *light_rect = new xz_rect(163, 393, 177, 382, 554, lightMat);
  list[i++] = new flip_normals(light_rect);
  scene->light = light_rect;
  list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
  list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
  list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
  list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
                            vec3(130, 0, 65));
  // material *alum = new metal(vec3(0.8, 0.85, 0.88), 0.0);
  list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
                            vec3(265, 0, 295));

  scene->world = new hitable_list(list, i);
}

Hitable *cornell_ball() {
  Hitable **list = new Hitable *[8];
  int i = 0;
  material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
  material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
  material *green = new lambertian(new constant_texture(vec3(0.12, 0.40, 0.15)));
  material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
  list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
  list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
  list[i++] = new xz_rect(163, 393, 177, 382, 554, light);
  list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
  list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
  list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
  list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
                            vec3(265, 0, 295));
  list[i++] = new sphere(vec3(190, 90, 190), 90, new dielectric(1.5));
  return new hitable_list(list, i);
}

Hitable *two_perlin_spheres() {
  texture *pertext = new noise_texture(4);
  Hitable **list = new Hitable *[2];
  list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
  list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
  return new hitable_list(list, 2);
}

Hitable *earth() {
  int nx, ny, nn;
  // unsigned char *tex_data = stbi_load("tiled.jpg", &nx, &ny, &nn, 0);
  // unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
  unsigned char *tex_data = stbi_load("earthmap2.png", &nx, &ny, &nn, 0);
  material *mat = new lambertian(new image_texture(tex_data, nx, ny));
  return new sphere(vec3(0, 0, 0), 2, mat);
}

Hitable *random_scene() {
  int n = 200;
  Hitable **list = new Hitable *[n + 1];
  texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)),
                                         new constant_texture(vec3(0.9, 0.9, 0.9)));
  list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
  int i = 1;
  for (int a = -10; a < 10; a++) {
    for (int b = -10; b < 10; b++) {
      float choose_mat = drand48();
      vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
      if ((center - vec3(4, 0.2, 0)).length() <= 0.9) {
        continue;
      }
      if (choose_mat < 0.8) {
        material *mat = new lambertian(new constant_texture(
            vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
        list[i++] =
            new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2, mat);
      } else if (choose_mat < 0.95) {
        list[i++] = new sphere(
            center, 0.2,
            new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())),
                      0.5 * drand48()));
      } else {
        list[i++] = new sphere(center, 0.2, new dielectric(1.5));
      }
    }
  }
  material *mat = new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1)));
  list[i++] = new sphere(vec3(-4, 1, 0), 1.0, mat);
  list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
  list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
  return new hitable_list(list, i);
}

Scene::Scene() : world(), light() {
  // cornell_box(this);
  final_scene(this);
}
