//#include <libINIT/headers/_struct_zdv_cfg.h>
//#include <libOFDM/headers/fft_ifft_radixN.h>
//#include "_nyFFT.h"
//#include <libOFDM/headers/ofdm_modulation.h>
//#include <libTS38/headers/TS_38211_RL16_Phy_channels_modulation.h>
//#include <libTS38/headers/TS_38212_RL16_Multiplexing_channelcoding.h>
//#include <libTS38/headers/TS_38212_nr_polar_kronecker_power_matrices.h>
//#include <libORAN/headers/ORAN_BS_SW_WG8.h>


#include <libINIT/headers/_init.h>
#include <libORAN/headers/ORAN_BS_SW_WG8.h>





int16_t txdataF[200000000] = {0};
int txdata[200000000] = {0};

int FrameN = 5; // number of frames for simulation



int main(int argc, char **argv)
{


    nr_config_full_t cfg; // full configuration structure,
    oran_pbch_proc_config_t oran_pbch_cfg; // config structure for pbch

    // initialize parameters for cell and save to configuration file
    INIT_CELL_CONFIG(&cfg, &oran_pbch_cfg);

    std::cout << "\nENTER Frame NUMBER FOR SIMULATION: ";
    int FrameN;
    std::cin >> FrameN;

    uint32_t SFN = 0;
    while(FrameN>0)
    {


//        nr_generate_pss(&cfg,
//                        &txdataF[0],
//                        512 // amp
//                       );
//
//        nr_generate_sss(&cfg,
//                        &txdataF[0],
//                        512 // amp
//                       );
//
//        nr_generate_pbch_dmrs(&cfg,
//                              &txdataF[0],
//                              512 // amp
//                             );

        ///  O-RAN Working Group 8
        ///  Base Station O-DU and O-CU Software Architecture and APIs
        ///  5.7 Physical Broadcast Channel .............................................. 17
        /// for each SFN it will generate txdataF, based on configuration
        Oran_Pbch_proc(SFN, &oran_pbch_cfg, &txdataF[0]);


        printf("\n txdataF(freq domain signal): \n ");
        for (int i = 0; i < cfg.samples_per_frame*2; ++i)
            if (txdataF[i] != 0)
                printf("[%d] = %hi    ", i,txdataF[i]);


        FrameN = FrameN-1;
        SFN = SFN+1;
    }


    return 0;
}
