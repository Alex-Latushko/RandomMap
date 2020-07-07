#include "main_window.h"
#include <QApplication>

void set_style(QApplication& a);

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    set_style(a);

    Main_window wgt;
    QWidget::connect(wgt.quit_action, SIGNAL(triggered()),
                     &a, SLOT(quit()));
    wgt.show();

    return a.exec();
}

void set_style(QApplication& a){
    QFile file_with_style(":/styles/res/RM_style.qss");
    file_with_style.open(QFile::ReadOnly);
    QString rm_style = QLatin1String(file_with_style.readAll());
    a.setStyleSheet(rm_style);
}
