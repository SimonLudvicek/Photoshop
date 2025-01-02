#ifndef PHOTOSHOP_H
#define PHOTOSHOP_H

typedef unsigned char byte;

typedef struct {
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    byte width[2];
    byte height[2];
    byte depth;
    byte descriptor;
} TGAHeader;

typedef struct {
    byte blue;
    byte green;
    byte red;
} Pixel;

void validate_file_exists(const char* file_path);
void read_tga_header(const char* file_path, TGAHeader* header);
void print_tga_info(const TGAHeader* header);
Pixel* load_pixels(const TGAHeader* header, const char* file_path);
void convert_to_grayscale(Pixel* pixels, int width, int height);
Pixel* resize_image(Pixel* pixels, int old_width, int old_height, int new_width, int new_height);
Pixel* crop_image(Pixel* pixels, int old_width, int x, int y, int width, int height);
void move_pixels(Pixel* pixels, int width, int height, int dx, int dy);
void write_tga_file(const char* file_path, TGAHeader* header, Pixel* pixels);
void copy_pixels(Pixel* pixels, int width, int x1, int y1, int w, int h, int x2, int y2);

#endif
