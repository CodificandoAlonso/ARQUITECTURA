//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include "common/progargs.hpp"

#include <common/AVLTree.hpp>
#include <common/struct-rgb.hpp>
#include <deque>
#include <unordered_map>
#include <vector>

using namespace std;
static int constexpr FIVE = 5;

class ImageSOA : public Image {
  public:
    ImageSOA(int argc, vector<string> const & argv);
    int process_operation();

  private:
    static deque<pair<__uint32_t, __uint16_t>>
        same_bgr_vector(deque<pair<__uint32_t, __uint16_t>> father_vector, int value,
                        size_t counter);
    static int check_and_delete(deque<pair<__uint32_t, __uint16_t>> & color_vector, int color,
                                unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                deque<pair<__uint32_t, __uint16_t>> & bluevalues);
    unordered_map<__uint32_t, __uint16_t> load_and_map_8(int width, ifstream input_file,
                                                         int height);
    static vector<__uint32_t> sort_and_map_keys(unordered_map<__uint32_t, __uint16_t> const & myMap,
                                         unordered_map<__uint32_t, size_t> & color_to_index);
    unordered_map<__uint64_t, __uint16_t> load_and_map_8BIG(int width, ifstream input_file,
                                                            int height);

    [[nodiscard]] int resize();
    int resize_min(ofstream & output_file);
    static array<rgb_small, 4> rsz_obtain_square_min(soa_rgb_small const & image,
                                                     array<unsigned int, FIVE> args);
    static rgb_small rsz_interpolate_min(double u_param, array<rgb_small, 4> square,
                                         double t_param);
    int resize_max(ofstream & output_file);
    static array<rgb_big, 4> rsz_obtain_square_max(soa_rgb_big const & image,
                                                   array<unsigned int, FIVE> args);
    static rgb_big rsz_interpolate_max(double u_param, array<rgb_big, 4> square, double t_param);
    [[nodiscard]] int compress();
    int compress_min();
    int compress_max();
    void cp_export_min(ofstream & output_file, AVLTree tree, soa_rgb_small const & image);
    void cp_export_max(ofstream & output_file, AVLTree tree, soa_rgb_big const & image);
    static void delete_from_deque(deque<pair<__uint32_t, __uint16_t>> & deque_general,
                                  size_t index);
    static size_t search_in_blue(deque<pair<__uint32_t, __uint16_t>> & pairs, __uint32_t & first);
    static unordered_map<__uint32_t, __uint32_t>
        check_colors_to_delete(unordered_map<__uint32_t, __uint32_t> Deleteitems, int num_left,
                               deque<pair<__uint32_t, __uint16_t>> bluevalues);
    static __uint32_t get_aitems(size_t index, vector<__uint32_t> const & sorted_colors,
                                 unordered_map<__uint32_t, __uint32_t> const & Deleteitems);

    void cutfreq_min(unordered_map<__uint32_t, __uint16_t> myMap);
    static void cutfreq_max(unordered_map<__uint64_t, __uint16_t> myMapBIG);
    [[nodiscard]] int cutfreq();
    soa_rgb_small read_image_rgb_small(ifstream & input_file) const;
    soa_rgb_big read_image_rgb_big(ifstream & input_file) const;
    soa_rgb_small soa_small;
    soa_rgb_small soa_big;
};

#endif  // IMAGESOA_HPP
