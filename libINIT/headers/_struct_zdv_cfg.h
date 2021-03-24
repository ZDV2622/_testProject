#ifndef ADD_struct_zdv_cfg_H
#define ADD_struct_zdv_cfg_H


#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
#include <libTS38/headers/TS_38331_RL16_RRC.h>
//#include <libTS38/headers/TS_38101_4_UE_tx_rx.h>
//#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>


//#define __x86_64__
//#define __i386__

//#define DEBUG_PSS
//#define DEBUG_SSS
//#define DEBUG_PBCH_DMRS
//#define DEBUG_FFT
//#define FFT_my


static int const band_supported[] = {77,78,79};
static int  const scs_supported[] = {0,1};
static int const ssb_offset_point_a_max = 2199;
static int const ssb_subcarrier_offset_max = 31;

// тут предполагаю хранить индексы к таблицам конфигов
typedef struct {
  int SS_raster_per_band_tabl_ind;
  int nr_global_frequency_raster_ind;
  int nr_bandtable_ind;
  int nr_bandwidth_nrb_table_ind;
} index_table_zdv_t;



// zdv не было в nfapi
typedef struct {
  int band_zdv;
  int nrb_zdv;

  //TS38104 Table B.5.2-2: EVM window length for normal CP, FR1, 30 kHz SCS
  int fft_size_zdv; // delete
  int fft_shift_zdv; // nrb/2

  lte_prefix_type_t lte_prefix_type;
  uint8_t threequarter_fs;   // 3/4 sampling
  uint16_t fft_size; //  Size of FFT
  uint16_t first_carrier_offset;  ///// Carrier offset in FFT buffer for first RE in PRB0
  uint16_t symbols_per_slot;  // /// Number of OFDM/SC-FDMA symbols in one slot
  uint16_t slots_per_subframe; // /// Number of slots per subframe
  uint16_t slots_per_frame; // /// Number of slots per frame

  uint16_t nb_prefix_samples;  // Number of prefix samples in all but first symbol of slot
  uint16_t nb_prefix_samples0;  // Number of prefix samples in first symbol of slot
  uint16_t samples_per_ofdmsymbol0;
  uint16_t samples_per_ofdmsymbolN0;
  uint32_t samples_per_slot0;  // /// Number of samples in 0th and center slot of a subframe
  uint32_t samples_per_slotN0; // /// Number of samples in other slots of the subframe
  uint32_t samples_per_subframe; // /// Number of samples in a subframe
  uint32_t samples_per_frame;  ///// Number of samples in a radio frame
  uint32_t samples_per_subframe_wCP; // /// Number of samples in a subframe without CP
  uint32_t samples_per_slot_wCP;  // /// Number of samples in a slot without CP
  uint32_t samples_per_frame_wCP;  // /// Number of samples in a radio frame without CP
  //get_samples_per_slot_t get_samples_per_slot; // Number of samples in current slot
  //get_slot_from_timestamp_t get_slot_from_timestamp; // slot calculation from timestamp
  //get_samples_slot_timestamp_t get_samples_slot_timestamp;  // Number of samples before slot




  uint32_t nr_gold_pbch_dmrs[2][64][10];   /// PBCH DMRS sequence
  nr_frequency_range_t frequency_range_zdv;
  int scs_ssb_zdv;  // kHz
  int Lmax_zdv;
  int n_hf;
  bool ssb_mask_zdv[64] = {0}; // each bit - ssb index transmitted// RRC conf ssb-PositionInBurst
  int ssb_start_ofdm_symb[64] = {0};
  index_table_zdv_t index_table_zdv; // индексы к таблицам 3GPP
  SSB_pattern_t SSB_pattern_zdv; // CaseA,CaseB,CaseC,CaseD,
  int TDD_UL_DL_configCommon_ind_zdv; // zdv
  nfapi_nr_param_tlv_t nfapi_nr_param_tlv;
  nfapi_nr_config_tlv_t nfapi_nr_config_tlv;
  TDD_UL_DL_configCommon_t TDD_UL_DL_configCommon;
} nr_config_full_t;


#endif // ADD_struct_zdv_cfg_H




