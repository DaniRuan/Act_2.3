/*
Actividad 2.2, efecto blurring
*/

#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <omp.h>
#define NUM_THREADS 100 //cantidad de procesos (threads)
using namespace std;

//función que crea el filtro (Matriz)
int filter_matrix(unsigned char* imagen, long ancho, long alto, int posicion, int filterSize)
{
     int prom = 0;
	 int fila = 0;
	 int p_vt = 0;
	 int pixel = 0;

   for (int i = -filterSize; i <= filterSize; i++)
	 {
      p_vt = posicion + i * ancho;
      if (p_vt  >= 0 && p_vt <= alto*ancho)
			{
         fila = (int)(p_vt / ancho);
         for (int j = -filterSize; j <= filterSize; j++)
				 {
            pixel = p_vt + j;

            if (pixel >= (fila * ancho) && pixel <= ((fila + 1) * ancho - 1))
						{
               prom += imagen[pixel];
            }
         }
      }
   }
   prom = prom / (pow((2 *filterSize  + 1),2));
   return prom;
}

//función para aplicar desenfoque a la imagen
void blurring(int filterSize, int num)
{
   double tm1, tm2, time;
   tm1 = omp_get_wtime();
   FILE *inputImage, *outputImage, *reads;
   string in = "input" + to_string(num) + ".bmp";
   inputImage = fopen(in.c_str(),"rb");          //input
   string out = "output" + to_string(num) + "blur" + ".bmp";
   outputImage = fopen(out.c_str(),"wb");    //output
   long width;
   long height;
   unsigned char b, g, r;

   unsigned char xx[54];
   for(int i=0; i<54; i++)
	 {
   	xx[i] = fgetc(inputImage);
   	fputc(xx[i], outputImage);
   }

   width = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
   height = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

   unsigned char* originalVec = (unsigned char*)malloc(width*height*3*sizeof(unsigned char));
   unsigned char* blurVec  = (unsigned char*)malloc(width*height*3*sizeof(unsigned char));
   unsigned char* original_G  = (unsigned char*)malloc(width*height*sizeof(unsigned char));
   unsigned char* original_R = (unsigned char*)malloc(width*height*sizeof(unsigned char));
   unsigned char* original_B = (unsigned char*)malloc(width*height*sizeof(unsigned char));
   unsigned char* blurGreen = (unsigned char*)malloc(width*height*sizeof(unsigned char));
   unsigned char* blurRed = (unsigned char*)malloc(width*height*sizeof(unsigned char));
   unsigned char* blurBlue = (unsigned char*)malloc(width*height*sizeof(unsigned char));


   int n = 0;
   while(!feof(inputImage))
	 {
         originalVec[n] = fgetc(inputImage);
         n++;
   }

   int j = 0;
   for(int i = 0; i < width*height*3; i+=3)
	 {
      original_B[j] = originalVec [i];
      original_G[j] = originalVec [i + 1];
      original_R[j] = originalVec [i + 2];
      j++;
   }
   for(int i = 0; i < width*height; i++)
	 {
      blurBlue[i] = filter_matrix(original_B, width, height, i, filterSize);
      blurRed[i] = filter_matrix(original_R, width, height, i, filterSize);
      blurGreen[i] = filter_matrix(original_G, width, height, i, filterSize);
   }

   j = 0;
   for(int i = 0; i < width*height*3; i += 3) //se crea nueva imagen
	 {
      blurVec[i] = blurBlue[j];
      blurVec[i + 1] = blurGreen[j];
      blurVec[i + 2] = blurRed[j];
      j++;
   }

   for(int i = 0; i < width*height*3; i++)
	 {
      fputc(blurVec [i], outputImage);
   }

   fclose(inputImage);
   fclose(outputImage);

   tm2=omp_get_wtime();
   time=tm2-tm1;
   printf("tomo (%lf) segundos\n", time);
}


int main()
{
   omp_set_num_threads(NUM_THREADS);
   #pragma omp parallel
   for (int i = 0; i < 3; i++)
	 {
      blurring(7, i+1);
     }
   return 0;
}
