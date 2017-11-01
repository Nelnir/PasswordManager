#include "passwordmanager.h"
#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    try{
        PasswordManager w;
        w.show();
        return a.exec();
    }
    catch(const std::logic_error& ex){
        QMessageBox::critical(nullptr, "Error", ex.what(), QMessageBox::Ok);
        return -1;
    }
}
