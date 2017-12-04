#include "functions.h"

CImg<> RGB_space(CImg<> img) {
    int width = img.width();
    int height = img.height();

    int half_width = width / 2;
    int half_height = height / 2;

    CImg<> red = img.get_channel(0);
    CImg<> green = img.get_channel(1);
    CImg<> blue = img.get_channel(2);

    CImg<> red_half(half_width, half_height, 1, 1);
    CImg<> blue_half(half_width, half_height, 1, 1);

    red.save_png("../img/img_02/red.png");
    green.save_png("../img/img_02/green.png");
    blue.save_png("../img/img_02/blue.png");

    for (int j = 0; j < half_height; j++) {
        for (int i = 0; i < half_width; i++) {
            red_half(i, j) = red(i*2, j*2);
            blue_half(i, j) = blue(i*2, j*2);
        }
    }

    int line;
    int col;
    float t;
    float u;
    float tmp;
    float d1, d2, d3, d4;
    float p1, p2, p3, p4; /* nearby pixels */

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            tmp = (float) j / (float) (height - 1) * (half_height - 1);
            line = (int) floor(tmp);

            if (line < 0)
                line = 0;
            else
                if (line >= half_height - 1)
                    line = half_height - 2;
            u = tmp - line;
            tmp = (float) (i) / (float) (width - 1) * (half_width - 1);
            col = (int) floor(tmp);
            if ( col < 0)
                col = 0;
            else
                if (col >= half_width - 1)
                    col = half_width - 2;
            t = tmp - col;

            /* Coefficients */
            d1 = (1 - t) * (1 - u);
            d2 = t * (1 - u);
            d3 = t * u;
            d4 = (1 - t) * u;

            /*neighborhood pixels*/
            p1 = red_half(col, line, 0);
            p2 = red_half(col + 1, line, 0);
            p3 = red_half(col + 1, (line + 1), 0);
            p4 = red_half(col, (line + 1), 0);
            red(i, j) = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;

            /*neighborhood pixels*/
            p1 = blue_half(col, line, 0);
            p2 = blue_half(col + 1, line, 0);
            p3 = blue_half(col + 1, (line + 1), 0);
            p4 = blue_half(col, (line + 1), 0);
            blue(i, j) = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;
        }
    }

    CImg<> result(width, height, 1, 3);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            result(i, j, 0, 0) = red(i, j);
            result(i, j, 0, 1) = green(i, j);
            result(i, j, 0, 2) = blue(i, j);
        }
    }


    red.save_png("../img/img_02/red_resampled.png");
    blue.save_png("../img/img_02/blue_resampled.png");
    red_half.save_png("../img/img_02/red_half.png");
    blue_half.save_png("../img/img_02/blue_half.png");

    return result;
}

CImg<> YCbCr_space(CImg<> img) {
    int width = img.width();
    int height = img.height();

    int half_width = width / 2;
    int half_height = height / 2;

    img.RGBtoYCbCr();

    CImg<> Y = img.get_channel(0);
    CImg<> Cb = img.get_channel(1);
    CImg<> Cr = img.get_channel(2);

    CImg<> Cb_half(half_width, half_height, 1, 1);
    CImg<> Cr_half(half_width, half_height, 1, 1);

    Y.save_png("../img/img_02/Y.png");
    Cb.save_png("../img/img_02/Cb.png");
    Cr.save_png("../img/img_02/Cr.png");

    for (int j = 0; j < half_height; j++) {
        for (int i = 0; i < half_width; i++) {
            Cb_half(i, j) = Cb(i*2, j*2);
            Cr_half(i, j) = Cr(i*2, j*2);
        }
    }

    int line;
    int col;
    float t;
    float u;
    float tmp;
    float d1, d2, d3, d4;
    float p1, p2, p3, p4; /* nearby pixels */

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            tmp = (float) j / (float) (height - 1) * (half_height - 1);
            line = (int) floor(tmp);

            if (line < 0)
                line = 0;
            else
                if (line >= half_height - 1)
                    line = half_height - 2;
            u = tmp - line;
            tmp = (float) (i) / (float) (width - 1) * (half_width - 1);
            col = (int) floor(tmp);
            if ( col < 0)
                col = 0;
            else
                if (col >= half_width - 1)
                    col = half_width - 2;
            t = tmp - col;

            /* Coefficients */
            d1 = (1 - t) * (1 - u);
            d2 = t * (1 - u);
            d3 = t * u;
            d4 = (1 - t) * u;

            /*neighborhood pixels*/
            p1 = Cb_half(col, line, 0);
            p2 = Cb_half(col + 1, line, 0);
            p3 = Cb_half(col + 1, (line + 1), 0);
            p4 = Cb_half(col, (line + 1), 0);
            Cb(i, j) = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;

            /*neighborhood pixels*/
            p1 = Cr_half(col, line, 0);
            p2 = Cr_half(col + 1, line, 0);
            p3 = Cr_half(col + 1, (line + 1), 0);
            p4 = Cr_half(col, (line + 1), 0);
            Cr(i, j) = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;
        }
    }

    CImg<> result(width, height, 1, 3);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            result(i, j, 0, 0) = Y(i, j);
            result(i, j, 0, 1) = Cb(i, j);
            result(i, j, 0, 2) = Cr(i, j);
        }
    }


    Cb.save_png("../img/img_02/Cb_resampled.png");
    Cr.save_png("../img/img_02/Cr_resampled.png");
    Cb_half.save_png("../img/img_02/Cb_half.png");
    Cr_half.save_png("../img/img_02/Cr_half.png");

    result.YCbCrtoRGB();
    img.YCbCrtoRGB();
    return result;
}

CImg<> HSV_space(CImg<> img) {
    int width = img.width();
    int height = img.height();

    int half_width = width / 2;
    int half_height = height / 2;

    img.RGBtoHSV();

    CImg<> H = img.get_channel(0);
    CImg<> S = img.get_channel(1);
    CImg<> V = img.get_channel(2);

    CImg<> S_half(half_width, half_height, 1, 1);
    CImg<> V_half(half_width, half_height, 1, 1);

    H.save_png("../img/img_02/H.png");
    S.save_png("../img/img_02/S.png");
    V.save_png("../img/img_02/V.png");

    for (int j = 0; j < half_height; j++) {
        for (int i = 0; i < half_width; i++) {
            S_half(i, j) = S(i*2, j*2);
            V_half(i, j) = V(i*2, j*2);
        }
    }

    int line;
    int col;
    float t;
    float u;
    float tmp;
    float d1, d2, d3, d4;
    float p1, p2, p3, p4; /* nearby pixels */

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            tmp = (float) j / (float) (height - 1) * (half_height - 1);
            line = (int) floor(tmp);

            if (line < 0)
                line = 0;
            else
                if (line >= half_height - 1)
                    line = half_height - 2;
            u = tmp - line;
            tmp = (float) (i) / (float) (width - 1) * (half_width - 1);
            col = (int) floor(tmp);
            if ( col < 0)
                col = 0;
            else
                if (col >= half_width - 1)
                    col = half_width - 2;
            t = tmp - col;

            /* Coefficients */
            d1 = (1 - t) * (1 - u);
            d2 = t * (1 - u);
            d3 = t * u;
            d4 = (1 - t) * u;

            /*neighborhood pixels*/
            p1 = S_half(col, line, 0);
            p2 = S_half(col + 1, line, 0);
            p3 = S_half(col + 1, (line + 1), 0);
            p4 = S_half(col, (line + 1), 0);
            S(i, j) = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;

            /*neighborhood pixels*/
            p1 = V_half(col, line, 0);
            p2 = V_half(col + 1, line, 0);
            p3 = V_half(col + 1, (line + 1), 0);
            p4 = V_half(col, (line + 1), 0);
            V(i, j) = p1 * d1 + p2 * d2 + p3 * d3 + p4 * d4;
        }
    }

    CImg<> result(width, height, 1, 3);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            result(i, j, 0, 0) = H(i, j);
            result(i, j, 0, 1) = S(i, j);
            result(i, j, 0, 2) = V(i, j);
        }
    }


    S.save_png("../img/img_02/S_resampled.png");
    V.save_png("../img/img_02/V_resampled.png");
    S_half.save_png("../img/img_02/S_half.png");
    V_half.save_png("../img/img_02/V_half.png");

    result.HSVtoRGB();
    img.HSVtoRGB();
    return result;
}
