cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0135 NEW) # Подавляет предупреждение о DOWNLOAD_EXTRACT_TIMESTAMP
project(pi_mpi LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Поиск MPI
find_package(MPI REQUIRED)

include(ExternalProject)

set(GSL_VERSION 2.8)
set(GSL_URL "https://ftp.gnu.org/gnu/gsl/gsl-${GSL_VERSION}.tar.gz")
set(GSL_INSTALL_DIR "${CMAKE_BINARY_DIR}/gsl_install")

# 2. Cоздание директорий и файлов-заглушек
file(MAKE_DIRECTORY ${GSL_INSTALL_DIR}/include/gsl)
file(MAKE_DIRECTORY ${GSL_INSTALL_DIR}/lib)
file(TOUCH ${GSL_INSTALL_DIR}/include/gsl/gsl_rng.h)
file(TOUCH ${GSL_INSTALL_DIR}/lib/libgsl.a)
file(TOUCH ${GSL_INSTALL_DIR}/lib/libgslcblas.a)

# 3. Настройка внешней сборки GSL из исходников
ExternalProject_Add(gsl_external
    URL ${GSL_URL}
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    PREFIX ${CMAKE_BINARY_DIR}/gsl_build
    
    SOURCE_DIR ${CMAKE_BINARY_DIR}/gsl_build/src/gsl_external/gsl-${GSL_VERSION}
    
    CONFIGURE_COMMAND <SOURCE_DIR>/configure 
        --prefix=${GSL_INSTALL_DIR} 
        --enable-static 
        --disable-shared 
        CC=${CMAKE_C_COMPILER} 
        CXX=${CMAKE_CXX_COMPILER}
    
    BUILD_COMMAND $(MAKE) 
    INSTALL_COMMAND $(MAKE) install
    
    BUILD_BYPRODUCTS 
        ${GSL_INSTALL_DIR}/lib/libgsl.a 
        ${GSL_INSTALL_DIR}/lib/libgslcblas.a
    
    LOG_BUILD ON
    LOG_INSTALL ON
)

# 4. Регистрация собранных библиотек для линковки
add_library(GSL::gsl STATIC IMPORTED)
add_dependencies(GSL::gsl gsl_external)
set_target_properties(GSL::gsl PROPERTIES
    IMPORTED_LOCATION "${GSL_INSTALL_DIR}/lib/libgsl.a"
    INTERFACE_INCLUDE_DIRECTORIES "${GSL_INSTALL_DIR}/include"
)

add_library(GSL::cblas STATIC IMPORTED)
add_dependencies(GSL::cblas gsl_external)
set_target_properties(GSL::cblas PROPERTIES
    IMPORTED_LOCATION "${GSL_INSTALL_DIR}/lib/libgslcblas.a"
)

# 5. Сборка основного проекта
set(SOURCES
    src/main.cpp
    src/pi_calculator.cpp
    src/mpi_helpers.cpp
)

add_executable(pi_mpi ${SOURCES})

target_include_directories(pi_mpi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(pi_mpi PRIVATE 
    MPI::MPI_CXX 
    GSL::gsl 
    GSL::cblas 
    m
)

target_compile_options(pi_mpi PRIVATE 
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:RelWithDebInfo>:-O2 -g>
)

install(TARGETS pi_mpi DESTINATION bin)