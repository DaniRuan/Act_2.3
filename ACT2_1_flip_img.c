#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define quantity 3 //NUM_THREADS



void rotated_img(){
  double t1, t2, tiempo;
  t1 = omp_get_wtime();
  char frst[16];
  snprintf(frst, 16, "bmp%i.bmp", omp_get_thread_num()+2); //archivo de entrada
  char scnd[16];
  snprintf(scnd, 16, "img_rot%i.bmp", omp_get_thread_num()+2); //archivo de salida

  long ancho, alto;
  unsigned char r, g, b;
  unsigned char* ptr;

  FILE *fptr_h; fptr_h=fopen(frst,"rb");
  FILE *fptr_w; fptr_w=fopen(scnd,"wb");

  unsigned char xx[54];
  int cuenta = 0;
  for(int i=0; i<54; i++) {
    xx[i] = fgetc(fptr_h);
    fputc(xx[i], fptr_w);   //se crea nueva imagen (head)
  }

  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

  ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));

  while(!feof(fptr_h)){
    b = fgetc(fptr_h);
    g = fgetc(fptr_h);
    r = fgetc(fptr_h);

    unsigned char pixel = 0.21*r+0.72*g+0.07*b;

    ptr[cuenta] = pixel; //b
    ptr[cuenta+1] = pixel; //g
    ptr[cuenta+2] = pixel; //r

    cuenta +=3;

  }

  for(int i=0; i<alto-1; i++){
    for(int j=(ancho*3); j>2; j-=3){ //pixeles
      fputc(ptr[(ancho*i*3)+j+1+162], fptr_w);
      fputc(ptr[(ancho*i*3)+j+2+162], fptr_w);
      fputc(ptr[(ancho*i*3)+j+3+162], fptr_w);
    }
  }


  fclose(fptr_h);
  fclose(fptr_w);

   t2=omp_get_wtime();
   tiempo=t2-t1;
   printf("tomo (%lf) segundos\n", tiempo);

}

int main(){
  #pragma omp parallel
  omp_set_num_threads(quantity);
    rotated_img(); //se llama la función
}
