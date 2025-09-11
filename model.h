#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>
#include <map>
#include <memory>
#include <QFile>
#include <qDebug>


#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/kernels/register.h>

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject* parent = nullptr);

    bool loadModel(const QString& modelPath);
    QImage imageTransform(const QImage& frame);
    std::map<int, float> frameProcessor(const QImage& frame);
    std::map<int, float> processOutput();


private:
    //tflite main vars
    std::unique_ptr<tflite::FlatBufferModel> mModel;
    std::unique_ptr<tflite::Interpreter> mInterpreter;
    tflite::ops::builtin::BuiltinOpResolver mResolver;
    float* mInput = nullptr;
    float* mOutput = nullptr;

    // YOLO model constants
    struct Constants {
        static constexpr int inputWidth = 640;
        static constexpr int inputHeight = 640;
        static constexpr int channels = 3;
        static constexpr size_t inputSize = inputWidth * inputHeight * channels;
        static constexpr float threshold = 0.5f;
        static QImage::Format imgFormat;
    };
};

#endif // MODEL_H
