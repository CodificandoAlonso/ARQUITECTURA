# Add to this list all files related to common library
add_library(common
        progargs.cpp
        binario.cpp
        struct-rgb.cpp
)
target_link_libraries (common
        PUBLIC
        common Microsoft.GSL::GSL)