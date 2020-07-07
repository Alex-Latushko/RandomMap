#include "map_func.h"

void gen_large_sq_area(Map& map, Map_settings* settings, char T, int cent_h, int cent_w);
void gen_small_areas(Map& map, Map_settings* settings);
void gen_small_area(Map& map, Map_settings* settings, int cent_h, int cent_w, char T);
void mountain_chains(Map& map, Map_settings* settings);
void mountain_chain_down(Map& map, Map_settings* settings, int start_h, int start_w);
void water_serapation(Map& map, Map_settings* settings);
void water_frame(Map& map, Map_settings* settings);
Cell* set_area(Cell* c, char t);


Map_settings::Map_settings(int h, int w): height(h), width(w){
    calc_main_size();
};

int Map_settings::get_height() const{
    return height;
}

int Map_settings::get_width() const{
    return width;
}

void Map_settings::set_height(int h){
    height = h;
    calc_main_size();
}

void Map_settings::set_width(int w){
    width = w;
    calc_main_size();
}

std::vector<char>& Map_settings::get_areas(){
    return areas;
}

void Map_settings::calc_main_size(){
     main_size = (height + width) * 0.5;

}

int Map_settings::get_main_size() const{
    return main_size;
}

void Map_settings::set_str_map(std::vector<std::string>& s_m){
    str_map = s_m;
}
std::vector<std::string>& Map_settings::get_str_map(){
    return str_map;
}




Grassland::Grassland(){
    set_type('G');
    set_color_t("\x1b[32m"); //green
}

Desert::Desert(){
    set_type('D');
    set_color_t("\x1b[33m"); //yellow
}


Forest::Forest(){
    set_type('F');
    set_color_t("\x1b[31m"); //red
}


Mountains::Mountains(){
    set_type('H');
    set_color_t("\x1b[35m"); // purpure
}

Swamp::Swamp(){
    set_type('S');
    set_color_t("\x1b[30m"); // black
}

Cell::Cell(char t, std::string col): type(t), color_t(col){};

void Cell::set_type(char t){
    type = t;
}

char Cell::get_type() const {
    return type;
}

const std::string Cell::get_color_t() const {
    return color_t;
}

void Cell::set_color_t(std::string col){
    color_t = col;
}

void print_map(const Map& map){

    for(const std::vector<Cell*>& row : map){
        for (const Cell* c : row){
            std::cout<< c->get_color_t()
                     << c->get_type()
                 << "\x1b[0m" <<" ";
        }
        std::cout<<std::endl;
    }
    std::cout <<"\x1b[0m";
}

void map_init(Map& map, Map_settings* settings){
    std::vector<Cell*> row;
    for(int i = 0; i < settings->get_height(); ++i){
        for (int j = 0; j < settings->get_width(); ++j){
            row.push_back(new Cell);

        }
        map.push_back(row);
        row.clear();
    }
}

void map_reset(Map& map, Map_settings* settings){
     map_free(map);
     map.clear();
     map_init(map, settings);
}

void map_free(Map& map){
    for(std::vector<Cell*> &row : map){
        for(Cell* c : row){
            delete c;
            c = nullptr;
        }
    }
}

void map_gen(Map& map, Map_settings* settings){
    std::vector<char> temp_areas(settings->get_areas());

    for (int i = 0; i < 5 && !temp_areas.empty(); ++i){
    char current_area = temp_areas[rand () % temp_areas.size()];
    auto it = find_if(temp_areas.begin(), temp_areas.end(), [current_area](char x){return x == current_area;} );
    temp_areas.erase(it);
    gen_large_sq_area(map, settings, current_area,
                  settings->sq_coord[2 * i] * settings->get_height(),
                  settings->sq_coord[2 * i + 1] * settings->get_width() );
    }

    gen_small_areas(map, settings);
    if (settings->mountain_chain_on){
        mountain_chains(map, settings);
    }
    if  (settings->water_frame_on){
        water_frame(map, settings);
    }
    if (settings->two_islands) {
        water_serapation(map, settings);
    }
    map_actualize_str_map(map, settings);
}

int distance(int x1, int y1, int x2, int y2){
    int result = 0;
    result = sqrt((x1  - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return result;
}

void gen_large_sq_area(Map& map, Map_settings* settings, char T, int cent_h, int cent_w){
    int i = 0;
    int j = 0; 
    for(std::vector<Cell*>& row : map){
        j = 0;
        for (Cell* &c : row){
            if ( ((rand() %
            (( settings->get_main_size() / 4) + settings->get_main_size() / 16))    // + x, x for distance of distibution
            + settings->get_main_size() / 6)                                    //size of 100% square
            > distance (i, j, cent_h, cent_w)){
                c = set_area(c, T);
            }
        j++;
        }
        i++;
    }
}

void gen_small_areas(Map& map, Map_settings* settings){
    int h = 0;
    int w = 0;
    int base_number = (settings->get_main_size() * settings->get_main_size()) / 80;
    while (base_number -= (rand() % 2)){
        h = rand() % settings->get_height();
        w = rand() % settings->get_width();

        int areas_size = settings->get_areas().size();

        char current_area;
        if (areas_size){
        current_area = settings->get_areas()[(rand() % areas_size)];
        } else {
            std::cerr<< "Zero areas_size" << std::endl;
            exit(1);
        }

        gen_small_area(map, settings, h, w, current_area);
    }
}

void gen_small_area(Map& map, Map_settings* settings, int cent_h, int cent_w, char T){
    int i = 0;
    int j = 0;
    for(std::vector<Cell*>& row : map){
        j = 0;
        for (Cell* &c : row){
            if ( ((rand() %
            ((settings->get_main_size() / 12) + 2))     // + x, x for distance of distibution
            + 1)                                        // size of 100% square
            > distance (i, j, cent_h, cent_w)){
                c = set_area(c, T);
            }
            j++;
        }
        i++;
    }
}

void mountain_chains(Map& map, Map_settings* settings){
    int max = settings->get_main_size() / 15;
    int i = 0;
    while ( i < max){
        mountain_chain_down(map, settings, rand () % (settings->get_height() * 3 / 4) , rand () % settings->get_width());
        i += rand() % 2;
    }
}

void mountain_chain_down(Map& map, Map_settings* settings, int h, int w){

    delete map[h][w];
    map[h][w] = new Mountains;

    while (rand() % settings->get_main_size() <= settings->get_main_size() - 2){
        int dir = rand() % 4;
        switch (dir){
        case 0: if ( w > 1){
                    --w;
                }
                break;
        case 1: if ( w + 1 < settings->get_width()){
                    ++w;
                }
                break;
        default: if ( h + 1 < settings->get_height()){
                    ++h;
                }
                break;
        }
        delete map[h][w];
        map[h][w] = new Mountains;
    }
}

void water_ver_half_band(Map& map, Map_settings* settings, int h, int w, int step){

    delete map[h][w];
    map[h][w] = new Cell;

    while ( ( h != 0 && h != settings->get_height() - 1)
         && ( w != 0 && w != settings->get_width() - 1)){

        delete map[h][w - 1];
        map[h][w - 1] = new Cell;
        delete map[h][w + 1];
        map[h][w + 1] = new Cell;

        int dir = rand() % 4;
        switch (dir){
        case 0: if ( w > 1){
                    --w;
                }
                break;
        case 1: if ( w + 1 < settings->get_width()){
                    ++w;
                }
                break;
        default:  h += step;
                 break;
        }

        delete map[h][w];
        map[h][w] = new Cell;

    }
}

void water_hor_half_band(Map& map, Map_settings* settings, int h, int w, int step){

    delete map[h][w];
    map[h][w] = new Cell;

    while ( ( h != 0 && h != settings->get_height() - 1)
         && ( w != 0 && w != settings->get_width() - 1)){

        delete map[h + 1][w];
        map[h + 1][w] = new Cell;
        delete map[h - 1][w];
        map[h - 1][w] = new Cell;

        int dir = rand() % 4;
        switch (dir){
        case 0: if ( h > 1){
                    --h;
                }
                break;
        case 1: if ( h + 1 < settings->get_height()){
                    ++h;
                }
                break;
        default:  w += step;
                 break;
        }

        delete map[h][w];
        map[h][w] = new Cell;
    }
}


void water_serapation(Map& map, Map_settings* settings){
    int start_h = settings->get_height() / 2;
    int start_w = settings->get_width() / 2;
    if (rand() % 2 > 0){
        water_ver_half_band(map, settings, start_h, start_w, 1);
        water_ver_half_band(map, settings, start_h - 1, start_w, -1);
    } else {
        water_hor_half_band(map, settings, start_h, start_w, 1);
        water_hor_half_band(map, settings, start_h, start_w - 1, -1);
    }
}

void water_ver_line(Map& map, Map_settings* settings, int w, int step){
    for (int i = 0; i < settings->get_height(); ++i){
        delete map[i][w];
        map[i][w] = new Cell;

        if (rand() % 2 > 0){
            delete map[i][w + step];
            map[i][w + step ] = new Cell;
            if (rand() % 4 > 2){
                delete map[i][w + 2 * step ];
                map[i][w + 2 * step ] = new Cell;
            }
        }
    }
}

void water_hor_line(Map& map, Map_settings* settings, int h, int step){
    for (int i = 0; i < settings->get_width(); ++i){
        delete map[h][i];
        map[h][i] = new Cell;

        if (rand() % 2 > 0){
            delete map[h + step][i];
            map[h + step][i] = new Cell;
            if (rand() % 4 > 2){
                delete map[h + 2 * step][i];
                map[h + 2 * step][i] = new Cell;
            }
        }
    }
}

void water_frame(Map& map, Map_settings* settings){

    water_ver_line(map, settings, 0, 1);
    water_ver_line(map, settings, settings->get_width() - 1, -1);

    water_hor_line(map, settings, 0, 1);
    water_hor_line(map, settings, settings->get_height() - 1, -1);
}

void map_load(Map& map, Map_settings* settings){
    int i = 0;
    int j = 0;
    for(std::vector<Cell*>& row : map){
        j = 0;
        for (Cell* &c : row){
            c = set_area(c, settings->get_str_map()[i][j]);
            j++;
        }
        i++;
    }
}

void map_actualize_str_map(Map& map, Map_settings* settings){
    settings->get_str_map().clear();
    int i = 0;
    int j = 0;
    std::string str;
    for(std::vector<Cell*>& row : map){
        j = 0;
        str = "";
        for (Cell* &c : row){
            str += c->get_type();
            j++;
        }
        settings->get_str_map().push_back(str);
        i++;
    }
}

Cell* set_area(Cell* c, char t){
    delete c;
    switch (t){
        case 'G': c = new Grassland;
              break;
        case 'D': c = new Desert;
              break;
        case 'F': c = new Forest;
              break;
        case 'H': c = new Mountains;
              break;
        case 'S': c = new Swamp;
              break;
        default : c = new Cell;
              break;
        }
    return c;
}
