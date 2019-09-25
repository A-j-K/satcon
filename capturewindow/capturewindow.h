#ifndef CAPTUREWINDOW_H
#define CAPTUREWINDOW_H

#include <QCamera>
#include <QActionGroup>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QCameraImageCapture>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { 
	class CaptureWindow;
}
QT_END_NAMESPACE

class CaptureWindow : public QMainWindow
{
    Q_OBJECT

public:
    CaptureWindow(QWidget *parent = nullptr);
    ~CaptureWindow();

	

private:

	void initMenu();
	void displayViewfinder();
	void displayCameraError();
	void displayCapturedImage();
	void readyForCapture(bool ready);
	void updateCameraState(QCamera::State state);
	void updateCameraDevice(QAction *inp_action);
	void setCamera(const QCameraInfo & cameraInfo);
	void processCapturedImage(int requestId, const QImage & img);
	void imageSaved(int id, const QString& fileName);
	void updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason);
	void displayCaptureError(int id, const QCameraImageCapture::Error error, const QString & errorString);

	bool _isCapturingImage;

    Ui::CaptureWindow *p_ui;
	QScopedPointer<QCamera> _camera;
	QScopedPointer<QCameraImageCapture> _imageCapture;
	
};
#endif // CAPTUREWINDOW_H
