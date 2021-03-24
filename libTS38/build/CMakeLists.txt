cmake_minimum_required (VERSION 3.5)

set (PROJECT libTS38)

project (${PROJECT})

set (HEADERS ../headers/TS_38101_4_UE_tx_rx.h
             ../headers/TS_38104_BS_Radio_tx_rx_table.h
             ../headers/TS_38211_RL16_Phy_channels_modulation.h
             ../headers/TS_38213_PHY_Layer_Procedure.h
             ../headers/TS_38331_RL16_RRC.h
             ../headers/TS_38212_RL16_Multiplexing_channelcoding.h
             ../headers/TS_38212_nr_polar_kronecker_power_matrices.h
             )


set (SOURCES ../sources/TS_38101_4_UE_tx_rx.cpp
             ../sources/TS_38104_BS_Radio_tx_rx_table.cpp
             ../sources/TS_38211_RL16_Phy_channels_modulation.cpp
             ../sources/TS_38213_PHY_Layer_Procedure.cpp
             ../sources/TS_38331_RL16_RRC.cpp
             ../sources/TS_38212_RL16_Multiplexing_channelcoding.cpp
             ../sources/TS_38212_nr_polar_kronecker_power_matrices.cpp
             )


#source_group ("Header Files" FILES ${HEADERS})
#source_group ("Source Files" FILES ${SOURCES})

add_library (${PROJECT} ${HEADERS} ${SOURCES})
