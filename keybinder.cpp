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

#include "keybinder.h"

KeyBinder::KeyBinder(QWidget *parent) :
    QLineEdit(parent)
{
    this->setReadOnly(true);
    this->setFocusPolicy(Qt::StrongFocus);
}

void KeyBinder::keyReleaseEvent(QKeyEvent *e)
{
    qDebug() << "Key released:" << e->key() << Qt::Key_Up;

    if(e->key() == Qt::Key_Up)
        this->setText("Up");
    else if(e->key() == Qt::Key_Down)
        this->setText("Down");
    else if(e->key() == Qt::Key_Left)
        this->setText("Left");
    else if(e->key() == Qt::Key_Right)
        this->setText("Right");
    else if(e->key() == Qt::Key_Space)
        this->setText("Space");
    else if(e->key() == Qt::Key_VolumeUp)
        this->setText("Volume Up");
    else if(e->key() == Qt::Key_VolumeDown)
        this->setText("Volume Down");
    else if(e->key() == Qt::Key_VolumeMute)
        this->setText("Volume Mute");
    else if(e->key() == Qt::Key_PageUp)
        this->setText("Page Up");
    else if(e->key() == Qt::Key_PageDown)
        this->setText("Page Down");
    else if(e->key() == Qt::Key_Delete)
        this->setText("Delete");
    else if(e->key() == Qt::Key_Insert)
        this->setText("Insert");
    else if(e->key() == Qt::Key_End)
        this->setText("End");
    else if(e->key() == Qt::Key_Home)
        this->setText("Home");
    else if(e->key() == Qt::Key_F1)
        this->setText("F1");
    else if(e->key() == Qt::Key_F2)
        this->setText("F2");
    else if(e->key() == Qt::Key_F3)
        this->setText("F3");
    else if(e->key() == Qt::Key_F4)
        this->setText("F4");
    else if(e->key() == Qt::Key_F5)
        this->setText("F5");
    else if(e->key() == Qt::Key_F6)
        this->setText("F6");
    else if(e->key() == Qt::Key_F7)
        this->setText("F7");
    else if(e->key() == Qt::Key_F8)
        this->setText("F8");
    else if(e->key() == Qt::Key_F9)
        this->setText("F9");
    else if(e->key() == Qt::Key_F10)
        this->setText("F10");
    else if(e->key() == Qt::Key_F11)
        this->setText("F11");
    else if(e->key() == Qt::Key_F12)
        this->setText("F12");
    else if(!e->text().isEmpty())
        this->setText(e->text());

   // this->setText( QString::number(e->key()) );
}
