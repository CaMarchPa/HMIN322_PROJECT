#include "functions.h"

extern float const EPSILON = 0.0001;

//**********functions implementation*************//
CImg<int> get_img_genereted_by_kmeans(CImg<int> img) {
    bool is_convergent = false;
    int height = img.height();
    int width = img.width();

    CImg<int> avg_img = img;
    CImg<int> result = img;

    int * clusters_red, * clusters_green, *clusters_blue;
    clusters_red = (int *) malloc(sizeof(int) * 255);
    clusters_green = (int *) malloc(sizeof(int) * 255);
    clusters_blue = (int *) malloc(sizeof(int) * 255);
    int * nb_of_pixel;
    int number_of_loop = 0;
    nb_of_pixel = (int *) malloc(sizeof(int) * 255);
    // initialize by zero
    for (int i = 0; i < 256; i++) {
        clusters_red[i] = 0;
        clusters_green[i] = 0;
        clusters_blue[i] = 0;
        nb_of_pixel[i] = 0;
    }


    // do {
        CImg<int> palette = get_palette(avg_img);
        CImg<int> labeled_img = get_labeled_img_from_palette(result, palette);
        // sum the value of each cluster
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                int index = labeled_img(i, j, 0);
                clusters_red[index] += result(i, j, 0, 0);
                clusters_green[index] += result(i, j, 0, 1);
                clusters_blue[index] += result(i, j, 0, 2);
                nb_of_pixel[index] ++;
            }
        }
        // compute the average of each cluster
        for (int i = 0; i < 256; i++) {
            if (nb_of_pixel[i] > 0) {
                clusters_red[i] /= nb_of_pixel[i];
                clusters_green[i] /= nb_of_pixel[i];
                clusters_blue[i] /= nb_of_pixel[i];
            }
        }
        // create image from avg palette and average value
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                int index = labeled_img(i, j, 0);
                avg_img(i, j, 0, 0) = clusters_red[index];
                avg_img(i, j, 0, 1) = clusters_green[index];
                avg_img(i, j, 0, 2) = clusters_blue[index];

                result(i, j, 0, 0) = palette(index, 0, 0, 0);
                result(i, j, 0, 1) = palette(index, 0, 0, 1);
                result(i, j, 0, 2) = palette(index, 0, 0, 2);
            }
        }

        // is_convergent = true;
        // for (int i = 0; i < 256; i++) {
        //     int color[3] = {clusters_red[i], clusters_green[i], clusters_blue[i]};
        //     int distance = 99999;
        //
        //     for (int j = 0; j < 256; j++) {
        //         int color_palette[3] = {palette(j, 0, 0, 0), palette(j, 0, 0, 1), palette(j, 0, 0, 2)};
        //         int current_dist = compute_distance(color_palette, color);
        //         distance = (current_dist < distance) ? current_dist : distance;
        //     }
        //     is_convergent = is_convergent && (distance == 0);
        //     if (is_convergent) {
                char pal_c[40] = "../img/img_01/palette.png";
                palette.save_png(pal_c);
    //         }
    //     }
    //     number_of_loop++;
    //
    //     std::cout << number_of_loop << "th loop : " << is_convergent  << '\n';
    //
    // } while(!is_convergent);

    delete clusters_red;
    delete clusters_green;
    delete clusters_blue;
    delete nb_of_pixel;

    // double psnr_a = img.PSNR(avg_img);
    // double psnr_r = img.PSNR(result);

    // char avg_c[40] = "../img/img_01/avg_img.png";
    // char res_c[40] = "../img/img_01/pal_img.png";

    // avg_img.save_png(avg_c);
    // result.save_png(res_c);

    return avg_img;
}

CImg<int> get_labeled_img_from_palette(const CImg<int> img, const CImg<int> palette) {
    int height = img.height();
    int width = img.width();

    CImg<int> result(width, height, 1, 1, 0);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int distance = 99999;
            int index = 0;
            int color[3] = {img(i, j, 0, 0), img(i, j, 0, 1), img(i, j, 0, 2)};

            for (int k = 0; k < 256; k++) {
                int color_palette[3] = { palette(k, 0, 0, 0), palette(k, 0, 0, 1), palette(k, 0, 0, 2)};
                int current_dist = compute_distance(color_palette, color);

                if (current_dist < distance) {
                    distance = current_dist;
                    index = k;
                }
            }
            // image result from palette
            result(i, j, 0, 0) = index;
        }
    }

    return result;
}

CImg<int> get_palette(CImg<int> img) {
    int height = img.height();
    int width = img.width();

    CImg<int> palette(255, 1, 1, 3, 255);
    int counter = 0;
    int stop = 0;
    while ( counter < 256 ) {
        int i = rand() % (width - 1);
        int j = rand() % (height - 1);
        int color[3] = {0, 0, 0};
		color[0] = img(i, j, 0, 0);
		color[1] = img(i, j, 0, 1);
		color[2] = img(i, j, 0, 2);
        if (!is_in_palette(palette, color)) {
            palette(counter, 0, 0, 0) = color[0];
            palette(counter, 0, 0, 1) = color[1];
            palette(counter, 0, 0, 2) = color[2];
            counter++;
        }
        stop++;
        if (stop >= height*width) {
            palette(counter, 0, 0, 0) = color[0];
            palette(counter, 0, 0, 1) = color[1];
            palette(counter, 0, 0, 2) = color[2];
            counter++;
        }
    }

    return palette;
}

bool is_in_palette(CImg<int> palette, int * color) {
    for (int i = 0; i < 256; i++) {
        bool test = (palette(i, 0, 0, 0) == color[0]) && (palette(i, 0, 0, 1) == color[1]) && (palette(i, 0, 0, 2) == color[2]);
        if (test) {
            return true;
        }
    }
    return false;
}

int compute_distance(int * color_palette, int * color) {
    int square_sum = pow(color[0] - color_palette[0], 2) + pow(color[1] - color_palette[1], 2) + pow(color[2] - color_palette[2], 2);
    return (int)sqrt(square_sum);
}
