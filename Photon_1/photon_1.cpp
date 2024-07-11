#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <chrono>
#include <ctime>
#include <ratio>
#include <omp.h>

#include "bmp_handler.h"
#include "affine.cpp"
#include "bmp_info.cpp"

using namespace std;
using namespace chrono;


int main(int argc, char** argv) {
    const char* path = "C:\\Users\\Higashi\\Desktop\\Фотон\\Images\\";
    while (true) {
        /*
                std::cout << "Print value of downscaling: ";
                int downscale_value;
                std::cin >> downscale_value;

                int method;
                while (true) {
                    std::cout << "Print method (1 - subsampling; 2 - averaging): ";
                    std::cin >> method;
                    if (method == 1) {
                        filename_downscaled.append("_").append(to_string(downscale_value)).append("_subsampling.bmp");
                        break;
                    }
                    else if (method == 2) {
                        filename_downscaled.append("_").append(to_string(downscale_value)).append("_averaging.bmp");
                        break;
                    }
                    else {
                        std::cout << "Print correct answer!";
                        continue;
                    }
                }

                strcpy(file_path_downscaled, path);
                strcat(file_path_downscaled, filename_downscaled.c_str());

                if (method == 1) {
                    auto start = high_resolution_clock::now();
                    downscale_bmp(bmp,file_path,downscale_value, file_path_downscaled,method);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(stop - start);
                    cout << "Scaling (Subsampling) time (ms): " << duration.count() / 1000.0 << "\n";
                }
                else {
                    auto start = high_resolution_clock::now();
                    downscale_bmp(bmp, file_path, downscale_value, file_path_downscaled, method);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(stop - start);
                    cout << "Scaling (Averaging) time (ms): " << duration.count() / 1000.0 << "\n";
                }
       */
        std::cout << "Print file name: ";
        std::string filename;
        std::cin >> filename;

        std::cout << "Print output file name: ";
        std::string output_filename;
        std::cin >> output_filename;


        char file_path[256];
        char output_file_path[256];
        char output_file[256];
        strcpy(file_path, path);
        strcat(file_path, filename.c_str());
        strcpy(output_file_path, path);
        strcat(output_file_path, output_filename.c_str());
        std::cout << "Full file path: " << file_path << std::endl;

        char* input_file = const_cast<char*>(file_path);
        char* output_prefix = const_cast<char*>(output_file_path);

        BMP* bmp = bopen(input_file);
        if (!bmp) {
            cout << "Failed to open BMP file." << endl;
            return 1;
        }

        //Получение информации о bmp файле --> bmp_pixel_info.cpp
        //print_bmp_info(bmp);

        //Получение значений математического ожидания и среднеквадратичного отклонения
        //get_MO_and_SKO(bmp);

        //Инвертирование bmp файла --> bmp_pixel_info.cpp
        //invert_bmp(bmp, path, output_filename);

        //Аффинные преобразования --> affine.cpp
        //affine_operations(bmp, input_file, output_prefix, path);

        //Контрастирование
        apply_contrast(bmp, 0.5);

        //apply_sobel_filter(bmp, ZERO, 280);

        //Срединная фильтрация
        int filter_size;
        while (true) {
            std::cout << "Print size of filter:  ";
            std::cin >> filter_size;

            if (filter_size % 2 != 1 || filter_size <= 0) {
                std::cout << "Unacceptable size of filter! Only odd and positive!" << std::endl;
                continue;
            }
            break;
        }

        BMP* filtered_bmp = (BMP*)malloc(sizeof(BMP));
        initialize_bmp(filtered_bmp, bmp, bmp->width, bmp->height, bmp->depth);

        omp_set_num_threads(12);
   


        // Apply median filter with REPLICATE padding
        auto start = high_resolution_clock::now();
        apply_median_filter(bmp, filtered_bmp, filter_size, REPLICATE);
        auto end = high_resolution_clock::now();
        duration<double> duration = end - start;
        cout << "REPLICATE Filter Time: " << duration.count() << " seconds" << endl;
        strcpy(output_file, output_file_path);
        strcat(output_file, "_replicate.bmp");
        bwrite(filtered_bmp, output_file);

        // Apply median filter with REFLECT padding
        start = high_resolution_clock::now();
        apply_median_filter(bmp, filtered_bmp, filter_size, REFLECT);
        end = high_resolution_clock::now();
        duration = end - start;
        cout << "REFLECT Filter Time: " << duration.count() << " seconds" << endl;
        strcpy(output_file, output_file_path);
        strcat(output_file, "_reflect.bmp");
        bwrite(filtered_bmp, output_file);

        // Apply median filter with ZERO padding
        start = high_resolution_clock::now();
        apply_median_filter(bmp, filtered_bmp, filter_size, ZERO);
        end = high_resolution_clock::now();
        duration = end - start;
        cout << "ZERO Filter Time: " << duration.count() << " seconds" << endl;
        strcpy(output_file, output_file_path);
        strcat(output_file, "_zero.bmp");
        bwrite(filtered_bmp, output_file);

        // Apply median filter with REPLICATE padding (float)
        start = high_resolution_clock::now();
        apply_median_filter_float(bmp, filtered_bmp, filter_size, REPLICATE);
        end = high_resolution_clock::now();
        duration = end - start;
        cout << "REPLICATE Filter Time (float): " << duration.count() << " seconds" << endl;
        strcpy(output_file, output_file_path);
        strcat(output_file, "_replicate_float.bmp");
        bwrite(filtered_bmp, output_file);

        bclose(bmp);
        bclose(filtered_bmp);
    }
    return 0;
}