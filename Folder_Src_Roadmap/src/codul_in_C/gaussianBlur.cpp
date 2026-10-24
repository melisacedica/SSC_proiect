//Deci la acest blur avem un kernel(care este o matrice 3x3 "mutabila",iar elementul din mijloc se pune peste fiecare pixel),
//se face suma si se imparte la 16 iar in noua imagine se pune acea valoare in locul pixelului respectiv
//k=[1][2][1]
//	[2][4][1]
//	[1][2][1]
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_write.h"
//n a fost nevoie sa mai scriu defineurile pentru ca erau deja definite in grayscale

#define ksum 16

void gaussianBlur(unsigned char* inputImg, unsigned char* outputImg, int width, int height) {

	//asa deci aici la primul set de for uri il fac intai pe cel cu y pentru ca este mai rapid sa pun pe fiecare rand termenii decat sa pun pe fiecare coloana
	//spre deosebire de opengl pe care l am folosit anul trecut si era column major,aici e mai eficient sa fac totul row major pentru ca imaginea va fi un vector

	int k[3][3] = { { 1,2,1 }, { 2,4,2 }, { 1,2,1 } };

	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {

			int Red = 0, Green = 0, Blue = 0;
			//Aici parcurg elementele din matricea peste care e pus kernelul, adica la pixelul respectiv ma uit la vecini ca sa pot aduna si imparti
			for (int ky = -1; ky <= 1; ky++) {
				for (int kx = -1; kx <= 1; kx++) {

					int vecinX = x + kx;
					int vecinY = y + ky;
					int indexVecin = (vecinY * width + vecinX) * 3; //*3 pentru ca sunt 3 canale(rgb)

					int weight = k[ky + 1][kx + 1];

					Red += inputImg[indexVecin] * weight;
					Green += inputImg[indexVecin + 1] * weight;
					Blue += inputImg[indexVecin + 2] * weight;
				}
			}
			int indexPixel = (y * width + x) * 3;
			outputImg[indexPixel] = (unsigned char)(Red / ksum);
			outputImg[indexPixel+1] = (unsigned char)(Green / ksum);
			outputImg[indexPixel+2] = (unsigned char)(Blue / ksum);
		}
	}



}

/*int main() {

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

	unsigned char* outputImg720 = (unsigned char*)malloc(pixels * 3*sizeof(unsigned char)); //720
	unsigned char* outputImg1080 = (unsigned char*)malloc(pixels2 *3* sizeof(unsigned char)); //1080
	unsigned char* outputImg4k = (unsigned char*)malloc(pixels3 *3* sizeof(unsigned char)); //4k

	
	gaussianBlur(inputImg720, outputImg720, width, height);
	stbi_write_jpg("blur720.jpg", width, height, 3, outputImg720, 100);
	//aici 1 e nr de canale: rgb sunt 3 iar 100 e calitatea compresiei jpg
	// 
	//int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality);(Luat din std_image_write.h)
	// free the loaded image -- this is just free()
	//STBIDEF void     stbi_image_free(void* retval_from_stbi_load);
	free(outputImg720);
	stbi_image_free(inputImg720);

	gaussianBlur(inputImg1080, outputImg1080, width2, height2);
	stbi_write_jpg("blur1080.jpg", width2, height2, 3, outputImg1080, 100);
	free(outputImg1080);
	stbi_image_free(inputImg1080);

	gaussianBlur(inputImg4k, outputImg4k, width3, height3);
	stbi_write_jpg("blur4k.jpg", width3, height3, 3, outputImg4k, 100);
	free(outputImg4k);
	stbi_image_free(inputImg4k);

	return 0;
}*/