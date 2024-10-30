//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include "common/progargs.hpp"

#include <common/struct-rgb.hpp>
#include <deque>
#include <map>
#include <vector>
using namespace std;

class ImageSOA : public Image {
  public:
    ImageSOA(int argc, vector<string> const & argv);
    int process_operation();

  private:
    bool obtain_args();
    static deque<pair<string, __uint8_t>>
        same_bgr_vector(deque<pair<string, __uint8_t>> father_vector, int value, size_t counter);
    static int check_and_delete(deque<pair<string, __uint8_t>> & color_vector,
                                deque<pair<string, __uint8_t>> left_elems, int color,
                                deque<pair<string, string>> & Deleteitems);
    map<string, __uint8_t> load_and_map_8();
    [[nodiscard]] int resize();
    [[nodiscard]] int compress();
    static void delete_from_deque(deque<pair<string, __uint8_t>> & deque_general, size_t index);
    static size_t search_in_blue(deque<pair<string, __uint8_t>> & pairs, string & first);
    static deque<pair<string, string>>
        check_colors_to_delete(deque<pair<string, string>> Deleteitems, int num_left,
                               deque<pair<string, unsigned char>> const & left_elems,
                               deque<pair<string, __uint8_t>> bluevalues);
    void cutfreq_min(map<__uint32_t, __uint8_t> myMap);
    void cutfreq_max(map<__uint64_t, __uint8_t> myMapBIG);
    [[nodiscard]] int cutfreq();
    int resize_min(ofstream & output_file);
    soa_rgb_small read_image_rgb_small(ifstream & input_file) const;
    soa_rgb_big read_image_rgb_big(ifstream & input_file) const;
    static array<rgb_small, 4> obtain_square(soa_rgb_small const & image,
                                             array<unsigned int, 5> args);
    static rgb_small interpolate(double u_param, array<rgb_small, 4> square, double t_param);
};

#endif  // IMAGESOA_HPP
