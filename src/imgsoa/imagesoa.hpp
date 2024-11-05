//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include "common/progargs.hpp"

#include <common/struct-rgb.hpp>
#include <deque>
#include <unordered_map>
#include <vector>


using namespace std;
static int constexpr CINCO = 5;

class ImageSOA : public Image {
  public:
    ImageSOA(int argc, vector<string> const & argv);
    int process_operation();
    void set_input_file(const std::string& inputFile);
    void set_output_file(const std::string& outputFile);
    int (*get_compress())();

  private:
    bool obtain_args();
    static deque<pair<__uint32_t, __uint16_t>> same_bgr_vector(deque<pair<__uint32_t, __uint16_t>> father_vector
                                                                                  , int value, size_t counter);
    static int check_and_delete(deque<pair<__uint32_t, __uint16_t>> & color_vector, int color,
                                unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                deque<pair<__uint32_t, __uint16_t>> & bluevalues);
    unordered_map<__uint32_t, __uint16_t> load_and_map_8(int width, ifstream input_file, int height);
    unordered_map<__uint64_t, __uint16_t> load_and_map_8BIG(int width, ifstream input_file,
      int height);

    [[nodiscard]] int resize();
    [[nodiscard]] int compress();
    static void delete_from_deque(deque<pair<__uint32_t, __uint16_t>> & deque_general, size_t index);
    static size_t search_in_blue(deque<pair<__uint32_t, __uint16_t>> & pairs, __uint32_t & first);
    static unordered_map<__uint32_t, __uint32_t> check_colors_to_delete(
                                  unordered_map<__uint32_t, __uint32_t> Deleteitems, int num_left,
                               deque<pair<__uint32_t, __uint16_t>> bluevalues);
    void cutfreq_min(unordered_map<__uint32_t, __uint16_t> myMap);
    static void cutfreq_max(unordered_map<__uint64_t, __uint16_t> myMapBIG);
    [[nodiscard]] int cutfreq();
    int resize_min(ofstream & output_file);
    soa_rgb_small read_image_rgb_small(ifstream & input_file) const;
    soa_rgb_big read_image_rgb_big(ifstream & input_file) const;
    static array<rgb_small, 4> obtain_square(soa_rgb_small const & image,
                                             array<unsigned int, CINCO> args);
    static rgb_small interpolate(double u_param, array<rgb_small, 4> square, double t_param);
    soa_rgb_small soa_small;
    soa_rgb_small soa_big;

    std::string input_file;
    std::string output_file;
};

#endif  // IMAGESOA_HPP
