# Add to this list all files related to imgsoa library
add_library(imgsoa
        imagesoa.cpp
)
#USE this
target_link_libraries (imgsoa
PRIVATE
common Microsoft.GSL::GSL)
