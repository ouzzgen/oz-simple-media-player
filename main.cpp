#include "ol_media_player.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OL_Media_Player w;
    w.show();

    return a.exec();
}
