#ifndef ADD_FAPI_SCF_222_H
#define ADD_FAPI_SCF_222_H

#include <stdint.h>

// Here will be all tables and structures from official FAPI SCF222 doc

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
//typedef long long int64_t;


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
//typedef unsigned long long uint64_t;

typedef int64_t int64;
typedef uint64_t uint64;

////////////////////////////////////////////////////////////////NFAPI 222/////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////NFAPI 222/////////////////////////////////////////////////////////////////////


// 3.3.1.4  PARAM TLVs  Table 3-7 TLV format

typedef struct {
	uint16_t tag;
	uint16_t length;
} nfapi_tl_t;

/// Generic strucutre for single tlv value.
typedef struct {
	nfapi_tl_t tl;
	uint16_t value;
} nfapi_uint16_tlv_t;

typedef struct {
	nfapi_tl_t tl;
	int16_t value;
} nfapi_int16_tlv_t;

typedef struct {
	nfapi_tl_t tl;
	uint8_t value;
} nfapi_uint8_tlv_t;


/// Extension to the generic structures for single tlv values
typedef struct {
	nfapi_tl_t tl;
	int32_t value;
} nfapi_int32_tlv_t;

typedef struct {
	nfapi_tl_t tl;
	uint32_t value;
} nfapi_uint32_tlv_t;

typedef struct {
	nfapi_tl_t tl;
	int64_t value;
} nfapi_int64_tlv_t;

typedef struct {
	nfapi_tl_t tl;
	uint64_t value;
} nfapi_uint64_tlv_t;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///---------------------------------------------------------------------------------------------------/
///3.3.1 PARAM

/// Table 3-4  PHY API message types
typedef enum {
  NFAPI_NR_PHY_MSG_TYPE_PARAM_REQUEST=  0x00,
  NFAPI_NR_PHY_MSG_TYPE_PARAM_RESPONSE= 0x01,
  NFAPI_NR_PHY_MSG_TYPE_CONFIG_REQUEST= 0x02,
  NFAPI_NR_PHY_MSG_TYPE_CONFIG_RESPONSE=0X03,
  NFAPI_NR_PHY_MSG_TYPE_START_REQUEST=  0X04,
  NFAPI_NR_PHY_MSG_TYPE_STOP_REQUEST=   0X05,
  NFAPI_NR_PHY_MSG_TYPE_STOP_INDICATION=0X06,
  NFAPI_NR_PHY_MSG_TYPE_ERROR_INDICATION=0X07,
  //RESERVED 0X08 ~ 0X7F
  NFAPI_NR_PHY_MSG_TYPE_DL_TTI_REQUEST= 0X80,
  NFAPI_NR_PHY_MSG_TYPE_UL_TTI_REQUEST= 0X81,
  NFAPI_NR_PHY_MSG_TYPE_SLOT_INDICATION=0X82,
  NFAPI_NR_PHY_MSG_TYPE_UL_DCI_REQUEST= 0X83,
  NFAPI_NR_PHY_MSG_TYPE_TX_DATA_REQUEST=0X54,
  NFAPI_NR_PHY_MSG_TYPE_RX_DATA_INDICATION=0X85,
  NFAPI_NR_PHY_MSG_TYPE_CRC_INDICATION= 0X86,
  NFAPI_NR_PHY_MSG_TYPE_UCI_INDICATION= 0X87,
  NFAPI_NR_PHY_MSG_TYPE_SRS_INDICATION= 0X88,
  NFAPI_NR_PHY_MSG_TYPE_RACH_INDICATION= 0X89
  //RESERVED 0X8a ~ 0xff
} nfapi_nr_phy_msg_type_e;


/// Table 3-9 Cell parameters  TAGs define
#define  NFAPI_NR_PARAM_TLV_RELEASE_CAPABILITY_TAG 0x0001
#define  NFAPI_NR_PARAM_TLV_PHY_STATE_TAG         0x0002
#define  NFAPI_NR_PARAM_TLV_SKIP_BLANK_DL_CONFIG_TAG 0x0003
#define  NFAPI_NR_PARAM_TLV_SKIP_BLANK_UL_CONFIG_TAG 0x0004
#define  NFAPI_NR_PARAM_TLV_NUM_CONFIG_TLVS_TO_REPORT_TAG 0x0005
#define  NFAPI_NR_PARAM_TLV_CYCLIC_PREFIX_TAG 0x0006
#define  NFAPI_NR_PARAM_TLV_SUPPORTED_SUBCARRIER_SPACINGS_DL_TAG 0x0007
#define  NFAPI_NR_PARAM_TLV_SUPPORTED_BANDWIDTH_DL_TAG 0x0008
#define  NFAPI_NR_PARAM_TLV_SUPPORTED_SUBCARRIER_SPACINGS_UL_TAG 0x0009
#define  NFAPI_NR_PARAM_TLV_SUPPORTED_BANDWIDTH_UL_TAG 0x000A
#define  NFAPI_NR_PARAM_TLV_CCE_MAPPING_TYPE_TAG 0x000B
#define  NFAPI_NR_PARAM_TLV_CORESET_OUTSIDE_FIRST_3_OFDM_SYMS_OF_SLOT_TAG 0x000C
#define  NFAPI_NR_PARAM_TLV_PRECODER_GRANULARITY_CORESET_TAG 0x000D
#define  NFAPI_NR_PARAM_TLV_PDCCH_MU_MIMO_TAG 0x000E
#define  NFAPI_NR_PARAM_TLV_PDCCH_PRECODER_CYCLING_TAG 0x000F
#define  NFAPI_NR_PARAM_TLV_MAX_PDCCHS_PER_SLOT_TAG 0x0010
#define  NFAPI_NR_PARAM_TLV_PUCCH_FORMATS_TAG 0x0011
#define  NFAPI_NR_PARAM_TLV_MAX_PUCCHS_PER_SLOT_TAG 0x0012
#define  NFAPI_NR_PARAM_TLV_PDSCH_MAPPING_TYPE_TAG 0x0013
#define  NFAPI_NR_PARAM_TLV_PDSCH_ALLOCATION_TYPES_TAG 0x0014
#define  NFAPI_NR_PARAM_TLV_PDSCH_VRB_TO_PRB_MAPPING_TAG 0x0015
#define  NFAPI_NR_PARAM_TLV_PDSCH_CBG_TAG 0x0016
#define  NFAPI_NR_PARAM_TLV_PDSCH_DMRS_CONFIG_TYPES_TAG 0x0017
#define  NFAPI_NR_PARAM_TLV_PDSCH_DMRS_MAX_LENGTH_TAG 0x0018
#define  NFAPI_NR_PARAM_TLV_PDSCH_DMRS_ADDITIONAL_POS_TAG 0x0019
#define  NFAPI_NR_PARAM_TLV_MAX_PDSCH_S_YBS_PER_SLOT_TAG 0x001A
#define  NFAPI_NR_PARAM_TLV_MAX_NUMBER_MIMO_LAYERS_PDSCH_TAG 0x001B
#define  NFAPI_NR_PARAM_TLV_SUPPORTED_MAX_MODULATION_ORDER_DL_TAG 0x001C
#define  NFAPI_NR_PARAM_TLV_MAX_MU_MIMO_USERS_DL_TAG 0x001D
#define  NFAPI_NR_PARAM_TLV_PDSCH_DATA_IN_DMRS_SYMBOLS_TAG 0x001E
#define  NFAPI_NR_PARAM_TLV_PREMPTION_SUPPORT_TAG 0x001F
#define  NFAPI_NR_PARAM_TLV_PDSCH_NON_SLOT_SUPPORT_TAG 0x0020
#define  NFAPI_NR_PARAM_TLV_UCI_MUX_ULSCH_IN_PUSCH_TAG 0x0021
#define  NFAPI_NR_PARAM_TLV_UCI_ONLY_PUSCH_TAG 0x0022
#define  NFAPI_NR_PARAM_TLV_PUSCH_FREQUENCY_HOPPING_TAG 0x0023
#define  NFAPI_NR_PARAM_TLV_PUSCH_DMRS_CONFIG_TYPES_TAG 0x0024
#define  NFAPI_NR_PARAM_TLV_PUSCH_DMRS_MAX_LEN_TAG 0x0025
#define  NFAPI_NR_PARAM_TLV_PUSCH_DMRS_ADDITIONAL_POS_TAG 0x0026
#define  NFAPI_NR_PARAM_TLV_PUSCH_CBG_TAG 0x0027
#define  NFAPI_NR_PARAM_TLV_PUSCH_MAPPING_TYPE_TAG 0x0028
#define  NFAPI_NR_PARAM_TLV_PUSCH_ALLOCATION_TYPES_TAG 0x0029
#define  NFAPI_NR_PARAM_TLV_PUSCH_VRB_TO_PRB_MAPPING_TAG 0x002A
#define  NFAPI_NR_PARAM_TLV_PUSCH_MAX_PTRS_PORTS_TAG 0x002B
#define  NFAPI_NR_PARAM_TLV_MAX_PDUSCHS_TBS_PER_SLOT_TAG 0x002C
#define  NFAPI_NR_PARAM_TLV_MAX_NUMBER_MIMO_LAYERS_NON_CB_PUSCH_TAG 0x002D
#define  NFAPI_NR_PARAM_TLV_SUPPORTED_MODULATION_ORDER_UL_TAG 0x002E
#define  NFAPI_NR_PARAM_TLV_MAX_MU_MIMO_USERS_UL_TAG 0x002F
#define  NFAPI_NR_PARAM_TLV_DFTS_OFDM_SUPPORT_TAG 0x0030
#define  NFAPI_NR_PARAM_TLV_PUSCH_AGGREGATION_FACTOR_TAG 0x0031
#define  NFAPI_NR_PARAM_TLV_PRACH_LONG_FORMATS_TAG 0x0032
#define  NFAPI_NR_PARAM_TLV_PRACH_SHORT_FORMATS_TAG 0x0033
#define  NFAPI_NR_PARAM_TLV_PRACH_RESTRICTED_SETS_TAG 0x0034
#define  NFAPI_NR_PARAM_TLV_MAX_PRACH_FD_OCCASIONS_IN_A_SLOT_TAG 0x0035
#define  NFAPI_NR_PARAM_TLV_RSSI_MEASUREMENT_SUPPORT_TAG 0x0036


/// Table 3-9 Cell parameters structure
typedef struct
{
  nfapi_uint16_tlv_t release_capability; //TAG 0x0001
  nfapi_uint16_tlv_t phy_state;
  nfapi_uint8_tlv_t  skip_blank_dl_config;
  nfapi_uint8_tlv_t  skip_blank_ul_config;
  nfapi_uint16_tlv_t num_config_tlvs_to_report;
  nfapi_uint8_tlv_t* config_tlvs_to_report_list;
} nfapi_nr_cell_param_t;

/// table 3-10 Carrier parameters structure
typedef struct
{
  nfapi_uint8_tlv_t  cyclic_prefix;//TAG 0x0006
  nfapi_uint16_tlv_t supported_subcarrier_spacings_dl;
  nfapi_uint16_tlv_t supported_bandwidth_dl;
  nfapi_uint8_tlv_t  supported_subcarrier_spacings_ul;
  nfapi_uint16_tlv_t supported_bandwidth_ul;

} nfapi_nr_carrier_param_t;

///table 3-11 PDCCH parameters
typedef struct
{
  nfapi_uint8_tlv_t  cce_mapping_type;
  nfapi_uint8_tlv_t  coreset_outside_first_3_of_ofdm_syms_of_slot;
  nfapi_uint8_tlv_t  coreset_precoder_granularity_coreset;
  nfapi_uint8_tlv_t  pdcch_mu_mimo;
  nfapi_uint8_tlv_t  pdcch_precoder_cycling;
  nfapi_uint8_tlv_t  max_pdcch_per_slot;//TAG 0x0010

} nfapi_nr_pdcch_param_t;

///table 3-12 PUCCH parameters
typedef struct
{
  nfapi_uint8_tlv_t pucch_formats;
  nfapi_uint8_tlv_t max_pucchs_per_slot;

} nfapi_nr_pucch_param_t;

///table 3-13 PDSCH parameters
typedef struct
{
  nfapi_uint8_tlv_t pdsch_mapping_type;
  nfapi_uint8_tlv_t pdsch_allocation_types;
  nfapi_uint8_tlv_t pdsch_vrb_to_prb_mapping;
  nfapi_uint8_tlv_t pdsch_cbg;
  nfapi_uint8_tlv_t pdsch_dmrs_config_types;
  nfapi_uint8_tlv_t pdsch_dmrs_max_length;
  nfapi_uint8_tlv_t pdsch_dmrs_additional_pos;
  nfapi_uint8_tlv_t max_pdsch_tbs_per_slot;
  nfapi_uint8_tlv_t max_number_mimo_layers_pdsch;
  nfapi_uint8_tlv_t supported_max_modulation_order_dl;
  nfapi_uint8_tlv_t max_mu_mimo_users_dl;
  nfapi_uint8_tlv_t pdsch_data_in_dmrs_symbols;
  nfapi_uint8_tlv_t premption_support;//TAG 0x001F
  nfapi_uint8_tlv_t pdsch_non_slot_support;

} nfapi_nr_pdsch_param_t;

///table 3-14 PUSCH parameters
typedef struct
{
  nfapi_uint8_tlv_t uci_mux_ulsch_in_pusch;
  nfapi_uint8_tlv_t uci_only_pusch;
  nfapi_uint8_tlv_t pusch_frequency_hopping;
  nfapi_uint8_tlv_t pusch_dmrs_config_types;
  nfapi_uint8_tlv_t pusch_dmrs_max_len;
  nfapi_uint8_tlv_t pusch_dmrs_additional_pos;
  nfapi_uint8_tlv_t pusch_cbg;
  nfapi_uint8_tlv_t pusch_mapping_type;
  nfapi_uint8_tlv_t pusch_allocation_types;
  nfapi_uint8_tlv_t pusch_vrb_to_prb_mapping;
  nfapi_uint8_tlv_t pusch_max_ptrs_ports;
  nfapi_uint8_tlv_t max_pduschs_tbs_per_slot;
  nfapi_uint8_tlv_t max_number_mimo_layers_non_cb_pusch;
  nfapi_uint8_tlv_t supported_modulation_order_ul;
  nfapi_uint8_tlv_t max_mu_mimo_users_ul;
  nfapi_uint8_tlv_t dfts_ofdm_support;
  nfapi_uint8_tlv_t pusch_aggregation_factor;//TAG 0x0031

} nfapi_nr_pusch_param_t;

///table 3-15 PRACH parameters
typedef struct
{
  nfapi_uint8_tlv_t prach_long_formats;
  nfapi_uint8_tlv_t prach_short_formats;
  nfapi_uint8_tlv_t prach_restricted_sets;
  nfapi_uint8_tlv_t max_prach_fd_occasions_in_a_slot;
} nfapi_nr_prach_param_t;

///table 3-16 Measurement parameters
typedef struct
{
  nfapi_uint8_tlv_t rssi_measurement_support;
} nfapi_nr_measurement_param_t;


///Table 3-8  PARAM TLVs
typedef struct {
  uint8_t num_tlv;
  nfapi_nr_cell_param_t cell_param;
  nfapi_nr_carrier_param_t      carrier_param;
  nfapi_nr_pusch_param_t pusch_param;
  nfapi_nr_prach_param_t prach_param;
  nfapi_nr_measurement_param_t measurement_param;

} nfapi_nr_param_tlv_t;


///---------------------------------------------------------------------------------------------------/
///3.3.2 CONFIG


typedef enum {
  NFAPI_NR_CONFIG_MSG_OK = 0,
  NFAPI_NR_CONFIG_MSG_INVALID_CONFIG  //The configuration provided has missing mandatory TLVs, or TLVs that are invalid or unsupported in this state.
} nfapi_nr_config_errors_e;

/*typedef struct {
	nfapi_nr_config_errors_e error_code;
  uint8_t number_of_invalid_tlvs_that_can_only_be_configured_in_idle;
  uint8_t unmber_of_missing_tlvs;
  //?
  nfapi_nr_config_tlv_t* tlv_invalid_list;
  nfapi_nr_config_tlv_t* tlv_invalid_idle_list;
  nfapi_nr_config_tlv_t* tlv_invalid_running_list;
  nfapi_nr_config_tlv_t* tlv_missing_list;

} nfapi_nr_config_response_t;*/

/// 3.2  TAGs define
#define NFAPI_NR_CONFIG_DL_BANDWIDTH_TAG 0x1001
#define NFAPI_NR_CONFIG_DL_FREQUENCY_TAG 0x1002
#define NFAPI_NR_CONFIG_DL_K0_TAG 0x1003
#define NFAPI_NR_CONFIG_DL_GRID_SIZE_TAG 0x1004
#define NFAPI_NR_CONFIG_NUM_TX_ANT_TAG 0x1005
#define NFAPI_NR_CONFIG_UPLINK_BANDWIDTH_TAG 0x1006
#define NFAPI_NR_CONFIG_UPLINK_FREQUENCY_TAG 0x1007
#define NFAPI_NR_CONFIG_UL_K0_TAG 0x1008
#define NFAPI_NR_CONFIG_UL_GRID_SIZE_TAG 0x1009
#define NFAPI_NR_CONFIG_NUM_RX_ANT_TAG 0x100A
#define NFAPI_NR_CONFIG_FREQUENCY_SHIFT_7P5KHZ_TAG 0x100B

#define NFAPI_NR_CONFIG_PHY_CELL_ID_TAG 0x100C
#define NFAPI_NR_CONFIG_FRAME_DUPLEX_TYPE_TAG 0x100D

#define NFAPI_NR_CONFIG_SS_PBCH_POWER_TAG 0x100E
#define NFAPI_NR_CONFIG_BCH_PAYLOAD_TAG 0x100F
#define NFAPI_NR_CONFIG_SCS_COMMON_TAG 0x1010

#define NFAPI_NR_CONFIG_PRACH_SEQUENCE_LENGTH_TAG 0x1011
#define NFAPI_NR_CONFIG_PRACH_SUB_C_SPACING_TAG 0x1012
#define NFAPI_NR_CONFIG_RESTRICTED_SET_CONFIG_TAG 0x1013
#define NFAPI_NR_CONFIG_NUM_PRACH_FD_OCCASIONS_TAG 0x1014
#define NFAPI_NR_CONFIG_PRACH_CONFIG_INDEX_TAG 0x1029
#define NFAPI_NR_CONFIG_PRACH_ROOT_SEQUENCE_INDEX_TAG 0x1015
#define NFAPI_NR_CONFIG_NUM_ROOT_SEQUENCES_TAG 0x1016
#define NFAPI_NR_CONFIG_K1_TAG 0x1017
#define NFAPI_NR_CONFIG_PRACH_ZERO_CORR_CONF_TAG 0x1018
#define NFAPI_NR_CONFIG_NUM_UNUSED_ROOT_SEQUENCES_TAG 0x1019
#define NFAPI_NR_CONFIG_UNUSED_ROOT_SEQUENCES_TAG 0x101A
#define NFAPI_NR_CONFIG_SSB_PER_RACH_TAG 0x101B
#define NFAPI_NR_CONFIG_PRACH_MULTIPLE_CARRIERS_IN_A_BAND_TAG 0x101C

#define NFAPI_NR_CONFIG_SSB_OFFSET_POINT_A_TAG 0x101D
#define NFAPI_NR_CONFIG_BETA_PSS_TAG 0x101E
#define NFAPI_NR_CONFIG_SSB_PERIOD_TAG 0x101F
#define NFAPI_NR_CONFIG_SSB_SUBCARRIER_OFFSET_TAG 0x1020
#define NFAPI_NR_CONFIG_MIB_TAG 0x1021
#define NFAPI_NR_CONFIG_SSB_MASK_TAG 0x1022
#define NFAPI_NR_CONFIG_BEAM_ID_TAG 0x1023
#define NFAPI_NR_CONFIG_SS_PBCH_MULTIPLE_CARRIERS_IN_A_BAND_TAG 0x1024
#define NFAPI_NR_CONFIG_MULTIPLE_CELLS_SS_PBCH_IN_A_CARRIER_TAG 0x1025
#define NFAPI_NR_CONFIG_TDD_PERIOD_TAG 0x1026
#define NFAPI_NR_CONFIG_SLOT_CONFIG_TAG 0x1027

#define NFAPI_NR_CONFIG_RSSI_MEASUREMENT_TAG 0x1028


/// Table 3-21 Carrier configuration structure
typedef struct
{
  nfapi_uint16_tlv_t dl_bandwidth;//Carrier bandwidth for DL in MHz [38.104, sec 5.3.2] Values: 5, 10, 15, 20, 25, 30, 40,50, 60, 70, 80,90,100,200,400
  nfapi_uint32_tlv_t dl_frequency_pA; //Absolute frequency of DL point A in KHz [38.104, sec5.2 and 38.211 sec 4.4.4.2] Value: 450000 -> 52600000
  nfapi_uint16_tlv_t dl_k0[5];//k_{0}^{mu} for each of the numerologies [38.211, sec 5.3.1] Value: 0 ->23699
  nfapi_uint16_tlv_t dl_grid_size[5];//Grid size N_{grid}^{size,mu} for each of the numerologies [38.211, sec 4.4.2] Value: 0->275 0 = this numerology not used
  nfapi_uint16_tlv_t num_tx_ant;//Number of Tx antennas
  nfapi_uint16_tlv_t uplink_bandwidth;//Carrier bandwidth for UL in MHz. [38.104, sec 5.3.2] Values: 5, 10, 15, 20, 25, 30, 40,50, 60, 70, 80,90,100,200,400
  nfapi_uint32_tlv_t uplink_frequency_pA;//Absolute frequency of UL point A in KHz [38.104, sec5.2 and 38.211 sec 4.4.4.2] Value: 450000 -> 52600000
  nfapi_uint16_tlv_t ul_k0[5];//k0 mu for each of the numerologies [38.211, sec 5.3.1] Value: : 0 ->23699
  nfapi_uint16_tlv_t ul_grid_size[5];//Grid size for each of the numerologies [38.211, sec 4.4.2]. Value: 0->275 0 = this numerology not used
  nfapi_uint16_tlv_t num_rx_ant;//
  nfapi_uint8_tlv_t  frequency_shift_7p5khz;//Indicates presence of 7.5KHz frequency shift. Value: 0 = false 1 = true

} nfapi_nr_carrier_config_t;

/// Table 3-22 Cell configuration
typedef struct
{
  nfapi_uint16_tlv_t phy_cell_id;//Physical Cell ID, N_{ID}^{Cell} [38.211, sec 7.4.2.1] Value: 0 ->1007
  nfapi_uint8_tlv_t frame_duplex_type;//Frame duplex type Value: 0 = FDD 1 = TDD

} nfapi_nr_cell_config_t;

///table 3-23 SSB configuration
typedef struct
{
  nfapi_uint32_tlv_t ss_pbch_power;//SSB Block Power Value: TBD (-60..50 dBm)
  nfapi_uint8_tlv_t  bch_payload;//Defines option selected for generation of BCH payload, see Table 3-13 (v0.0.011 Value: 0: MAC generates the full PBCH payload 1: PHY generates the timing PBCH bits 2: PHY generates the full PBCH payload
  nfapi_uint8_tlv_t  scs_common;//subcarrierSpacing for common, used for initial access and broadcast message. [38.211 sec 4.2] Value:0->3
} nfapi_nr_ssb_config_t;

///Table 3-25 SSB Table
typedef struct
{
  nfapi_uint32_tlv_t ssb_mask;//Bitmap for actually transmitted SSB. MSB->LSB of first 32 bit number corresponds to SSB 0 to SSB 31 MSB->LSB of second 32 bit number corresponds to SSB 32 to SSB 63 Value for each bit: 0: not transmitted 1: transmitted

} nfapi_nr_ssb_mask_size_2_t;
//Table 3-25 SSB Table
typedef struct
{
  nfapi_uint8_tlv_t beam_id[64];//BeamID for each SSB in SsbMask. For example, if SSB mask bit 26 is set to 1, then BeamId[26] will be used to indicate beam ID of SSB 26. Value: from 0 to 63

} nfapi_nr_ssb_mask_size_64_t;
//Table 3-25 SSB Table
typedef struct
{
  //nfapi_uint16_tlv_t ssb_Subcarrier_Spacing; // zdv_extn - //manuall or from < 38.104 v15.1.0 - Table 5.4.3.3-1: Applicable SS raster entries per operating band (FR1) >
  nfapi_uint16_tlv_t ssb_offset_point_a;//Offset of lowest subcarrier of lowest resource block used for SS/PBCH block. Given in PRB [38.211, section 4.4.4.2] Value: 0->2199
  nfapi_uint8_tlv_t  beta_pss;//PSS EPRE to SSS EPRE in a SS/PBCH block [38.213, sec 4.1] Values: 0 = 0dB
  nfapi_uint8_tlv_t  ssb_period;//SSB periodicity in msec Value: 0: ms5 1: ms10 2: ms20 3: ms40 4: ms80 5: ms160
  nfapi_uint8_tlv_t  ssb_subcarrier_offset;//ssbSubcarrierOffset or, kssb
  nfapi_uint32_tlv_t MIB;//MIB payload, where the 24 MSB are used and represent the MIB in [38.331 MIB IE] and represent 0 1 2 3 1 , , , ,..., A− a a a a a [38.212, sec 7.1.1]
  nfapi_nr_ssb_mask_size_2_t ssb_mask_list[2];
  nfapi_nr_ssb_mask_size_64_t* ssb_beam_id_list;//64
  //nfapi_uint8_tlv_t  ss_pbch_multiple_carriers_in_a_band;//0 = disabled 1 = enabled
  //nfapi_uint8_tlv_t  multiple_cells_ss_pbch_in_a_carrier;//Indicates that multiple cells will be supported in a single carrier 0 = disabled 1 = enabled
} nfapi_nr_ssb_table_t;


/// Table 3-26 TDD table ---------------------------------------------------------------------------------------------------------------------------
//This table contains the configuration parameters used for storing a TDD pattern (up to 63
//SSB) in the PHY. The PARAM.response message will have indicated which of these parameters
//is supported by the PHY and in which PHY states these parameters are mandatory or optional
typedef struct
{
  nfapi_uint8_tlv_t slot_config;//For each symbol in each slot a uint8_t value is provided indicating: 0: DL slot 1: UL slot 2: Guard slot

} nfapi_nr_max_num_of_symbol_per_slot_t;
// Table 3-26 TDD table
typedef struct
{
  nfapi_nr_max_num_of_symbol_per_slot_t* max_num_of_symbol_per_slot_list;

} nfapi_nr_max_tdd_periodicity_t;
// Table 3-26 TDD table
typedef struct
{
  nfapi_uint8_tlv_t tdd_period;//DL UL Transmission Periodicity. Value:0: ms0p5 1: ms0p625 2: ms1 3: ms1p25 4: ms2 5: ms2p5 6: ms5 7: ms10 8: ms3 9: ms4
  nfapi_nr_max_tdd_periodicity_t* max_tdd_periodicity_list;

} nfapi_nr_tdd_table_t;

/// Table 3-27 Measurement Config
///
typedef struct
{
  nfapi_uint8_tlv_t rssi_measurement;//RSSI measurement unit. See Table 3-16 for RSSI definition. Value: 0: Do not report RSSI 1: dBm 2: dBFS

} nfapi_nr_measurement_config_t;



///Table 3-20 CONFIG TLVs ---------------------------------------------------------------------------------------------------------------------------
typedef struct {
  uint8_t num_tlv;
  nfapi_nr_carrier_config_t     carrier_config;
  nfapi_nr_cell_config_t        cell_config;
  nfapi_nr_ssb_config_t         ssb_config;
  //nfapi_nr_prach_config_t       prach_config;
  nfapi_nr_ssb_table_t          ssb_table;
  //nfapi_nr_tdd_table_t          tdd_table;
  //nfapi_nr_measurement_config_t measurement_config;
} nfapi_nr_config_tlv_t;




///  3.4.2.4    SSB PDU


/// Table 3-41 MAC generated MIB PDU
typedef struct
{
  uint32_t bch_payload;//BCH payload. The valid bits are indicated in the PARAM/CONFIG TLVs. If PARAM/CONFIG TLVs indicate MAC generates full bchPayload then the payload length is 31 bits with the 8 LSB bits being. Otherwise timing PBCH bits are generated by the PHY. And for bchPayload the 24 LSB are used.

} nfapi_nr_mac_generated_mib_pdu_t;

///  Table 3-42  PHY generated MIB PDU
typedef struct
{
  uint8_t  dmrs_type_a_position;//The position of the first DM-RS for downlink and uplink.
  uint8_t  pdcch_config_sib1;//The parameter PDCCH-ConfigSIB1 that determines a common ControlResourceSet (CORESET) a common search space and necessary PDCCH parameters.
  uint8_t  cell_barred;//The flag to indicate whether the cell is barred
  uint8_t  intra_freq_reselection;//The flag to controls cell selection/reselection to intrafrequency cells when the highest ranked cell is barred, or treated as barred by the UE. value 0 1

} nfapi_nr_phy_generated_mib_pdu_t;


typedef struct
{
  nfapi_nr_mac_generated_mib_pdu_t* mac_generated_mib_pdu;
  nfapi_nr_phy_generated_mib_pdu_t* phy_generated_mib_pdu;

} nfapi_nr_bch_payload_t;


/// Table 3-43 Tx Precoding and Beamforming PDU
typedef struct {
  /// Number of PRGs spanning this allocation. Value : 1->275
  uint16_t numPRGs;
  /// Size in RBs of a precoding resource block group (PRG) – to which same precoding and digital beamforming gets applied. Value: 1->275
  uint16_t prgSize;
  /// Number of STD ant ports (parallel streams) feeding into the digBF Value: 0->255
  uint8_t digBFInterfaces;
  // Depends on numPRGs
  uint16_t PMIdx[275];
  // Depends on digBFInterfaces
  uint16_t beamIdx[256];
} nfapi_nr_tx_precoding_and_beamforming_t;

/// Table 3-40 SSB/PBCH PDU
typedef struct {
  /// Physical Cell ID Value 0~>1007
  uint16_t PhysCellId;
  ///PSS EPRE to SSS EPRE in a SS/PBCH block 0 = 0dB 1 = 3dB
  uint8_t  BetaPss;
  ///SS/PBCH block index within a SSB burst set. Required for PBCH DMRS scrambling. Value: 0->63 (Lmax)
  uint8_t  SsbBlockIndex;
  /// ssbSubcarrierOffset or (TS38.211, section 7.4.3.1) Value: 0->31
  uint8_t  SsbSubcarrierOffset;
  ///Offset of lowest subcarrier of lowest resource block used for SS/PBCH block. Value: 0->2199
  uint16_t ssbOffsetPointA;
  /// A value indicating how the BCH payload is generated. This should match the PARAM/CONFIG TLVs. Value: 0: MAC generates the full PBCH payload, see Table 3-41, where bchPayload has 31 bits 1: PHY generates the timing PBCH bits, see Table 3-41, where the bchPayload has 24 bits 2: PHY generates the full PBCH payload
  uint8_t  bchPayloadFlag;
  uint32_t bchPayload;
  nfapi_nr_tx_precoding_and_beamforming_t precoding_and_beamforming;
} nfapi_nr_dl_tti_ssb_pdu_rel15_t;




typedef struct {
  nfapi_nr_dl_tti_ssb_pdu_rel15_t ssb_pdu_rel15;
} nfapi_nr_dl_tti_ssb_pdu;







#endif // ADD_FAPI_SCF_222_H

