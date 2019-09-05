#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "elementlist.h"
#include "element.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // -------------- add ----------------
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);

    ElementList elementList;
    engine.rootContext()->setContextProperty("elementList", &elementList);
    qmlRegisterType<Element>("Element", 1, 0, "Element");
    // -----------------------------------

    engine.load(url);

    return app.exec();
}
