#ifndef ADD_TS_38104_H
#define ADD_TS_38104_H

#include <libNFAPI/headers/FAPI_SCF_222.h>

#define NR_BANDTABLE_SIZE (sizeof(nr_bandtable)/sizeof(nr_bandentry_t))
#define NR_BW_BANDTABLE_SIZE (sizeof(nr_bandwidthtable)/sizeof(nr_bandwidth_t))


/// Params and tables from TS 38104
/// Base Station (BS) radio transmission and reception



/// Table 5.3.5-1: BS channel bandwidths and SCS per operating band in FR1 - now band is limited ---------------------------------------------------------------------

typedef struct nr_bandwidth_s
{
    int16_t band;
    uint8_t scs_index;
    int available_bandwidth[13];
} nr_bandwidth_t;


static const nr_bandwidth_t nr_bandwidthtable[] =
{
    {77, 15, {0,10,15,20,25,30,40,50,0,0,0,0,0}},
    {77, 30, {0,10,15,20,25,30,40,50,60,70,80,90,100}},
    {77, 60, {0,10,15,20,25,30,40,50,60,70,80,90,100}},
    {78, 15, {0,10,15,20,25,30,40,50,0,0,0,0,0}},
    {78, 30, {0,10,15,20,25,30,40,50,60,70,80,90,100}},
    {78, 60, {0,10,15,20,25,30,40,50,60,70,80,90,100}},
    {79, 15, {0,0,5,0,0,0,40,50,0,0,0,0,0}},
    {79, 30, {0,0,15,0,0,0,40,50,60,0,80,0,100}},
    {79, 60, {0,10,15,20,25,30,40,50,60,0,80,0,100}}
};


///  Table 5.3.2-1: Transmission bandwidth configuration N_RB for FR1 - limited bands --------------------------------------------------------------------------------

typedef struct nr_bandwidth_nrb_s
{
    uint8_t scs_index;
    int16_t bandwidth;
    int n_rb;
} nr_bandwidth_nrb_t;

static const nr_bandwidth_nrb_t nr_bandwidth_nrb_table[] =
{
    {15, 5,25},
    {15,10,52},
    {15,15,79},
    {15,20,106},
    {15,25,133},
    {15,30,160},
    {15,40,216},
    {15,50,270},
    {30, 5,11},
    {30,10,24},
    {30,15,38},
    {30,20,52},
    {30,25,65},
    {30,30,78},
    {30,40,106},
    {30,50,133},
    {30,60,162},
    {30,70,189},
    {30,80,217},
    {30,90,245},
    {30,100,273},
    {60,10,11},
    {60,15,18},
    {60,20,24},
    {60,25,31},
    {60,30,38},
    {60,40,51},
    {60,50,65},
    {60,60,79},
    {60,70,93},
    {60,80,107},
    {60,90,121},
    {60,100,135}
};


/// Table 5.4.2.3-1: Applicable NR-ARFCN per operating band in FR1  ----------------------------------------------------------------------------------------------------------
/// Table 5.2-1: NR operating bands in FR1
typedef struct nr_bandentry_s
{
    int16_t band;
    uint64_t ul_min_arfcn;
    uint64_t ul_max_arfcn;
    uint64_t dl_min_arfcn;
    uint64_t dl_max_arfcn;
    uint64_t ul_min_freq;
    uint64_t ul_max_freq;
    uint64_t dl_min_freq;
    uint64_t dl_max_freq;
    uint64_t step_size_arfcn;
    uint8_t deltaf_raster;
} nr_bandentry_t;


static const nr_bandentry_t nr_bandtable[] =
{
    {77,  620000, 680000, 620000, 680000, 3300,4200,3300,4200, 1, 15},
    {77,  620000, 680000, 620000, 680000, 3300,4200,3300,4200, 2, 30},
    {78,  620000, 653333, 620000, 653333, 3300,3800,3300,3800, 1, 15},
    {78,  620000, 653332, 620000, 653332, 3300,3800,3300,3800, 2, 30},
    {79,  693334, 733333, 693334, 733333, 4400,5000,4400,5000, 1, 15},
    {79,  693334, 733332, 693334, 733332, 4400,5000,4400,5000, 2, 30}
};



/// Table 5.4.2.1-1: NR-ARFCN parameters for the global frequency raster  ----------------------------------------------------------------------------------

typedef struct nr_global_frequency_raster_s
{
    uint64_t freq_min;
    uint64_t freq_max;
    uint16_t deltaFglobal;
    float F_REF_Offs;
    uint64_t N_REF_Offs;
    uint64_t arfcn_min;
    uint64_t arfcn_max;
} nr_global_frequency_raster_t;


static nr_global_frequency_raster_t nr_global_frequency_raster[] =
{
    {0, 3000, 5, 0, 0, 0, 599999},
    {3000, 24250, 15, 3000, 600000, 600000, 2016666},
    {24250, 100000, 60, 24250.080, 2016667,3279165}
};


///  --------------------------------------------------------------------------------------------------------------------------------------------------------



/// Table 5.4.3.3-1: Applicable SS raster entries per operating band (FR1)  ---------------------------------------------------------------------------------
///

typedef enum
{
    ssb_case_A = 0,
    ssb_case_B,
    ssb_case_C,
    ssb_case_D, //FR2
    ssb_case_E  //FR2
} SSB_pattern_t;

typedef struct SS_raster_per_band_s
{
    int16_t band; //kHz
    uint16_t ssb_scs; // kHz
    SSB_pattern_t SSB_pattern;
    uint64_t ssb_gscn_min;
    uint64_t ssb_gscn_max;
    uint64_t step_ssb_gscn;

} SS_raster_per_band_t;

static const SS_raster_per_band_t SS_raster_per_band_tabl[] =
{
    {77, 30, ssb_case_C, 7711, 8329, 1},
    {78, 30, ssb_case_C, 7711, 8051, 1},
    {79, 30, ssb_case_C, 8480, 8880, 16}
};


/// Table 5.1-1: Definition of frequency ranges ------------------------------------------------------------------------------------------------
///

typedef enum
{
    nr_FR1 = 0,
    nr_FR2
} nr_frequency_range_t;

typedef struct frequency_ranges_s
{
    nr_frequency_range_t fr; //kHz
    uint64_t freq_min;
    uint64_t freq_max;
} frequency_ranges_t;

static const frequency_ranges_t frequency_ranges_tabl[] =
{
    {nr_FR1, 410000, 7125000},
    {nr_FR2, 24250000, 52600000}
};


/// B.5.2    Window length (not full, extended by threequarter_fs)  -------------------------------------------------------------------------------------------------------------------------
/// Table B.5.2-1, B.5.2-2, B.5.2-3 specify the EVM window length (W) for normal CP.
/// Table B.5.2-1: EVM window length for normal CP, FR1, 15 kHz SCS
/// Table B.5.2-2: EVM window length for normal CP, FR1, 30 kHz SCS
/// Table B.5.2-3: EVM window length for normal CP, FR1, 60 kHz SCS
/// Table B.5.2-4: EVM window length for extended CP, FR1, 60 kHz SCS

typedef enum {EXTENDED=1,NORMAL=0} lte_prefix_type_t;

typedef struct fft_sample_config_s
{
    lte_prefix_type_t lte_prefix_type; //kHz
    int numerology;
    int16_t bandwidth;
    int fft_size;
    int threequarter_fs;
    //uint16_t nb_prefix_samples;  // Number of prefix samples in all but first symbol of slot
    //uint16_t nb_prefix_samples0;  // Number of prefix samples in first symbol of slot
    //uint32_t samples_per_slot0;  // /// Number of samples in 0th and center slot of a subframe
    //uint32_t samples_per_slotN0; // /// Number of samples in other slots of the subframe
} fft_sample_config_t;

static const fft_sample_config_t fft_sample_config_tabl[] =
{
    {NORMAL, 0, 5, 512,0 },
    {NORMAL, 0, 10, 1024,0 },
    {NORMAL, 0, 15, 1536,0 },
    {NORMAL, 0, 20, 2048,0 },
    {NORMAL, 0, 25, 2048,0 },
    {NORMAL, 0, 30, 3072,0 },
    {NORMAL, 0, 40, 4096,0 },
    {NORMAL, 0, 50, 4096,0 },
    {NORMAL, 1, 5, 256,0 },
    {NORMAL, 1, 10, 512,0 },
    {NORMAL, 1, 15, 768,0 },
    {NORMAL, 1, 20, 1024,0 },
    {NORMAL, 1, 25, 1024,0 },
    {NORMAL, 1, 30, 1536,0 },
    {NORMAL, 1, 40, 2048,0 },
    {NORMAL, 1, 50, 2048,0 },
    {NORMAL, 1, 60, 3072,0 },
    {NORMAL, 1, 70, 3072,0 },
    {NORMAL, 1, 80, 4096,0 },
    {NORMAL, 1, 90, 4096,0 },
    {NORMAL, 1, 100, 4096,0 }

};



#endif // ADD_TS_38104_H

















