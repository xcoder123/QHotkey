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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(checkKeyPress()));
    timer->setInterval(100);
    timer->start();


    configAction = new QAction( QIcon(":/images/config.png"), tr("Configure"), this);
    connect(configAction,SIGNAL(triggered()), this, SLOT(showNormal()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(configAction);
    trayIconMenu->addAction(ui->actionAbout);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(ui->actionQuit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu( trayIconMenu );

    QIcon icon(":/images/k-hot-keys-icon.png");
    trayIcon->setIcon(icon);

    trayIcon->show();

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));


    modifier = 0;
    connect(ui->modifierComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(modifierChanged()));
    connect(ui->volumeUpEdit,SIGNAL(textChanged(QString)),this, SLOT(volumeUpKeyChanged(QString)));
    connect(ui->volumeDownEdit, SIGNAL(textChanged(QString)), this, SLOT(volumeDownKeyChanged(QString)));
    connect(ui->muteEdit, SIGNAL(textChanged(QString)), this, SLOT(volumeMuteKeyChanged(QString)));

    volumeUp = 0;
    volumeDown = 0;
    muteButton = 0;

    volumePopup = new VolumePopup();

    this->hide();

    trayIcon->showMessage("Information",tr("The program will keep running in the "
                                           "system tray. To terminate the program, "
                                           "choose \"Quit\" in the context menu "
                                           "of the system tray entry."),
                          QSystemTrayIcon::Information,
                          2 * 1000);
//    volumePopup->hide();

    settings = new QSettings("HKEY_CURRENT_USER\\Software\\morf\\QHotkey",
                                                QSettings::NativeFormat);


    ui->modifierComboBox->setCurrentText( settings->value("modifier", "Alt").toString() );
    ui->volumeUpEdit->setText(settings->value("volume_up", "Page Up").toString());
    ui->volumeDownEdit->setText(settings->value("volume_down", "Page Down").toString());
    ui->muteEdit->setText(settings->value("mute", "End").toString());

    QSettings settingsRun("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if(settingsRun.value("QHotkey", "nop").toString() != "nop")
        ui->runCheckbox->setChecked(true);

    connect(ui->runCheckbox, SIGNAL(toggled(bool)), this, SLOT(runStartup(bool)));

    muteReleased = true;

    aboutDialog = new AboutDialog(this);
}

void MainWindow::runStartup(bool run)
{
    QSettings settingsRun("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(run)
    {
        QString value = "\"" + QCoreApplication::applicationFilePath() + "\"";
        value.replace("/","\\");
        settingsRun.setValue("QHotkey", value);
    }
    else
        settingsRun.remove("QHotkey");
}

void MainWindow::about()
{
    aboutDialog->exec();
    qDebug() << "DAFUQ0";
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        break;
    default:
        ;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
         QMessageBox::information(this, tr("Systray"),
                                  tr("The program will keep running in the "
                                     "system tray. To terminate the program, "
                                     "choose <b>Quit</b> in the context menu "
                                     "of the system tray entry."));
         hide();
         event->ignore();
     }
}

void MainWindow::volumeUpKeyChanged(QString key)
{
    volumeUp = mapWindowsKey(key);
    settings->setValue( "volume_up", key );
    if(volumeUp == 0x3a)
        ui->volumeUpEdit->setText("Page Up");
}

void MainWindow::volumeDownKeyChanged(QString key)
{
    volumeDown = mapWindowsKey(key);
    settings->setValue( "volume_down", key );
    if(volumeDown == 0x3a)
        ui->volumeDownEdit->setText("Page Down");
}

void MainWindow::volumeMuteKeyChanged(QString key)
{
    muteButton = mapWindowsKey(key);
    settings->setValue( "mute", key );
    if(muteButton == 0x3a)
        ui->muteEdit->setText("mute");
}

MainWindow::~MainWindow()
{
    delete volumePopup;
    delete aboutDialog;
    delete ui;
}

void MainWindow::modifierChanged()
{
    switch(ui->modifierComboBox->currentIndex())
    {
        case 1: modifier = VK_CONTROL; break;
        case 2: modifier = VK_MENU; break;
        case 3: modifier = VK_SHIFT; break;
        default: modifier = 0; break;
    }

    settings->setValue( "modifier", ui->modifierComboBox->currentText() );
}

void MainWindow::checkKeyPress()
{
    if(this->isActiveWindow())
        return;

    if(modifier != 0)
    {
        if((unsigned short)(GetAsyncKeyState(modifier)) == 0)
        {
            muteReleased = true;
            return;
        }
//        qDebug() << "Modifier: " <<  GetAsyncKeyState(modifier);
    }

    if(volumeUp != 0)
    {
//        qDebug() << "Volume Up: " <<  GetAsyncKeyState(volumeUp);
        if((unsigned short)(GetAsyncKeyState(volumeUp)) != 0)
        {
            changeVolume(0.05, true);
            volumeStatus status = getCurrentVolume();
            volumePopup->showAudio(status.mute,status.volume);
        }
    }

    if(volumeDown != 0)
    {
//        qDebug() << "Volume Down: " <<  GetAsyncKeyState(volumeDown);
        if((unsigned short)(GetAsyncKeyState(volumeDown)) != 0)
        {
            changeVolume(-0.05, true);
            volumeStatus status = getCurrentVolume();
            volumePopup->showAudio(status.mute,status.volume);
        }
    }

    if(muteButton != 0)
    {
//        qDebug() << "Mute: " <<  GetAsyncKeyState(muteButton);
        if((unsigned short)(GetAsyncKeyState(muteButton)) != 0)
        {
            if(muteReleased)
            {
                triggerMuteVolume();
                muteReleased = false;
            }

            volumeStatus status = getCurrentVolume();
            volumePopup->showAudio(status.mute,status.volume);
        }
        else
            muteReleased = true;
    }



}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    qDebug() << e;
    qDebug() << GetAsyncKeyState(VK_MENU);
    if(e->key() == Qt::Key_Down)
        changeVolume(-0.02,true);
    if(e->key() == Qt::Key_Up)
        changeVolume(0.02,true);


}

volumeStatus MainWindow::getCurrentVolume()
{
    HRESULT hr=NULL;
    volumeStatus status;
    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    IAudioEndpointVolume *endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
         CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;

    // -------------------------
    float currentVolume = 0;
    //endpointVolume->GetMasterVolumeLevel(&currentVolume);
    //printf("Current volume in dB is: %f\n", currentVolume);

    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);


    BOOL mute;
    hr = endpointVolume->GetMute(&mute);
    status.mute = mute;

    endpointVolume->Release();

    CoUninitialize();

    status.volume = currentVolume;

    return status;
}

void MainWindow::triggerMuteVolume()
{
    HRESULT hr=NULL;

    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    IAudioEndpointVolume *endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
         CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;

    BOOL mute;
    hr = endpointVolume->GetMute(&mute);

    mute = !mute;
    hr = endpointVolume->SetMute(mute, NULL);

    endpointVolume->Release();

    CoUninitialize();
}

bool MainWindow::changeVolume(double dVolume, bool bScalar)
{
    HRESULT hr=NULL;
    double newVolume=0;

    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    IAudioEndpointVolume *endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
         CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;

    // -------------------------
    float currentVolume = 0;
    endpointVolume->GetMasterVolumeLevel(&currentVolume);
    //printf("Current volume in dB is: %f\n", currentVolume);

    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
    //CString strCur=L"";
    //strCur.Format(L"%f",currentVolume);
    //AfxMessageBox(strCur);
    newVolume = currentVolume + dVolume;
    if(newVolume < 0)
        newVolume = 0.0;

    if(newVolume > 1.0)
        newVolume = 1.0;

//     qDebug("Current volume as a scalar is: %f\n", currentVolume);
    if (bScalar==false)
    {
        hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
    }
    else if (bScalar==true)
    {
        hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
    }
    endpointVolume->Release();

    CoUninitialize();

    return FALSE;
}

char MainWindow::mapWindowsKey(QString key)
{
    if(key.isEmpty())
        return 0x3a;

    if(key.toLower() == "space" )
        return VK_SPACE;
    if(key.toLower() == "page up" )
        return VK_PRIOR;
    if(key.toLower() == "page down" )
        return VK_NEXT;
    if(key.toLower() == "end" )
        return VK_END;
    if(key.toLower() == "home" )
        return VK_HOME;
    if(key.toLower() == "left" )
        return VK_LEFT;
    if(key.toLower() == "up" )
        return VK_UP;
    if(key.toLower() == "right" )
        return VK_RIGHT;
    if(key.toLower() == "down" )
        return VK_DOWN;
    if(key.toLower() == "select" )
        return VK_SELECT;
    if(key.toLower() == "insert" )
        return VK_INSERT;
    if(key.toLower() == "delete" )
        return VK_DELETE;
    if(key.toLower().at(0) >= '0' && key.toLower().at(0) <= '9' )
        return key.toLower().at(0).toLatin1();
    if(key.toLower().at(0) >= 'a' && key.toLower().at(0) <= 'z' )
        return key.toUpper().at(0).toLatin1();
    if(key.toLower() == "+" )
        return VK_ADD;
    if(key.toLower() == "*" )
        return VK_MULTIPLY;
    if(key.toLower() == "-" )
        return VK_SUBTRACT;
    if(key.toLower() == "." )
        return VK_DECIMAL;
    if(key.toLower() == "/" )
        return VK_DIVIDE;
    if(key.toLower() == "f1" )
        return VK_F1;
    if(key.toLower() == "f2" )
        return VK_F2;
    if(key.toLower() == "f3" )
        return VK_F3;
    if(key.toLower() == "f4" )
        return VK_F4;
    if(key.toLower() == "f5" )
        return VK_F5;
    if(key.toLower() == "f6" )
        return VK_F6;
    if(key.toLower() == "f7" )
        return VK_F7;
    if(key.toLower() == "f8" )
        return VK_F8;
    if(key.toLower() == "f9" )
        return VK_F9;
    if(key.toLower() == "f10" )
        return VK_F10;
    if(key.toLower() == "f11" )
        return VK_F11;
    if(key.toLower() == "f12" )
        return VK_F12;
    if(key.toLower() == "volume up" )
        return VK_VOLUME_UP;
    if(key.toLower() == "volume down" )
        return VK_VOLUME_DOWN;
    if(key.toLower() == "volume mute" )
        return VK_VOLUME_MUTE;
    if(key.toLower() == "," )
        return VK_OEM_COMMA;

    return 0x3a;
}
