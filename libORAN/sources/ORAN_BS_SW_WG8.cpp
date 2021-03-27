#include <libORAN/headers/ORAN_BS_SW_WG8.h>

//#include <libINIT/headers/_struct_zdv_cfg.h>
//#include <libINIT/headers/_init.h>
//#include <libOFDM/headers/_fft.h>
//#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
//#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
//#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>
//
////#include <libOFDM/headers/_fft.h>
//
//#include <tmmintrin.h>
//
////#include "_nyFFT.h"
//#include <libOFDM/headers/fft_ifft_radixN.h>
//#include <libOFDM/headers/ofdm_modulation.h>


/*************************************************************************************************************************************************************

In this file you can find functional blocks from:
O-RAN Working Group 8
Base Station O-DU and O-CU Software Architecture and APIs

Chapter 4 O-DU Software Architecture ........................................ 14
Chapter 5 O-DU L1 Functional Blocks ..........................................14
5.1 Physical Uplink Shared Channel .......................................... 15
5.2 Uplink Control Channels ................................................. 16
5.3 Uplink Reference Signals ................................................ 16
5.4 Physical Downlink Shared Channel......................................... 16
5.5 Physical Downlink Control Channel ....................................... 17
5.6 Downlink Reference Signals .............................................. 17
5.7 Physical Broadcast Channel .............................................. 17
5.8 Physical Random Access Channel .......................................... 18
5.9 L1 Tasks Processing Flow ................................................ 18
5.10 Front Haul Module ...................................................... 20
5.11 O-DU Timing Synchronization ............................................ 20
Chapter 6 O-DU L2 Functional Blocks ..........................................21
6.1 L2 MAC Scheduler ........................................................ 23
6.2 Supporting E2 service models ............................................ 24
6.3 O-DU Cloudification ..................................................... 25


*************************************************************************************************************************************************************/



/*************************************************************************************************************************************************************

Function Oran_Pbch_proc

5.7 Physical Broadcast Channel .............................................. 17
Figure 5-3: PBCH processing flow diagram
-- PBCH Payload Generation: Creates the PBCH payload message, Refer to Subclause 7.1.1 in 3GPP TS38.212 [5] for details.
-- Scrambling: Refer to Subclause 7.1.2 in 3GPP TS38.212 [5] for details.
-- CRC attachment: Refer to Subclause 7.1.3 in 3GPP TS38.212 [5] for details.
-- Channel Encoding: Refer to Subclause 7.1.4 in 3GPP TS38.212 [5] for details.
-- Rate Matching: Refer to Subclause 7.1.5 in 3GPP TS38.212 [5] for details.
-- Data Scrambling: Refer to Subclause 7.3.1.3 in 3GPP TS38.211 [4] for details.
-- Data Modulation: Refer to Subclause 7.3.3.2 in 3GPP TS38.211 [4] for details.
-- Resource Element Mapping: Refer to Subclause 7.3.3.3 in 3GPP TS38.211 [4] for details.
-- DMRS Sequence Generation: Refer to Subclause 7.4.1.4 in 3GPP TS38.211 [4] for details.
-- DMRS Scrambling: Refer to Subclause 7.4.1.4 in 3GPP TS38.211 [4] for details.

All function description can be found in TSxxx docs
PBCH_payload_generation()
nr_PBCH_tb_crc_attach()
nr_polar_output_length()
.....


*************************************************************************************************************************************************************/


void Oran_Pbch_proc(uint32_t sfn, oran_pbch_proc_config_t *oran_pbch_proc_config, int16_t *txdataF)
{

        uint32_t bchPayload = oran_pbch_proc_config->bchPayload;
        uint16_t Lmax = oran_pbch_proc_config->Lmax;
        SSB_pattern_t SSB_pattern = oran_pbch_proc_config->SSB_pattern;
        uint16_t ssb_index = oran_pbch_proc_config->ssb_index;

        uint8_t n_hf = oran_pbch_proc_config->n_hf;
        int16_t amp = oran_pbch_proc_config->amp;

        uint16_t fft_size = oran_pbch_proc_config->fft_size;
        uint16_t fft_shift = oran_pbch_proc_config->fft_shift; // 2*(fftsize-Nrb*12/2)
        uint16_t ssb_offset_point_a = oran_pbch_proc_config->ssb_offset_point_a; // in rb number
        uint16_t ssb_subcarrier_offset = oran_pbch_proc_config->ssb_subcarrier_offset; // kssb

        //scrample param
        uint16_t pci = oran_pbch_proc_config->pci;
        uint16_t origBitlength = oran_pbch_proc_config->origBitlength; // bit len for input data

        //crc param
        int crcParityBitsN = oran_pbch_proc_config->crcParityBitsN; // for PBCH
        uint8_t ones_flag = oran_pbch_proc_config->ones_flag;

        uint16_t K = oran_pbch_proc_config->K;
        uint16_t E = oran_pbch_proc_config->E; // (the rate matching output sequence length as given in Clause 5.4.1)
        uint8_t n_max = oran_pbch_proc_config->n_max;
        int8_t n_PC = oran_pbch_proc_config->n_PC;
        uint8_t I_il = oran_pbch_proc_config->I_il;
        uint8_t Npc_wm = oran_pbch_proc_config->Npc_wm;

        uint32_t PBCH_payload;
        PBCH_payload = PBCH_payload_generation((uint32_t)bchPayload,
                                               (uint8_t)Lmax,
                                               (uint32_t)ssb_subcarrier_offset,
                                               (uint8_t)ssb_index,
                                               (int)sfn,
                                               (uint8_t)n_hf,
                                               (int16_t)amp);



        uint64_t nr_pbch_scrambled = nr_pbch_scrambling( PBCH_payload,
                                     pci,
                                     Lmax,
                                     origBitlength,
                                     sfn);

        uint64_t pbch_B[4]; //(data+crc)
        nr_PBCH_tb_crc_attach( &nr_pbch_scrambled,
                               //crc,
                               origBitlength,
                               //crcParityBitsN, // for
                               pbch_B
                             );

        uint16_t N = nr_polar_output_length( K, //(data+crc lenght)
                                             E, // (the rate matching output sequence length as given in Clause 5.4.1)
                                             n_max);

        uint8_t* pbch_D = (uint8_t*)malloc(sizeof(uint8_t) * (N));
        polar_encoder_PBCH( pbch_B,
                            pbch_D,
                            K,
                            E
                          );


        uint32_t* pbch_F = (uint32_t*)malloc(sizeof(uint32_t) * (NR_POLAR_PBCH_E_DWORD));
        //uint32_t pbch_e[NR_POLAR_PBCH_E_DWORD];
        rate_matching_do(pbch_D, // input d array
                         pbch_F,  // f array
                         N,
                         K
                        );

        pbch_data_scrambling(pbch_F, // input pbch_f, after rate_match, 864 bits, will be scrapbeled
                             pci,
                             Lmax,
                             E, // NR_POLAR_PBCH_E = 864 - number of bits for scrample
                             sfn,
                             ssb_index
                            );


        int16_t* modData = (int16_t*)malloc(sizeof(int16_t) * (E));
        pbch_data_modulation( pbch_F, // iinput data after scrambling
                              modData // modulated data
                            );

        uint16_t ssb_index_mask[64];
        uint32_t nr_gold_pbch_dmrs[2][64][10];
        DMRS_Seq_Generation_pbch( pci,   // used for scrambling
                                  Lmax,   //
                                  ssb_index_mask,
                                  nr_gold_pbch_dmrs//(&nr_gold_pbch_dmrs)[2][64][10]    //uint32_t ***out
                                );

        int16_t modDataPbchDmrs[NR_PBCH_DMRS_LENGTH << 1];
        pbch_data_modulation( &nr_gold_pbch_dmrs[0][0][0], // input data after scrambling
                              modDataPbchDmrs // modulated data out
                            );




        uint16_t ssb_start_symb_mask[64] = {0};
        for(int i = 0; i<Lmax; i++)
        {
            ssb_start_symb_mask[i] = nr_get_ssb_start_symbol_fromInd(ssb_index, SSB_pattern);

        };

        pbch_dmrs_resource_mapper( modDataPbchDmrs, // modulated data pbch dmrs
                                   txdataF,
                                   ssb_subcarrier_offset,   //uint16_t fft_shift ssb_subcarrier_offset
                                   fft_size, //uint16_t fft_size,
                                   fft_shift, //uint16_t fft_shift,
                                   ssb_offset_point_a, //uint16_t ssb_offset_point_a,
                                   ssb_start_symb_mask, //uint16_t ssb_start_symb_mask[64],
                                   pci, //uint16_t pci,
                                   amp//int amp
                                 );

        for(int i = 0; i<Lmax; i++)
        {
            if( ssb_start_symb_mask[i] > 0 )
            {
                    pbch_data_resource_mapper( modData, // modulated data
                                   txdataF,
                                   ssb_index,
                                   SSB_pattern,
                                   ssb_subcarrier_offset,
                                   fft_shift,
                                   ssb_offset_point_a,
                                   fft_size,
                                   pci,
                                   amp
                                 );

            }

        };




}



