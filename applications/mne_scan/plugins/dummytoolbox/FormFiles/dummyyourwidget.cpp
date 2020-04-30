//=============================================================================================================
/**
 * @file     dummyyourwidget.cpp
 * @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
 *           Lorenz Esch <lesch@mgh.harvard.edu>;
 *           Viktor Klueber <Viktor.Klueber@tu-ilmenau.de>
 * @since    0.1.0
 * @date     February, 2013
 *
 * @section  LICENSE
 *
 * Copyright (C) 2013, Christoph Dinh, Lorenz Esch, Viktor Klueber. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 * the following conditions are met:
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *       following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *       the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @brief    Definition of the DummyYourWidget class.
 *
 */

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "dummyyourwidget.h"
#include "../ui_dummyyourwidget.h"

//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSettings>

//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace DUMMYTOOLBOXPLUGIN;

//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

DummyYourWidget::DummyYourWidget(const QString& sSettingsPath,
                                 QWidget *parent)
: QWidget(parent)
, m_pUi(new Ui::DummyYourWidgetGui)
, m_sSettingsPath(sSettingsPath)
{
    m_pUi->setupUi(this);

    loadSettings(m_sSettingsPath);
}

//=============================================================================================================

DummyYourWidget::~DummyYourWidget()
{
    saveSettings(m_sSettingsPath);

    delete m_pUi;
}

//=============================================================================================================

void DummyYourWidget::saveSettings(const QString& settingsPath)
{
    if(settingsPath.isEmpty()) {
        return;
    }

    QSettings settings;

    settings.setValue(settingsPath + QString("/dummy"), m_pUi->m_pDoubleSpinBox_dummy->value());
}

//=============================================================================================================

void DummyYourWidget::loadSettings(const QString& settingsPath)
{
    if(settingsPath.isEmpty()) {
        return;
    }

    QSettings settings;

    m_pUi->m_pDoubleSpinBox_dummy->setValue(settings.value(settingsPath + QString("/dummy"), 10).toInt());
}
