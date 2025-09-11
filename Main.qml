import QtQuick
import QtMultimedia

Window {
    width: 640
    height: 480
    visible: true

    MediaDevices {
        id: mediaDevices
    }

    CaptureSession {
        id: session
        camera: Camera {
            id: camera
            cameraDevice: mediaDevices.defaultVideoInput
            active: true
        }
        videoOutput: preview
    }

    VideoOutput {
        id: preview
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit
    }
}

