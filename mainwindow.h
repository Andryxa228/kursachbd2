#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_btnAppointment_clicked();
    void on_btnDeleteAppointment_clicked();

    // Авторизация и регистрация
    void on_btnLogin_clicked();
    void on_btnRegister_clicked();
    void logout();

    // Новые кнопки для функционала Врача
    void on_btnDoctorEditApp_clicked();
    void on_btnDoctorCancelApp_clicked();
    void on_btnPatientDeleteApp_clicked();

private:
    Ui::MainWindow *ui;
    int currentPatientId = -1; // ID вошедшего пациента
    int currentDoctorId = -1;  // ID вошедшего врача
};

#endif // MAINWINDOW_H
