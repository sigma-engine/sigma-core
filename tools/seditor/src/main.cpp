#include <GameView.hpp>
#include <EditorContext.hpp>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    qmlRegisterType<sigma::GameView>("sigma", 1, 0, "GameView");

    sigma::EditorContext ctx;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("context", &ctx);
    engine.load(QUrl(QLatin1String("qrc:/ui/main.qml")));

    return app.exec();
}
