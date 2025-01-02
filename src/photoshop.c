#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photoshop.h"

void validate_file_exists(const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Chyba: Nelze otevřít soubor '%s'.\n", file_path);
        exit(1);
    }
    fclose(file);
}

void read_tga_header(const char* file_path, TGAHeader* header) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Chyba: Nelze otevřít soubor '%s'.\n", file_path);
        exit(1);
    }

    if (fread(header, sizeof(TGAHeader), 1, file) != 1) {
        fprintf(stderr, "Chyba: Nelze načíst hlavičku TGA souboru.\n");
        fclose(file);
        exit(1);
    }
    fclose(file);
}

Pixel* load_pixels(const TGAHeader* header, const char* file_path) {
    int width = 0, height = 0;
    memcpy(&width, header->width, 2);
    memcpy(&height, header->height, 2);

    size_t pixel_count = width * height;  

    Pixel* pixels = (Pixel*)malloc(sizeof(Pixel) * pixel_count);
    if (pixels == NULL) {
        fprintf(stderr, "Chyba: Nedostatek paměti pro načtení pixelů.\n");
        exit(1);
    }

    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Chyba: Nelze otevřít soubor '%s'.\n", file_path);
        free(pixels);
        exit(1);
    }

    fseek(file, sizeof(TGAHeader), SEEK_SET);

    if (fread(pixels, sizeof(Pixel), pixel_count, file) != pixel_count) {  
        fprintf(stderr, "Chyba: Selhalo načtení pixelových dat.\n");
        fclose(file);
        free(pixels);
        exit(1);
    }

    fclose(file);
    return pixels;
}

void print_tga_info(const TGAHeader* header) {
    int width = 0, height = 0;

    memcpy(&width, header->width, 2);
    memcpy(&height, header->height, 2);

    printf("Informace o TGA souboru:\n");
    printf("Typ obrázku: %d\n", header->image_type);
    printf("Šířka: %d px\n", width);
    printf("Výška: %d px\n", height);
    printf("Barevná hloubka: %d bitů\n", header->depth);
    printf("Nová šířka: %d px, Nová výška: %d px\n", header->width[0] | (header->width[1] << 8), header->height[0] | (header->height[1] << 8));

    if (header->image_type != 2 && header->image_type != 3) {
        fprintf(stderr, "Upozornění: Nepodporovaný typ obrázku (%d).\n", header->image_type);
    }
}

void convert_to_grayscale(Pixel* pixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        byte gray = (pixels[i].red + pixels[i].green + pixels[i].blue) / 3;
        pixels[i].red = gray;
        pixels[i].green = gray;
        pixels[i].blue = gray;
    }
}

Pixel* resize_image(Pixel* pixels, int old_width, int old_height, int new_width, int new_height) {
    Pixel* new_pixels = (Pixel*)malloc(sizeof(Pixel) * new_width * new_height);
    if (new_pixels == NULL) {
        fprintf(stderr, "Chyba: Nedostatek paměti pro změnu velikosti obrázku.\n");
        exit(1);
    }

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            int old_x = x * old_width / new_width;
            int old_y = y * old_height / new_height;
            new_pixels[y * new_width + x] = pixels[old_y * old_width + old_x];
        }
    }

    return new_pixels;
}

Pixel* crop_image(Pixel* pixels, int old_width, int x, int y, int width, int height) {
    Pixel* new_pixels = (Pixel*)malloc(sizeof(Pixel) * width * height);
    if (new_pixels == NULL) {
        fprintf(stderr, "Chyba: Nedostatek paměti pro výřez obrázku.\n");
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            new_pixels[i * width + j] = pixels[(y + i) * old_width + (x + j)];
        }
    }

    return new_pixels;
}

void move_pixels(Pixel* pixels, int width, int height, int dx, int dy) {
    Pixel* temp = (Pixel*)malloc(sizeof(Pixel) * width * height);
    if (temp == NULL) {
        fprintf(stderr, "Chyba: Nedostatek paměti pro posun pixelů.\n");
        exit(1);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int new_x = (x + dx + width) % width;
            int new_y = (y + dy + height) % height;
            temp[new_y * width + new_x] = pixels[y * width + x];
        }
    }

    memcpy(pixels, temp, sizeof(Pixel) * width * height);
    free(temp);
}

void write_tga_file(const char* file_path, TGAHeader* header, Pixel* pixels) {
    FILE* file = fopen(file_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "Chyba: Nelze otevřít soubor pro zápis '%s'.\n", file_path);
        exit(1);
    }

    fwrite(header, sizeof(TGAHeader), 1, file);

    int width = 0, height = 0;
    memcpy(&width, header->width, 2);
    memcpy(&height, header->height, 2);

    fwrite(pixels, sizeof(Pixel), width * height, file);
    fclose(file);
}

void copy_pixels(Pixel* pixels, int width, int x1, int y1, int w, int h, int x2, int y2) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pixels[(y2 + i) * width + (x2 + j)] = pixels[(y1 + i) * width + (x1 + j)];
        }
    }
}
