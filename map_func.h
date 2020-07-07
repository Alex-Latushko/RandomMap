#ifndef MAP_FUNC_H
#define MAP_FUNC_H

#include <iostream>
#include <ctime>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>



class Map_settings{
private:
    int height;
    int width;
    int main_size;
    std::vector<char> areas{'G', 'D', 'F', 'H'};
    void calc_main_size();
    std::vector<std::string> str_map;
public:
    const size_t min_side_length = 15;
    const size_t max_side_length = 100;
    double sq_coord[10] { 1.0 / 4, 1.0 / 4,
                          1.0 / 4, 3.0 / 4,
                          3.0 / 4, 1.0 / 4,
                          3.0 / 4, 3.0 / 4,
                          1.0 / 2, 1.0 / 2 };
    bool mountain_chain_on = true;
    bool water_frame_on = true;
    bool two_islands = true;
    Map_settings(int h = 16, int w = 20);
    void set_height(int h);
    int get_height() const;

    int get_width() const;
    void set_width(int w);

    int get_main_size() const;
    std::vector<char>& get_areas();

    void set_str_map(std::vector<std::string>& s_m);
    std::vector<std::string>& get_str_map();
};

class Cell{
private:
    char type;
    std::string color_t;
public:
    char get_type() const ;
    void set_type(char t);
    const std::string get_color_t() const ;
    void set_color_t(std::string col);
    Cell(char t = 'W', std::string col = "\x1b[34m" ); //blue
};

class Grassland:public Cell{
    public:
    Grassland ();
};

class Desert:public Cell{
    public:
    Desert ();
};

class Forest:public Cell{
    public:
    Forest ();
};

class Mountains:public Cell{
    public:
    Mountains();
};

class Swamp:public Cell{
    public:
    Swamp();
};

typedef std::vector< std::vector<Cell*> > Map;

void map_init(Map& map, Map_settings* settings);
void map_gen(Map& map, Map_settings* settings);
void map_reset(Map& map, Map_settings* settings);
void map_free(Map& map);
void map_load(Map& map, Map_settings* settings);
void map_actualize_str_map(Map& map, Map_settings* settings);


#endif // MAP_FUNC_H
