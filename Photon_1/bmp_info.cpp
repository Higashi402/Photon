#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <chrono>
#include <ctime>
#include <ratio>
#include <omp.h>

#include "bmp_handler.h"


using namespace std;
using namespace chrono;

void static print_bmp_info(BMP* bmp) {
    unsigned int x, y, width, height, bytes;

    width = get_width(bmp);
    height = get_height(bmp);
    std::cout << "Width: " << width << "\n";
    std::cout << "Height: " << height << "\n";
    std::cout << "Size: " << bmp->file_byte_number << " bytes" << "\n";
    std::cout << "Depth: " << bmp->depth << " bits" << "\n";
    std::cout << "Amount of chanels: " << bmp->depth / (sizeof(unsigned char) * 8) << "\n"; 
        
    int r, g, b;
    get_pixel_rgb(bmp, 0, 0, &r, &g, &b);
    std::cout << "Bottom-left pixel: " << r << " : " << g << " : " << b << "\n";

    get_pixel_rgb(bmp, width - 1, 0, &r, &g, &b);
    std::cout << "Bottom-right pixel: " << r << " : " << g << " : " << b << "\n";

    get_pixel_rgb(bmp, width - 1, height - 1, &r, &g, &b);
    std::cout << "Top-right pixel: " << r << " : " << g << " : " << b << "\n";

    get_pixel_rgb(bmp, 0, height - 1, &r, &g, &b);
    std::cout << "Top-left pixel: " << r << " : " << g << " : " << b << "\n";

    get_pixel_rgb(bmp, std::floor(width / 2), std::floor(height / 2), &r, &g, &b);
    std::cout << "Central pixel: " << r << " : " << g << " : " << b << "\n \n";
}
void static invert_bmp(BMP* bmp, const char* path, std::string filename) {
    unsigned int x, y, width, height;
    int r, g, b;
    width = get_width(bmp);
    height = get_height(bmp);


    char output_file_path[256];
    strcpy(output_file_path, path);
    strcat(output_file_path, filename.c_str());
    strcat(output_file_path, "_inverted.bmp");

    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            get_pixel_rgb(bmp, x, y, &r, &g, &b);
            set_pixel_rgb(bmp, x, y, 255 - r, 255 - g, 255 - b);
        }
    }

    bwrite(bmp, output_file_path);
}

void static get_MO_and_SKO(BMP* bmp) {
    std::cout.precision(8);
    auto first_method_start = high_resolution_clock::now();
    auto start = high_resolution_clock::now();
    get_MO(bmp, 'r');
    auto stop = high_resolution_clock::now();
    auto duration_MO_r = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    get_MO(bmp, 'g');
    stop = high_resolution_clock::now();
    auto duration_MO_g = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    get_MO(bmp, 'b');
    stop = high_resolution_clock::now();
    auto duration_MO_b = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    get_SKO(bmp, 'r');
    stop = high_resolution_clock::now();
    auto duration_SKO_r = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    get_SKO(bmp, 'g');
    stop = high_resolution_clock::now();
    auto duration_SKO_g = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    get_SKO(bmp, 'b');
    stop = high_resolution_clock::now();
    auto duration_SKO_b = duration_cast<microseconds>(stop - start);

    std::cout << "MO_r = " << round(bmp->MO_r * 1000000) / 1000000 << "\n";
    std::cout << "MO_g = " << round(bmp->MO_g * 1000000) / 1000000 << "\n";
    std::cout << "MO_b = " << round(bmp->MO_b * 1000000) / 1000000 << "\n\n";
    cout << "MO_r_time (ms): " << (double)(duration_MO_r.count() / 1000.0) << "\n";
    cout << "MO_g_time (ms): " << (double)(duration_MO_g.count() / 1000.0) << "\n";
    cout << "MO_b_time (ms): " << (double)(duration_MO_b.count() / 1000.0) << "\n\n";

    std::cout << "SKO_r = " << round(bmp->SKO_r * 1000000) / 1000000 << "\n";
    std::cout << "SKO_g = " << round(bmp->SKO_g * 1000000) / 1000000 << "\n";
    std::cout << "SKO_b = " << round(bmp->SKO_b * 1000000) / 1000000 << "\n\n";
    cout << "SKO_r_time (ms): " << (double)(duration_SKO_r.count() / 1000.0) << "\n";
    cout << "SKO_g_time (ms): " << (double)(duration_SKO_g.count() / 1000.0) << "\n";
    cout << "SKO_b_time (ms): " << (double)(duration_SKO_b.count() / 1000.0) << "\n";

    auto first_method_stop = high_resolution_clock::now();
    auto duration_first_method = duration_cast<microseconds>(first_method_stop - first_method_start);
    cout << "First method time (ms): " << (double)(duration_first_method.count() / 1000.0) << "\n\n";

    auto second_method_start = high_resolution_clock::now();
    start = high_resolution_clock::now();
    get_SKO_modifyed(bmp, 'r');
    stop = high_resolution_clock::now();
    auto duration_SKO_modifyed_r = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    get_SKO_modifyed(bmp, 'g');
    stop = high_resolution_clock::now();
    auto duration_SKO_modifyed_g = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    get_SKO_modifyed(bmp, 'b');
    stop = high_resolution_clock::now();
    auto duration_SKO_modifyed_b = duration_cast<microseconds>(stop - start);

    std::cout << "SKO_modifyed_r = " << round(bmp->SKO_r * 1000000) / 1000000 << "\n";
    std::cout << "SKO_modifyed_g = " << round(bmp->SKO_g * 1000000) / 1000000 << "\n";
    std::cout << "SKO_modifyed_b = " << round(bmp->SKO_b * 1000000) / 1000000 << "\n\n";
    cout << "SKO_modifyed_r_time (ms): " << (double)(duration_SKO_r.count() / 1000.0) << "\n";
    cout << "SKO_modifyed_g_time (ms): " << (double)(duration_SKO_g.count() / 1000.0) << "\n";
    cout << "SKO_modifyed_b_time (ms): " << (double)(duration_SKO_b.count() / 1000.0) << "\n";

    auto second_method_stop = high_resolution_clock::now();
    auto duration_second_method = duration_cast<microseconds>(second_method_stop - second_method_start);
    cout << "Second method time (ms): " << (double)(duration_second_method.count() / 1000.0) << "\n\n";
}
