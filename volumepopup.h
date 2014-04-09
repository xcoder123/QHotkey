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

#ifndef VOLUMEPOPUP_H
#define VOLUMEPOPUP_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class VolumePopup;
}

class VolumePopup : public QDialog
{
    Q_OBJECT

public:
    explicit VolumePopup(QWidget *parent = 0);
    ~VolumePopup();

    void showAudio(bool mute, double volume);

public slots:
    void hideAudio();

private:
    Ui::VolumePopup *ui;
    QTimer* timeOut;

    void setRoundedCorners(int radius_tl, int radius_tr, int radius_bl, int radius_br);
};

#endif // VOLUMEPOPUP_H
