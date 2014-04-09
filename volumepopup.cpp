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

#include "volumepopup.h"
#include "ui_volumepopup.h"

VolumePopup::VolumePopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolumePopup)
{
    ui->setupUi(this);

    timeOut = new QTimer(this);

    connect(timeOut,SIGNAL(timeout()), this, SLOT(hideAudio()));

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);

    this->setAutoFillBackground(true);
    this->setWindowFlags( windowFlags() | Qt::SplashScreen );
    //this->show();
    setRoundedCorners(10,10,10,10);
}



void VolumePopup::hideAudio()
{
    Qt::WindowFlags eFlags = this->windowFlags();
    eFlags &= ~Qt::WindowStaysOnTopHint;
    this->setWindowFlags(eFlags);
}

void VolumePopup::showAudio(bool mute, double volume)
{
    QPixmap icon;

    if(mute)
    {
        icon = QPixmap(":/images/Status-audio-volume-muted-icon.png");
    }
    else
    {
        if(volume <= 0.35)
            icon = QPixmap(":/images/Status-audio-volume-low-icon.png");
        else if(volume <= 0.75)
            icon = QPixmap(":/images/Status-audio-volume-medium-icon.png");
        else
            icon = QPixmap(":/images/Status-audio-volume-high-icon.png");
    }

    ui->iconLbl->setPixmap( icon );

    ui->progressBar->setValue( volume*100 );
    this->show();
    Qt::WindowFlags eFlags = this->windowFlags();
    eFlags |= Qt::WindowStaysOnTopHint;
    this->setWindowFlags(eFlags);

    timeOut->start(1000);
}

VolumePopup::~VolumePopup()
{
    delete ui;
}

void VolumePopup::setRoundedCorners(int radius_tl, int radius_tr, int radius_bl, int radius_br)
{
    QRegion region(0, 0, width(), height(), QRegion::Rectangle);

    // top left
    QRegion round (0, 0, 2*radius_tl, 2*radius_tl, QRegion::Ellipse);
    QRegion corner(0, 0, radius_tl, radius_tl, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));

    // top right
    round = QRegion(width()-2*radius_tr, 0, 2*radius_tr, 2*radius_tr, QRegion::Ellipse);
    corner = QRegion(width()-radius_tr, 0, radius_tr, radius_tr, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));

    // bottom right
    round = QRegion(width()-2*radius_br, height()-2*radius_br, 2*radius_br, 2*radius_br, QRegion::Ellipse);
    corner = QRegion(width()-radius_br, height()-radius_br, radius_br, radius_br, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));

    // bottom left
    round = QRegion(0, height()-2*radius_bl, 2*radius_bl, 2*radius_bl, QRegion::Ellipse);
    corner = QRegion(0, height()-radius_bl, radius_bl, radius_bl, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));

    setMask(region);
}
