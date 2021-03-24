#ifndef FFT_IFFT_RADIXN_H_INCLUDED
#define FFT_IFFT_RADIXN_H_INCLUDED

#include <iostream>
//#include "conio.h"
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <complex>
#include <fstream>


//complex number method:

void fun_fourier_transform_FFT_radix_2_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_3_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_4_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_5_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_7_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp99,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_11_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp99,std::complex<double> z[]);

void fun_fourier_transform_FFT_radix_2_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_3_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_4_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_5_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z_rx5[]);
void fun_fourier_transform_FFT_radix_7_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_fourier_transform_FFT_radix_11_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);

void fun_inverse_fourier_transform_FFT_radix_2_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_3_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_4_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_5_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp6,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_7_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp99,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_11_stg_rest(std::complex<double> tab[],int nb1,int nb2,int nb3,int nb4,int nb5_stg_previous,double tmp99,std::complex<double> z[]);

void fun_inverse_fourier_transform_FFT_radix_2_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_3_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_4_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_5_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z_rx5[]);
void fun_inverse_fourier_transform_FFT_radix_7_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);
void fun_inverse_fourier_transform_FFT_radix_11_stg_first(std::complex<double> tab[],int stg_first,double fi2,std::complex<double> z[]);

int fft_ifft_radixN(int *txdataF,int *txdata,int fft_size, int mod);



void fun_inverse_table_FFT(int M,std::complex<double> tab[]);
static int radix_base(int N,int stg[]);

void fun_fourier_transform_FFT_mixed_radix(int N,std::complex<double> tab[]);
void fun_inverse_fourier_transform_FFT_mixed_radix(int N,std::complex<double> tab[]);
void fun_fourier_transform_DFT(int N,std::complex<double> tab[]);





////////////////////////////////////////////////////
/////////////////////
static double fi=0.00;//for first stage FFT and for DFT
/////////////////////
///////////////////////////////////////////////////






static int tb[600][3]={};


#endif // FFT_IFFT_RADIXN_H_INCLUDED
