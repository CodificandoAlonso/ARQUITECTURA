//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include "common/progargs.hpp"
#include <deque>
#include <common/struct-rgb.hpp>
#include <map>
#include <vector>
using namespace std;

class ImageSOA : public Image {
  public:
    ImageSOA(int argc, vector<string> const & argv);
    int process_operation();
  private:
    bool obtain_args();
    static deque<pair<string, __uint8_t>> same_bgr_vector(deque<pair<string, __uint8_t>> father_vector,
      int value, size_t counter);
    static int check_and_delete(deque<pair<string, __uint8_t>> &color_vector, deque<pair<string, __uint8_t>> left_elems,
      int color,
        deque<pair<string, string>> &Deleteitems);
    map<string, __uint8_t> load_and_map_8();
    [[nodiscard]] int resize();
    [[nodiscard]] int compress();
    static void delete_from_deque(deque<pair<string, __uint8_t>> &deque_general, size_t index);
    static size_t search_in_blue(deque<pair<string, __uint8_t>> & pairs, string & first);
    static deque<pair<string, string>> check_colors_to_delete(deque<pair<string, string>> Deleteitems, int num_left,
                                const deque<pair<string, unsigned char>>& left_elems,
                                deque<pair<string, __uint8_t>> bluevalues);
    [[nodiscard]] int cutfreq();
    string input_file;
    string output_file;
    ifstream if_input_file;
    ofstream of_output_file;
    string format ;
    int width =0;
    int height =0 ;
    int maxval=0;
};

#endif  // IMAGESOA_HPP
