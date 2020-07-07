#pragma once
#include "map_func.h"

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QVector>

#include "profiler.h"

typedef  QVector< QVector<QLabel*> > Map_qlb;

class Map_QView : public QWidget
{
    Q_OBJECT
private:
    void init_qlb_map();
    void free_qlb_map();
    bool size_changed_qlb_map();
    void generate_map();
    void show_map();
public:
    explicit Map_QView(QWidget *parent = nullptr);
    std::map<std::string, QPixmap> images;
    Map_settings* settings;
    Map map;
    Map_qlb* map_qlb;
    QGridLayout* grid_layout;
public slots:
void generate_new_map();
void load_map();
signals:
void new_map();
};
