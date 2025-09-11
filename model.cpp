#include "model.h"
QImage::Format Model::Constants::imgFormat = QImage::Format_RGB888;

Model::Model(QObject* parent) : QObject(parent) {}

bool Model::loadModel(const QString& resourcePath)
{
    QFile resourceFile(resourcePath);
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open resource:" << resourcePath;
        return false;
    }
    QByteArray modelData = resourceFile.readAll();
    resourceFile.close();
    mModel = tflite::FlatBufferModel::BuildFromBuffer(modelData.constData(), modelData.size());
    if(!mModel){
        qDebug() << "Faild to open model from file";
        return false;
    }

    // create intepreter
    tflite::InterpreterBuilder builder(*mModel, mResolver);
    TfLiteStatus status = builder(&mInterpreter);
    if(status != kTfLiteOk){
        qDebug() << "Failed to create interpreter. Builder status: " << status;
        return false;
    }

    status = mInterpreter->AllocateTensors();
    if(status == kTfLiteOk){
        mInput = mInterpreter->typed_input_tensor<float>(0);
    }

    qDebug() << "Model loaded successful";
    return true;
}

QImage Model::imageTransform(const QImage &frame)
{
    return frame.scaled(Constants::inputWidth, Constants::inputWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation).convertedTo(Constants::imgFormat);
}

std::map<int, float> Model::frameProcessor(const QImage& frame)
{
    if(mInput == nullptr){
        qDebug() << "Failed to inference frame. Model input data equal null";
        return {};
    }

    QImage transformed = imageTransform(frame);
    const auto* inputFrame = transformed.bits();
    if(!inputFrame){
        qDebug() << "Input image eqaul null";
        return {};
    }

    std::memcpy(mInput, inputFrame, transformed.sizeInBytes());
    TfLiteStatus status = mInterpreter->Invoke();
    if(status == kTfLiteOk){
        return processOutput();
    }

    qDebug() << "Cannot make inference";
    return {};
}

std::map<int, float> Model::processOutput()
{
    // TODO: make data output processing
    return {};
}
