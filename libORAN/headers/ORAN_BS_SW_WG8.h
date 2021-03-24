#ifndef ORAN_BS_SW_WG8_H_INCLUDED
#define ORAN_BS_SW_WG8_H_INCLUDED

#include <stdlib.h>
//#include <libOFDM/headers/_fft.h>
#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>



//#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>

//#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>
//#include <tmmintrin.h>

//#include "_nyFFT.h"
//#include <libOFDM/headers/fft_ifft_radixN.h>
//#include <libOFDM/headers/ofdm_modulation.h>

///Table 3-20 CONFIG TLVs ---------------------------------------------------------------------------------------------------------------------------
typedef struct {
        uint32_t bchPayload;
        uint16_t Lmax;                 // initialized
        SSB_pattern_t SSB_pattern;     //in init function
        uint16_t ssb_index = 0;        //
        uint16_t ssb_index_mask[64];
        uint16_t ssb_start_symb_mask[64];
        //int sfn = 0;
        uint8_t n_hf;
        int16_t amp;

        uint16_t fft_size;
        uint16_t fft_shift; // 2*(fftsize-Nrb*12/2)
        uint16_t ssb_offset_point_a; // in rb number
        uint16_t ssb_subcarrier_offset; // kssb

        //scrample param
        uint16_t pci;
        uint16_t origBitlength;
        int32_t crcmask;
        //crc param
        int crcParityBitsN; // for PBCH L=24
        uint8_t ones_flag; //


        uint16_t K;
        uint16_t E; // (the rate matching output sequence length as given in Clause 5.4.1)
        uint8_t n_max;
        int8_t n_PC;
        uint8_t I_il;
        uint8_t Npc_wm;

} oran_pbch_proc_config_t;


void Oran_Pbch_proc(uint32_t sfn, oran_pbch_proc_config_t *oran_pbch_proc_config, int16_t *txdataF);



#endif // ORAN_BS_SW_WG8_H_INCLUDED
