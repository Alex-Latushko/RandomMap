#pragma once
#include "Map_QView.h"
#include "settingswindow.h"

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMenuBar>
#include <QTabWidget>
#include <QFileDialog>
#include <QScrollArea>

class Main_window : public QWidget
{
    Q_OBJECT
private:
    Map_QView* map = NULL;
    Map_settings* settings = NULL;
    QMenuBar* main_menu = NULL;
    QTabWidget* tab = NULL;
    void set_menu();
    void set_tab_widget();
    void set_layout();
public:
    QAction* quit_action = NULL;
    explicit Main_window(QWidget *parent = nullptr);
public slots:
    void open_map();
    void save_map();
    void set_max_size();
signals:
    void map_opened();
};
