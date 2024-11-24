#include "mainwindow.h"

#include <QApplication>
#include <QCursor>
#include <QImage>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QImage image(500, 500, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    int sqrRadius = radius*radius;

    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j)
        {
            int dx = i - image.width() / 2;
            int dy = j - image.height() / 2;
            int sqrDist = dx * dx + dy * dy;

            if (sqrDist <= sqrRadius) {
                image.setPixel(i, j, qRgba(0, 255, 255, 55));
            }
        }
    }
    QCursor cursor(QPixmap::fromImage(image));

    w.setCursor(cursor);
    w.show();
    return a.exec();
}
