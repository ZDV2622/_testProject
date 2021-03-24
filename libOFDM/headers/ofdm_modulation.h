#ifndef OFDM_MODULATION_H_INCLUDED
#define OFDM_MODULATION_H_INCLUDED

#include <libOFDM/headers/_fft.h>
#include <libOFDM/headers/fft_ifft_radixN.h>

#include <libINIT/headers/_struct_zdv_cfg.h>
#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
//#include "_fft.h"
//
//


/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void PHY_ofdm_mod_oai(int *input,                       /// pointer to complex input
                  int *output,                      /// pointer to complex output
                  int fftsize,                      /// FFT_SIZE
                  unsigned char nb_symbols,         /// number of OFDM symbols
                  unsigned short nb_prefix_samples,  /// cyclic prefix length
                  Extension_t etype                /// type of extension
                 );


void PHY_ofdm_mod_radixN(int *input,                       /// pointer to complex input
                  int *output,                      /// pointer to complex output
                  int fftsize,                      /// FFT_SIZE
                  unsigned char nb_symbols,         /// number of OFDM symbols
                  unsigned short nb_prefix_samples,  /// cyclic prefix length
                  Extension_t etype                /// type of extension
                 );






#endif // OFDM_MODULATION_H_INCLUDED
