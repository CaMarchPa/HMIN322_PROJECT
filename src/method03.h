#include "functions.h"

CImg<> decomposition(CImg<> img, int nbr_of_decomp, int quality) {
    int width = img.width();
    int height = img.height();

    img.RGBtoYCbCr();
    CImg<> Y = img.get_channel(0);
    CImg<> Cb = img.get_channel(1);
    CImg<> Cr = img.get_channel(2);

    Y.save_png("../img/img_03/Y.png");
    Cb.save_png("../img/img_03/Cb.png");
    Cr.save_png("../img/img_03/Cr.png");

    CImg<> tmp_img[3] = {Y, Cb, Cr};
    CImg<> dcmp_img[3] = tmp_img;
    CImg<> quantified(height, height, 3, 1, 0);


    // matrice des Coefficients
    CImg<> coefs(height, height, 1, 1, 0);
    int step = (int) log2(height) / log2(nbr_of_decomp);

    for (int j = 0; j < step; j++) {
        for (int i = 0; i < step; i++) {
            for (int k = 0; k < 3; k++)
                coefs(i, j) = 1;
        }
    }

    int half = height;
    int height_temp;
    float score;
    float q = 50 * pow(2, nbr_of_decomp);
    for (int loop = 1; loop < nbr_of_decomp; loop++) {
        height_temp = half;
        half /= 2;

        for (int j = 0; j < height_temp; j += 2) {
            for (int i = 0; i < height_temp; i += 2) {
                for (int channel = 0; channel < 3; channel++) {
                    /***/
                    float A = tmp_img[channel](i, j);
                    float B = tmp_img[channel](i + 1, j);
                    float C = tmp_img[channel](i, j + 1);
                    float D = tmp_img[channel](i + 1, j + 1);
                    float x = (A + B) / 2;
                    float y = (C + D) / 2;
                    float K = A - B;
                    float L = C - D;
                    float I = (A + B + C + D)  / 4;
                    // top - left
                    dcmp_img[channel](i/2, j/2) = I;

                    score = 0.8 * ((float)(nbr_of_decomp + 4 - loop) / (float)(nbr_of_decomp + 4));
                    // top - right
                    dcmp_img[channel](i/2 + half, j/2) = (x - y + 128 > 128) ? 128 : x - y + 128;
                    coefs(i/2 + half, j/2) = ((float)quality / (float)100) * score + 1;

                    // bottom - left
                    dcmp_img[channel](i/2, j/2 + half) = ((K + L) / 2 + 128 > 128) ? 128 : (K + L) / 2 + 128;
                    coefs(i/2, j/2 + half) = ((float)quality / (float)100) * score + 1;

                    // bottom - right
                    dcmp_img[channel](i/2 + half, j/2 + half) = (K - L + 128 > 128) ? 128 : K - L + 128;
                    coefs(i/2 + half, j/2 + half) = ((float)quality / (float)100) * score + 1;

                    // quantified
                    quantified(i/2, j/2, channel) = dcmp_img[channel](i/2, j/2);
                    quantified(i/2 + half, j/2, channel) = dcmp_img[channel](i/2 + half, j/2) - 128 / (q/2);
                    quantified(i/2, j/2 + half, channel) = dcmp_img[channel](i/2, j/2 + half) - 128 / (q/2);
                    quantified(i/2 + half, j/2 + half, channel) = dcmp_img[channel](i/2 + half, j/2 + half) - 128 / q;
                }
            }
        }
        // swap
        for (int channel = 0; channel < 3; channel++) {
            tmp_img[channel] = dcmp_img[channel];
        }
        q /= 2;
    }

    dcmp_img[0].save_png("../img/img_03/Y_decomp.png");
    dcmp_img[1].save_png("../img/img_03/Cb_decomp.png");
    dcmp_img[2].save_png("../img/img_03/Cr_decomp.png");

    // Quantification *****************************
    CImg<> recomposition[3] = {Y, Cb, Cr};
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < width; i++) {
            if (coefs(i, j) == 0)
                coefs(i, j) = 1;
            for (int channel = 0; channel < 3; channel++) {
                recomposition[channel](i, j) = dcmp_img[channel](i, j) / coefs(i, j);
            }
        }
    }

    recomposition[0].save_png("../img/img_03/Y_decompQ.png");
    recomposition[1].save_png("../img/img_03/Cb_decompQ.png");
    recomposition[2].save_png("../img/img_03/Cr_decompQ.png");

    // Recomposition *****************************
    CImg<> rec_tmp[3] = {recomposition[0], recomposition[1], recomposition[2]};

    for (int loop = 1; loop < nbr_of_decomp; loop++) {
        height = half;

        half *= 2;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < height; i++) {
                for (int channel = 0; channel < 3; channel++) {
                    /***/
                    float A = rec_tmp[channel](i, j);
                    float B = rec_tmp[channel](i + half/2, j);
                    float C = rec_tmp[channel](i, j + half/2);
                    float D = rec_tmp[channel](i + half/2, j + half/2);
                    float x = (A + B) / 2;
                    float y = (C + D) / 2;
                    float K = A - B;
                    float L = C - D;
                    float I = (A + B + C + D)  / 4;

                    recomposition[channel](i*2, j*2) = A;
                    recomposition[channel](i*2+1, j*2) = B;
                    recomposition[channel](i*2, j*2+1) = C;
                    recomposition[channel](i*2+1, j*2+1) = D;

                    // recomposition[channel](i*2, j*2) = rec_tmp[channel](i, j);
                    // recomposition[channel](i*2+1, j*2) = (rec_tmp[channel](i, j) + rec_tmp[channel](i, j + half/2)) / 2;
                    // recomposition[channel](i*2, j*2+1) = (rec_tmp[channel](i, j) + rec_tmp[channel](i + half/2, j)) / 2;
                    // recomposition[channel](i*2+1, j*2+1) = (rec_tmp[channel](i, j) + rec_tmp[channel](i + half/2, j + half/2)) / 2;
                }
            }
        }
        for (int channel = 0; channel < 3; channel++) {
            rec_tmp[channel] = recomposition[channel];
        }
    }

    recomposition[0].save_png("../img/img_03/Y_recomp.png");
    recomposition[1].save_png("../img/img_03/Cb_recomp.png");
    recomposition[2].save_png("../img/img_03/Cr_recomp.png");

    CImg<> result(width, width, 1, 3, 0);
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < width; i++) {
            result(i, j, 0, 0) = recomposition[0](i, j);
            result(i, j, 0, 1) = recomposition[1](i, j);
            result(i, j, 0, 2) = recomposition[2](i, j);
        }
    }

    result.YCbCrtoRGB();
    return result;
}
