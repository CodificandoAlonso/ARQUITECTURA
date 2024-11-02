  //
  // Created by beto on 4/10/24.
  //

  #ifndef PROGARGS_HPP
  #define PROGARGS_HPP

  #include <fstream>
  #include <string>
  #include <vector>
  using namespace std;

  class Image {
    public:
      Image(int argc, vector<string> const & argv);
      static bool info_constraints(int argc);
      bool maxval_constraints(int argc, vector<string> const & argv);
      bool resize_constraints(int argc, vector<string> const & argv);
      bool cutfreq_constraints(int argc, vector<string> const & argv);
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
  };

  #endif  // PROGARGS_HPP
