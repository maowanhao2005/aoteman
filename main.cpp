#include "mp4.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("idokeno");
    app.setApplicationVersion("1.0");

    Mp4 window;
    window.show();

    return app.exec();
}
