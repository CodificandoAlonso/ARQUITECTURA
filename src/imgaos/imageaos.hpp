//
// Created by beto on 4/10/24.
//

#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include "common/progargs.hpp"

#include <common/struct-rgb.hpp>
#include <list>
#include <unordered_map>
#include <vector>

using namespace std;
static int constexpr FIVE = 5;

class ImageAOS : public Image {
  public:
    ImageAOS(int argc, std::vector<string> const & argv);
    int process_operation();

  private:
    [[nodiscard]] int resize();
    int resize_min(ofstream & output_file);
    static array<rgb_small, 4> rsz_obtain_square_min(vector<rgb_small> const & image,
                                                     array<unsigned int, FIVE> args);
    static rgb_small rsz_interpolate_min(double u_param, array<rgb_small, 4> square,
                                         double t_param);
    int resize_max(ofstream & output_file);
    static array<rgb_big, 4> rsz_obtain_square_max(vector<rgb_big> const & image,
                                                   array<unsigned int, FIVE> args);
    static rgb_big rsz_interpolate_max(double u_param, array<rgb_big, 4> square, double t_param);
    vector<rgb_small> read_image_rgb_small(ifstream & input_file) const;
    vector<rgb_big> read_image_rgb_big(ifstream & input_file) const;
    [[nodiscard]] int compress();
    int compress_min();
    int compress_max();
    static void cp_export(ofstream & output_file,
                          unordered_map<unsigned int, unsigned int> const & color_map,
                          list<unsigned int> const & indexes);
    static void cp_export_BIG(ofstream & output_file,
                              unordered_map<unsigned long int, unsigned int> const & color_map,
                              list<unsigned int> const & indexes);

    unordered_map<__uint32_t, __uint16_t> cf_load_and_map_8(int width, ifstream input_file,
                                                            int height);

    unordered_map<__uint64_t, __uint16_t> cf_load_and_map_8BIG(int width, ifstream input_file,
                                                               int height);

    void cf_add_nodes();

    void cf_add_nodes_BIG(__uint16_t POCOBIG, __uint16_t MEDIOBIG, __uint16_t ALTOBIG);

    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> cf_generate_graph();
    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> cf_generate_graph_2(
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph);

    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> cf_generate_graph_3(
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph);
    unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> cf_generate_graph_4(
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> & graph);
    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> cf_generate_graph_BIG();
    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> cf_generate_graph_BIG_2(
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> & graph);
    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> cf_generate_graph_BIG_3(
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> & graph);
    unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> cf_generate_graph_BIG_4(
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> & graph);

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
    static void cf_search_in_graph_small(
        unordered_map<__uint32_t, __uint32_t> & Deleteitems,
        unordered_map<__uint32_t, pair<vector<__uint32_t>, vector<__uint32_t>>> graph);
    static void cf_search_in_graph_BIG(
        unordered_map<__uint64_t, __uint64_t> & Deleteitems,
        unordered_map<__uint64_t, pair<vector<__uint64_t>, vector<__uint64_t>>> graph);

    void cutfreq_min(unordered_map<__uint32_t, __uint16_t> const & myMap);

    void cutfreq_max(unordered_map<__uint64_t, __uint16_t> const & myMapBIG);
    [[nodiscard]] int cutfreq();

    vector<__uint32_t> nod;
    vector<__uint64_t> nodBIG;
    vector<rgb_small> array_small;
    vector<rgb_big> array_big;

    friend class ImageAOSTest;
    friend class ImageAOSTest_RszObtainSquareMax_Test;
    friend class ImageAOSTest_RszObtainSquareMaxFailure_Test;
    friend class ImageAOSTest_RszObtainSquareMin_Test;
    friend class ImageAOSTest_RszObtainSquareMinFailure_Test;
    friend class ImageAOSTest_RszInterpolateMinSuccess_Test;
    friend class ImageAOSTest_RszInterpolateMinFailure_Test;
    friend class ImageAOSTest_RszInterpolateMaxSuccess_Test;
    friend class ImageAOSTest_RszInterpolateMaxOutOfBounds_Test;
    friend class ImageAOSTest_RszInterpolateMaxInvalidUParam_Test;
    friend class ImageAOSTest_RszInterpolateMaxInvalidTParam_Test;
    friend class ImageAOSTest_ReadImageRGBSmallSuccess_Test;
    friend class ImageAOSTest_ReadImageRGBSmallFileNotFound_Test;
    friend class ImageAOSTest_ReadImageRGBBig_Success_Test;
    friend class ImageAOSTest_ReadImageRGBBig_FileNotOpen_Test;
    friend class ImageAOSTest_LoadAndMap8_Success_Test;
    friend class ImageAOSTest_CfLoadAndMap8BIGSuccess_Test;
    friend class ImageAOSTest_CfLoadAndMap8BIG_FileNotOpen_Test;
    friend class ImageAOSTest_CfLoadAndMap8BIG_InvalidWidth_Test;
    friend class ImageAOSTest_CfLoadAndMap8BIG_InvalidHeight_Test;
    friend class ImageAOSTest_CfLoadAndMap8BIG_NegativeWidth_Test;
    friend class ImageAOSTest_CfLoadAndMap8BIG_NegativeHeight_Test;
    friend class ImageAOSTest_CfAddNodes_Test;
    friend class ImageAOSTest_CfAddNodes_Failure_Test;
    friend class ImageAOSTest_CfAddNodesBIG_Success_Test;
    friend class ImageAOSTest_CfAddNodesBIG_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraph_Success_Test;
    friend class ImageAOSTest_CfGenerateGraph_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraph2_Success_Test;
    friend class ImageAOSTest_CfGenerateGraph2_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraph3_Success_Test;
    friend class ImageAOSTest_CfGenerateGraph3_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraph4_Success_Test;
    friend class ImageAOSTest_CfGenerateGraph4_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG_Success_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG2_Success_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG2_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG3_Success_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG3_Failure_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG4_Success_Test;
    friend class ImageAOSTest_CfGenerateGraphBIG4_Failure_Test;
    friend class ImageAOSTest_CfSearchInGraphSmall_SomeColorsInDeleteitems_Test;
    friend class ImageAOSTest_CfSearchInGraphSmall_AllColorsInDeleteitems_Test;
    friend class ImageAOSTest_CfSearchInGraphBIG_SomeColorsInDeleteitems_Test;
    friend class ImageAOSTest_CfSearchInGraphBIG_AllColorsInDeleteitems_Test;
    friend class ImageAOSTest_CpExport_LessThan256Colors_Test;
    friend class ImageAOSTest_CpExport_LessThan65536Colors_Test;
    friend class ImageAOSTest_CpExport_MoreThan65536Colors_Test;
    friend class ImageAOSTest_CpExportBIG_LessThan256Colors_Test;
    friend class ImageAOSTest_CpExportBIG_LessThan65536Colors_Test;
    friend class ImageAOSTest_CpExportBIG_LessThan4294967296Colors_Test;

};


#endif  // IMAGEAOS_HPP
