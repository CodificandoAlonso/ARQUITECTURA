//
// Created by beto on 4/10/24.
//

#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <deque>
#include <fstream>
#include <memory>

#include <unordered_map>
#include <vector>
using namespace std;

struct params_finish_graph {
    unordered_map<__uint32_t, __uint16_t> const * myMap;
    unordered_map<__uint32_t, __uint32_t> * Deleteitems;
    unordered_map<__uint32_t, __uint32_t> * toSave;
    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> * graph;
};

struct params_finish_graph_BIG {
    unordered_map<__uint64_t, __uint16_t> const * myMap;
    unordered_map<__uint64_t, __uint64_t> * Deleteitems;
    unordered_map<__uint64_t, __uint64_t> * toSave;
    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> * graph;
};

struct cf_find_neigh_small {
    __uint32_t color_to_delete;
    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> const * graph;
    vector<__uint32_t> const * neighbors;
    double * min_distance;
    unordered_map<__uint32_t, __uint8_t> * visited_node;
};

struct cf_find_neigh_BIG {
    __uint64_t color_to_delete;
    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> const * graph;
    vector<__uint64_t> const * neighbors;
    double * min_distance;
    unordered_map<__uint64_t, __uint8_t> * visited_node;
};

struct params_same_vector_small {
    deque<pair<__uint32_t, __uint16_t>> father_vector;
    int value = 0;
    size_t counter = 0;
};

struct params_same_vector_BIG {
    deque<pair<__uint64_t, __uint16_t>> father_vector;
    int value = 0;
    size_t counter = 0;
};

struct params_equal_blu {
    unordered_map<__uint32_t, __uint32_t> * Deleteitems;
    int * num_left;
    deque<pair<__uint32_t, __uint16_t>> * bluevalues;
    size_t my_index;
    int * my_meanwhile;
};

struct params_equal_blu_BIG {
    unordered_map<__uint64_t, __uint64_t> * Deleteitems;
    int * num_left;
    deque<pair<__uint64_t, __uint16_t>> * bluevalues;
    size_t my_index;
    int * my_meanwhile;
};



class Image {
  public:
    Image(int argc, std::vector<std::string> const & argv);
    static bool info_constraints(int argc);
    bool maxval_constraints(int argc, std::vector<std::string> const & argv);
    bool resize_constraints(int argc, std::vector<std::string> const & argv);
    bool cutfreq_constraints(int argc, std::vector<std::string> const & argv);
    static bool compress_constraints(int argc);
    [[nodiscard]] int check_args();
    [[nodiscard]] int info() const;
    int maxlevel();
    void write_out(int level);
    void get_imgdata();

    [[nodiscard]] string get_input_file() const { return input_file; }

    [[nodiscard]] string get_output_file() const { return output_file; }

    [[nodiscard]] string get_optype() const { return optype; }

    [[nodiscard]] vector<int> get_args() const { return args; }

    [[nodiscard]] string get_format() const { return format; }

    [[nodiscard]] int get_width() const { return width; }

    [[nodiscard]] int get_height() const { return height; }

    [[nodiscard]] int get_maxval() const { return maxval; }

    [[nodiscard]] ifstream get_if_input_file() { return std::move(if_input_file); }

    [[nodiscard]] ofstream get_of_output_file() { return std::move(of_output_file); }

    static deque<pair<__uint32_t, __uint16_t>> cf_same_bgr_vector(params_same_vector_small params);

    static deque<pair<__uint64_t, __uint16_t>>
        cf_same_bgr_vector_BIG(params_same_vector_BIG params);

    static int cf_check_and_delete(deque<pair<__uint32_t, __uint16_t>> & color_vector, int color,
                                   unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                   deque<pair<__uint32_t, __uint16_t>> & bluevalues);

    static int cf_check_and_delete_BIG(deque<pair<__uint64_t, __uint16_t>> & color_vector,
                                       int color,
                                       unordered_map<__uint64_t, __uint64_t> & Deleteitems,
                                       deque<pair<__uint64_t, __uint16_t>> & bluevalues);
    static void cf_delete_from_deque_BIG(deque<pair<__uint64_t, __uint16_t>> & deque_general,
                                         size_t index);

    static void cf_delete_from_deque(deque<pair<__uint32_t, __uint16_t>> & deque_general,
                                     size_t index);

    static size_t cf_search_in_blue_BIG(deque<pair<__uint64_t, unsigned short>> & pairs,
                                        __uint64_t & first);

    static size_t cf_search_in_blue(deque<pair<__uint32_t, __uint16_t>> & pairs,
                                    __uint32_t & first);
    static void cf_delete_and_rest(unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                   int & num_left, deque<pair<__uint32_t, __uint16_t>> & bluevalues,
                                   size_t index);
    static int cf_check_just_blue(unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                  deque<pair<__uint32_t, __uint16_t>> & bluevalues, int & num_left);
    static void cf_delete_first_blue_value(unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                           int & num_left,
                                           deque<pair<__uint32_t, __uint16_t>> & bluevalues);
    static void cf_delete_first_blue_value_BIG(unordered_map<__uint64_t, __uint64_t> & Deleteitems,
                                        int & num_left,
                                        deque<pair<__uint64_t, __uint16_t>> & bluevalues);
    static void cf_equal_blue_case(params_equal_blu * params);
    static void cf_equal_blue_case_BIG(params_equal_blu_BIG * params);

    static unordered_map<__uint32_t, __uint32_t>
        cf_check_colors_to_delete(unordered_map<__uint32_t, __uint32_t> Deleteitems, int num_left,
                                  deque<pair<__uint32_t, __uint16_t>> bluevalues);

    static unordered_map<__uint64_t, __uint64_t>
        cf_check_colors_to_delete_BIG(unordered_map<__uint64_t, __uint64_t> Deleteitems,
                                      int num_left, deque<pair<__uint64_t, __uint16_t>> bluevalues);

    static __uint32_t cf_find_closest_in_neighbors(cf_find_neigh_small const * params);

    static __uint64_t cf_find_closest_in_neighbors_BIG(cf_find_neigh_BIG const * params);

    static void cf_finish_graph(params_finish_graph const * params);

    static void cf_finish_graph_BIG(params_finish_graph_BIG const * params);
      const std::vector<std::string>& getArgv() const;


  private:
    void min_min();
    void max_min();
    void min_max();
    void max_max();
    string input_file;
    string output_file;
    ifstream if_input_file;
    ofstream of_output_file;
    string optype;
    vector<int> args;
    int argc;
    vector<string> argv;
    string format;
    int width  = 0;
    int height = 0;
    int maxval = 0;
    std::unique_ptr<Image> image;
  
  friend class ImageTest_InfoFunction_Test;
  friend class ImageSOATest_ReadImageRGBSmallMemoryError_Test;
};

#endif  // PROGARGS_HPP
