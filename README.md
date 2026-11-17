Proiectul de semestru la materia SSC

Voi face proiectul 2 de nota 8: Optimizarea algoritmilor de procesare a imaginilor folosind CUDA

Clipuri/websites folosite pentru research:

1.Algoritmii alesi:
-https://www.youtube.com/watch?v=5gO2PwGS2kk(Greyscale)
-https://www.youtube.com/watch?v=jThDqulaxyA(Greyscale)
-https://docs.opencv.org/4.13.0/d6/d6d/tutorial_mat_the_basic_image_container.html(Storing methods,ultimul paragraf.Pentru ca eu sa pot transmite imaginea mai rapid pentru transferul pe GPU,folosesc unsigned char si nu int[datorita memoriei ocupate])
-https://github.com/nothings/stb/blob/master/stb_image.h (acesta e pentru libraria ce se ocupa cu citirea imaginilor, adica sa ia o imagine sa extraga pixel cu pixel si sa ii stocheze in memoria ram sub forma de vector ca sa poata fi procesata de algoritmii facuti de mine.
https://github.com/nothings/stb/blob/master/stb_image_write.h (libraria aceasta ia rezultatul matematic si il transforma in poza si il scrie inapoi pe disc)
-https://www.youtube.com/watch?v=-AuwMJAqjJc(Gaussian Blur)
-https://www.youtube.com/watch?v=uihBwtPIBxM(Filtrul Sobel)

2.Cuda
-https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/intro-to-cuda-cpp.html
-https://www.youtube.com/watch?v=2NgpYFdsduY
-https://www.youtube.com/watch?v=XYrPJbdswvw
-https://docs.nvidia.com/cuda/cuda-runtime-api/cuda_runtime_api/group__CUDART__DEVICE.html
