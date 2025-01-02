#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photoshop.h"

// Barvy pro výstupy
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

void print_usage_and_exit() {
    fprintf(stderr, RED "Použití: photoshop --input <cesta_k_souboru> --output <cesta_k_souboru>\n" RESET);
    exit(1);
}

void parse_arguments(int argc, char* argv[], char** input_file, char** output_file) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0) {
            if (*input_file != NULL) {
                fprintf(stderr, RED "Chyba: Argument --input byl zadán vícekrát.\n" RESET);
                print_usage_and_exit();
            }
            if (i + 1 < argc) {
                *input_file = argv[++i];
            } else {
                fprintf(stderr, RED "Chyba: Chybí cesta k souboru po --input.\n" RESET);
                print_usage_and_exit();
            }
        } else if (strcmp(argv[i], "--output") == 0) {
            if (*output_file != NULL) {
                fprintf(stderr, RED "Chyba: Argument --output byl zadán vícekrát.\n" RESET);
                print_usage_and_exit();
            }
            if (i + 1 < argc) {
                *output_file = argv[++i];
            } else {
                fprintf(stderr, RED "Chyba: Chybí cesta k souboru po --output.\n" RESET);
                print_usage_and_exit();
            }
        } else {
            fprintf(stderr, RED "Chyba: Neznámý argument '%s'.\n" RESET, argv[i]);
            print_usage_and_exit();
        }
    }

    if (*input_file == NULL || *output_file == NULL) {
        fprintf(stderr, RED "Chyba: Chybí argument --input nebo --output.\n" RESET);
        print_usage_and_exit();
    }
}

int main(int argc, char* argv[]) {
    char* input_file = NULL;
    char* output_file = NULL;

    parse_arguments(argc, argv, &input_file, &output_file);
    validate_file_exists(input_file);

    TGAHeader header;
    read_tga_header(input_file, &header);
    print_tga_info(&header);

    Pixel* pixels = load_pixels(&header, input_file);

    int width = 0, height = 0;
    memcpy(&width, header.width, 2);
    memcpy(&height, header.height, 2);

    printf(YELLOW "Zadejte příkazy. Zadejte 'exit' pro ukončení.\n" RESET);
    char command[256];

    while (1) {
        printf(BLUE "> " RESET);
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        // Nahrazení čárek za mezery
        for (char* ptr = command; *ptr; ptr++) {
            if (*ptr == ',') *ptr = ' ';
        }

        char* cmd = strtok(command, " \n");
        if (cmd == NULL) {
            continue;
        }

        if (strcmp(cmd, "resize") == 0) {
            int new_width, new_height;
            if (sscanf(strtok(NULL, " \n"), "%d", &new_width) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &new_height) != 1) {
                fprintf(stderr, RED "Chyba: Špatné argumenty pro resize.\n" RESET);
                continue;
            }
            Pixel* resized_pixels = resize_image(pixels, width, height, new_width, new_height);
            free(pixels);
            pixels = resized_pixels;
            width = new_width;
            height = new_height;
            header.width[0] = new_width & 0xFF;
            header.width[1] = (new_width >> 8) & 0xFF;
            header.height[0] = new_height & 0xFF;
            header.height[1] = (new_height >> 8) & 0xFF;
            printf(GREEN "Obrázek změněn na velikost %d x %d.\n" RESET, new_width, new_height);

        } else if (strcmp(cmd, "crop") == 0) {
            int x, y, crop_width, crop_height;
            if (sscanf(strtok(NULL, " \n"), "%d", &x) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &y) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &crop_width) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &crop_height) != 1) {
                fprintf(stderr, RED "Chyba: Špatné argumenty pro crop.\n" RESET);
                continue;
            }
            if (x < 0 || y < 0 || x + crop_width > width || y + crop_height > height) {
                fprintf(stderr, RED "Chyba: Výřez přesahuje obrázek.\n" RESET);
                continue;
            }
            Pixel* cropped_pixels = crop_image(pixels, width, x, y, crop_width, crop_height);
            free(pixels);
            pixels = cropped_pixels;
            width = crop_width;
            height = crop_height;
            header.width[0] = crop_width & 0xFF;
            header.width[1] = (crop_width >> 8) & 0xFF;
            header.height[0] = crop_height & 0xFF;
            header.height[1] = (crop_height >> 8) & 0xFF;
            printf(GREEN "Obrázek oříznut na %d x %d.\n" RESET, crop_width, crop_height);

        } else if (strcmp(cmd, "move") == 0) {
            int dx, dy;
            if (sscanf(strtok(NULL, " \n"), "%d", &dx) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &dy) != 1) {
                fprintf(stderr, RED "Chyba: Špatné argumenty pro move.\n" RESET);
                continue;
            }
            move_pixels(pixels, width, height, dx, dy);
            printf(GREEN "Obrázek posunut o %d, %d.\n" RESET, dx, dy);

        } else if (strcmp(cmd, "copy") == 0) {
            int x1, y1, copy_width, copy_height, x2, y2;
            if (sscanf(strtok(NULL, " \n"), "%d", &x1) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &y1) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &copy_width) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &copy_height) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &x2) != 1 ||
                sscanf(strtok(NULL, " \n"), "%d", &y2) != 1) {
                fprintf(stderr, RED "Chyba: Špatné argumenty pro copy.\n" RESET);
                continue;
            }
            if (x1 < 0 || y1 < 0 || x1 + copy_width > width || y1 + copy_height > height ||
                x2 < 0 || y2 < 0 || x2 + copy_width > width || y2 + copy_height > height) {
                fprintf(stderr, RED "Chyba: Výřez nebo cíl přesahuje obrázek.\n" RESET);
                continue;
            }
            copy_pixels(pixels, width, x1, y1, copy_width, copy_height, x2, y2);
            printf(GREEN "Oblast [%d,%d,%d,%d] zkopírována na [%d,%d].\n" RESET,
                   x1, y1, copy_width, copy_height, x2, y2);

        } else if (strcmp(cmd, "bw") == 0) {
            convert_to_grayscale(pixels, width, height);
            printf(GREEN "Obrázek převeden na stupně šedi.\n" RESET);

        } else if (strcmp(cmd, "save") == 0) {
            write_tga_file(output_file, &header, pixels);
            printf(GREEN "Obrázek uložen do '%s'.\n" RESET, output_file);

        } else if (strcmp(cmd, "info") == 0) {
            printf(GREEN "Rozměry obrázku: %d x %d px.\n" RESET, width, height);

        } else if (strcmp(cmd, "exit") == 0) {
            write_tga_file(output_file, &header, pixels);
            printf(GREEN "Obrázek uložen do '%s'. Ukončuji program.\n" RESET, output_file);
            break;

        } else {
            fprintf(stderr, RED "Neznámý příkaz '%s'.\n" RESET, cmd);
        }
    }

    free(pixels);
    return 0;
}
