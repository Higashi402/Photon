#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include "bmp_handler.h"
#include <chrono>
#include <ctime>
#include <ratio>

#include <omp.h>

using namespace std;
using namespace chrono;

void static affine_operations(BMP* bmp, char* input_file, char* output_prefix, const char* path) {
    std::cout << "Print new width: ";
    int new_width;
    std::cin >> new_width;
    std::cout << "Print new height: ";
    int new_height;
    std::cin >> new_height;

    float Sx = static_cast<float>(new_width) / bmp->width;
    float Sy = static_cast<float>(new_height) / bmp->height;

    float inverse_matrix_scaling[2][2];

    create_inverse_scaling_matrix(Sx, Sy, inverse_matrix_scaling);

    cout << "Inverse Affine Transformation Matrix:" << endl;
    print_matrix(inverse_matrix_scaling);

    BMP* scaled_bmp_nn = (BMP*)malloc(sizeof(BMP));
    initialize_bmp(scaled_bmp_nn, bmp, new_width, new_height, bmp->depth);

    BMP* scaled_bmp_bln = (BMP*)malloc(sizeof(BMP));
    initialize_bmp(scaled_bmp_bln, bmp, new_width, new_height, bmp->depth);

    auto start_nn = high_resolution_clock::now();
    scale_bmp_nearest_neighbor(bmp, scaled_bmp_nn, new_width, new_height, inverse_matrix_scaling);
    auto end_nn = high_resolution_clock::now();
    duration<double> duration_nn = duration_cast<duration<double>>(end_nn - start_nn);
    cout << "Nearest neighbor scaling time: " << duration_nn.count() << " seconds" << endl;

    auto start_bln = high_resolution_clock::now();
    scale_bmp_bilinear(bmp, scaled_bmp_bln, new_width, new_height, inverse_matrix_scaling);
    auto end_bln = high_resolution_clock::now();
    duration<double> duration_bln = duration_cast<duration<double>>(end_bln - start_bln);
    cout << "Bilinear interpolation scaling time: " << duration_bln.count() << " seconds" << endl;

    char output_file_nn[256];
    snprintf(output_file_nn, sizeof(output_file_nn), "%s_scale_nn.bmp", output_prefix);
    bwrite(scaled_bmp_nn, output_file_nn);

    char output_file_bln[256];
    snprintf(output_file_bln, sizeof(output_file_bln), "%s_scale_bln.bmp", output_prefix);
    bwrite(scaled_bmp_bln, output_file_bln);

    bclose(scaled_bmp_nn);
    bclose(scaled_bmp_bln);


    cout << "========================================" << endl;


    std::cout << "Print the angle of rotating: ";
    int angle;
    std::cin >> angle;

    int threads;

    while (true) {
        string answer;
        std::cout << "Print the amount of threads: ";
        std::cin >> answer;

        if (answer == "all") {
            threads = omp_get_max_threads();
            break;
        }

        if (stoi(answer) <= 0 || stoi(answer) > 12) {
            std::cout << "Wrong amount of threads! (Max = 12)" << std::endl;
            continue;
        }

        threads = stoi(answer);
        break;
    }

    int new_width_rotating, new_height_rotating;
    calculate_new_dimensions(bmp->width, bmp->height, angle, new_width_rotating, new_height_rotating);

    float inverse_matrix_rotating[2][2];
    //create_inverse_rotating_matrix(angle, inverse_matrix_rotating,bmp->width,bmp->height);
    create_combined_matrix(angle, inverse_matrix_rotating, bmp->width, bmp->height);
    cout << "Inverse Affine Transformation Matrix:" << endl;
    print_matrix(inverse_matrix_rotating);

    BMP* rotated_bmp_nn = (BMP*)malloc(sizeof(BMP));
    //initialize_bmp(rotated_bmp_nn, bmp, bmp->width, bmp->height, bmp->depth);

    BMP* rotated_bmp_bln = (BMP*)malloc(sizeof(BMP));
    //initialize_bmp(rotated_bmp_bln, bmp, bmp->width, bmp->height, bmp->depth);
    initialize_bmp(rotated_bmp_nn, bmp, new_width_rotating, new_height_rotating, bmp->depth);
    initialize_bmp(rotated_bmp_bln, bmp, new_width_rotating, new_height_rotating, bmp->depth);

    auto rotate_start_nn = high_resolution_clock::now();
    //rotate_bmp_nearest_neighbor(bmp, rotated_bmp_nn, inverse_matrix_rotating);
    rotate_bmp_nearest_neighbor_full(bmp, rotated_bmp_nn, inverse_matrix_rotating, new_width_rotating, new_height_rotating, threads);
    auto rotate_end_nn = high_resolution_clock::now();
    duration<double> rotate_duration_nn = duration_cast<duration<double>>(rotate_end_nn - rotate_start_nn);
    cout << "Nearest neighbor scaling time: " << rotate_duration_nn.count() << " seconds" << endl;

    auto rotate_start_bln = high_resolution_clock::now();
    //rotate_bmp_bilinear(bmp, rotated_bmp_bln, inverse_matrix_rotating);
    rotate_bmp_bilinear_full(bmp, rotated_bmp_bln, inverse_matrix_rotating, new_width_rotating, new_height_rotating, threads);
    auto rotate_end_bln = high_resolution_clock::now();
    duration<double> rotate_duration_bln = duration_cast<duration<double>>(rotate_end_bln - rotate_start_bln);
    cout << "Bilinear interpolation scaling time: " << rotate_duration_bln.count() << " seconds" << endl;

    char output_file_rotate_nn[256];
    snprintf(output_file_rotate_nn, sizeof(output_file_rotate_nn), "%s_rotate_nn.bmp", output_prefix);
    bwrite(rotated_bmp_nn, output_file_rotate_nn);

    char output_file_rotate_bln[256];
    snprintf(output_file_rotate_bln, sizeof(output_file_rotate_bln), "%s_rotate_bln.bmp", output_prefix);
    bwrite(rotated_bmp_bln, output_file_rotate_bln);

    bclose(rotated_bmp_nn);
    bclose(rotated_bmp_bln);


    cout << "========================================" << endl;
}