#define _CRT_SECURE_NO_WARNINGS //acesta l am pus ca mi dadea o eroare ciudata din cauza fisierelor .h si nu ma lasa sa rulez
#include <stdio.h>
#include <stdlib.h>

// i.e. it should look like this: (Luat din stb_image.h)
//#include ...
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Before #including,
//#define STB_IMAGE_WRITE_IMPLEMENTATION(Luat din stb_image_write.h)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define Rvalue 0.2126
#define Gvalue 0.7152
#define Bvalue 0.0722

/*void greyscale(unsigned char* inputImg, unsigned char* outputImg, int width, int height) {

	int pixels = width * height;

	for (int i = 0; i < pixels; i++) {

		int indexRGB = i * 3; //aici pastrez indexul ca valorile sunt grupate cate 3 si sa stiu de unde extrag

		unsigned char red =inputImg[indexRGB];
		unsigned char green = inputImg[indexRGB+1];
		unsigned char blue = inputImg[indexRGB+2];
		unsigned char grey=(unsigned char)(red*Rvalue +blue*Bvalue+green*Gvalue);

		outputImg[i] = grey;
	}
	
}
*/
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

	unsigned char* outputImg720 = (unsigned char*) malloc(pixels * sizeof(unsigned char)); //720
	unsigned char* outputImg1080 = (unsigned char*)malloc(pixels2 * sizeof(unsigned char)); //1080
	unsigned char* outputImg4k = (unsigned char*)malloc(pixels3 * sizeof(unsigned char)); //4k

	greyscale(inputImg720, outputImg720, width, height);
	stbi_write_jpg("rez_gri720.jpg", width, height, 1, outputImg720, 100); 
	//aici 1 e nr de canale: rgb sunt 3 dar grayscale e 1 iar 100 e calitatea compresiei jpg
	// 
	//int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality);(Luat din std_image_write.h)
	// free the loaded image -- this is just free()
	//STBIDEF void     stbi_image_free(void* retval_from_stbi_load);
	free(outputImg720);
	stbi_image_free(inputImg720);

	greyscale(inputImg1080, outputImg1080, width2, height2);
	stbi_write_jpg("rez_1080.jpg", width2, height2, 1, outputImg1080, 100);
	free(outputImg1080);
	stbi_image_free(inputImg1080);

	greyscale(inputImg4k, outputImg4k, width3, height3);
	stbi_write_jpg("rez_4k.jpg", width3, height3, 1, outputImg4k, 100);
	free(outputImg4k);
	stbi_image_free(inputImg4k);

	return 0;
}*/