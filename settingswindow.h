#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>
#include <QCheckBox>

#include <Map_QView.h>


class SettingsWindow : public QWidget
{
    Q_OBJECT
private slots:
    void apply_settings_slot();
public:
    explicit SettingsWindow(QWidget *parent = nullptr, Map_QView* p_map_qview = nullptr);
    std::vector<QCheckBox*> ch_box_areas;
    Map_QView* p_map;
    QPushButton* apply_settings;
    QLineEdit* width_edit;
    QLineEdit* height_edit;

    QGroupBox* areas_box;
    QCheckBox* grassland_check_box;
    QCheckBox* forest_check_box;
    QCheckBox* desert_check_box;
    QCheckBox* mountains_check_box;
    QCheckBox* swamp_check_box;

    QCheckBox* mountain_chain_check_box;
    QCheckBox* water_frame_check_box;
    QCheckBox* two_islands_check_box;

signals:
    void apply_settings_signal();
};
