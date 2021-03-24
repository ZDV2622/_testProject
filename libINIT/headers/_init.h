#ifndef ADD_INIT_H
#define ADD_INIT_H

//#include <stdio.h>
//#include <stdlib.h>
#include <iostream>

#include <libINIT/headers/_struct_zdv_cfg.h>
#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
#include <libORAN/headers/ORAN_BS_SW_WG8.h>

//#include <stdio.h>
//#include <iostream>
//
//#include <libINIT/headers/_struct_zdv_cfg.h>
//#include <libORAN/headers/ORAN_BS_SW_WG8.h>
//
//#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>
//#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
//#include <libTS38/headers/TS_38331_RL16_RRC.h>
//#include <libTS38/headers/TS_38101_4_UE_tx_rx.h>
//#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
//#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>




int get_nr_bwtable_idx(int nr_bandP, uint8_t scs_index);

int get_nr_table_idx(int nr_bandP, uint8_t scs_index);

int get_delta_duplex(int nr_bandP, uint8_t scs_index);

int get_frame_type(uint16_t current_band, uint8_t scs_index);

int get_delta_arfcn(int i, uint32_t nrarfcn, uint64_t N_OFFs);

uint32_t to_nrarfcn(int nr_bandP,
                    uint64_t dl_CarrierFreq, // Mhz
                    uint8_t scs_index);

uint32_t from_nrarfcn(int nr_bandP,
                    uint64_t nrarfcn, // Mhz
                    uint8_t scs_index);

//oran_pbch_proc_config_t cfg;

void INIT_CELL_CONFIG(nr_config_full_t *cfg, oran_pbch_proc_config_t *oran_pbch_cfg);

#endif // ADD_INIT_H
