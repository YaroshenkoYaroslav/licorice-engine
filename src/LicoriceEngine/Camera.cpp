#include <LicoriceEngine/Camera.hpp>

namespace LicEngine
{

void Camera::Render(Uint32 *const buffer, int buff_w, int buff_h,
                    const Hittable *const *const map, int map_w) {
  for (int x = 0; x < buff_w; ++x) {
    double camera_x = 2 * x / static_cast<double>(buff_w) - 1;
    double ray_direction_x = direction_x + viewing_plane_x * camera_x;
    double ray_direction_y = direction_y + viewing_plane_y * camera_x;

    int map_x = static_cast<int>(position_x);
    int map_y = static_cast<int>(position_y);

    double delta_distance_x = std::abs(1 / ray_direction_x);
    double delta_distance_y = std::abs(1 / ray_direction_y);

    double side_distance_x;
    double side_distance_y;

    int step_x;
    int step_y;

    if (ray_direction_x < 0) {
      step_x = -1;
      side_distance_x = (position_x - map_x) * delta_distance_x;
    } else {
      step_x = 1;
      side_distance_x = (map_x - position_x + 1) * delta_distance_x;
    }
    if (ray_direction_y < 0) {
      step_y = -1;
      side_distance_y = (position_y - map_y) * delta_distance_y;
    } else {
      step_y = 1;
      side_distance_y = (map_y - position_y + 1) * delta_distance_y;
    }

    bool is_hit = false; 
    int side;
    while (!is_hit) {
      if (side_distance_x < side_distance_y) {
        side_distance_x += delta_distance_x;
        map_x += step_x;
        side = 0;
      } else {
        side_distance_y += delta_distance_y;
        map_y += step_y;
        side = 1;
      }

      if (map[map_x + map_y * map_w] != nullptr) is_hit = true;
    }
    const Hittable* hittable = map[map_x + map_y * map_w];
    
    double hit_distance;
    if (side == 0) hit_distance = (side_distance_x - delta_distance_x);
    else           hit_distance = (side_distance_y - delta_distance_y);

    int line_height = static_cast<int>(buff_h / hit_distance);
    int line_y1 = std::max((buff_h - line_height) / 2, 0);
    int line_y2 = std::min((buff_h + line_height) / 2, buff_h - 1);


    double hit_position;
    if (side == 0) hit_position = position_y + hit_distance * ray_direction_y;
    else           hit_position = position_x + hit_distance * ray_direction_x;
    hit_position -= static_cast<int>(hit_position);
    int texture_x = static_cast<int>(hit_position * hittable->pixels_width);
    if(side == 0 && ray_direction_x > 0) texture_x = hittable->pixels_width - texture_x - 1;
    if(side == 1 && ray_direction_y < 0) texture_x = hittable->pixels_width - texture_x - 1;

    double texture_step = hittable->pixels_height / static_cast<double>(line_height);
    double texture_position = (line_y1 + (line_height - buff_h) / 2) * texture_step;
    for (int y = line_y1; y < line_y2; ++y) {
      int texture_y = texture_position;
      texture_position += texture_step;
      Uint32 color = hittable->pixels[hittable->pixels_width * texture_y + texture_x];
      if(side == 1) color = (color >> 1) & 0x7F7F7F7Fu;
      *(buffer + x + y * buff_w) = color;
    }
  }
}

void Camera::Rotate(double angle) {
  //   cos(angle) 	sin(angle)
  //  -sin(angle) 	cos(angle)
  double cos_angle = std::cos(angle);
  double sin_angle = std::sin(angle);
  double old_direction_x = direction_x;
  double old_viewing_plane_x = viewing_plane_x;

  direction_x     = direction_x         *  cos_angle  +  direction_y     * sin_angle;
  direction_y     = old_direction_x     * -sin_angle  +  direction_y     * cos_angle;

  viewing_plane_x = viewing_plane_x     *  cos_angle  +  viewing_plane_y * sin_angle;
  viewing_plane_y = old_viewing_plane_x * -sin_angle  +  viewing_plane_y * cos_angle;
}

}
