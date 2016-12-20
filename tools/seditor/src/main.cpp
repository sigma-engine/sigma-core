#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include <iostream>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/ui/main.qml")));

    return app.exec();
}
