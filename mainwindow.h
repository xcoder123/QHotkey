/*
 * Official webpage: http://morf.lv
 *
The MIT License (MIT)

Copyright (c) 2014 Raivis Strogonovs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include "aboutdialog.h"


#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "volumepopup.h"

namespace Ui {
class MainWindow;
}

struct volumeStatus
{
    float volume;
    bool mute;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool changeVolume(double dVolume, bool bScalar);
    void triggerMuteVolume();

public slots:
    void checkKeyPress();
    void modifierChanged();
    void volumeUpKeyChanged(QString key);
    void volumeDownKeyChanged(QString key);
    void volumeMuteKeyChanged(QString key);

    char mapWindowsKey(QString key);
    volumeStatus getCurrentVolume();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void about();

    void runStartup(bool run);


private:
    Ui::MainWindow *ui;
    VolumePopup * volumePopup;
    QTimer* timer;

    char modifier;
    char volumeUp;
    char volumeDown;
    char muteButton;

    QAction *configAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu* trayIconMenu;

    QSettings *settings;
    AboutDialog * aboutDialog;

    bool muteReleased;



protected:
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H
