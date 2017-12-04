#include "functions.h"
#include "method01.h"
#include "method02.h"
#include "method03.h"

//g++ -o main functions.h main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
int main(int argc, char const ** argv) {
    char inImgName[250];
	sscanf (argv[1],"%s",inImgName);
    CImg <> img_read;
	img_read.load(inImgName);

    // CImg<unsigned char> display_img;
    // display_img.load(argv[1]);

    char resume = '\n';
	if (argc != 2) {
		std::cout << "USAGE : (UNIX) ./project image_name" << std::endl;
		std::cout << "        (WINDOWS) project.exe image_name)" << '\n';
		return 1;
	}

    int compression_method = 0;
    int nbr_of_decomp = 3;
    int quality = 50;
    CImgDisplay img_read_disp(img_read, "Image origianle");

    if(img_read_disp.is_closed() || img_read_disp.is_keyESC()) {
        std::cout << "\t\t    ----------- PROGRAMME ARRÊTÉ -------------- " << std::endl;
        return 0;
    }
    do {
		std::cout << "\t             +--------------------------------------+" << std::endl;
		std::cout << "\t      -------| Choisissez le méthode de compression |-------" << std::endl;
		std::cout << "\t      -------|    en tapant le chiffre 1, 2 ou 3    |-------" << std::endl;
		std::cout << "\t             +--------------------------------------+" << std::endl;
		std::cout << "\t     +------------------------------------------------------+" << std::endl;
		std::cout << "\t     |              1/ En utilisant une palette             |" << std::endl;
		std::cout << "\t     |                2/ Par rééchantillonnage              |" << std::endl;
		std::cout << "\t     |        3/ En utilisant la transformation de Haar     |" << std::endl;
        std::cout << "\t     |                4/ La combinaison de 1 et 2           |" << std::endl;
        std::cout << "\t     |                5/ La combinaison de 2 et 1           |" << std::endl;
		std::cout << "\t     +---------------------------------------------------------+" << std::endl;
		std::cout << "\t                   COMPRESSION METHOD : " << '\t';
		std::cin >> compression_method;
		while (std::cin.fail()) {
			std::cout << "\t             /!\\ ERROR : enter a integer " << '\t';
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> compression_method;
		}

		std::cout << std::endl;

		if (compression_method == 3) {
			std::cout << "\t      +---------------------------------------------------+ " << std::endl;
			std::cout << "\t      |   Saisissez le nombre d'itération et la qualité   | " << std::endl;
			std::cout << "\t      | entre 1 ~ 6 pour le nombre d'itération et 1 ~ 100 | " << std::endl;
			std::cout << "\t      +---------------------------------------------------+ " << std::endl;
			std::cout << "\t                   NOMBRE D'ITÉRATION (1 ~ 6): " << '\t';
			std::cin >> nbr_of_decomp;
			while (std::cin.fail()) {
				std::cout << "\t             /!\\ ERROR : Saisissez un entier " << '\t';
				std::cin.clear();
				std::cin.ignore(256, '\n');
				std::cin >> nbr_of_decomp;
			}
            std::string qual;
            std::cout << "\n\t                   QUALITÉ (1% ~ 100%) : " << '\t';
            std::cin >> qual;
            while (qual.find_first_not_of("1234567890.-") != std::string::npos) {
				std::cout << "\t             /!\\ ERROR : Saisissez un nombre réel " << '\t';
				std::cin.clear();
				std::cin.ignore(256, '\n');
				std::cin >> qual;
			}
            quality = atof(qual.c_str());
            std::cout << std::endl;
		}

        CImg<> img_tmp;
        CImg<> tmp = img_tmp;
		switch(compression_method) {

			case 1:
					std::cout << "\t\t   LA MÉTHODE DE LA PALETTE " << std::endl;
					img_tmp = get_img_genereted_by_kmeans(img_read);
					break;

			case 2:
					std::cout << "\t\t   LA MÉTHODE PAR RÉÉCHANTILLONNAGE " << std::endl;
					img_tmp = YCbCr_space(img_read);
					break;

			case 3:
					std::cout << "\t\t   LA MÉTHODE À PARTIR DE HAAR TRANSFOM " << std::endl;

					img_tmp = decomposition(img_read, nbr_of_decomp, (int)quality);
					break;

			case 4:
					std::cout << "\t\t   MÉTHODE 1 o MÉTHODE 2 " << std::endl;
                    tmp = get_img_genereted_by_kmeans(img_read);
                    img_tmp = YCbCr_space(tmp);
					break;

			case 5:
					std::cout << "\t\t   MÉTHODE 2 o MÉTHODE 1 " << std::endl;
                    tmp = YCbCr_space(img_read);
                    img_tmp = get_img_genereted_by_kmeans(tmp);
					break;

			default:
					std::cout << "\t\t La méthode par défaut est celle du rééchantillonnage, dans l'espace RGB " << std::endl;
					img_tmp = RGB_space(img_read);
					break;
		}

        char compressedName[30] = "../img/result.png";
        img_tmp.save_png(compressedName);
        double psnr = img_read.PSNR(img_tmp);
        std::cout << "\t\t\t    Le PSNR est de : " << psnr << "dB." << std::endl;

        std::ifstream original(argv[1], std::ifstream::ate | std::ifstream::binary);
        std::ifstream compressed(compressedName, std::ifstream::ate | std::ifstream::binary);
        double size_or = original.tellg();
        double size_co = compressed.tellg();
        double rate = size_or / size_co;

        original.close();
        compressed.close();
        std::cout << "\t\t    Le taux de compression est de : ( " << size_or << " / " << size_co << " ) = " << rate << std::endl;

        CImgDisplay result_disp(img_tmp, "Image compressée");

        while (!result_disp.is_keyESC() && !result_disp.is_closed()) {
			if(result_disp.button()&1) {
	            const int x = result_disp.mouse_x();
                const int y = result_disp.mouse_y();
                std::cout << "(" << x << ", " << y << ") = " << img_tmp(x, y) << '\n';
	        }
			result_disp.wait();
		}
		result_disp.close();

		std::cin.clear();
		std::cout << "\n\t  Appuyez sur ENTRER pour revenir au menu ou n'importe quel touche+ENTRER pour quitter : \t ";
		std::cin.ignore();
		std::cin.get(resume);
		if (resume == '\n')
			std::cout << "\033[2J\033[1;1H"; //clear the console
	} while (resume == '\n');

	std::cout << "\t\t    ----------- PROGRAMME TERMINÉ -------------- " << std::endl;

	return 0;
}
