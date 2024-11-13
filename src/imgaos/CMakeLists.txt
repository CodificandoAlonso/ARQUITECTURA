# Add to this list all files related to imgaos library
add_library(imgaos
        imageaos.cpp
)
target_link_libraries (imgaos
        PRIVATE
        common Microsoft.GSL::GSL)
