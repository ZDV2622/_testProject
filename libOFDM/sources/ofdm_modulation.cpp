#include <libOFDM/headers/ofdm_modulation.h>

#include <libINIT/headers/_struct_zdv_cfg.h>
#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>

//#include "_fft.h"
//
//


/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//typedef enum {
//  CYCLIC_PREFIX,
//  CYCLIC_SUFFIX,
//  ZEROS,
//  NONE
//} Extension_t;



void PHY_ofdm_mod_oai(int *input,                       /// pointer to complex input
                  int *output,                      /// pointer to complex output
                  int fftsize,            /// FFT_SIZE
                  unsigned char nb_symbols,         /// number of OFDM symbols
                  unsigned short nb_prefix_samples,  /// cyclic prefix length
                  Extension_t etype                /// type of extension
                 )
{

  if(nb_symbols == 0) return;

  int16_t temp[2*2*6144*4] __attribute__((aligned(32)));
  int i,j;

  volatile int *output_ptr=(int*)0;

  int *temp_ptr=(int*)0;
  idft_size_idx_t idftsize;

  switch (fftsize) {
  case 128:
    idftsize = IDFT_128;
    break;

  case 256:
    idftsize = IDFT_256;
    break;

  case 512:
    idftsize = IDFT_512;
    break;

  case 1024:
    idftsize = IDFT_1024;
    break;

  case 1536:
    idftsize = IDFT_1536;
    break;

  case 2048:
    idftsize = IDFT_2048;
    break;

  case 3072:
    idftsize = IDFT_3072;
    break;

  case 4096:
    idftsize = IDFT_4096;
    break;

  case 6144:
    idftsize= IDFT_6144;
    break;

 case 12288:
    idftsize= IDFT_12288;
    break;

 case 24576:
    idftsize= IDFT_24576;
    break;

  default:
    idftsize = IDFT_512;
    break;
  }

#ifdef DEBUG_OFDM_MOD
  printf("[PHY] OFDM mod (size %d,prefix %d) Symbols %d, input %p, output %p\n",
      fftsize,nb_prefix_samples,nb_symbols,input,output);
#endif

fftsize = fftsize*2;

  for (i=0; i<nb_symbols; i++) {

//#ifdef DEBUG_PBCH_DMRS
    printf("\n[PHY] symbol %d/%d offset %d (%p,%p -> %p)\n",i,nb_symbols,i*fftsize+(i*nb_prefix_samples),input,&input[i*fftsize],&output[(i*fftsize) + ((i)*nb_prefix_samples)]);
//#endif

#ifndef __AVX2__
    // handle 128-bit alignment for 128-bit SIMD (SSE4,NEON,AltiVEC)
    idft(idftsize,(int16_t *)&input[i*fftsize],
         (fftsize==128) ? (int16_t *)temp : (int16_t *)&output[(i*fftsize) + ((1+i)*nb_prefix_samples)],
         1);
#else
    // on AVX2 need 256-bit alignment
    idft(idftsize,(int16_t *)&input[i*fftsize],
         (int16_t *)temp,
         1);

#endif

#ifdef FFT_my

    fft_ifft_radixN(&input[i*fftsize],&output[(i*fftsize) + ((1+i)*nb_prefix_samples)],fftsize/2,0);

#endif // FFT_my

    // Copy to frame buffer with Cyclic Extension
    // Note:  will have to adjust for synchronization offset!

    switch (etype) {
    case CYCLIC_PREFIX:
      output_ptr = &output[(i*fftsize) + ((1+i)*nb_prefix_samples)];
      temp_ptr = (int *)temp;


      //      msg("Doing cyclic prefix method\n");

#ifndef __AVX2__
      if (fftsize==128)
#endif
      {
        memcpy((void*)output_ptr,(void*)temp_ptr,fftsize<<2);
      }
      memcpy((void*)&output_ptr[-nb_prefix_samples],(void*)&output_ptr[fftsize-nb_prefix_samples],nb_prefix_samples<<2);
      break;

    case CYCLIC_SUFFIX:


      output_ptr = &output[(i*fftsize)+ (i*nb_prefix_samples)];

      temp_ptr = (int *)temp;

      //      msg("Doing cyclic suffix method\n");

      for (j=0; j<fftsize ; j++) {
        output_ptr[j] = temp_ptr[2*j];
      }


      for (j=0; j<nb_prefix_samples; j++)
        output_ptr[fftsize+j] = output_ptr[j];

      break;

    case ZEROS:

      break;

    case NONE:

      //      msg("NO EXTENSION!\n");
      output_ptr = &output[fftsize];

      temp_ptr = (int *)temp;

      for (j=0; j<fftsize ; j++) {
        output_ptr[j] = temp_ptr[2*j];


      }

      break;

    default:
      break;

    }



  }

}


void PHY_ofdm_mod_radixN(int *input,                       /// pointer to complex input
                  int *output,                      /// pointer to complex output
                  int fftsize,                      /// FFT_SIZE
                  unsigned char nb_symbols,         /// number of OFDM symbols
                  unsigned short nb_prefix_samples,  /// cyclic prefix length
                  Extension_t etype                /// type of extension
                 )
{

  if(nb_symbols == 0) return;

  int16_t temp[2*2*6144*4] __attribute__((aligned(32)));
  int i,j;

  volatile int *output_ptr=(int*)0;

  int *temp_ptr=(int*)0;
  idft_size_idx_t idftsize;



fftsize = fftsize*2;

  for (i=0; i<nb_symbols; i++) {

//#ifdef DEBUG_PBCH_DMRS
    printf("\n[PHY] symbol %d/%d offset %d (%p,%p -> %p)\n",i,nb_symbols,i*fftsize+(i*nb_prefix_samples),input,&input[i*fftsize],&output[(i*fftsize) + ((i)*nb_prefix_samples)]);
//#endif


    fft_ifft_radixN(&input[i*fftsize],&output[(i*fftsize) + ((1+i)*nb_prefix_samples)],fftsize/2,0);


    // Copy to frame buffer with Cyclic Extension
    // Note:  will have to adjust for synchronization offset!

    switch (etype) {
    case CYCLIC_PREFIX:
      output_ptr = &output[(i*fftsize) + ((1+i)*nb_prefix_samples)];
      temp_ptr = (int *)temp;


      //      msg("Doing cyclic prefix method\n");

#ifndef __AVX2__
      if (fftsize==128)
#endif
      {
        memcpy((void*)output_ptr,(void*)temp_ptr,fftsize<<2);
      }
      memcpy((void*)&output_ptr[-nb_prefix_samples],(void*)&output_ptr[fftsize-nb_prefix_samples],nb_prefix_samples<<2);
      break;

    case CYCLIC_SUFFIX:


      output_ptr = &output[(i*fftsize)+ (i*nb_prefix_samples)];

      temp_ptr = (int *)temp;

      //      msg("Doing cyclic suffix method\n");

      for (j=0; j<fftsize ; j++) {
        output_ptr[j] = temp_ptr[2*j];
      }


      for (j=0; j<nb_prefix_samples; j++)
        output_ptr[fftsize+j] = output_ptr[j];

      break;

    case ZEROS:

      break;

    case NONE:

      //      msg("NO EXTENSION!\n");
      output_ptr = &output[fftsize];

      temp_ptr = (int *)temp;

      for (j=0; j<fftsize ; j++) {
        output_ptr[j] = temp_ptr[2*j];


      }

      break;

    default:
      break;

    }

  }

}

//void nr_normal_prefix_mod(int32_t *txdataF,int32_t *txdata,uint8_t nsymb,nr_config_full_t *cfg)
//{
//
//  PHY_ofdm_mod(txdataF,        // input
//	       txdata,         // output
//	       cfg->fft_size,
//
//	       1,                 // number of symbols
//	       cfg->nb_prefix_samples0,               // number of prefix samples
//	       CYCLIC_PREFIX);
//  PHY_ofdm_mod(txdataF+cfg->fft_size,        // input
//	       txdata+cfg->fft_size + cfg->nb_prefix_samples0,        // output
//	       cfg->fft_size,
//	       nsymb-1,
//	       cfg->nb_prefix_samples,               // number of prefix samples
//	       CYCLIC_PREFIX);
//
//}


