//La filtrul Sobel se folosesc 2 kerneluri si e folosit sa detecteze margini
//Gx=[-1][0][1],[-2][0][2],[-1][0][1] si Gy=[-1][-2][-1],[0][0][0],[1][2][1].Depinde de semnul outputului daca marginea va fi neagra sau alba.G vine de la gradient
//Magnitudinea gradientului se afla folosind teoria lui pitagora.Depinde de output gradientul.Orientarea marginii va fi arctan(Gy/Gx).
//Nu exista rgb, doar alb si negru de la 0 la 255.Convolutie se numeste cand folosesti un kernel peste fiecare pixel,deci trebuie intai transformata in alb-negru.

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "stb_image.h"
#include "stb_image_write.h"

#define Rvalue 0.2126
#define Gvalue 0.7152
#define Bvalue 0.0722

//am luat filtrul greyscale facut anterior pentru ca am nevoie de el

void greyscale(unsigned char* inputImg, unsigned char* outputImg, int width, int height) {

	int pixels = width * height;

	for (int i = 0; i < pixels; i++) {

		int indexRGB = i * 3; 

		unsigned char red = inputImg[indexRGB];
		unsigned char green = inputImg[indexRGB + 1];
		unsigned char blue = inputImg[indexRGB + 2];
		unsigned char grey = (unsigned char)(red * Rvalue + blue * Bvalue + green * Gvalue);

		outputImg[i] = grey;
	}

}

void sobel(unsigned char* inputImg, unsigned char* outputImg, int width, int height) {

	int Gx[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
	int Gy[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };

	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			
			int valx = 0;
			int valy = 0;

			for (int ky = -1; ky <= 1; ky++) {
				for (int kx = -1; kx <= 1; kx++) {

					int vecinX = x + kx;
					int vecinY = y + ky;

					int indexVecin = (vecinY * width + vecinX);

					int weightX = Gx[ky + 1][kx + 1];
					int weightY = Gy[ky + 1][kx + 1];

					 valx+=inputImg[indexVecin] * weightX;
					 valy += inputImg[indexVecin] * weightY;
				}
			}

			int magnitudine = (int)sqrt(valx * valx + valy * valy);
			if (magnitudine > 255) magnitudine = 255;

			int indexPixel = (y * width + x) * 3;
			outputImg[indexPixel] = (unsigned char)magnitudine;
			outputImg[indexPixel+1] = (unsigned char)magnitudine;
			outputImg[indexPixel+2] = (unsigned char)magnitudine;

		}
	}

}

int main() {

	int width, height, channels;
	int width2, height2, channels2;
	int width3, height3, channels3;

	unsigned char* inputImg720 = stbi_load("test720p.jpg", &width, &height, &channels, 3);
	unsigned char* inputImg1080 = stbi_load("test1080p.jpg", &width2, &height2, &channels2, 3);
	unsigned char* inputImg4k = stbi_load("test4kp.jpg", &width3, &height3, &channels3, 3);

	// Basic usage (see HDR discussion below for HDR usage):
	//    int x,y,n;
	//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	// ... process data if not NULL ... (Luat din stb_image.h)

	if (inputImg1080 == NULL) {
		perror("Imaginea nu a putut fi gasita");
		return -1;
	}

	if (inputImg720 == NULL) {
		perror("Imaginea nu a putut fi gasita");
		return -2;
	}

	if (inputImg4k == NULL) {
		perror("Imaginea nu a putut fi gasita");
		return -3;
	}

	int pixels = width * height; //720
	int pixels2 = width2 * height2; //1080
	int pixels3 = width3 * height3; //4k

	unsigned char* outputImg720 = (unsigned char*)malloc(pixels * 1 * sizeof(unsigned char)); //720
	unsigned char* outputImg1080 = (unsigned char*)malloc(pixels2 * 1 * sizeof(unsigned char)); //1080
	unsigned char* outputImg4k = (unsigned char*)malloc(pixels3 * 1 * sizeof(unsigned char)); //4k

	unsigned char* finalImg720 = (unsigned char*)malloc(pixels * 3 * sizeof(unsigned char)); //720
	unsigned char* finalImg1080 = (unsigned char*)malloc(pixels2 * 3 * sizeof(unsigned char)); //1080
	unsigned char* finalImg4k = (unsigned char*)malloc(pixels3 * 3 * sizeof(unsigned char)); //4k


	greyscale(inputImg720, outputImg720, width, height);
	sobel(outputImg720, finalImg720, width, height);

	stbi_write_jpg("grey720.jpg", width, height, 1, outputImg720, 100);
	stbi_write_jpg("sobel720.jpg", width, height, 3, finalImg720, 100);

	//aici 1 e nr de canale: rgb sunt 3 iar 100 e calitatea compresiei jpg
	// 
	//int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality);(Luat din std_image_write.h)
	// free the loaded image -- this is just free()
	//STBIDEF void     stbi_image_free(void* retval_from_stbi_load);
	free(outputImg720);
	free(finalImg720);
	stbi_image_free(inputImg720);

	greyscale(inputImg1080, outputImg1080, width2, height2);
	sobel(outputImg1080, finalImg1080, width2, height2);
	stbi_write_jpg("grey1080.jpg", width2, height2, 1, outputImg1080, 100);
	stbi_write_jpg("sobel1080.jpg", width2, height2, 3, finalImg1080, 100);

	free(outputImg1080);
	free(finalImg1080);
	stbi_image_free(inputImg1080);

	greyscale(inputImg4k, outputImg4k, width3, height3);
	sobel(outputImg4k, finalImg4k, width3, height3);
	stbi_write_jpg("grey4k.jpg", width3, height3, 1, outputImg4k, 100);
	stbi_write_jpg("sobel4k.jpg", width3, height3, 3, finalImg4k, 100);

	free(outputImg4k);
	free(finalImg4k);
	stbi_image_free(inputImg4k);
	return 0;
}