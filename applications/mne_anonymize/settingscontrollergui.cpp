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

#include <QObject>
#include <QThread>
#include <QDir>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QDateTime>
#include <QtGlobal>

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

    qApp->setAttribute(Qt::AA_DontUseNativeMenuBar);

    m_pWin->show();



    if(m_pAnonymizer->isFileInSet())
    {
        readData();
    }

    QObject().thread()->usleep(1000*2000);
    m_pWin->statusMsg("Howdy!",1000);
}

void SettingsControllerGui::executeAnonymizer()
{
    m_pWin->statusMsg("Anonymizing the input file into the output file.");
    m_pAnonymizer->anonymizeFile();
    m_pWin->statusMsg("Your file is ready!");
}

void SettingsControllerGui::readData()
{
    m_pWin->statusMsg("Reading input File information...",0);
    //set output to a randomFilename
    QString stringTempDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    QString fileOutStr(QDir(stringTempDir).filePath(generateRandomFileName()));
    m_pAnonymizer->setFileOut(fileOutStr);
    m_pWin->setDefaultStateExtraInfo();
    bool verboseMode(m_pAnonymizer->getVerboseMode());
    m_pAnonymizer->setVerboseMode(false);
    m_pAnonymizer->anonymizeFile();
    QFile fileOut(fileOutStr);
    fileOut.remove();
    m_pAnonymizer->setFileOut(m_fiOutFileInfo.absoluteFilePath());
    m_pAnonymizer->setVerboseMode(verboseMode);
    m_pWin->statusMsg("Information read correctly.");
}

void SettingsControllerGui::fileInChanged(const QString& strInFile)
{
    QFileInfo newfiInFile(strInFile);

    if(newfiInFile.isDir())
    {
        m_pWin->statusMsg("Invalid input file. That's a directory");
        m_pWin->setLineEditInFile(m_fiInFileInfo.absoluteFilePath());
        return;
    }
    if(QString::compare(newfiInFile.suffix(),QString("fif")) != 0)
    {
        m_pWin->statusMsg("The input file extension must be \".fif\" 0.");
        m_pWin->setLineEditOutFile(m_fiOutFileInfo.absoluteFilePath());
        return;
    }
    QFileInfo inDir(newfiInFile.absolutePath());
    if(!inDir.isReadable())
    {
        m_pWin->statusMsg("You might not have reading permissions to this folder");
        m_pWin->setLineEditInFile(m_fiInFileInfo.absoluteFilePath());
        return;
    }

    if( m_fiInFileInfo != newfiInFile )
    {
        m_fiInFileInfo.setFile(newfiInFile.absoluteFilePath());
        m_pAnonymizer->setFileIn(m_fiInFileInfo.absoluteFilePath());

        if(m_fiInFileInfo.isFile() && m_fiInFileInfo.isReadable())
        {
            readData();
        }
    generateDefaultOutputFileName();
    m_pWin->setLineEditInFile(m_fiInFileInfo.absoluteFilePath());
    m_pWin->setLineEditOutFile(m_fiOutFileInfo.absoluteFilePath());
    }
}

void SettingsControllerGui::fileOutChanged(const QString& strOutFile)
{
    QFileInfo newfiOutFile(strOutFile);
    if(m_fiOutFileInfo.isDir())
    {
        QString fileOutDefaultName(newfiOutFile.absolutePath() + m_fiInFileInfo.baseName() +
                        "_anonymized." + m_fiInFileInfo.completeSuffix());
        m_fiOutFileInfo.setFile(fileOutDefaultName);
        m_pWin->setLineEditOutFile(m_fiOutFileInfo.absoluteFilePath());
        return;
    }

    if(QString::compare(newfiOutFile.suffix(),QString("fif")) != 0)
    {
        m_pWin->statusMsg("The output file extension must be \".fif\" 0.");
        m_pWin->setLineEditOutFile(m_fiOutFileInfo.absoluteFilePath());
        return;
    }

    QFileInfo outDir(newfiOutFile.absolutePath());
    if(!outDir.isWritable())
    {
        m_pWin->winPopup("You might not have writing permissions to this folder");
        m_pWin->setLineEditOutFile(m_fiOutFileInfo.absoluteFilePath());
        return;
    }

    if(m_fiOutFileInfo != newfiOutFile)
    {
        m_fiOutFileInfo.setFile(strOutFile);
        m_pAnonymizer->setFileOut(m_fiOutFileInfo.absoluteFilePath());
    }
}

void SettingsControllerGui::setupCommunication()
{
    //from view to controller (to model)
    QObject::connect(m_pWin.data(),&MainWindow::fileInChanged,
                     this,&SettingsControllerGui::fileInChanged);
    QObject::connect(m_pWin.data(),&MainWindow::fileOutChanged,
                     this,&SettingsControllerGui::fileOutChanged);

    //from view to model
    QObject::connect(m_pWin.data(),&MainWindow::bruteModeChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setBruteMode);
    QObject::connect(m_pWin.data(),&MainWindow::measurementDateChanged,
                     m_pAnonymizer.data(),QOverload<const QDateTime&>::of(&FiffAnonymizer::setMeasurementDate));
    QObject::connect(m_pWin.data(),&MainWindow::useMeasurementOffset,
                     m_pAnonymizer.data(),&FiffAnonymizer::setUseMeasurementDateOffset);
    QObject::connect(m_pWin.data(),&MainWindow::measurementDateOffsetChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setMeasurementDateOffset);
    QObject::connect(m_pWin.data(),&MainWindow::birthdayDateChanged,
                     m_pAnonymizer.data(),QOverload<const QDateTime&>::of(&FiffAnonymizer::setSubjectBirthday));
    QObject::connect(m_pWin.data(),&MainWindow::useBirthdayOffset,
                     m_pAnonymizer.data(),&FiffAnonymizer::setUseSubjectBirthdayOffset);
    QObject::connect(m_pWin.data(),&MainWindow::birthdayOffsetChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setSubjectBirthdayOffset);
    QObject::connect(m_pWin.data(),&MainWindow::subjectHisIdChanged,
                     m_pAnonymizer.data(),&FiffAnonymizer::setSubjectHisId);


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
                     m_pWin.data(),&MainWindow::setComboBoxSubjectSex);
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
    if(m_pAnonymizer->isFileInSet())
    {
        m_pWin->setLineEditInFile(m_fiInFileInfo.absoluteFilePath());
    }
    if(m_pAnonymizer->isFileOutSet())
    {
        m_pWin->setLineEditOutFile(m_fiOutFileInfo.absoluteFilePath());
    }

    m_pWin->setCheckBoxBruteMode(m_pAnonymizer->getBruteMode());
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
