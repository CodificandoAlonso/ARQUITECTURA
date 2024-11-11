  //
  // Created by beto on 4/10/24.
  //

  #ifndef PROGARGS_HPP
  #define PROGARGS_HPP

  #include <fstream>
#include <memory>
#include <string>
  #include <vector>
#include<deque>
#include<unordered_map>
  using namespace std;

  class Image {
    public:
      Image(int argc, const std::vector<std::string>& argv);
      static bool info_constraints(int argc);
      bool maxval_constraints(int argc, const std::vector<std::string>& argv);
      bool resize_constraints(int argc, const std::vector<std::string>& argv);
      bool cutfreq_constraints(int argc, const std::vector<std::string>& argv);
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

      [[nodiscard]] ifstream get_if_input_file() { return move(if_input_file); }
      [[nodiscard]] ofstream get_of_output_file() { return move(of_output_file); }

      const std::vector<std::string>& getArgv() const;

      static const unique_ptr<Image>& getImage();

static deque<pair<__uint32_t, __uint16_t>>
        cf_same_bgr_vector(deque<pair<__uint32_t, __uint16_t>> father_vector, int value,
                        size_t counter);

    static deque<pair<__uint64_t, __uint16_t>>
        cf_same_bgr_vector_BIG(deque<pair<__uint64_t, __uint16_t>> father_vector, int value,
                               size_t counter);

    static int cf_check_and_delete(deque<pair<__uint32_t, __uint16_t>> & color_vector, int color,
                                unordered_map<__uint32_t, __uint32_t> & Deleteitems,
                                deque<pair<__uint32_t, __uint16_t>> & bluevalues);

    static int cf_check_and_delete_BIG(deque<pair<__uint64_t, __uint16_t>> & color_vector, int color,
                                unordered_map<__uint64_t, __uint64_t> & Deleteitems,
                                deque<pair<__uint64_t, __uint16_t>> & bluevalues);
    static void cf_delete_from_deque_BIG(deque<pair<__uint64_t, __uint16_t>> & deque_general,
                                  size_t index);

    static void cf_delete_from_deque(deque<pair<__uint32_t, __uint16_t>> & deque_general,
                                 size_t index);

    static size_t cf_search_in_blue_BIG(deque<pair<__uint64_t, unsigned short>> & pairs,
                                 __uint64_t & first);

    static size_t cf_search_in_blue(deque<pair<__uint32_t, __uint16_t>> & pairs, __uint32_t & first);

    static unordered_map<__uint32_t, __uint32_t>
        cf_check_colors_to_delete(unordered_map<__uint32_t, __uint32_t> Deleteitems, int num_left,
                               deque<pair<__uint32_t, __uint16_t>> bluevalues);

    static unordered_map<__uint64_t, __uint64_t>
        cf_check_colors_to_delete_BIG(unordered_map<__uint64_t, __uint64_t> Deleteitems,
                                      int num_left, deque<pair<__uint64_t, __uint16_t>> bluevalues);

    static __uint32_t cf_find_closest_in_neighbors(
        __uint32_t color_to_delete,
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> const & graph,
        vector<__uint32_t> const & neighbors, double & min_distance,
        unordered_map<__uint32_t, __uint8_t> & visited_node);

    __uint64_t cf_find_closest_in_neighbors_BIG(
        __uint64_t color_to_delete,
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> const & graph,
        vector<__uint64_t> const & neighbors, double & min_distance,
        unordered_map<__uint64_t, __uint8_t> & visited_node);

    static void cf_finish_graph(
        const unordered_map<__uint32_t, __uint16_t>& myMap,
        unordered_map<__uint32_t, __uint32_t> & Deleteitems,
        unordered_map<__uint32_t, __uint32_t> & toSave,
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph);

    static void cf_finish_graph_BIG(
        unordered_map<__uint64_t, __uint16_t> myMap,
        unordered_map<__uint64_t, __uint64_t> & Deleteitems,
        unordered_map<__uint64_t, __uint64_t> & toSave,
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> & graph);




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

  };

  #endif  // PROGARGS_HPP
