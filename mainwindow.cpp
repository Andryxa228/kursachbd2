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
    query.prepare("SELECT Appointments.id, Doctors.full_name AS [Врач], Doctors.specialization AS [Спец], Appointments.appointment_date AS [Дата/Время] "
                  "FROM Appointments JOIN Doctors ON Appointments.doctor_id = Doctors.id WHERE Appointments.patient_id = :id;");
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
    // Если модель уже есть, удаляем старую во избежание утечек памяти
    QAbstractItemModel *oldModel = ui->tableView->model();
    if (oldModel) {
        oldModel->deleteLater();
    }

    QSqlTableModel *model = new QSqlTableModel(ui->tableView);
    model->setTable("Doctors");
    model->select(); // Загружаем данные из таблицы

    // Красивые названия колонок (индексы зависят от структуры твоей БД)
    // Предполагаю, что: 0-id, 1-full_name, 2-specialization, 3-cabinet, 4-login, 5-password
    model->setHeaderData(1, Qt::Horizontal, "ФИО врача");
    model->setHeaderData(2, Qt::Horizontal, "Специализация");
    model->setHeaderData(3, Qt::Horizontal, "Кабинет");

    ui->tableView->setModel(model);

    // ОБЯЗАТЕЛЬНО скрываем технические колонки, чтобы пациенты не видели пароли!
    ui->tableView->hideColumn(0); // Скрываем id

    // ВАЖНО: Укажи правильные индексы колонок логина и пароля, чтобы скрыть их.
    // Если они 4 и 5, то:
    ui->tableView->hideColumn(4); // Скрываем login
    ui->tableView->hideColumn(5); // Скрываем password (хеш)

    ui->tableView->resizeColumnsToContents();
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

    // --- СКРИПТ АВТОМАТИЧЕСКОЙ ЗАМЕНЫ ПАРОЛЕЙ НА ХЕШИ ---
    QStringList tables = {"Admins", "Patients", "Doctors"};
    for(const QString &table : tables) {
        QSqlQuery q("SELECT id, password FROM " + table);
        while(q.next()){
            QString pass = q.value(1).toString();
            // Если длина пароля не 64 (длина SHA-256), значит он не хеширован
            if(pass.length() != 64) {
                QSqlQuery up;
                up.prepare("UPDATE " + table + " SET password = :p WHERE id = :id");
                up.bindValue(":p", hashPassword(pass));
                up.bindValue(":id", q.value(0).toInt());
                up.exec();
            }
        }
    }
    // --- КОНЕЦ СКРИПТА ---

    populateDoctors(ui);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dtDoctorEdit->setDateTime(QDateTime::currentDateTime());

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
        // Здесь используем login вместо id, так как мы выяснили, что id в таблице Admins может не быть
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
        // ВАЖНО 1: Мы добавили full_name в SELECT, иначе базе нечего будет возвращать!
        query.prepare("SELECT id, full_name FROM Patients WHERE login = :l AND password = :p");
        query.bindValue(":l", login);
        query.bindValue(":p", hashed);
        query.exec();

        if (query.next()) {
            currentPatientId = query.value(0).toInt();

            // ВАЖНО 2: Вытаскиваем ФИО (это индекс 1, так как id имеет индекс 0)
            QString patientName = query.value(1).toString();

            // ВАЖНО 3: Подставляем ФИО в наш Label
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

// === ОСТАЛЬНЫЕ ФУНКЦИИ ===

void MainWindow::on_btnAppointment_clicked() {
    int d_id = ui->cbDoctors->currentData().toInt();
    QString dt = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm");

    QSqlQuery ch;
    ch.prepare("SELECT id FROM Appointments WHERE doctor_id = :d AND appointment_date = :dt");
    ch.bindValue(":d", d_id); ch.bindValue(":dt", dt);
    ch.exec();
    if(ch.next()){
        QMessageBox::warning(this, "Занято", "Врач занят на это время!");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO Appointments (patient_id, doctor_id, appointment_date) VALUES (:p, :d, :dt)");
    q.bindValue(":p", currentPatientId); q.bindValue(":d", d_id); q.bindValue(":dt", dt);
    if(q.exec()){
        QMessageBox::information(this, "Успех", "Запись создана!");
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


void MainWindow::on_btnExitApp_clicked()
{
    this->close();
}

