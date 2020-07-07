#include "main_window.h"

Main_window::Main_window(QWidget *parent) : QWidget(parent)
{
   // this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    map = new Map_QView;

    QWidget::connect(this, SIGNAL(map_opened()),
                     map, SLOT(load_map()));

    QWidget::connect(map, SIGNAL(new_map()),
                     this, SLOT(set_max_size()));

    set_menu();
    set_tab_widget();
    set_layout();

    resize(10 + 50 * 20, 60 + 50 * 16);
    setMaximumSize(10 + 50 * 20, 60 + 50 * 16);
}

void Main_window::open_map(){
    QString name = QFileDialog::getOpenFileName(0, "Open map", "d:\\Prog\\Qt\\RandomMap_1\\RandomMap_1\\", "*.randmap" );
    QFile file(name);
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()){
       QMessageBox::critical(0, "Wrong file", "Couldn't open file!");
       return;
    }
    QTextStream t_stream(&file);
    if ( t_stream.readLine() != "Random Map"){
        QMessageBox::critical(0, "Wrong format", "Open file with wrong format");
        return;
    }

    size_t height;
    height = t_stream.readLine().toInt();
    size_t width;
    width = t_stream.readLine().toInt();
    if ( width < map->settings->min_side_length || height < map->settings->min_side_length ||
         width > map->settings->max_side_length || height > map->settings->max_side_length ){
        QMessageBox::critical(this, "Invalid height or width", "Height and width must be integer between "
                             + QString::number(map->settings->min_side_length) + " and "
                             + QString::number(map->settings->max_side_length) + "!");
        return;
    }

    std::vector<std::string> strings_map;
    std::string str;
    for (size_t i = 0; i < height; ++i){
        if (t_stream.atEnd()){
            QMessageBox::critical(0, "Not enough data", "This file contains less lines in map, then required!");
            return;
        }
        str = t_stream.readLine().toStdString();
        if (str.size() != width){
            QMessageBox::critical(0, "Not enough data", "A line in the map contains less cells, then required!");
            return;
        }
        strings_map.push_back(str);
    }

    map->settings->set_height(height);
    map->settings->set_width(width);
    map->settings->set_str_map(strings_map);
    file.close();
    emit(map_opened());
}

void Main_window::save_map(){
    QString name = QFileDialog::getSaveFileName(0, "Save map", "d:\\Prog\\Qt\\RandomMap_1\\RandomMap_1\\", "*.randmap" );
    QFile file(name);
    file.open(QIODevice::WriteOnly);
    if (!file.isOpen()){
       QMessageBox::critical(0, "Wrong file", "Couldn't open file!");
       return;
    }

    QTextStream t_stream(&file);
    t_stream << "Random Map\n";
    t_stream << map->settings->get_height() << "\n";
    t_stream << map->settings->get_width() << "\n";
    for (int i = 0; i < map->settings->get_height(); ++i){
        std::string str = map->settings->get_str_map()[i];
        QString str1 = QString::fromStdString(str);
        t_stream << str1  << "\n";
    }
    file.close();
    QMessageBox::information(0, "Map saved", "Map have been saved.");
}

void Main_window::set_max_size(){
    setMaximumSize(10 + 50 * map->settings->get_width(), 60 + 50 * map->settings->get_height());
}

void Main_window::set_menu(){
    main_menu = new QMenuBar(this);

    QMenu* file_menu = new QMenu(this);
    file_menu->setFixedHeight(20);
    file_menu = main_menu->addMenu("&File");

    QAction* new_action = new QAction("&New");
    new_action->setShortcut( QKeySequence(Qt::ALT + Qt::Key_N));
    file_menu->addAction(new_action);
    QWidget::connect(new_action, SIGNAL(triggered()),
                     map, SLOT(generate_new_map()));

    QAction* open_map_action = new QAction("&Open map");
    open_map_action->setShortcut( QKeySequence(Qt::ALT + Qt::Key_O));
    file_menu->addAction(open_map_action);
    QWidget::connect(open_map_action, SIGNAL(triggered()),
                     this, SLOT(open_map()));

    QAction* save_map_action = new QAction("Sa&ve map");
    save_map_action->setShortcut( QKeySequence(Qt::ALT + Qt::Key_V));
    file_menu->addAction(save_map_action);
    QWidget::connect(save_map_action, SIGNAL(triggered()),
                     this, SLOT(save_map()));

    quit_action = new QAction("&Quit");
    quit_action->setShortcut( QKeySequence(Qt::ALT + Qt::Key_Q));
    file_menu->addAction(quit_action);
}

void Main_window::set_tab_widget(){
    tab = new QTabWidget;
    QScrollArea* sa = new QScrollArea ;
    sa->setWidget(map);
    tab->addTab(sa, "&Map");

    SettingsWindow* settings_w = new SettingsWindow(0, map);
    tab->addTab(settings_w, "&Settings");

    QWidget::connect(settings_w, SIGNAL(apply_settings_signal()),
                     map, SLOT(generate_new_map()));
}

void Main_window::set_layout(){
    QVBoxLayout* ver_layout = new QVBoxLayout;
    ver_layout->addWidget(main_menu);
    ver_layout->addWidget(tab);

    ver_layout->setContentsMargins(0, 0, 0, 0);
    ver_layout->setSpacing(0);
    setLayout(ver_layout);
}
