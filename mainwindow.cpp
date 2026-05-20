#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDateTime>
#include <QCryptographicHash>

// Функция хеширования
QString hashPassword(QString password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

// === ФУНКЦИИ ОБНОВЛЕНИЯ ИНТЕРФЕЙСА ===

void updateAdminTable(Ui::MainWindow *ui) {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT Appointments.id, Patients.full_name AS [Пациент], Doctors.full_name AS [Врач], Appointments.appointment_date AS [Дата/Время] "
                    "FROM Appointments JOIN Patients ON Appointments.patient_id = Patients.id JOIN Doctors ON Appointments.doctor_id = Doctors.id;");
    ui->appointmentsTableView->setModel(model);
    ui->appointmentsTableView->resizeColumnsToContents();
}

void updatePatientTable(Ui::MainWindow *ui, int patientId) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    // LEFT JOIN к таблице Services выводит название процедуры и её стоимость в историю приемов пациента
    query.prepare("SELECT Appointments.id, Doctors.full_name AS [Врач], Doctors.specialization AS [Спец], "
                  "Services.service_name AS [Услуга], Services.price AS [Цена (руб.)], Appointments.appointment_date AS [Дата/Время] "
                  "FROM Appointments "
                  "JOIN Doctors ON Appointments.doctor_id = Doctors.id "
                  "LEFT JOIN Services ON Appointments.service_id = Services.id "
                  "WHERE Appointments.patient_id = :id;");
    query.bindValue(":id", patientId);
    query.exec();
    model->setQuery(std::move(query));
    ui->myAppointmentsTableView->setModel(model);
    ui->myAppointmentsTableView->hideColumn(0);
    ui->myAppointmentsTableView->resizeColumnsToContents();
}

void updateDoctorTable(Ui::MainWindow *ui, int doctorId) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT Appointments.id, Patients.full_name AS [Пациент], Appointments.appointment_date AS [Дата/Время] "
                  "FROM Appointments JOIN Patients ON Appointments.patient_id = Patients.id WHERE Appointments.doctor_id = :docId;");
    query.bindValue(":docId", doctorId);
    query.exec();
    model->setQuery(std::move(query));
    ui->doctorTableView->setModel(model);
    ui->doctorTableView->hideColumn(0);
    ui->doctorTableView->resizeColumnsToContents();
}

void updateAllDoctorsTable(Ui::MainWindow *ui) {
    QAbstractItemModel *oldModel = ui->tableView->model();
    if (oldModel) {
        oldModel->deleteLater();
    }

    QSqlTableModel *model = new QSqlTableModel(ui->tableView);
    model->setTable("Doctors");
    model->select();

    model->setHeaderData(1, Qt::Horizontal, "ФИО врача");
    model->setHeaderData(2, Qt::Horizontal, "Специализация");
    model->setHeaderData(3, Qt::Horizontal, "Кабинет");

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(4);
    ui->tableView->hideColumn(5);

    ui->tableView->resizeColumnsToContents();
}

// ФУНКЦИЯ ДЛЯ ВЫВОДА РАСПИСАНИЯ И ФИЛЬТРАЦИИ УСЛУГ ВРАЧА
void updateDoctorDetails(Ui::MainWindow *ui, int doctorId) {
    // 1. Получаем расписание выбранного врача из БД
    QSqlQuery schedQuery;
    schedQuery.prepare("SELECT day_of_week, start_time, end_time FROM Schedules WHERE doctor_id = :docId;");
    schedQuery.bindValue(":docId", doctorId);
    schedQuery.exec();

    QString scheduleText = "График работы: ";
    bool hasSchedule = false;

    while (schedQuery.next()) {
        hasSchedule = true;
        scheduleText += schedQuery.value(0).toString() + " (" +
                        schedQuery.value(1).toString() + "-" +
                        schedQuery.value(2).toString() + "); ";
    }
    if (!hasSchedule) {
        scheduleText += "не задан";
    }
    // Выводим текст в твою новую метку Label
    ui->lblDoctorSchedule->setText(scheduleText);

    // 2. Получаем список услуг конкретно этого врача
    ui->cbServices->clear(); // Стираем старые услуги в комбобоксе
    QSqlQuery servQuery;
    servQuery.prepare("SELECT id, service_name, price FROM Services WHERE doctor_id = :docId;");
    servQuery.bindValue(":docId", doctorId);
    servQuery.exec();

    while (servQuery.next()) {
        QString itemText = servQuery.value(1).toString() + " (" + servQuery.value(2).toString() + " руб.)";
        ui->cbServices->addItem(itemText, servQuery.value(0).toInt()); // Кладем имя, прячем внутри id услуги
    }
}

void populateDoctors(Ui::MainWindow *ui) {
    ui->cbDoctors->clear();
    QSqlQuery query("SELECT id, full_name, specialization FROM Doctors;");
    while (query.next()) {
        ui->cbDoctors->addItem(query.value(1).toString() + " (" + query.value(2).toString() + ")", query.value(0).toInt());
    }
}

// === КОНСТРУКТОР С МИГРАЦИЕЙ ПАРОЛЕЙ ===

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Информационная система поликлиники");
    this->setWindowIcon(QIcon(":/icon.png"));

    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hospital.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка", "БД не открыта!");
        return;
    }

    // Скрипт автоматической замены паролей на хеши
    QStringList tables = {"Admins", "Patients", "Doctors"};
    for(const QString &table : tables) {
        QSqlQuery q("SELECT id, password FROM " + table);
        while(q.next()){
            QString pass = q.value(1).toString();
            if(pass.length() != 64) {
                QSqlQuery up;
                up.prepare("UPDATE " + table + " SET password = :p WHERE id = :id");
                up.bindValue(":p", hashPassword(pass));
                up.bindValue(":id", q.value(0).toInt());
                up.exec();
            }
        }
    }

    populateDoctors(ui);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dtDoctorEdit->setDateTime(QDateTime::currentDateTime());

    // АВТОМАТИКА: при смене врача в списке, автоматически обновляем расписание и услуги
    connect(ui->cbDoctors, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        if (ui->cbDoctors->currentIndex() != -1) {
            int doctorId = ui->cbDoctors->currentData().toInt();
            updateDoctorDetails(ui, doctorId);
        }
    });

    // Вызываем один раз при старте для первого врача
    if (ui->cbDoctors->count() > 0) {
        updateDoctorDetails(ui, ui->cbDoctors->currentData().toInt());
    }

    connect(ui->btnLogout1, &QPushButton::clicked, this, &MainWindow::logout);
    connect(ui->btnLogout2, &QPushButton::clicked, this, &MainWindow::logout);
    connect(ui->btnLogout3, &QPushButton::clicked, this, &MainWindow::logout);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::logout() {
    currentPatientId = -1;
    currentDoctorId = -1;
    ui->lnLogin->clear();
    ui->lnPassword->clear();
    ui->tabWidget->setCurrentIndex(0);
}

// === АВТОРИЗАЦИЯ ===

void MainWindow::on_btnLogin_clicked() {
    QString role = ui->cbAuthRole->currentText().trimmed();
    QString login = ui->lnLogin->text().trimmed();
    QString rawPass = ui->lnPassword->text().trimmed();

    if (login.isEmpty() || rawPass.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Заполните все поля!");
        return;
    }

    QString hashed = hashPassword(rawPass);

    if (role == "Администратор" || role == "Админ") {
        QSqlQuery query;
        query.prepare("SELECT login FROM Admins WHERE login = :l AND password = :p");
        query.bindValue(":l", login);
        query.bindValue(":p", hashed);
        query.exec();

        if (query.next()) {
            ui->tabWidget->setCurrentIndex(2);
            updateAdminTable(ui);
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль админа!");
        }
    }
    else if (role == "Пациент") {
        QSqlQuery query;
        query.prepare("SELECT id, full_name FROM Patients WHERE login = :l AND password = :p");
        query.bindValue(":l", login);
        query.bindValue(":p", hashed);
        query.exec();

        if (query.next()) {
            currentPatientId = query.value(0).toInt();
            QString patientName = query.value(1).toString();
            ui->lblPatientName->setText("Вы вошли как: " + patientName);

            ui->tabWidget->setCurrentIndex(1);
            updatePatientTable(ui, currentPatientId);
            updateAllDoctorsTable(ui);
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль пациента!");
        }
    }
    else if (role == "Врач") {
        QSqlQuery query;
        query.prepare("SELECT id FROM Doctors WHERE login = :l AND password = :p");
        query.bindValue(":l", login);
        query.bindValue(":p", hashed);
        query.exec();

        if (query.next()) {
            currentDoctorId = query.value(0).toInt();
            ui->tabWidget->setCurrentIndex(3);
            updateDoctorTable(ui, currentDoctorId);
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль врача!");
        }
    }
}

// === РЕГИСТРАЦИЯ И ДОБАВЛЕНИЕ С ПРОВЕРКОЙ ЛОГИНА ===

void MainWindow::on_btnRegister_clicked() {
    QString name = ui->lnRegName->text().trimmed();
    QString login = ui->lnLogin->text().trimmed();
    QString pass = ui->lnPassword->text().trimmed();

    QSqlQuery check;
    check.prepare("SELECT login FROM Patients WHERE login = :l UNION SELECT login FROM Admins WHERE login = :l");
    check.bindValue(":l", login);
    check.exec();
    if(check.next()){
        QMessageBox::warning(this, "Ошибка", "Логин занят!");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO Patients (full_name, login, password) VALUES (:n, :l, :p)");
    q.bindValue(":n", name);
    q.bindValue(":l", login);
    q.bindValue(":p", hashPassword(pass));
    if(q.exec()) QMessageBox::information(this, "Успех", "Вы зарегистрированы!");
}

void MainWindow::on_pushButton_2_clicked() {
    QString name = ui->lineEdit_2->text().trimmed();
    QString spec = ui->lineEdit_3->text().trimmed();
    QString cab = ui->lineEdit_4->text().trimmed();
    QString log = ui->lnDocLogin->text().trimmed();
    QString pass = ui->lnDocPassword->text().trimmed();

    QSqlQuery check;
    check.prepare("SELECT login FROM Doctors WHERE login = :l");
    check.bindValue(":l", log);
    check.exec();
    if(check.next()){
        QMessageBox::warning(this, "Ошибка", "Логин врача занят!");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO Doctors (full_name, specialization, cabinet, login, password) VALUES (:n, :s, :c, :l, :p)");
    q.bindValue(":n", name); q.bindValue(":s", spec); q.bindValue(":c", cab);
    q.bindValue(":l", log); q.bindValue(":p", hashPassword(pass));
    if(q.exec()){
        QMessageBox::information(this, "Успех", "Врач добавлен!");
        populateDoctors(ui);
    }
}

// === ОБНОВЛЕННАЯ ЗАПИСЬ НА ПРИЕМ С ПРОВЕРКОЙ РАБОЧЕГО ВРЕМЕНИ ===

void MainWindow::on_btnAppointment_clicked() {
    if (ui->cbDoctors->currentIndex() == -1 || ui->cbServices->currentIndex() == -1) {
        QMessageBox::warning(this, "Ошибка", "Выберите врача и услугу!");
        return;
    }

    int d_id = ui->cbDoctors->currentData().toInt();
    int s_id = ui->cbServices->currentData().toInt();

    // Получаем выбранную дату и время
    QDateTime selectedDateTime = ui->dateTimeEdit->dateTime();
    QString dtStr = selectedDateTime.toString("yyyy-MM-dd HH:mm");

    // 1. ОПРЕДЕЛЯЕМ ДЕНЬ НЕДЕЛИ НА РУССКОМ
    int dayOfWeekNum = selectedDateTime.date().dayOfWeek();
    QString dayOfWeekRu;
    switch (dayOfWeekNum) {
    case 1: dayOfWeekRu = "Понедельник"; break;
    case 2: dayOfWeekRu = "Вторник"; break;
    case 3: dayOfWeekRu = "Среда"; break;
    case 4: dayOfWeekRu = "Четверг"; break;
    case 5: dayOfWeekRu = "Пятница"; break;
    case 6: dayOfWeekRu = "Суббота"; break;
    case 7: dayOfWeekRu = "Воскресенье"; break;
    }

    QString selectedTimeStr = selectedDateTime.toString("HH:mm");

    // 2. ПРОВЕРКА ПО ТАБЛИЦЕ РАСПИСАНИЯ (Schedules)
    QSqlQuery schedCheck;
    schedCheck.prepare("SELECT start_time, end_time FROM Schedules WHERE doctor_id = :d AND day_of_week = :day");
    schedCheck.bindValue(":d", d_id);
    schedCheck.bindValue(":day", dayOfWeekRu);
    schedCheck.exec();

    if (!schedCheck.next()) {
        QMessageBox::warning(this, "Ошибка", QString("Врач не работает в этот день (%1)!").arg(dayOfWeekRu));
        return;
    }

    // Если рабочий день найден, проверяем часы смены
    QString startTimeStr = schedCheck.value(0).toString();
    QString endTimeStr = schedCheck.value(1).toString();

    if (selectedTimeStr < startTimeStr || selectedTimeStr > endTimeStr) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Врач в этот день работает с %1 до %2.\nВы выбрали время: %3.")
                                 .arg(startTimeStr).arg(endTimeStr).arg(selectedTimeStr));
        return;
    }

    // 3. ПРОВЕРКА НА ЗАНЯТОСТЬ НА ЭТО ЖЕ ВРЕМЯ
    QSqlQuery ch;
    ch.prepare("SELECT id FROM Appointments WHERE doctor_id = :d AND appointment_date = :dt");
    ch.bindValue(":d", d_id);
    ch.bindValue(":dt", dtStr);
    ch.exec();
    if(ch.next()){
        QMessageBox::warning(this, "Занято", "Врач уже занят на это точное время!");
        return;
    }

    // 4. ЗАПИСЬ В БАЗУ ДАННЫХ
    QSqlQuery q;
    q.prepare("INSERT INTO Appointments (patient_id, doctor_id, service_id, appointment_date) VALUES (:p, :d, :s, :dt)");
    q.bindValue(":p", currentPatientId);
    q.bindValue(":d", d_id);
    q.bindValue(":s", s_id); // Сохраняем ID выбранной услуги
    q.bindValue(":dt", dtStr);
    if(q.exec()){
        QMessageBox::information(this, "Успех", "Запись на услугу успешно создана!");
        updatePatientTable(ui, currentPatientId);
    }
}

void MainWindow::on_btnDoctorEditApp_clicked() {
    QModelIndex idx = ui->doctorTableView->currentIndex();
    if(!idx.isValid()) return;
    int id = ui->doctorTableView->model()->data(ui->doctorTableView->model()->index(idx.row(), 0)).toInt();
    QString dt = ui->dtDoctorEdit->dateTime().toString("yyyy-MM-dd HH:mm");

    QSqlQuery q;
    q.prepare("UPDATE Appointments SET appointment_date = :dt WHERE id = :id");
    q.bindValue(":dt", dt); q.bindValue(":id", id);
    if(q.exec()) updateDoctorTable(ui, currentDoctorId);
}

void MainWindow::on_btnDoctorCancelApp_clicked() {
    QModelIndex idx = ui->doctorTableView->currentIndex();
    if(!idx.isValid()) return;
    int id = ui->doctorTableView->model()->data(ui->doctorTableView->model()->index(idx.row(), 0)).toInt();
    QSqlQuery q;
    q.prepare("DELETE FROM Appointments WHERE id = :id");
    q.bindValue(":id", id);
    if(q.exec()) updateDoctorTable(ui, currentDoctorId);
}

void MainWindow::on_btnDeleteAppointment_clicked() {
    QModelIndex idx = ui->appointmentsTableView->currentIndex();
    if(!idx.isValid()) return;
    int id = ui->appointmentsTableView->model()->data(ui->appointmentsTableView->model()->index(idx.row(), 0)).toInt();
    QSqlQuery q;
    q.prepare("DELETE FROM Appointments WHERE id = :id");
    q.bindValue(":id", id);
    if(q.exec()) updateAdminTable(ui);
}

void MainWindow::on_btnPatientDeleteApp_clicked() {
    QModelIndex idx = ui->myAppointmentsTableView->currentIndex();
    if(!idx.isValid()) return;
    int id = ui->myAppointmentsTableView->model()->data(ui->myAppointmentsTableView->model()->index(idx.row(), 0)).toInt();
    QSqlQuery q;
    q.prepare("DELETE FROM Appointments WHERE id = :id");
    q.bindValue(":id", id);
    if(q.exec()) updatePatientTable(ui, currentPatientId);
}

void MainWindow::on_pushButton_clicked() {
    QString f = ui->lineEdit->text();
    QSqlTableModel *m = qobject_cast<QSqlTableModel*>(ui->tableView->model());
    if(m){ m->setFilter(QString("full_name LIKE '%%1%'").arg(f)); m->select(); }
}

void MainWindow::on_btnExitApp_clicked() {
    this->close();
}
