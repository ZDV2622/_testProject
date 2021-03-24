#include <stdio.h>
#include <stdint.h>


#include <libINIT/headers/_init.h>
//#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>

//#include <libTS38/headers/TS_38213_PHY_Layer_Procedure.h>
//#include <libTS38/headers/TS_38104_BS_Radio_tx_rx_table.h>
//#include <libTS38/headers/TS_38331_RL16_RRC.h>
//#include <libTS38/headers/TS_38101_4_UE_tx_rx.h>
//#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
//#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
//#include <libINIT/headers/_struct_zdv_cfg.h>
//#include <libINIT/headers/_init.h>
//#include <libORAN/headers/ORAN_BS_SW_WG8.h>




// Returns the corresponding row index of the NR bandwidth table : Table 5.3.2-1: Transmission bandwidth configuration N_RB for FR1
int get_nr_bwtable_idx(int nr_bandP, uint8_t scs_index)
{

    int i, j;
    int scs_khz = 15 << scs_index;

    for (i = 0; i < NR_BW_BANDTABLE_SIZE; i++)
    {
        if ((nr_bandwidthtable[i].band == nr_bandP) && (nr_bandwidthtable[i].scs_index == scs_khz))
        {
            break;
        }
    }
    return i;
}

// Returns the corresponding row index of the NR band table : Table 5.4.2.3-1: Applicable NR-ARFCN per operating band in FR1
int get_nr_table_idx(int nr_bandP, uint8_t scs_index)
{
    int i, j;
    int scs_khz = 15 << scs_index;

    for (i = 0; i < NR_BANDTABLE_SIZE; i++)
    {
        if ((nr_bandtable[i].band == nr_bandP) && (nr_bandtable[i].deltaf_raster == scs_khz))
        {
            break;
        }
    }
    return i;
}

// Computes the duplex spacing (either positive or negative) in KHz - далее для оптеделения TDD/FDD
// -1 supplementary

int get_delta_duplex(int nr_bandP, uint8_t scs_index)
{
    int supplementary_bands[] = {29,75,76,80,81,82,83,84,86,89,95};
    size_t s = sizeof(supplementary_bands)/sizeof(supplementary_bands[0]);
    int sflag = 0;
    int delta_duplex, j, nr_table_idx;

    nr_table_idx = get_nr_table_idx(nr_bandP, scs_index);

    // if supplementary_bands
    for(j = 0; j < s; j++)
    {
        if (nr_bandP == supplementary_bands[j])
        {
            delta_duplex = -1;
            sflag = 1;
        }

    }
    if(sflag == 0)
    {
        delta_duplex = (nr_bandtable[nr_table_idx].ul_min_arfcn - nr_bandtable[nr_table_idx].dl_min_arfcn);
    }
    else
        delta_duplex = -1;

    return delta_duplex;
}

/// define TDD/FDD/Supl
int get_frame_type(uint16_t current_band,
                   uint8_t scs_index)
{
    int current_offset;
    int lte_type;
    current_offset = get_delta_duplex(current_band, scs_index);

    if (current_offset == 0)
        lte_type = 1; // TDD
    else if (current_offset == -1)
        lte_type = -1;
    else
        lte_type = 0;
    return lte_type;
}

int get_delta_arfcn(int i, uint32_t nrarfcn, uint64_t N_OFFs)
{
// проверка на четность для nrarfcn
    uint32_t delta_arfcn = nrarfcn - N_OFFs;
    return (delta_arfcn%(nr_bandtable[i].step_size_arfcn));
}

uint32_t to_nrarfcn(int nr_bandP,
                    uint64_t dl_CarrierFreq, // kHz
                    uint8_t scs_index)// index
{
    uint64_t dl_CarrierFreq_by_1k = dl_CarrierFreq;
    uint32_t nrarfcn;

    int deltaFglobal;
    uint32_t N_REF_Offs;
    float F_REF_Offs_khz;
    int j;

    size_t s = (sizeof(nr_global_frequency_raster)/sizeof(nr_global_frequency_raster[0]));

    // take params from TS_38104 Table 5.4.2.1-1: NR-ARFCN parameters for the global frequency raster
    for(j = 0; j < s; j++)
    {
        if ((dl_CarrierFreq/1000 >= nr_global_frequency_raster[j].freq_min) && (dl_CarrierFreq/1000 <= nr_global_frequency_raster[j].freq_max))
        {
            deltaFglobal = nr_global_frequency_raster[j].deltaFglobal;
            N_REF_Offs = nr_global_frequency_raster[j].N_REF_Offs;
            F_REF_Offs_khz = nr_global_frequency_raster[j].F_REF_Offs*1000;
        }

    }
    // F_REF=F_REF_Offs + deltaF_Global(N_REF-NREF_REF_Offs)
    nrarfcn =  (((dl_CarrierFreq_by_1k - F_REF_Offs_khz)/deltaFglobal)+N_REF_Offs);
    return nrarfcn;
}

uint32_t from_nrarfcn(int nr_bandP,
                      uint64_t nrarfcn, // Mhz
                      uint8_t scs_index)// kHz
{

    uint64_t freq;
    int deltaFglobal;
    uint32_t N_REF_Offs;
    float F_REF_Offs_khz;
    int j;

    size_t s = (sizeof(nr_global_frequency_raster)/sizeof(nr_global_frequency_raster[0]));

    // take params from TS_38104 Table 5.4.2.1-1: NR-ARFCN parameters for the global frequency raster
    for(j = 0; j < s; j++)
    {
        if ((nrarfcn >= nr_global_frequency_raster[j].arfcn_min) && (nrarfcn <= nr_global_frequency_raster[j].arfcn_max))
        {
            deltaFglobal = nr_global_frequency_raster[j].deltaFglobal;
            N_REF_Offs = nr_global_frequency_raster[j].N_REF_Offs;
            F_REF_Offs_khz = nr_global_frequency_raster[j].F_REF_Offs*1000;
        }

    }
    // F_REF=F_REF_Offs + deltaF_Global(N_REF-NREF_REF_Offs) +
    freq = F_REF_Offs_khz + deltaFglobal*(nrarfcn - N_REF_Offs);
    return freq;
}




void INIT_CELL_CONFIG(nr_config_full_t *cfg, oran_pbch_proc_config_t *oran_pbch_cfg)
{

    int flag = 0;
    int band;
    int bandtable_ind;
    int bandwidth;
    int nrb;
    int dl_frequency;
    int ul_frequency;
    int dl_arfcn;
    int ul_arfcn;
    int scs_index;
    int scs_ssb;
    SSB_pattern_t ssb_pattern;
    bool ssb_mask[64] = {0};
    int ssb_offset_point_a;
    int ssb_subcarrier_offset;
    int pci;
    int lte_frame_type;
    int TDD_UL_DL_configuration;
    int i;

    // странная муть, если это убрать, не видит функции из 38212, при вызове в ORAN? нужно вчитаться, похоже проблема с линками !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //uint32_t PBCH_payload;
//    PBCH_payload = PBCH_payload_generation(0,
//                                               0,
//                                               0,
//                                               0,
//                                               0,
//                                               0,
//                                               0);
    // странная муть, если это убрать, не видит функции из 38212, при вызове в ORAN  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Band
    while(flag==0)
    {

        std::cout << "\nENTER BAND: ";
        for (int i = 0; i < (sizeof(band_supported)/sizeof(band_supported[0])); ++i)
        {
            std::cout << band_supported[i] << " ";
        }
        std::cout << " : ";


        std::cin >> band;
        for (int i = 0; i < sizeof(band_supported)/sizeof(band_supported[0]); ++i)
        {
            if (band==band_supported[i])
            {
                printf("OK, band=%d",band);
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("ERROR, band %d is not supported, try again",band);
        }
    }
    flag = 0;
    cfg->band_zdv = band;

    // ENTER SCS index
    while(flag==0)
    {

        std::cout << "\n\nENTER mu index from range: ";
        for (int i = 0; i < (sizeof(scs_supported)/sizeof(scs_supported[0])); ++i)
        {
            std::cout << (scs_supported[i]) << " ";
        }
        std::cout << " : ";

        std::cin >> scs_index;
        for (int i = 0; i < sizeof(scs_supported)/sizeof(scs_supported[0]); ++i)
        {
            if (scs_index==scs_supported[i])
            {
                printf("OK, scs=%d",scs_index);
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("ERROR, scs %d is not supported, try again",scs_index);
        }
    }
    flag = 0;
    // save to structure
    cfg->nfapi_nr_config_tlv.ssb_config.scs_common.value = 15<<scs_index;

    printf("\n\nFrom band=%d and scs=%d kHz",band,15<<scs_index);
    // normal/extended CP save, limitation, only normal cp
    cfg->lte_prefix_type = NORMAL;
    printf("\nCP type: %s",cfg->lte_prefix_type==0 ? "NORMAL":"EXTENDED");
    bandtable_ind = get_nr_table_idx(band, scs_index);
    i = bandtable_ind;
    lte_frame_type = get_frame_type(band,scs_index);
    printf("\nLTE Frame type: %s",lte_frame_type==1 ? "TDD":"FDD");
    // save to str
    cfg->nfapi_nr_config_tlv.cell_config.frame_duplex_type.value = lte_frame_type;
    printf("\nFrom 3GPP TS 38104 Table 5.4.2.3-1: and Table 5.2-1");
    printf("\nBand = %d\ndl_min = %lu\ndl_max = %lu\nul_min = %lu\nul_max = %lu\n", nr_bandtable[bandtable_ind].band, nr_bandtable[bandtable_ind].dl_min_arfcn,nr_bandtable[bandtable_ind].dl_max_arfcn,nr_bandtable[bandtable_ind].ul_min_arfcn,nr_bandtable[i].ul_max_arfcn);

    int nr_bwtable_idx = get_nr_bwtable_idx(band,scs_index);

    // take sample configuration  from    lte_prefix_type_t and numerology;
    for (int i = 0; i < (sizeof(number_OFDMsymbols_slots_tabl)/sizeof(number_OFDMsymbols_slots_tabl[0])); ++i)
    {
        if ((number_OFDMsymbols_slots_tabl[i].lte_prefix_type == cfg->lte_prefix_type) && (number_OFDMsymbols_slots_tabl[i].numerology == scs_index))
        {
            printf("\nOK, from 3GPP TS 38211 table 4.3.2-1 4.3.2-2 ");
            cfg->slots_per_frame = number_OFDMsymbols_slots_tabl[i].slots_per_frame;
            cfg->slots_per_subframe = number_OFDMsymbols_slots_tabl[i].slots_per_subframe;
            cfg->symbols_per_slot = number_OFDMsymbols_slots_tabl[i].ofdm_symbols_per_slot;
            printf("\ncfg->slots_per_frame = %d ",  cfg->slots_per_frame);
            printf("\ncfg->slots_per_subframe = %d ",  cfg->slots_per_subframe);
            printf("\ncfg->symbols_per_slot = %d ",  cfg->symbols_per_slot);
        }
    }


    // bandwidth
    while(flag==0)
    {
        printf("\n\nFrom TS_38104 Table 5.3.5-1: BS channel bandwidths and SCS per operating band in FR1 - now band is limited");
        std::cout << "\nENTER bandwidth from range: ";
        for (int i = 0; i < (sizeof(nr_bandwidthtable[nr_bwtable_idx].available_bandwidth)/sizeof(nr_bandwidthtable[nr_bwtable_idx].available_bandwidth[0])); ++i)
        {
            //for (int i = 0; i < 13; ++i) {
            std::cout << nr_bandwidthtable[nr_bwtable_idx].available_bandwidth[i] << " ";
        }
        std::cout << " : ";

        std::cin >> bandwidth;
        for (int i = 0; i < (sizeof(nr_bandwidthtable[nr_bwtable_idx].available_bandwidth)/sizeof(nr_bandwidthtable[nr_bwtable_idx].available_bandwidth[0])); ++i)
        {
            if (bandwidth==nr_bandwidthtable[nr_bwtable_idx].available_bandwidth[i] && bandwidth != 0)
            {
                printf("OK, bandwidth=%d",bandwidth);
                // тут определим NRB из таблицы по 3GPP
                for (int j = 0; j < (sizeof(nr_bandwidth_nrb_table)/sizeof(nr_bandwidth_nrb_table[0])); ++j)
                {
                    if ((nr_bandwidth_nrb_table[j].scs_index==15<<scs_index) && (nr_bandwidth_nrb_table[j].bandwidth==bandwidth))
                    {
                        nrb = nr_bandwidth_nrb_table[j].n_rb;
                        cfg->nrb_zdv = nrb;
                        break;
                    }
                }
                printf("\nFrom TS_38104 Table 5.3.2-1: Transmission bandwidth configuration N_RB for FR1 - limited bands in table");
                printf("\nOK, nrb=%d",nrb);
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("ERROR, bandwidth %d is not supported, try again",bandwidth);
        }
    }
    flag = 0;

    // save to structere
    cfg->nfapi_nr_config_tlv.carrier_config.dl_bandwidth.value = bandwidth;
    if(lte_frame_type==1)
    {
        std::cout << "\nul_bandwidth = dl_bandwidth =  "<<bandwidth;
        cfg->nfapi_nr_config_tlv.carrier_config.uplink_bandwidth.value = bandwidth;

    }

    printf("\nOK, take samples number from 3GPP TS_38211 5.3.1 OFDM baseband signal generation for all channels except PRACH: ");
    cfg->nb_prefix_samples0 = ncp_calculate_no_prach(scs_index, 0,cfg->lte_prefix_type);
    cfg->nb_prefix_samples = ncp_calculate_no_prach(scs_index, 1, cfg->lte_prefix_type);
    printf("\ncfg->nb_prefix_samples0 = %d ",  cfg->nb_prefix_samples0);
    printf("\ncfg->nb_prefix_samples = %d ",  cfg->nb_prefix_samples);

    // take fft size from table;
    for (int i = 0; i < (sizeof(fft_sample_config_tabl)/sizeof(fft_sample_config_tabl[0])); ++i)
    {
        if ((fft_sample_config_tabl[i].lte_prefix_type == cfg->lte_prefix_type) && (fft_sample_config_tabl[i].numerology == scs_index) && (fft_sample_config_tabl[i].bandwidth == bandwidth))
        {
            printf("\nOK, fft_size from 3GPP TS3104 table B.5.2-1-4: ");
            cfg->fft_size = fft_sample_config_tabl[i].fft_size;
            oran_pbch_cfg->fft_size = fft_sample_config_tabl[i].fft_size;
            printf("\ncfg->fft_size = %d ",  cfg->fft_size);

        }
    }

    cfg->samples_per_subframe_wCP = cfg->fft_size * cfg->symbols_per_slot * cfg->slots_per_subframe;
    cfg->samples_per_frame_wCP = 10 * cfg->samples_per_subframe_wCP;
    cfg->samples_per_slot_wCP = cfg->symbols_per_slot*cfg->fft_size;
    cfg->samples_per_ofdmsymbolN0 = cfg->nb_prefix_samples + cfg->fft_size;
    cfg->samples_per_ofdmsymbol0  = cfg->nb_prefix_samples0 + cfg->fft_size;
    //cfg->samples_per_slotN0 = (cfg->nb_prefix_samples + cfg->fft_size) * cfg->symbols_per_slot;
    //cfg->samples_per_slot0 = cfg->nb_prefix_samples0 + ((cfg->symbols_per_slot-1)*cfg->nb_prefix_samples) + (cfg->symbols_per_slot*cfg->fft_size);
    cfg->samples_per_subframe = (cfg->nb_prefix_samples0 + cfg->fft_size) * 2 +
                                (cfg->nb_prefix_samples + cfg->fft_size) * (cfg->symbols_per_slot * cfg->slots_per_subframe - 2);
    //cfg->get_samples_per_slot = &get_samples_per_slot;
    //cfg->get_samples_slot_timestamp = &get_samples_slot_timestamp;
    //cfg->get_slot_from_timestamp = &get_slot_from_timestamp;
    cfg->samples_per_frame = 10 * cfg->samples_per_subframe;
//    printf("\ncfg->nb_prefix_samples0  = %d",  cfg->nb_prefix_samples0);
//    printf("\ncfg->nb_prefix_samples  = %d",  cfg->nb_prefix_samples);
//    printf("\ncfg->slots_per_subframe  = %d",  cfg->slots_per_subframe);
//    printf("\ncfg->cfg->symbols_per_slot  = %d",  cfg->symbols_per_slot);
//    printf("\ncfg->fft_size  = %d",  cfg->fft_size);
    printf("\nsample rate  = %d ",  cfg->samples_per_frame*100);



    // dl_frequence(Point A)
    while(flag==0)
    {
        std::cout << "\n\nENTER dl_frequence(Point A) from range, taken into account your band =  "<<band;
        printf("\nFrom Table 5.4.2.3-1 and Table 5.2-1");
        int real_bandwidth = nrb*12*(15<<scs_index);
        std::cout << "\ndl available range: " << nr_bandtable[bandtable_ind].dl_min_freq*1000 << " -- "<< nr_bandtable[bandtable_ind].dl_max_freq*1000 - real_bandwidth<< " delta = "<< nr_bandtable[bandtable_ind].step_size_arfcn*(nr_bandtable[bandtable_ind].deltaf_raster) <<" kHz :";
        std::cin >> dl_frequency;
        // cgeck range for dl_frequency and raster
        if ((dl_frequency<=nr_bandtable[bandtable_ind].dl_max_freq*1000 - real_bandwidth) && (dl_frequency>=nr_bandtable[bandtable_ind].dl_min_freq*1000) && (((dl_frequency-nr_bandtable[bandtable_ind].dl_min_freq*1000)%(nr_bandtable[bandtable_ind].step_size_arfcn*(nr_bandtable[bandtable_ind].deltaf_raster))==0)))
        {
            printf("OK, dl_frequency(Point A)=%d",dl_frequency);
            dl_arfcn = to_nrarfcn(band, dl_frequency,15<<scs_index);
            printf("\nOK, dl_arfcn(Point A)=%d",dl_arfcn);
            flag = 1;
            break;
        }
        if (flag == 0)
        {
            printf("ERROR, dl_frequency(Point A) %d is not supported, try again",dl_frequency);
        }
    }
    flag = 0;
    // save to structere
    cfg->nfapi_nr_config_tlv.carrier_config.dl_frequency_pA.value = dl_frequency;

    if(lte_frame_type==1)
    {
        std::cout << "\nul_frequence = dl_frequency(Point A) =  "<<dl_frequency;
        cfg->nfapi_nr_config_tlv.carrier_config.uplink_frequency_pA.value = dl_frequency;
    }

    // ul_frequency(Point A)
    flag = 0;
    while(flag==0)
    {
        if(lte_frame_type == 1)
        {
            break;
        }
        std::cout << "\nENTER ul_frequency(Point A) from range, taken into account your band =  "<<band;
        std::cout << "\nul available range(Point A): " << nr_bandtable[bandtable_ind].ul_min_freq << " -- "<< nr_bandtable[bandtable_ind].ul_max_freq - bandwidth*1000 << " :";
        std::cin >> ul_frequency;
        if((ul_frequency<=nr_bandtable[bandtable_ind].ul_max_freq - bandwidth*1000) && (ul_frequency>=nr_bandtable[bandtable_ind].ul_min_freq))
        {
            printf("\nOK, ul_frequency(Point A)=%d",ul_frequency);
            cfg->nfapi_nr_config_tlv.carrier_config.uplink_frequency_pA.value = ul_frequency;
            flag = 1;
            break;
        }
        if (flag == 0)
        {
            printf("\nERROR, ul_frequency(Point A) %d is not supported, try again",ul_frequency);
        }
    }
    flag = 0;

    // take FR1/FR2
    for (int i = 0; i < sizeof(frequency_ranges_tabl)/sizeof(frequency_ranges_tabl[0]); ++i)
    {
        if ((dl_frequency<=frequency_ranges_tabl[i].freq_max) && (dl_frequency>=frequency_ranges_tabl[i].freq_min))
        {
            printf("\nFR = %d",frequency_ranges_tabl[i].fr);
            cfg->frequency_range_zdv = frequency_ranges_tabl[i].fr;
            break;
        }
    }



    std::cout << "\n\nENTER PCI: ";
    std::cin >> pci;
    printf("OK, pci = %d",pci);
    // save to str
    cfg->nfapi_nr_config_tlv.cell_config.phy_cell_id.value = pci;




    //SELECT TDD_UL_DL_configuration
    std::cout << "\n\nSELECT TDD_UL_DL_configuration:\n ";
    for (int j = 0; j < (sizeof(TDD_UL_DL_configuration_tabl)/sizeof(TDD_UL_DL_configuration_tabl[0])); ++j)
    {
        if ((TDD_UL_DL_configuration_tabl[j].referenceSubcarrierSpacing==15<<scs_index))
        {
            printf("\nTDD_UL_DL_configuration: %d ",j);
            printf("\nTDD_UL_DL_configuration.referenceSubcarrierSpacing: %d ",TDD_UL_DL_configuration_tabl[j].referenceSubcarrierSpacing);
            printf("\nTDD_Slot_Configuration_pattern: ");
            for (int i1 = 0; i1 < 10; i1++)
            {
                printf(" %c ",TDD_UL_DL_configuration_tabl[j].TDD_Slot_Configuration_pattern[i1]);
            }
            //printf("\n");
            printf("\nSpecial_Slot_Configuration: ");
            for (int i2 = 0; i2 < 14; i2++)
            {
                printf(" %c ",TDD_UL_DL_configuration_tabl[j].Special_Slot_Configuration[i2]);
            }
            printf("\n");;
        }
    }
    while(flag==0)
    {
        std::cout << "\nENTER TDD_UL_DL_configuration from range: ";
        for (int j = 0; j < (sizeof(TDD_UL_DL_configuration_tabl)/sizeof(TDD_UL_DL_configuration_tabl[0])); ++j)
        {
            if ((TDD_UL_DL_configuration_tabl[j].referenceSubcarrierSpacing==15<<scs_index))
            {
                std::cout << j << " ";
            }

        }
        std::cout << " : ";
        std::cin >> TDD_UL_DL_configuration;
        for (int j = 0; j < (sizeof(TDD_UL_DL_configuration_tabl)/sizeof(TDD_UL_DL_configuration_tabl[0])); ++j)
        {
            if ((TDD_UL_DL_configuration_tabl[j].referenceSubcarrierSpacing==15<<scs_index) && (j == TDD_UL_DL_configuration))
            {
                printf("OK, TDD_UL_DL_configuration = %d",TDD_UL_DL_configuration);
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("ERROR, TDD_UL_DL_configuration %d is not supported, try again",TDD_UL_DL_configuration);
        }
    }
    flag = 0;
    cfg->TDD_UL_DL_configCommon.pattern1.dl_UL_TransmissionPeriodicity = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].dl_UL_TransmissionPeriodicity;
    cfg->TDD_UL_DL_configCommon.pattern1.nrofDownlinkSlots = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofDownlinkSlots;
    cfg->TDD_UL_DL_configCommon.pattern1.nrofDownlinkSymbols = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofDownlinkSymbols;
    cfg->TDD_UL_DL_configCommon.pattern1.nrofUplinkSlots = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofUplinkSlots;
    cfg->TDD_UL_DL_configCommon.pattern1.nrofUplinkSymbols = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofUplinkSymbols;
    cfg->TDD_UL_DL_configCommon_ind_zdv;


    // ENTER SCS SSB from range
    flag = 0;
    while(flag==0)
    {
        std::cout << "\n\nENTER SCS SSB from range: ";
        for (int j = 0; j < (sizeof(SS_raster_per_band_tabl)/sizeof(SS_raster_per_band_tabl[0])); ++j)
        {
            if ((SS_raster_per_band_tabl[j].band==band))
            {
                printf(" %d ",SS_raster_per_band_tabl[j].ssb_scs);
            }

        }
        std::cout << " : ";
        std::cin >> scs_ssb;
        for (int j = 0; j < (sizeof(SS_raster_per_band_tabl)/sizeof(SS_raster_per_band_tabl[0])); ++j)
        {
            //printf("test");
            if (scs_ssb==SS_raster_per_band_tabl[j].ssb_scs)
            {
                printf("OK, scs_ssb = %d kHz",scs_ssb);
                ssb_pattern = SS_raster_per_band_tabl[j].SSB_pattern;
                printf("OK, ssb_pattern = %d kHz",ssb_pattern);
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("ERROR, scs_ssb %d is not supported, try again",scs_ssb);
        }
    }
    flag = 0;

    cfg->scs_ssb_zdv = scs_ssb;
    cfg->SSB_pattern_zdv = ssb_pattern;

    get_ssb_start_symbol_t ssb_conf;
    nr_get_ssb_start_symbol(ssb_pattern, dl_frequency,&ssb_conf);
    cfg->Lmax_zdv = ssb_conf.lmax;
    printf("\nLmax =  %d ",ssb_conf.lmax);
    printf("\nssb start symb : ");
    for(int i = 0; i<ssb_conf.lmax; ++i)
    {
        printf(" %d  ",ssb_conf.ssb_start_ofdm_symb[i]);
    }

    // ssb index mask
    flag = 0;
    int mask;
    int j_start = 0;
    while(flag==0)
    {
        std::cout << "\n\nENTER SSB index mask(1-included,0-not incl): \n";
        for (int j = j_start; j < ssb_conf.lmax; ++j)
        {
            std::cout << "ssb_" << j << " : ";
            std::cin >> mask;
            if((mask==0)^(mask==1))
            {
                printf("OK\n");
                ssb_mask[j] = mask;
                oran_pbch_cfg->ssb_index_mask[i] = mask;
                cfg->ssb_mask_zdv[j] = mask;
                flag = 1;
                //break;
            }
            else
            {
                flag = 0;
                j_start = j;
                break;
            }

        }
        //flag = 1;
        if (flag == 0)
        {
            printf("ERROR, scs_ssb %d is not supported, try again",scs_ssb);
        }
    }
    flag = 0;
    j_start = 0;
    printf("\nOK, SSB INDEX MASK: ");
    for(int i = 0; i<ssb_conf.lmax; ++i)
    {
        printf(" %d  ",ssb_mask[i]);
    }
    printf("\nOK, SSB SRART SYMBOL MASK: ");
    for(int i = 0; i<ssb_conf.lmax; ++i)
    {
        printf(" %d  ",ssb_mask[i]*ssb_conf.ssb_start_ofdm_symb[i]);
        cfg->ssb_start_ofdm_symb[i] = ssb_mask[i]*ssb_conf.ssb_start_ofdm_symb[i];
        oran_pbch_cfg->ssb_start_symb_mask[i] = ssb_mask[i]*ssb_conf.ssb_start_ofdm_symb[i];

    }

    // ssb position in freq domain
    while(flag==0)
    {
        std::cout << "\nENTER ssb_offset_point_a from range: "<<" 0 -- " << ssb_offset_point_a_max<< " :";
        std::cin >> ssb_offset_point_a;
        if ((ssb_offset_point_a>=0) && (ssb_offset_point_a<ssb_offset_point_a_max))
        {
            printf("\nOK, ssb_offset_point_a=%d",ssb_offset_point_a);
            flag = 1;
            cfg->nfapi_nr_config_tlv.ssb_table.ssb_offset_point_a.value = ssb_offset_point_a;
            break;
        }
        if (flag == 0)
        {
            printf("\nERROR, ssb_offset_point_a %d is not supported, try again",ssb_offset_point_a);
        }
    }
    flag = 0;

    while(flag==0)
    {
        std::cout << "\nENTER ssb_subcarrier_offset from range: "<<" 0 -- " << ssb_subcarrier_offset_max<< " :";
        std::cin >> ssb_subcarrier_offset;
        if((ssb_subcarrier_offset>=0) && (ssb_subcarrier_offset<=ssb_subcarrier_offset_max))
        {
            printf("\nOK, ssb_subcarrier_offset=%d",ssb_subcarrier_offset);
            flag = 1;
            cfg->nfapi_nr_config_tlv.ssb_table.ssb_subcarrier_offset.value = ssb_subcarrier_offset;
            break;
        }
        if (flag == 0)
        {
            printf("\nERROR, ssb_subcarrier_offset %d is not supported, try again",ssb_subcarrier_offset);
        }
    }
    flag = 0;


    // oran_pbch_proc_config_t *oran_pbch_proc_config save to oran pbch config
    oran_pbch_cfg->amp = AMP;
    oran_pbch_cfg->Lmax = nr_get_ssb_Lmax(dl_frequency, // supported frequency
                                          ssb_pattern
                                         );
    oran_pbch_cfg->SSB_pattern = ssb_pattern;
    oran_pbch_cfg->n_hf = 0;
    oran_pbch_cfg->fft_shift = 2*(oran_pbch_cfg->fft_size - nrb*12/2);
    oran_pbch_cfg->ssb_offset_point_a = ssb_offset_point_a;
    oran_pbch_cfg->ssb_subcarrier_offset = ssb_subcarrier_offset;
    oran_pbch_cfg->pci = pci;
    oran_pbch_cfg->origBitlength = NR_POLAR_PBCH_PAYLOAD_BITS;
    oran_pbch_cfg->crcmask = 0;
    oran_pbch_cfg->crcParityBitsN = NR_POLAR_PBCH_CRC_PARITY_BITS;
    oran_pbch_cfg->ones_flag = 0;

    oran_pbch_cfg->K = NR_POLAR_PBCH_CRC_PARITY_BITS + NR_POLAR_PBCH_PAYLOAD_BITS;
    oran_pbch_cfg->E = NR_POLAR_PBCH_E; // (the rate matching output sequence length as given in Clause 5.4.1)
    oran_pbch_cfg->n_max = NR_POLAR_PBCH_N_MAX;
    oran_pbch_cfg->n_PC = NR_POLAR_PBCH_N_PC;
    oran_pbch_cfg->I_il = NR_POLAR_PBCH_I_IL;
    oran_pbch_cfg->Npc_wm = NR_POLAR_PBCH_N_PC_WM;

    printf("\nPARAMETES FOR PBCH Proc:\n");
    printf("\noran_pbch_cfg->Lmax = %d \n", oran_pbch_cfg->Lmax);



/// ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
/// разобраться с SSB

    /*
        cfg->band_zdv = band;
        cfg->nfapi_nr_config_tlv.carrier_config.dl_bandwidth.value = bandwidth;
        cfg->nfapi_nr_config_tlv.ssb_config.scs_common.value = 15<<scs_index;
        cfg->nfapi_nr_config_tlv.cell_config.frame_duplex_type.value = lte_frame_type;
        cfg->nfapi_nr_config_tlv.carrier_config.dl_frequency_pA.value = dl_frequency;
        cfg->nfapi_nr_config_tlv.carrier_config.uplink_frequency_pA.value = ul_frequency;
        cfg->nfapi_nr_config_tlv.cell_config.phy_cell_id.value = pci;
        cfg->TDD_UL_DL_configCommon.referenceSubcarrierSpacing = 15<<scs_index;
        cfg->TDD_UL_DL_configCommon.pattern1.dl_UL_TransmissionPeriodicity = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].dl_UL_TransmissionPeriodicity;
        cfg->TDD_UL_DL_configCommon.pattern1.nrofDownlinkSlots = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofDownlinkSlots;
        cfg->TDD_UL_DL_configCommon.pattern1.nrofDownlinkSymbols = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofDownlinkSymbols;
        cfg->TDD_UL_DL_configCommon.pattern1.nrofUplinkSlots = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofUplinkSlots;
        cfg->TDD_UL_DL_configCommon.pattern1.nrofUplinkSymbols = TDD_UL_DL_configuration_tabl[TDD_UL_DL_configuration].nrofUplinkSymbols;
        cfg->TDD_UL_DL_configCommon_ind_zdv;
        cfg->scs_ssb_zdv = scs_ssb;
        cfg->scs_ssb_zdv = scs_ssb;
        cfg->SSB_pattern_zdv = ssb_pattern;

    */


}



