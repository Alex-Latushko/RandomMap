#include "settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, Map_QView* p_map_qview) : QWidget(parent), p_map(p_map_qview)
{
    std::map<std::string, QPixmap> images{{"pix_Water", QPixmap(":/my_images/res/Water.jpg")}};
    std::vector<std::string> str_areas {"Grassland", "Forest", "Desert", "Mountains", "Swamp"};
    for(std::string& area : str_areas){
        images.insert({"pix_" + area, QPixmap(":/my_images/res/" + QString::fromStdString(area) +".jpg")});
    }


    QLabel* width_label = new QLabel("&Width");
    QLabel* height_label = new QLabel("&Height");
    width_edit = new QLineEdit("20");
    width_label->setBuddy(width_edit);
    height_edit = new QLineEdit("16");
    height_label->setBuddy(height_edit);

    apply_settings = new QPushButton("&Apply and Generate");
    apply_settings->setObjectName("Apply_Button");
    QWidget::connect(apply_settings, SIGNAL(clicked()),
                     this, SLOT(apply_settings_slot()));

    areas_box = new QGroupBox("Areas");

    for(std::string& area : str_areas){
        QCheckBox* check_box = new QCheckBox(QString::fromStdString(area));
        check_box->setIcon(images["pix_" + area]);
        check_box->setChecked(true);
        check_box->setIconSize(QSize(50, 50));
        ch_box_areas.push_back(check_box);
    }
    ch_box_areas[4]->setChecked(false);  // Swamp turn off by default

    QGroupBox* options_box = new QGroupBox("Options");
    mountain_chain_check_box = new QCheckBox("Mountain chain");
    mountain_chain_check_box->setChecked(true);
    water_frame_check_box = new QCheckBox("Water frame");
    water_frame_check_box->setChecked(true);
    two_islands_check_box = new QCheckBox("Two islands");
    two_islands_check_box->setChecked(true);
    QVBoxLayout* options_layout = new QVBoxLayout;
    options_layout->addWidget(mountain_chain_check_box);
    options_layout->addWidget(water_frame_check_box);
    options_layout->addWidget(two_islands_check_box);
    options_box->setLayout(options_layout);

    QVBoxLayout* ver_size_options_layout = new QVBoxLayout;


    QHBoxLayout* hor1_layout = new QHBoxLayout;
    QFormLayout* size_form_layout = new QFormLayout;
    QVBoxLayout* ver_layout = new QVBoxLayout;

    QGroupBox* size_box = new QGroupBox("Size");
    size_box->setObjectName("Size_Box");
    size_form_layout->addRow(width_label, width_edit);
    size_form_layout->addRow(height_label, height_edit);

    size_box->setLayout(size_form_layout);


    QVBoxLayout* ch_box_ver_layout = new QVBoxLayout;
    for(QCheckBox* ch_box : ch_box_areas){
       ch_box_ver_layout->addWidget(ch_box);
    }
    areas_box->setLayout(ch_box_ver_layout);

    ver_size_options_layout->addWidget(size_box);
    ver_size_options_layout->addWidget(options_box);

    hor1_layout->addLayout(ver_size_options_layout);
    hor1_layout->addWidget(areas_box);

    ver_layout->addLayout(hor1_layout);
    ver_layout->addWidget(apply_settings);
    ver_layout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout* hor_layout = new QHBoxLayout;
    hor_layout->addStretch(1);
    hor_layout->addLayout(ver_layout);
    hor_layout->addStretch(1);

    QVBoxLayout* main_ver_layout = new QVBoxLayout;
    main_ver_layout->addLayout(hor_layout);
    main_ver_layout->addStretch(1);
    setLayout(main_ver_layout);
}


void SettingsWindow::apply_settings_slot(){
    bool no_error = true;
    if ( (size_t)width_edit->text().toInt() < p_map->settings->min_side_length ||
        (size_t)height_edit->text().toInt() < p_map->settings->min_side_length ||
        (size_t)width_edit->text().toInt() > p_map->settings->max_side_length ||
        (size_t)height_edit->text().toInt() > p_map->settings->max_side_length ){
        no_error = false;
        QMessageBox::warning(this, "Invalid height or width", "Height and width must be integer between "
                           + QString::number(p_map->settings->min_side_length) + " and "
                           + QString::number(p_map->settings->max_side_length) + "!");
    }

    std::vector<char> temp_areas;
    if (ch_box_areas[0]->isChecked()) { temp_areas.push_back('G');  }
    if (ch_box_areas[1]->isChecked()) { temp_areas.push_back('F');  }
    if (ch_box_areas[2]->isChecked()) { temp_areas.push_back('D');  }
    if (ch_box_areas[3]->isChecked()) { temp_areas.push_back('H');  }
    if (ch_box_areas[4]->isChecked()) { temp_areas.push_back('S');  }

    if (temp_areas.empty()){
        no_error = false;
        QMessageBox::warning(this, "Not enough area types", "At least one type of area must be choosen!");
    }

    if (no_error){
        p_map->settings->set_width(width_edit->text().toInt());
        p_map->settings->set_height(height_edit->text().toInt());

        p_map->settings->get_areas().clear();
        p_map->settings->get_areas() = temp_areas;

        p_map->settings->mountain_chain_on = mountain_chain_check_box->isChecked();
        p_map->settings->water_frame_on = water_frame_check_box->isChecked();
        p_map->settings->two_islands = two_islands_check_box->isChecked();
        emit (apply_settings_signal());
    }
}
