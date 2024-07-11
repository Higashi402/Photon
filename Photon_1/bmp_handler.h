#ifndef CBMP_CBMP_H
#define CBMP_CBMP_H

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct pixel_data
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
    } pixel;

    typedef struct BMP_data
    {
        unsigned int file_byte_number;
        unsigned char* file_byte_contents;

        unsigned int pixel_array_start;
        unsigned int width;
        unsigned int height;
        unsigned int depth;
        int MO_r;
        int MO_g;
        int MO_b;

        float SKO_r;
        float SKO_g;
        float SKO_b;

        pixel* pixels;
    } BMP;

    enum PaddingType {
        REPLICATE,
        REFLECT,
        ZERO
    };

    BMP* bopen(char* file_path);
    int get_width(BMP* bmp);
    int get_height(BMP* bmp);
    unsigned int get_depth(BMP* bmp);
    void get_pixel_rgb(BMP* bmp, int x, int y, int* r, int* g, int* b);
    void get_pixel_r(BMP* bmp, int x, int y, int* r);
    void get_pixel_g(BMP* bmp, int x, int y, int* g);
    void get_pixel_b(BMP* bmp, int x, int y, int* b);
    void set_pixel_rgb(BMP* bmp, int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void bwrite(BMP* bmp, char* file_name);
    void bclose(BMP* bmp);
    void bwrite(BMP* bmp, char* file_name);
    void get_MO(BMP* bmp, char channel);
    void get_SKO(BMP* bmp, char channel);
    void get_SKO_modifyed(BMP* bmp, char channel);
    BMP* b_deep_copy(BMP* to_copy);
    //void downscale_bmp_subsampling(BMP* bmp, char* file_path, int n, char* output_file);
    //void downscale_bmp_averaging(BMP* bmp, char* file_path, int n, char* output_file);
    void downscale_bmp(BMP* bmp, char* file_path, int n, char* output_file, int method);

    void scale_bmp_nearest_neighbor(BMP* bmp, BMP* scaled_bmp, int new_width, int new_height, float inverse_matrix[2][2]);
    void scale_bmp_bilinear(BMP* bmp, BMP* scaled_bmp, int new_width, int new_height, float inverse_matrix[2][2]);
    void initialize_bmp(BMP* bmp,BMP* initial_bmp, int width, int height, int depth);
    void print_matrix(float matrix[2][2]);
    void calculate_new_dimensions(int width, int height, float angle, int& new_width, int& new_height);

    void create_inverse_scaling_matrix(float Sx, float Sy, float inverse_matrix[2][2]);
    //void create_inverse_rotating_matrix(float angle, float inverse_matrix[2][2], int width, int height);
    void create_combined_matrix(float angle, float inverse_matrix[2][2], int width, int height);


    //void rotate_bmp_nearest_neighbor(BMP* bmp, BMP* rotated_bmp, float inverse_matrix[2][2]);
    //void rotate_bmp_bilinear(BMP* bmp, BMP* rotated_bmp, float inverse_matrix[2][2]);
    void rotate_bmp_nearest_neighbor_full(BMP* bmp, BMP* rotated_bmp, float inverse_matrix[2][2], int new_width, int new_height, int threads);
    void rotate_bmp_bilinear_full(BMP* bmp, BMP* rotated_bmp, float inverse_matrix[2][2], int new_width, int new_height, int threads);

    BMP* pad_image(BMP* bmp, int k, PaddingType padding_type);
    void apply_median_filter(BMP* bmp, BMP* filtered_bmp, int k, PaddingType padding_type);
    void apply_median_filter_float(BMP* bmp, BMP* filtered_bmp, int k, PaddingType padding_type);

    void find_min_max_brightness(BMP* bmp, int* min_brightness, int* max_brightness);
    void apply_contrast(BMP* bmp, float alpha);
    void apply_sobel_filter(BMP* bmp, PaddingType padding_type, int threshold);


#ifdef __cplusplus
}
#endif

#endif  