#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "utilfct.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sstream>
#include <QCloseEvent>
#include <QMenu>
#include <QIcon>
#include <string>
#include <QString>
#include <QFileDialog>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createTrayIcon();

    trayIcon->show();

    remoteBinPath = "";
    remoteFilePath = "";

    on_launching();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete trayIcon;
    delete trayIconMenu;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void MainWindow::launch_audio_mixer()
{
    system("xfce4-mixer &");
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    QAction *_show = trayIconMenu->addAction("&Show Window");
    connect(_show, SIGNAL(triggered()), this, SLOT(show()));

    QAction *_mix = trayIconMenu->addAction("&Audio Mixer");
    connect(_mix, SIGNAL(triggered()), this, SLOT(launch_audio_mixer()));

    trayIconMenu->addSeparator();

    QAction *_quit = trayIconMenu->addAction("&Quit");
    connect(_quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setIcon(QIcon(":/images/hdmi_logo.png"));
    trayIcon->setContextMenu(trayIconMenu);

    connect(
            trayIcon,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
           );
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        this->activateWindow();
        this->show();
        this->setFocus();
    }
}

void MainWindow::set_remote_session(bool arg)
{
    //set variables to avoid repetitive fct calls
    bool isExeChosen = is_exe_chosen();
    bool isFileChosen = is_file_chosen();

    //up remote session laucher
    ui->checkBox->setEnabled(!arg);
    ui->pushButton->setEnabled(!arg);
    ui->actionStart_Openbox->setEnabled(!arg);

    //session killing button
    ui->actionKillall_Remote_Sessions->setEnabled(arg);

    //remote shell
    ui->pushButton_2->setEnabled(arg);
    ui->lineEdit->setEnabled(arg);
    ui->pushButton_3->setEnabled(arg);
    ui->checkBox_2->setEnabled(arg);
    ui->label->setEnabled(arg);

    //remote Exe Launcher
    ui->label_4->setEnabled(arg);
    ui->label_3->setEnabled(arg);
    ui->label_2->setEnabled(arg && isExeChosen);
    ui->label_6->setEnabled(arg);
    ui->label_5->setEnabled(arg);
    ui->label_6->setVisible(isExeChosen);
    ui->label_5->setVisible(isFileChosen && isExeChosen);
    ui->pushButton_8->setVisible(arg && isExeChosen);
    ui->pushButton_7->setVisible(arg && isFileChosen && isExeChosen);
    if(arg && isExeChosen)
        ui->label_6->setText(get_qstr_path(remoteBinPath, false, false));
    if(arg && isExeChosen && isFileChosen)
        ui->label_5->setText(get_qstr_path(remoteFilePath, false, false));
    ui->pushButton_5->setEnabled(arg);
    ui->pushButton_4->setEnabled(arg && isExeChosen);
    ui->pushButton_6->setEnabled(arg && isExeChosen);
}

bool MainWindow::is_exe_chosen()
{
    return true ? remoteBinPath.length() > 0 : false;
}

bool MainWindow::is_file_chosen()
{
    return true ? remoteFilePath.length() > 0 : false;
}

void MainWindow::set_xss_running(bool arg)
{
    ui->actionKillall_xscreensaver->setEnabled(arg);
    ui->actionRestart_xscreensaver->setEnabled(!arg);
}

void MainWindow::on_launching()
{
    std::string checker1 = exec("ps cax | grep openbox | wc -l");
    std::string checker2 = exec("ps cax | grep xscreensaver | wc -l");
    ui->actionKillall_Remote_Sessions->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    if(checker1 == "0\n")
        set_remote_session(false);
    else
        set_remote_session(true);

    if(checker2 == "0\n")
        set_xss_running(false);
    else
        set_xss_running(true);
}

void MainWindow::on_pushButton_clicked()
{
    set_remote_session(true);

    system("honeyhdmi_obwrap");

    if(ui->checkBox->isChecked())
    {
        system("killall xscreensaver");
        set_xss_running(false);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);

    const char * home = getenv("HOME");
    QString filePath = dialog.getOpenFileName(this, tr("Open Executable"), home);

    if(filePath != NULL)
        remoteBinPath = filePath;
    set_remote_session(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);

    const char * home = getenv("HOME");
    QString filePath = dialog.getOpenFileName(this, tr("Open File"), tr(home));

    if(filePath != NULL)
        remoteFilePath = filePath;
    set_remote_session(true);
}

void MainWindow::on_pushButton_6_clicked()
{
    std::ostringstream command;
    command << "export DISPLAY=:8; "
            << get_qstr_path(remoteBinPath, true, true).toStdString()
            << ' '
            << get_qstr_path(remoteFilePath, true, true).toStdString()
            << " &";

    std::cout << command.str().c_str();
    system(command.str().c_str());
}

void MainWindow::on_pushButton_3_clicked()
{
    std::string command = "export DISPLAY=:8; "
            + ui->lineEdit->text().toStdString();

    if(ui->checkBox_2->isChecked())
        command += " &";

    command += "\n";

    system(command.c_str());
    ui->lineEdit->setText(QString(""));
}

void MainWindow::on_pushButton_2_clicked()
{
    on_actionKillall_Remote_Sessions_triggered();
}

void MainWindow::on_actionKillall_xscreensaver_triggered()
{
    system("killall xscreensaver");
    set_xss_running(false);
}

void MainWindow::on_actionKillall_Remote_Sessions_triggered()
{
    system("killall openbox");
    set_remote_session(false);
}

void MainWindow::on_actionRestart_xscreensaver_triggered()
{
    system("xscreensaver -no-splash &");
    set_xss_running(true);
}

void MainWindow::on_actionRefresh_UI_triggered()
{
    on_launching();
}

void MainWindow::on_actionStart_Openbox_triggered()
{
    on_pushButton_clicked();
}

void MainWindow::on_actionQuit_2_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionAudio_Mixer_triggered()
{
    launch_audio_mixer();
}

void MainWindow::on_pushButton_8_clicked()
{
    remoteBinPath = "";
    set_remote_session(true);
}

void MainWindow::on_pushButton_7_clicked()
{
    remoteFilePath = "";
    set_remote_session(true);
}
