# Use the code in this section for standalone projects
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Rx_Interaction)
if(NOT TARGET Garfield::Garfield)
  find_package(Garfield REQUIRED)
endif()

# ---Build executable------------------------------------------------------------
add_executable(Rx_Interaction RX_Interaction_GEM.C)
target_link_libraries(Rx_Interaction Garfield::Garfield)