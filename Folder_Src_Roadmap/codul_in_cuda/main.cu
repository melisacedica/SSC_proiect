//Am facut tot in main.cu pentru ca poza din greyscale sa mearga in blur si dupa in sobel 

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//acestea sunt pentru a citi si scrie imaginea pe disc
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/*__global__ void addKernel(int* c, const int* a, const int* b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}
Acest snippet de cod e luat din kernel.cu,fisierul cu care vine proiectul cand e creat
*/ 

__global__ void greyscaleKernel(unsigned char* d_input, unsigned char* d_output, int width,int height)
{
    //Pentru "formula" asta am folosit al 3-lea clip din bibliografie.
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < width && y < height) {
        int indexRGB = (y * width + x) * 3;
        int indexGray = y * width + x;

        unsigned char r = d_input[indexRGB];
        unsigned char g = d_input[indexRGB + 1];
        unsigned char b = d_input[indexRGB + 2];

        d_output[indexGray] = (unsigned char)(r * 0.2126f + g * 0.7152f + b * 0.0722f);
    }
}

__global__ void gaussianBlurKernel(unsigned char* d_input, unsigned char* d_output, int width, int height) {

    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    // Omit marginile pozei ca sa nu fie probleme la kernel
    if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
        int sum =
            d_input[(y - 1) * width + (x - 1)] * 1 +
            d_input[(y - 1) * width + x] * 2 +
            d_input[(y - 1) * width + (x + 1)] * 1 +
            d_input[y * width + (x - 1)] * 2 +
            d_input[y * width + x] * 4 +
            d_input[y * width + (x + 1)] * 2 +
            d_input[(y + 1) * width + (x - 1)] * 1 +
            d_input[(y + 1) * width + x] * 2 +
            d_input[(y + 1) * width + (x + 1)] * 1;

        d_output[y * width + x] = (unsigned char)(sum / 16);
    }
}
__global__ void sobelKernel(unsigned char* d_input, unsigned char* d_output, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
        int tl = d_input[(y - 1) * width + (x - 1)];
        int t = d_input[(y - 1) * width + x];
        int tr = d_input[(y - 1) * width + (x + 1)];
        int l = d_input[y * width + (x - 1)];
        int r = d_input[y * width + (x + 1)];
        int bl = d_input[(y + 1) * width + (x - 1)];
        int b = d_input[(y + 1) * width + x];
        int br = d_input[(y + 1) * width + (x + 1)];

        int valx = -tl + tr - 2 * l + 2 * r - bl + br;
        int valy = -tl - 2 * t - tr + bl + 2 * b + br;

        int magnitudine = (int)sqrtf((float)(valx * valx + valy * valy));
        if (magnitudine > 255) magnitudine = 255;

        int indexRGB = (y * width + x) * 3;

        //am folosit 3 canale rbg chiar daca sobel necesita o imagine greyscale pentru ca daca eventual vreau sa pun ceva color pe poza
        //sau sa deschid pe alt pc sa nu fie vreo problema 

        d_output[indexRGB] = (unsigned char)magnitudine;
        d_output[indexRGB + 1] = (unsigned char)magnitudine;
        d_output[indexRGB + 2] = (unsigned char)magnitudine;
    }
}

int main() {

    int width, height, channels; const char* input_poza[] = { "test720p.jpg", "test1080p.jpg", "test4kp.jpg" };
    const char* nume_gray[] = { "gray_720p.jpg", "gray_1080p.jpg", "gray_4k.jpg" };
    const char* nume_blur[] = { "blur_720p.jpg", "blur_1080p.jpg", "blur_4k.jpg" };
    const char* nume_sobel[] = { "sobel_720p.jpg", "sobel_1080p.jpg", "sobel_4k.jpg" };

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    for (int i = 0; i < 3; i++) {

        float timp_gray = 0.0f, timp_blur = 0.0f, timp_sobel = 0.0f;

        printf("Se proceseaza poza:%s \n", input_poza[i]);

        //Acestea sunt ca la cpu, luate din librariile de r/w de imagini

        unsigned char* img1 = stbi_load(input_poza[i], &width, &height, &channels, 3);

        if (img1 == NULL) {
            printf("Eroare la citirea imaginii!\n");
            return -1;
        }

        int pixels = width * height;

        // Aici aloc spatiu pe RAM ca sa pot pune pozele inapoi 1 canal pt greyscale, 1 pentru blurred si 3 pentru sobel

        unsigned char* img_gray = (unsigned char*)malloc(pixels * 1 * sizeof(unsigned char));
        unsigned char* img_blurred = (unsigned char*)malloc(pixels * 1 * sizeof(unsigned char));
        unsigned char* img_sobel = (unsigned char*)malloc(pixels * 3 * sizeof(unsigned char));

        unsigned char* input_rgb, * out_grayscale, * out_gaussianblur, * out_sobel;

        //Aici aloc spatiu pe VRAM
        cudaMalloc(&input_rgb, pixels * 3 * sizeof(unsigned char));
        cudaMalloc(&out_grayscale, pixels * 1 * sizeof(unsigned char));
        cudaMalloc(&out_gaussianblur, pixels * 1 * sizeof(unsigned char));
        cudaMalloc(&out_sobel, pixels * 3 * sizeof(unsigned char));

        // Trimit poza initiala catre gpu
        cudaMemcpy(input_rgb, img1, pixels * 3 * sizeof(unsigned char), cudaMemcpyHostToDevice);

        //Se organizeaza poze in block-uri de 16x16
        dim3 blockSize(16, 16);
        dim3 gridSize((width + blockSize.x - 1) / blockSize.x, (height + blockSize.y - 1) / blockSize.y);

        // 1. rgb->grayscale
        cudaEventRecord(start);
        greyscaleKernel << <gridSize, blockSize >> > (input_rgb, out_grayscale, width, height);
        cudaDeviceSynchronize();
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&timp_gray, start, stop);

        // 2. grayscale->gaussian
        cudaEventRecord(start);
        gaussianBlurKernel << <gridSize, blockSize >> > (out_grayscale, out_gaussianblur, width, height);
        cudaDeviceSynchronize();
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&timp_blur, start, stop);

        // 3.gaussian->sobel
        cudaEventRecord(start);
        sobelKernel << <gridSize, blockSize >> > (out_gaussianblur, out_sobel, width, height);
        cudaDeviceSynchronize();
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&timp_sobel, start, stop);

        // Aici aduc pozele inapoi pe disc
        cudaMemcpy(img_gray, out_grayscale, pixels * 1 * sizeof(unsigned char), cudaMemcpyDeviceToHost);
        cudaMemcpy(img_blurred, out_gaussianblur, pixels * 1 * sizeof(unsigned char), cudaMemcpyDeviceToHost);
        cudaMemcpy(img_sobel, out_sobel, pixels * 3 * sizeof(unsigned char), cudaMemcpyDeviceToHost);

        // Aici le salvez pe disc
        stbi_write_jpg(nume_gray[i], width, height, 1, img_gray, 100);
        stbi_write_jpg(nume_blur[i], width, height, 1, img_blurred, 100);
        stbi_write_jpg(nume_sobel[i], width, height, 3, img_sobel, 100);

        //Functia de cudaFree am folosit o din kernel.cu
        cudaFree(input_rgb);
        cudaFree(out_grayscale);
        cudaFree(out_gaussianblur);
        cudaFree(out_sobel);

        free(img_gray);
        free(img_blurred);
        free(img_sobel);
        stbi_image_free(img1);
        printf("Grayscale: %.4f ms,Gaussian: %.4f ms,Sobel: %.4f ms\n", timp_gray, timp_blur, timp_sobel);
    }
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}

//Inainte de eficientizare dura 7 secunde sa se compileze.
// 
//im show, sa scot forurile ( sa fac profiling, sa vad cat dureaza fiecare kernel)