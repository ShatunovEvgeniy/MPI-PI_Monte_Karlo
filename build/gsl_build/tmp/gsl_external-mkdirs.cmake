# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/src/gsl_external/gsl-2.8"
  "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/src/gsl_external-build"
  "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build"
  "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/tmp"
  "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/src/gsl_external-stamp"
  "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/src"
  "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/src/gsl_external-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/src/gsl_external-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/evgeniy/MPI-PI_Monte_Karlo/build/gsl_build/src/gsl_external-stamp${cfgdir}") # cfgdir has leading slash
endif()
