#include <libORAN/headers/ORAN_BS_DU_CU_SWA_API.h>

#include <libINIT/headers/_struct_zdv_cfg.h>
#include <libINIT/headers/_init.h>
//#include <libOFDM/headers/_fft.h>
#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>
#include <tmmintrin.h>

//#include "_nyFFT.h"
#include <libOFDM/headers/fft_ifft_radixN.h>
#include <libOFDM/headers/ofdm_modulation.h>



void Oran_Pbch_proc(uint32_t sfn, oran_pbch_proc_config_t *oran_pbch_proc_config, int16_t *txdataF)
{

        // input for test
        uint32_t bchPayload = oran_pbch_proc_config->bchPayload;
        uint16_t Lmax = oran_pbch_proc_config->Lmax;
        SSB_pattern_t SSB_pattern = oran_pbch_proc_config->SSB_pattern;
        uint16_t ssb_index = oran_pbch_proc_config->ssb_index;
        //int sfn = 0;
        uint8_t n_hf = oran_pbch_proc_config->n_hf;
        int16_t amp = oran_pbch_proc_config->amp;

        //uint16_t ssb_start_subcarrier = 0;
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
        PBCH_payload = PBCH_payload_generation(bchPayload,
                                               Lmax,
                                               ssb_subcarrier_offset,
                                               ssb_index,
                                               sfn,
                                               n_hf,
                                               amp);


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

        uint16_t N = nr_polar_output_length( K, //(data+crc nits number)
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

//        printf("\nr_polar_F:\n");
//        for (int i=0; i< E; i++)
//            printf(" %i ", pbch_F[i]);

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

        //template <size_t X, size_t Y, size_t Z>
        uint16_t ssb_index_mask[64];
        uint32_t nr_gold_pbch_dmrs[2][64][10];
        DMRS_Seq_Generation_pbch( pci,   // used for scrambling
                                  Lmax,   //
                                  ssb_index_mask,
                                  nr_gold_pbch_dmrs//(&nr_gold_pbch_dmrs)[2][64][10]    //uint32_t ***out
                                );

        //printf("\nDMRS_Seq_Generation_pbch = %x  ", nr_gold_pbch_dmrs[0][0][0]);

        //int16_t* modDataPbchDmrs = (int16_t*)malloc(sizeof(int16_t) * (E));
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



