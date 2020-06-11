//=============================================================================================================
/**
 * @file     settingscontrollergui.cpp
 * @author   Juan GPC <juangpc@gmail.com>
 * @since    0.1.0
 * @date     May, 2020
 *
 * @section  LICENSE
 *
 * Copyright (C) 2020, Juan GPC. All rights reserved.
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
 * @brief    SettingsControllerGUI class definition.
 *
 */

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "settingscontrollergui.h"
#include "mainwindow.h"

//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDir>
#include <QStandardPaths>

//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MNEANONYMIZE;

//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================

//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

SettingsControllerGui::SettingsControllerGui(const QStringList& arguments)
: m_pWin(QSharedPointer<MainWindow> (new MainWindow(this)))
{
    initParser();
    parseInputs(arguments);
    setupCommunication();

    initializeOptionsState();

    m_pWin->show();

    if(m_pAnonymizer->isFileInSet())
    {
        readData();
    }
}

void SettingsControllerGui::executeAnonymizer()
{
    m_pAnonymizer->anonymizeFile();
}

void SettingsControllerGui::readData()
{
    //set output to a randomFilename
    QString stringTempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    QString fileOutStr(QDir(stringTempDir).filePath(generateRandomFileName()));
    m_pAnonymizer->setFileOut(fileOutStr);
    m_pAnonymizer->anonymizeFile();
    QFile fileOut(fileOutStr);
    fileOut.remove();
    m_pAnonymizer->setFileOut(m_fiOutFileInfo.absoluteFilePath());
}

void SettingsControllerGui::fileInChanged(const QString& strInFile)
{
    m_fiInFileInfo.setFile(strInFile);
    m_pAnonymizer->setFileIn(m_fiInFileInfo.absoluteFilePath());

    if(m_fiInFileInfo.isFile())
    {
        readData();
    }

}

void SettingsControllerGui::fileOutChanged(const QString& strOutFile)
{
    m_fiOutFileInfo.setFile(strOutFile);
    m_pAnonymizer->setFileOut(m_fiOutFileInfo.absoluteFilePath());
}

void SettingsControllerGui::setupCommunication()
{

    //from view to model
    QObject::connect(m_pWin.data(),&MainWindow::fileInChanged,
                     this,&SettingsControllerGui::fileInChanged);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     this,&SettingsControllerGui::fileOutChanged);

    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setFileOut);

    //from model to view
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingIdFileVersion,
                     m_pWin.data(),&MainWindow::setLineEditIdFileVersion);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingIdMeasurementDate,
                     m_pWin.data(),&MainWindow::setLineEditIdMeasurementDate);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingIdMac,
                     m_pWin.data(),&MainWindow::setLineEditIdMacAddress);

    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingFileMeasurementDate,
                     m_pWin.data(),&MainWindow::setLineEditFileMeasurementDate);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingFileComment,
                     m_pWin.data(),&MainWindow::setLineEditFileComment);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingFileExperimenter,
                     m_pWin.data(),&MainWindow::setLineEditFileExperimenter);

    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectId,
                     m_pWin.data(),&MainWindow::setLineEditSubjectId);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectFirstName,
                     m_pWin.data(),&MainWindow::setLineEditSubjectFirstName);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectMiddleName,
                     m_pWin.data(),&MainWindow::setLineEditSubjectMiddleName);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectLastName,
                     m_pWin.data(),&MainWindow::setLineEditSubjectLastName);

    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectBirthday,
                     m_pWin.data(),&MainWindow::setLineEditSubjectBirthday);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectSex,
                     m_pWin.data(),&MainWindow::setLineEditSubjectSex);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectHand,
                     m_pWin.data(),&MainWindow::setLineEditSubjectHand);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectWeight,
                     m_pWin.data(),&MainWindow::setLineEditSubjectHand);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectHeight,
                     m_pWin.data(),&MainWindow::setLineEditSubjectHeight);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectComment,
                     m_pWin.data(),&MainWindow::setLineEditSubjectComment);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingSubjectHisId,
                     m_pWin.data(),&MainWindow::setLineEditSubjectHisId);


    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingProjectId,
                     m_pWin.data(),&MainWindow::setLineEditProjectId);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingProjectName,
                     m_pWin.data(),&MainWindow::setLineEditProjectName);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingProjectAim,
                     m_pWin.data(),&MainWindow::setLineEditProjectAim);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingProjectPersons,
                     m_pWin.data(),&MainWindow::setLineEditProjectPersons);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::readingProjectComment,
                     m_pWin.data(),&MainWindow::setLineEditProjectComment);
    QObject::connect(m_pAnonymizer.data(),&FiffAnonymizer::mriDataFoundInFile,
                     m_pWin.data(),&MainWindow::setLabelMriDataFoundVisible);
}

void SettingsControllerGui::initializeOptionsState()
{
    m_pWin->setLineEditInFile(m_fiInFileInfo.absoluteFilePath());
    m_pWin->setLineEditOutFile(m_fiOutFileInfo.absoluteFilePath());

    m_pWin->setCheckBoxBruteMode(m_pAnonymizer->getBruteMode());
//    m_pWin->setCheckBoxDeleteInputFileAfter(m_bDeleteInputFileAfter);
//    m_pWin->setCheckBoxAvoidDeleteConfirmation(m_bDeleteInputFileConfirmation);
    m_pWin->setMeasurementDate(m_pAnonymizer->getMeasurementDate());
    m_pWin->setCheckBoxMeasurementDateOffset(m_pAnonymizer->getUseMeasurementDayOffset());
    m_pWin->setMeasurementDateOffset(m_pAnonymizer->getMeasurementDayOffset());
    m_pWin->setCheckBoxSubjectBirthdayOffset(m_pAnonymizer->getUseSubjectBirthdayOffset());
    m_pWin->setSubjectBirthdayOffset(m_pAnonymizer->getSubjectBirthdayOffset());
    if(m_bHisIdSpecified)
    {
        m_pWin->setSubjectHis(m_pAnonymizer->getSubjectHisID());
    }
}
