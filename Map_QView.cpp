#include "Map_QView.h"

Map_QView::Map_QView(QWidget *parent) : QWidget(parent)
{
    settings = new Map_settings;
    map_init(map, settings);
    map_qlb = new Map_qlb;
    grid_layout = new QGridLayout;
    grid_layout->setSpacing(0);
    grid_layout->setContentsMargins(0, 0, 0, 0);
    generate_map();
    setLayout(grid_layout);
}

void Map_QView::generate_map(){
    LOG_DURATION("generate_map");

    init_qlb_map();
    { LOG_DURATION("map_reset");
        map_reset(map, settings);
    }
    {LOG_DURATION("map_gen");
    map_gen(map, settings);
    }
    {LOG_DURATION("show_map");
    show_map();
    }
    emit(new_map());
}

void Map_QView::generate_new_map(){ // slot
    generate_map();
}

void Map_QView::init_qlb_map(){
   LOG_DURATION("map_qlb_init");
    QVector<QLabel*> row;
    if (!map_qlb->empty()){
        free_qlb_map();
        map_qlb->clear();
    }
    for (int i = 0; i < settings->get_height(); ++i){
        for (int j = 0; j < settings->get_width(); ++j){
            QLabel* c = new QLabel;
            c->resize(50, 50);
            grid_layout->addWidget(c, i, j);
            row.push_back(c);
        }
        map_qlb->push_back(row);
        row.clear();
    }

}

void Map_QView::free_qlb_map(){
    LOG_DURATION("map_qlb_free");
    for(Map_qlb::iterator it = map_qlb->begin();
        it != map_qlb->end();
        ++it){
        for( QVector<QLabel*>::iterator c = it->begin();
             c != it->end();
             ++c){
             delete *c;
             *c = nullptr;
        }
    }
}

void Map_QView::show_map(){
    images.insert({"pix_Water", QPixmap(":/my_images/res/Water.jpg")});
    images.insert({"pix_Grass", QPixmap(":/my_images/res/Grassland.jpg")});
    images.insert({"pix_Forest", QPixmap(":/my_images/res/Forest.jpg")});
    images.insert({"pix_Desert", QPixmap(":/my_images/res/Desert.jpg")});
    images.insert({"pix_Mountains", QPixmap(":/my_images/res/Mountains.jpg")});
    images.insert({"pix_Swamp", QPixmap(":/my_images/res/Swamp.jpg")});
    int i = 0;
    int j = 0;
    foreach(std::vector<Cell*> row, map){
        j = 0;
        foreach (Cell* c, row){
            switch( c->get_type() ){
            case 'G': (*map_qlb)[i][j]->setPixmap(images["pix_Grass"]);
                      break;
            case 'F': (*map_qlb)[i][j]->setPixmap(images["pix_Forest"]);
                      break;
            case 'D': (*map_qlb)[i][j]->setPixmap(images["pix_Desert"]);
                      break;
            case 'H': (*map_qlb)[i][j]->setPixmap(images["pix_Mountains"]);
                      break;
            case 'S': (*map_qlb)[i][j]->setPixmap(images["pix_Swamp"]);
                      break;
            default: (*map_qlb)[i][j]->setPixmap(images["pix_Water"]);
                      break;
            }
        j++;
        }
    i++;
    }
    setFixedSize(50 * settings->get_width(), 50 * settings->get_height());
    emit(new_map());
}

void Map_QView::load_map(){
    init_qlb_map();
    map_reset(map, settings);
    map_load(map, settings);
    show_map();
}
