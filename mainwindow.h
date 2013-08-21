#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QString>

std::string exec(const char* cmd);
QString get_qstr_path(const QString remotePath, bool full_path=true, bool escaped=false);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void on_launching();

    void set_remote_session(bool arg);

    void set_xss_running(bool arg);

    bool is_exe_chosen();

    bool is_file_chosen();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionKillall_xscreensaver_triggered();

    void on_actionKillall_Remote_Sessions_triggered();

    void on_actionRestart_xscreensaver_triggered();

    void on_actionRefresh_UI_triggered();

    void on_actionStart_Openbox_triggered();

    void trayIconClicked(QSystemTrayIcon::ActivationReason);

    void launch_audio_mixer();

    void on_actionQuit_2_triggered();

    void on_actionAudio_Mixer_triggered();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

private:
    void createTrayIcon();
    void closeEvent(QCloseEvent *);

    Ui::MainWindow *ui;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QString remoteBinPath;
    QString remoteFilePath;

};

#endif // MAINWINDOW_H
