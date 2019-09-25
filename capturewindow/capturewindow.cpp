
#include <QTimer>
#include <QMessageBox>
#include <QCameraInfo>
#include <QMediaService>
#include <QMediaRecorder>
#include <QMediaMetaData>
#include <QCameraViewfinder>

#include "capturewindow.h"
#include "./ui_capturewindow.h"

Q_DECLARE_METATYPE(QCameraInfo)

CaptureWindow::CaptureWindow(QWidget *parent)
    : QMainWindow(parent)
    , p_ui(new Ui::CaptureWindow)
	, _isCapturingImage(false)
{
    p_ui->setupUi(this);
	initMenu();
}

CaptureWindow::~CaptureWindow()
{
    delete p_ui;
}

void CaptureWindow::updateCameraDevice(QAction *inp_action)
{
	setCamera(qvariant_cast<QCameraInfo>(inp_action->data()));
}

void CaptureWindow::setCamera(const QCameraInfo & in_cameraInfo)
{
	_camera.reset(new QCamera(in_cameraInfo));

	connect(_camera.data(), &QCamera::stateChanged, this, &CaptureWindow::updateCameraState);
	connect(_camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &CaptureWindow::displayCameraError);
	_imageCapture.reset(new QCameraImageCapture(_camera.data()));
	//_camera->setViewfinder(p_ui->viewfinder);

	updateCameraState(_camera->state());
	updateLockStatus(_camera->lockStatus(), QCamera::UserRequest);

	connect(_imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged, this, &CaptureWindow::readyForCapture);
	connect(_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &CaptureWindow::processCapturedImage);
	connect(_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &CaptureWindow::imageSaved);
	connect(_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString&>::of(&QCameraImageCapture::error),
		this, &CaptureWindow::displayCaptureError);

	connect(_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of(&QCamera::lockStatusChanged),
		this, &CaptureWindow::updateLockStatus);

	//p_ui->captureWidget->setTabEnabled(0, (_camera->isCaptureModeSupported(QCamera::CaptureStillImage)));
	//p_ui->captureWidget->setTabEnabled(1, (_camera->isCaptureModeSupported(QCamera::CaptureVideo)));

	//updateCaptureMode();
	_camera->start();
}

void CaptureWindow::processCapturedImage(int requestId, const QImage& img)
{
#if 0
	Q_UNUSED(requestId)
	QImage scaledImage = img.scaled(p_ui->viewfinder->size(),
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation);

	p_ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

	// Display captured image for 4 seconds.
	displayCapturedImage();
	QTimer::singleShot(100, this, &CaptureWindow::displayViewfinder);
#endif
}

void CaptureWindow::displayViewfinder()
{
	//p_ui->stackedWidget->setCurrentIndex(0);
}

void CaptureWindow::updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
	QColor indicationColor = Qt::black;

	switch (status) {
	case QCamera::Searching:
		indicationColor = Qt::yellow;
		p_ui->statusbar->showMessage(tr("Focusing..."));
		//ui->lockButton->setText(tr("Focusing..."));
		break;
	case QCamera::Locked:
		indicationColor = Qt::darkGreen;
		//ui->lockButton->setText(tr("Unlock"));
		p_ui->statusbar->showMessage(tr("Focused"), 2000);
		break;
	case QCamera::Unlocked:
		indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
		//ui->lockButton->setText(tr("Focus"));
		if (reason == QCamera::LockFailed) {
			p_ui->statusbar->showMessage(tr("Focus Failed"), 2000);
		}
	}

	//QPalette palette = p_ui->lockButton->palette();
	//palette.setColor(QPalette::ButtonText, indicationColor);
	//p_ui->lockButton->setPalette(palette);
}

void CaptureWindow::displayCapturedImage()
{
	//p_ui->stackedWidget->setCurrentIndex(1);
}

void CaptureWindow::imageSaved(int id, const QString& fileName)
{
	Q_UNUSED(id)

	//p_ui->statusbar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

	_isCapturingImage = false;
	//if (_applicationExiting) {
	//	close();
	//}
}


void CaptureWindow::updateCameraState(QCamera::State state)
{
	switch (state) {
	case QCamera::ActiveState:
		//p_ui->actionStartCamera->setEnabled(false);
		//p_ui->actionStopCamera->setEnabled(true);
		//p_ui->captureWidget->setEnabled(true);
		//p_ui->actionSettings->setEnabled(true);
		break;
	case QCamera::UnloadedState:
	case QCamera::LoadedState:
		//p_ui->actionStartCamera->setEnabled(true);
		//p_ui->actionStopCamera->setEnabled(false);
		//p_ui->captureWidget->setEnabled(false);
		//p_ui->actionSettings->setEnabled(false);
		break;
	default:
		break;
	}
}

void CaptureWindow::displayCameraError()
{
	QMessageBox::warning(this, "Camera Error", _camera->errorString());
}

void CaptureWindow::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString& errorString)
{
	Q_UNUSED(id)
	Q_UNUSED(error)

	QMessageBox::warning(this, tr("Image Capture Error"), errorString);
	_isCapturingImage = false;
}


void CaptureWindow::readyForCapture(bool ready)
{
	//p_ui->takeImageButton->setEnabled(ready);
}


void 
CaptureWindow::initMenu()
{
	QActionGroup *p_videoDevicesGroup = new QActionGroup(this);
	p_videoDevicesGroup->setExclusive(true);
	const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
	for (const QCameraInfo& cameraInfo : availableCameras) {
		QAction *p_videoDeviceAction = new QAction(cameraInfo.description(), p_videoDevicesGroup);
		p_videoDeviceAction->setCheckable(true);
		p_videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
		if (cameraInfo == QCameraInfo::defaultCamera()) {
			p_videoDeviceAction->setChecked(true);
		}
		p_ui->menuDevices->addAction(p_videoDeviceAction);
	}
	connect(p_videoDevicesGroup, &QActionGroup::triggered, this, &CaptureWindow::updateCameraDevice);
	//connect(ui->captureWidget, &QTabWidget::currentChanged, this, &CaptureWindow::updateCaptureMode);
	setCamera(QCameraInfo::defaultCamera());
}

