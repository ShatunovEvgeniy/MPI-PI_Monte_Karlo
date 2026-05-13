cmake_minimum_required(VERSION 3.10)
project(pi_mpi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск MPI
find_package(MPI REQUIRED)

# Поиск GSL
find_package(GSL QUIET)
if(NOT GSL_FOUND)
    find_library(GSL_LIB gsl HINTS /usr/lib /usr/local/lib)
    find_library(GSL_CBLAS_LIB gslcblas HINTS /usr/lib /usr/local/lib)
    find_path(GSL_INCLUDE gsl/gsl_rng.h HINTS /usr/include /usr/local/include)
    
    if(GSL_LIB AND GSL_INCLUDE)
        add_library(GSL::gsl INTERFACE IMPORTED)
        set_target_properties(GSL::gsl PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${GSL_INCLUDE}"
            INTERFACE_LINK_LIBRARIES "${GSL_LIB};${GSL_CBLAS_LIB};m"
        )
        set(GSL_FOUND TRUE)
    else()
        message(FATAL_ERROR "GSL library not found. Install libgsl-dev")
    endif()
endif()

# Исходные файлы
set(HEADERS
    include/config.hpp
    include/pi_calculator.hpp
    include/mpi_helpers.hpp
)

set(SOURCES
    src/main.cpp
    src/pi_calculator.cpp
    src/mpi_helpers.cpp
)

# Создаём исполняемый файл
add_executable(pi_mpi ${SOURCES} ${HEADERS})

# Подключаем заголовочные файлы
target_include_directories(pi_mpi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Линкуем зависимости
target_link_libraries(pi_mpi PRIVATE 
    MPI::MPI_CXX 
    GSL::gsl
)

# Флаги оптимизации для релиза
target_compile_options(pi_mpi PRIVATE 
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:RelWithDebInfo>:-O2 -g>
)

# Установка (опционально)
install(TARGETS pi_mpi DESTINATION bin)