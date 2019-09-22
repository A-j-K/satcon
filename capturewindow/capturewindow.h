#ifndef CAPTUREWINDOW_H
#define CAPTUREWINDOW_H

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
    Ui::CaptureWindow *ui;
};
#endif // CAPTUREWINDOW_H
