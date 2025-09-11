/* TODO:
1 enable camera
2 make inference on camera action:
    get frame from camera(QImage)
    transform frame to model params(w, h, etc.)
    put data in input tensor
    inference it(use interpreter() -> invoke())
    output result
3 simple qml ui: print output res by inference
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QStandardPaths>

// camera headers
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QMediaDevices>
#include <QPermission>

#include <model.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // MODEL LOAD
    Model model;
    model.loadModel(":/best_usable_float32.tflite");

    // register to qml
    QQmlApplicationEngine engine;
    qmlRegisterSingletonInstance("App", 1, 0, "Model", &model);
    engine.loadFromModule("mush_app", "Main");

    return app.exec();
}
