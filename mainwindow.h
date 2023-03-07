#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Tello.h>
#include <TelloEnumTypes.h>
#include <TelloCommand.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();
    void updateGUI();
    void processResponseSignal(TelloResponse, QString);
    void processSNR(int snr);
    void displayStream(QPixmap frame);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    Tello *tello;
};
#endif // MAINWINDOW_H
