/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QComboBox *cbAuthRole;
    QLineEdit *lnLogin;
    QLineEdit *lnPassword;
    QPushButton *btnLogin;
    QPushButton *btnRegister;
    QLineEdit *lnRegName;
    QWidget *tab;
    QPushButton *pushButton;
    QTableView *tableView;
    QLineEdit *lineEdit;
    QLineEdit *lnPatientName;
    QPushButton *btnAppointment;
    QComboBox *cbDoctors;
    QDateTimeEdit *dateTimeEdit;
    QTableView *myAppointmentsTableView;
    QPushButton *btnPatientDeleteApp;
    QPushButton *btnLogout1;
    QWidget *tab_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton_2;
    QTableView *appointmentsTableView;
    QPushButton *btnDeleteAppointment;
    QPushButton *btnLogout2;
    QLineEdit *lnDocLogin;
    QLineEdit *lnDocPassword;
    QWidget *tab_4;
    QPushButton *btnLogout3;
    QTableView *doctorTableView;
    QDateTimeEdit *dtDoctorEdit;
    QPushButton *btnDoctorEditApp;
    QPushButton *btnDoctorCancelApp;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 801, 581));
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        cbAuthRole = new QComboBox(tab_3);
        cbAuthRole->addItem(QString());
        cbAuthRole->addItem(QString());
        cbAuthRole->addItem(QString());
        cbAuthRole->setObjectName("cbAuthRole");
        cbAuthRole->setGeometry(QRect(230, 40, 221, 22));
        lnLogin = new QLineEdit(tab_3);
        lnLogin->setObjectName("lnLogin");
        lnLogin->setGeometry(QRect(230, 70, 231, 22));
        lnPassword = new QLineEdit(tab_3);
        lnPassword->setObjectName("lnPassword");
        lnPassword->setGeometry(QRect(230, 100, 231, 22));
        lnPassword->setEchoMode(QLineEdit::EchoMode::Password);
        btnLogin = new QPushButton(tab_3);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setGeometry(QRect(230, 180, 121, 31));
        btnRegister = new QPushButton(tab_3);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setGeometry(QRect(350, 180, 201, 31));
        lnRegName = new QLineEdit(tab_3);
        lnRegName->setObjectName("lnRegName");
        lnRegName->setGeometry(QRect(230, 130, 231, 22));
        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        pushButton = new QPushButton(tab);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(450, 20, 91, 24));
        tableView = new QTableView(tab);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(10, 60, 421, 341));
        tableView->setAutoScroll(true);
        tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(10, 20, 421, 31));
        lnPatientName = new QLineEdit(tab);
        lnPatientName->setObjectName("lnPatientName");
        lnPatientName->setGeometry(QRect(20, 410, 361, 31));
        btnAppointment = new QPushButton(tab);
        btnAppointment->setObjectName("btnAppointment");
        btnAppointment->setGeometry(QRect(390, 450, 131, 31));
        cbDoctors = new QComboBox(tab);
        cbDoctors->setObjectName("cbDoctors");
        cbDoctors->setGeometry(QRect(20, 450, 361, 22));
        dateTimeEdit = new QDateTimeEdit(tab);
        dateTimeEdit->setObjectName("dateTimeEdit");
        dateTimeEdit->setGeometry(QRect(20, 481, 271, 31));
        dateTimeEdit->setCalendarPopup(true);
        myAppointmentsTableView = new QTableView(tab);
        myAppointmentsTableView->setObjectName("myAppointmentsTableView");
        myAppointmentsTableView->setGeometry(QRect(480, 70, 311, 192));
        btnPatientDeleteApp = new QPushButton(tab);
        btnPatientDeleteApp->setObjectName("btnPatientDeleteApp");
        btnPatientDeleteApp->setGeometry(QRect(570, 290, 141, 24));
        btnLogout1 = new QPushButton(tab);
        btnLogout1->setObjectName("btnLogout1");
        btnLogout1->setGeometry(QRect(674, 3, 101, 41));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        lineEdit_2 = new QLineEdit(tab_2);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(10, 10, 271, 31));
        lineEdit_3 = new QLineEdit(tab_2);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(10, 60, 271, 31));
        lineEdit_4 = new QLineEdit(tab_2);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(10, 110, 271, 31));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(320, 110, 101, 24));
        appointmentsTableView = new QTableView(tab_2);
        appointmentsTableView->setObjectName("appointmentsTableView");
        appointmentsTableView->setGeometry(QRect(0, 160, 461, 341));
        btnDeleteAppointment = new QPushButton(tab_2);
        btnDeleteAppointment->setObjectName("btnDeleteAppointment");
        btnDeleteAppointment->setGeometry(QRect(470, 250, 181, 41));
        btnLogout2 = new QPushButton(tab_2);
        btnLogout2->setObjectName("btnLogout2");
        btnLogout2->setGeometry(QRect(690, 10, 101, 41));
        lnDocLogin = new QLineEdit(tab_2);
        lnDocLogin->setObjectName("lnDocLogin");
        lnDocLogin->setGeometry(QRect(290, 10, 201, 31));
        lnDocPassword = new QLineEdit(tab_2);
        lnDocPassword->setObjectName("lnDocPassword");
        lnDocPassword->setGeometry(QRect(290, 60, 201, 31));
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        btnLogout3 = new QPushButton(tab_4);
        btnLogout3->setObjectName("btnLogout3");
        btnLogout3->setGeometry(QRect(670, 10, 101, 41));
        doctorTableView = new QTableView(tab_4);
        doctorTableView->setObjectName("doctorTableView");
        doctorTableView->setGeometry(QRect(30, 30, 256, 192));
        dtDoctorEdit = new QDateTimeEdit(tab_4);
        dtDoctorEdit->setObjectName("dtDoctorEdit");
        dtDoctorEdit->setGeometry(QRect(30, 230, 194, 22));
        dtDoctorEdit->setCalendarPopup(true);
        btnDoctorEditApp = new QPushButton(tab_4);
        btnDoctorEditApp->setObjectName("btnDoctorEditApp");
        btnDoctorEditApp->setGeometry(QRect(230, 230, 111, 24));
        btnDoctorCancelApp = new QPushButton(tab_4);
        btnDoctorCancelApp->setObjectName("btnDoctorCancelApp");
        btnDoctorCancelApp->setGeometry(QRect(230, 260, 111, 24));
        tabWidget->addTab(tab_4, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        cbAuthRole->setItemText(0, QCoreApplication::translate("MainWindow", "\320\222\321\200\320\260\321\207", nullptr));
        cbAuthRole->setItemText(1, QCoreApplication::translate("MainWindow", "\320\237\320\260\321\206\320\270\320\265\320\275\321\202", nullptr));
        cbAuthRole->setItemText(2, QCoreApplication::translate("MainWindow", "\320\220\320\264\320\274\320\270\320\275\320\270\321\201\321\202\321\200\320\260\321\202\320\276\321\200", nullptr));

        cbAuthRole->setCurrentText(QCoreApplication::translate("MainWindow", "\320\222\321\200\320\260\321\207", nullptr));
        lnLogin->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\273\320\276\320\263\320\270\320\275...", nullptr));
        lnPassword->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214...", nullptr));
        btnLogin->setText(QCoreApplication::translate("MainWindow", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        btnRegister->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214\321\201\321\217 \320\272\320\260\320\272 \320\277\320\260\321\206\320\270\320\265\320\275\321\202", nullptr));
        lnRegName->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\265\320\264\320\270\321\202\320\265 \320\244\320\230\320\236...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\271\321\202\320\270 \320\262\321\200\320\260\321\207\320\260", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\244\320\230\320\236 \320\262\321\200\320\260\321\207\320\260...", nullptr));
        lnPatientName->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\262\320\260\321\210\320\265 \320\244\320\230\320\236...", nullptr));
        btnAppointment->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214\321\201\321\217 \320\275\320\260 \320\277\321\200\320\270\321\221\320\274", nullptr));
        btnPatientDeleteApp->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\274\320\276\321\216 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        btnLogout1->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\320\237\320\260\321\206\320\270\320\265\320\275\321\202", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\321\201\321\202\320\270 \320\244\320\230\320\236  \320\262\321\200\320\260\321\207\320\260", nullptr));
        lineEdit_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\321\201\321\202\320\270 \321\201\320\277\320\265\321\206\320\270\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\216", nullptr));
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\321\201\321\202\320\270 \320\272\320\260\320\261\320\270\320\275\320\265\321\202", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\262\321\200\320\260\321\207\320\260", nullptr));
        btnDeleteAppointment->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\321\203\321\216 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        btnLogout2->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        lnDocLogin->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\321\201\321\202\320\270 \320\273\320\276\320\263\320\270\320\275 \320\262\321\200\320\260\321\207\320\260", nullptr));
        lnDocPassword->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\321\201\321\202\320\270 \320\277\320\260\321\200\320\276\320\273\321\214 \320\262\321\200\320\260\321\207\320\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\320\220\320\264\320\274\320\270\320\275", nullptr));
        btnLogout3->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        btnDoctorEditApp->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\275\320\265\321\201\321\202\320\270 \320\277\321\200\320\270\321\221\320\274", nullptr));
        btnDoctorCancelApp->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214 \320\277\321\200\320\270\321\221\320\274", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "\320\222\321\200\320\260\321\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
