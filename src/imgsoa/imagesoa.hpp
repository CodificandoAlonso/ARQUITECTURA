//
// Created by beto on 4/10/24.
//

#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include "common/progargs.hpp"

#include <common/AVLTree.hpp>
#include <common/struct-rgb.hpp>
#include <deque>
#include <list>
#include <unordered_map>
#include <vector>




using namespace std;
static int constexpr FIVE = 5;

class ImageSOA : public Image {
  public:
    ImageSOA(int argc, vector<string> const & argv);
    int process_operation();

  private:

    unordered_map<__uint32_t, __uint16_t> cf_load_and_map_8(int width, ifstream input_file,
                                                         int height);
    unordered_map<__uint64_t, __uint16_t> cf_load_and_map_8BIG(int width, ifstream input_file,
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
    static void cp_export(ofstream & output_file,
                          unordered_map<unsigned int, unsigned int> const & color_map,
                          list<unsigned int> const & indexes);
    static void cf_delete_from_deque(deque<pair<__uint32_t, __uint16_t>> & deque_general,
                                  size_t index);

    void cf_add_nodes();

    void cf_add_nodes_BIG(__uint16_t POCOBIG, __uint16_t MEDIOBIG, __uint16_t ALTOBIG);

    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> cf_generate_graph();

    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> cf_generate_graph_BIG();

    deque<pair<__uint32_t, __uint16_t>>
        cf_check_first_part_small(unordered_map<__uint32_t, __uint16_t> myMap,
                                  unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                  int & num_left) const;

    deque<pair<__uint64_t, __uint16_t>>
        cf_check_first_part_BIG(unordered_map<__uint64_t, __uint16_t> myMapBIG,
                                unordered_map<__uint64_t, __uint64_t> & Deleteitems,
                                int & num_left) const;

    void cf_write_in_exit(unordered_map<__uint32_t, __uint32_t> Deleteitems);

    void cf_write_in_exit_BIG(unordered_map<__uint64_t, __uint64_t> Deleteitems);

    void cutfreq_min(const unordered_map<__uint32_t, __uint16_t>& myMap);

    void cutfreq_max(const unordered_map<__uint64_t, __uint16_t>& myMapBIG);

    [[nodiscard]] int cutfreq();

    soa_rgb_small read_image_rgb_small(ifstream & input_file) const;

    soa_rgb_big read_image_rgb_big(ifstream & input_file) const;

    soa_rgb_small soa_small;
    soa_rgb_big soa_big;
    vector<__uint32_t> nod;
    vector<__uint64_t> nodBIG;

    friend class ImageSOATest_CompressOperation_Test;
};

#endif  // IMAGESOA_HPP
