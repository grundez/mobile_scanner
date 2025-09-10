#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QStandardPaths>

// tflite headers
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/kernels/register.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    std::unique_ptr<tflite::Interpreter> interpreter;
    std::unique_ptr<tflite::FlatBufferModel> model;
    tflite::ops::builtin::BuiltinOpResolver resolver;
    float* input;

    // model init
    QFile modelFile(":/best_usable_float32.tflite");
    modelFile.open(QIODevice::ReadOnly);
    QByteArray modelData = modelFile.readAll();
    modelFile.close();
    model = tflite::FlatBufferModel::BuildFromBuffer(modelData.constData(), modelData.size());
    if(!model){
        qDebug() << "Failed to load model";
        return -1;
    }

    // interpreter build
    tflite::InterpreterBuilder builder(*model, resolver);
    TfLiteStatus status = builder(&interpreter);
    if (status != kTfLiteOk || !interpreter) {
        qDebug() << "Failed to create interpreter, status:" << status;
        return -1;
    }

    // allocate tensors by interpreter
    status = interpreter->AllocateTensors();
    if(status != kTfLiteOk){
        qDebug() << "Failed to allocate tensors, status:" << status;
        return -1;
    }
    else if (status == kTfLiteOk)
        input = interpreter->typed_input_tensor<float>(0); // some dummy data

    qDebug() << "Model loaded successfully!";



    /* TODO:
    1 enable camera
    2 make inference on camera action
    3 simple qml ui: print output data by inference on mobile screen
    */

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("mush_app", "Main");

    return app.exec();
}
