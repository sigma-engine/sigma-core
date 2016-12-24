#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>

#include <game_view.hpp>
#include <qt_context.hpp>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    qmlRegisterType<sigma::game_view>("sigma", 1, 0, "GameView");
    qmlRegisterType<sigma::qt_context>("sigma", 1, 0, "Context");

    sigma::qt_context ctx;
    QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("context", &ctx);
	engine.load(QUrl(QLatin1String("qrc:/ui/main.qml")));

    return app.exec();
}
