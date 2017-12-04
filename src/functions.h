#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "CImg.h"

using namespace cimg_library;
//**********functions declarations*************//

//Compression from the documentation of the TP1
CImg<int> get_img_genereted_by_kmeans(CImg<int> img);
CImg<int> get_labeled_img_from_palette(const CImg<int> img, const CImg<int> palette);
CImg<int> get_palette(CImg<int> img);
bool is_in_palette(CImg<int> palette, int * color);
int compute_distance(int * color_palette, int * color);
//******************************************//

//Compression from the documentation of the TP2
CImg<> RGB_space(CImg<> img);
CImg<> YCbCr_space(CImg<> img);
CImg<> HSV_space(CImg<> img);
//******************************************//

//Compression from the documentation of the TP3

//******************************************//

//Compression from the documentation of the TP4
CImg<> decomposition(CImg<> img, int nbr_of_decomp, int quality);
//******************************************//
