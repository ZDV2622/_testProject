#ifndef TS_38211_RL16_PHY_CHANNELS_MODULATION_H_INCLUDED
#define TS_38211_RL16_PHY_CHANNELS_MODULATION_H_INCLUDED

#include <libINIT/headers/_struct_zdv_cfg.h>
#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>
//#include "_fft.h"
//
//

//#define  NR_SSS_LENGTH 127
//#define  NR_PSS_LENGTH 636
//#define  NR_PBCH_DMRS_LENGTH_DWORD  10
//#define  NR_PBCH_DMRS_LENGTH  144
#define  K_TS_TC 64


/// Table 4.3.2-1: Number of OFDM symbols per slot, slots per frame, and slots per subframe for normal  cyclic prefix. -------------------------------------------------------------------------------------------------------------------------
/// Table 4.3.2-2: Number of OFDM symbols per slot, slots per frame, and slots per subframe for extended cyclic prefix.
///



typedef struct number_OFDMsymbols_slots_s
{
    lte_prefix_type_t lte_prefix_type; //kHz
    int numerology;
    uint16_t ofdm_symbols_per_slot;  // /// Number of OFDM/SC-FDMA symbols in one slot
    uint16_t slots_per_frame; // /// Number of slots per subframe
    uint16_t slots_per_subframe; // /// Number of slots per frame
} number_OFDMsymbols_slots_t;

static number_OFDMsymbols_slots_t number_OFDMsymbols_slots_tabl[] =
{
    {NORMAL, 0, 14, 10, 1},
    {NORMAL, 1, 14, 20, 2},
    {NORMAL, 2, 14, 40, 4},
    {NORMAL, 3, 14, 80, 8},
    {NORMAL, 4, 14, 160, 16},
    {EXTENDED, 2, 12, 40, 4}
};




/// -----------------------------------------------------------------------------------------------------------------------






/// 5.2.1 Pseudo-random sequence generation  ///// генерит последовательность голда для DMRS // input: две m-последовательности
extern uint32_t lte_gold_generic(uint32_t* x1, uint32_t* x2, uint8_t reset);

/// TS_38211 5.3.1 OFDM baseband signal generation for all channels except PRACH and RIM-RS ------------------------------------------------------------------------------------------
///
uint16_t ncp_calculate_no_prach(uint8_t mu,
                                uint8_t l, // l - it is ofdm symbol number in SFN
                                lte_prefix_type_t lte_prefix_type
                               );



void nr_generate_pss(nr_config_full_t *cfg,//input params
                     int *txdataF, // output
                     int amp
                     //int fft_size,
                     //int subc_test,// for test
                     //int ofdm_symb_test//for test
                    );

void nr_generate_sss(nr_config_full_t *cfg,//input params
                     int *txdataF, // output
                     int amp
                     //int fft_size,
                     //int subc_test,// for test
                     //int ofdm_symb_test//for test
                    );

void nr_generate_modulation_table();
//
///// 5.2.1 Pseudo-random sequence generation(gold sequence)  -------------------------------------------------------------------------------------------------------------
//// генерит последовательность голда для DMRS
//// input: две m-последовательности
///// 7.4.1.4.1 Demodulation reference signals for PBCH Sequence generation
///// using Table 7.4.3.1-1: Resources within an SS/PBCH block for PSS, SSS, PBCH, and DM-RS for PBCH.
//// генерим pbch
void nr_generate_pbch_dmrs(nr_config_full_t *cfg,//input params
                           int *txdataF, // output
                           int16_t amp
                           //int fft_size,
                           //int subc_test,// for test
                           //int ofdm_symb_test//for test
                          );

void pbch_dmrs_resource_mapper( int16_t *mod_dmrs, // input, modulated data pbch dmrs
                                int16_t *txdataF,  // output
                                uint16_t ssb_subcarrier_offset, // offset in freq, kssb
                                uint16_t fft_size,  // estimated: 2*(fftsize - NRb*12 / 2)
                                uint16_t fft_shift,
                                uint16_t ssb_offset_point_a, // in Nrb
                                uint16_t* ssb_start_symb_mask,  // taken from ssb_index_mask
                                uint16_t pci,
                                int amp
                              );


/*******************************************************************************************************************************************

TS_38211  7.3.3.1  Data Scrambling

********************************************************************************************************************************************/
// final fun: TS_38212  7.1.2  Scrambling
// input: bchPayloadInterleaved - a from TS
//        length - number of input bits
//        encoded
//        amp
//  output: a'
void pbch_data_scrambling( uint32_t *in, // input pbch_f, after rate_match, 864 bits, will be scrapbeled
                           uint32_t pci,
                           uint8_t Lmax,
                           uint16_t length, // NR_POLAR_PBCH_E = 864 - number of bits for scrample
                           int sfn,
                           int ssb_index
                         );


/*******************************************************************************************************************************************

-- TS_38211  7.3.3.2 PBCH Data Modulation
-- QPSK modulated as described in clause 5.1.3,
resulting in a block of complex-valued modulation symbols d PBCH ( 0 ) , ... , d PBCH ( M symb −1)

********************************************************************************************************************************************/

void pbch_data_modulation( uint32_t *in, // iinput data after scrambling
                           int16_t *out // modulated data
                         );


/*******************************************************************************************************************************************

-- TS_38211  5.1 Modulation mapper
-- generates tables for modulation

********************************************************************************************************************************************/

#define NR_MOD_TABLE_SIZE_SHORT 686
#define NR_MOD_TABLE_BPSK_OFFSET 1
#define NR_MOD_TABLE_QPSK_OFFSET 3
#define NR_MOD_TABLE_QAM16_OFFSET 7
#define NR_MOD_TABLE_QAM64_OFFSET 23
#define NR_MOD_TABLE_QAM256_OFFSET 87

static short nr_qpsk_mod_table[8];
static int32_t nr_16qam_mod_table[16];
static int64_t nr_16qam_byte_mod_table[1024];
static int64_t nr_64qam_mod_table[4096];
static int32_t nr_256qam_mod_table[512];

void nr_generate_modulation_table();


void pbch_data_resource_mapper( int16_t *in, // modulated data
                                int16_t *txdataF,
                                uint16_t ssb_index,
                                SSB_pattern_t SSB_pattern,
                                uint16_t ssb_subcarrier_offset,
                                uint16_t fft_shift,
                                uint16_t ssb_offset_point_a,
                                uint16_t fft_size,
                                uint16_t pci,
                                int amp
                              );


/*******************************************************************************************************************************************

TS_38211 7.4.1.4  DMRS Sequence Generation


********************************************************************************************************************************************/
//template <size_t N, size_t M, size_t K>

void DMRS_Seq_Generation_pbch ( uint16_t pci,   // used for scrambling
                                uint16_t Lmax,   //
                                uint16_t *ssb_index_mask,
                                uint32_t (&out)[2][64][10]//uint32_t ***out
                                );


#endif // TS_38211_RL16_PHY_CHANNELS_MODULATION_H_INCLUDED
