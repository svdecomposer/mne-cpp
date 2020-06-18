//=============================================================================================================
/**
 * @file     mainwindow.h
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
 * @brief     Mainwindow class declaration.
 *
 */

#ifndef MNEANONYMIZE_MAINWINDOW_H
#define MNEANONYMIZE_MAINWINDOW_H

//=============================================================================================================
// INCLUDES
//=============================================================================================================

//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QMainWindow>
#include <QDateTime>
#include <QFileInfo>

//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Ui {
class MainWindow;
}

//=============================================================================================================
// DEFINE NAMESPACE MNEANONYMIZE
//=============================================================================================================

namespace MNEANONYMIZE {

//=============================================================================================================
// MNEANONYMIZE FORWARD DECLARATIONS
//=============================================================================================================

class SettingsControllerGui;

//=============================================================================================================
/**
 * MainWindow class stores the view for the GUI of mne_anonymize application. Creates and sets up all elements of
 * the GUI. It manages the events generated in the UI class and connects to the controller. It also manages the
 * conneections comming from the controller and accordingly modifies the user interface.
 *
 * @brief GUI-related event manager class.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //=========================================================================================================
    /**
    * Constructs a MainWindow object.
    */
    explicit MainWindow(SettingsControllerGui* controller = nullptr);

    /**
    * Destructs MainWindow object.
    */
    ~MainWindow() override;

    //=========================================================================================================
    /**
     * @brief Sets the input file member variable and updates the information in the user interaface related to
     * the path and name of the input path.
     *
     * @details Allows the controller to set the input file name and path.QFileInfo m_fiInFile member variable will be udpated
     * during this member function.
     *
     * @param [in] s QString containing the complete absolute path of the input file.
     *
     */
    void setLineEditInFile(const QString& s);

    //=========================================================================================================
    /**
     * @brief Sets the output file member variable and updates the information in the user interaface related to
     * the path and name of the output path.
     *
     * @details Allows the controller to set the output file name and path. QFileInfo m_fiOutFile member variable will be udpated
     * during this member function.
     *
     * @param [in] s QString containing the complete absolute path of the output file.
     *
     */
    void setLineEditOutFile(const QString &s);

    //=========================================================================================================
    /**
     * @brief Sets the state of the checkbox for selecting Brute Mode on.
     *
     * @details Allows the controller to set state of the Brute Mode, according to the command-line arguments of the
     * call to MNE_Anonymize application.
     *
     * @param [in] b bool variable containing the desired state to set the checkbox to.
     *
     */
    void setCheckBoxBruteMode(bool b);

    //=========================================================================================================
    /**
     * @brief Sets the state of the Measuremenet date control in the user interface..
     *
     * @details Allows the controller to set state of the measurement date control, according to the command-line arguments of the
     * call to MNE_Anonymize application.
     *
     * @param [in] QDateTime dt reference variable containing the desired measurement date to be used when anonymizing the input fif file..
     *
     */
    void setMeasurementDate(const QDateTime& dt);

    //=========================================================================================================
    /**
     * @brief Sets the state of the checkbox for selecting measurement date offset on.
     *
     * @details Allows the controller to set state of the use of the measurement date offset related checkbox, according to the command-line arguments of the
     * call to MNE_Anonymize application.
     *
     * @param [in] o bool variable representing the use or not of measurement offset when updating the mesasurement date of a file.
     *
     */
    void setCheckBoxMeasurementDateOffset(bool o);

    //=========================================================================================================
    /**
     * @brief Sets the number of days to offset the measurement date with.
     *
     * @details Measurement date offset. Minimal, Maximum values [-10000, 10000]. This method will update the user interface, according according
     * to the command-line arguments of the call to MNE_Anonymize application.
     *
     * @param [in] d Int. Number of days to offset the measurement date with.
     *
     */
    void setMeasurementDateOffset(int d);

    //=========================================================================================================
    /**
     * @brief Sets the date of the subject's birthday.
     *
     * @details Allows to modify the date of the subject's birthday with a specific date.
     *
     * @param [in] d Birthday of the subject.
     *
     */
    void setSubjectBirthday(const QDateTime& d);

    //=========================================================================================================
    /**
     * @brief Modify the birthday of the subject by a number of days.
     *
     * @details Instead of using a specific date to modify the subject's birthday. offset by #days the original
     * Birthday.
     *
     * @param [in] b Offset by number of days.
     *
     */
    void setCheckBoxSubjectBirthdayOffset(bool b);

    //=========================================================================================================
    /**
     * @brief Modify the birthday of the subject by a number of days.
     *
     * @details Birtday offset by #days.
     *
     * @param [in] d Offset days the birthday of the subject.
     *
     */
    void setSubjectBirthdayOffset(int d);

    //=========================================================================================================
    /**
     * @brief Specify text Hospital ID of the subject to be used if that tag appears in the file.
     *
     * @details If the HisId tag appears in the file, its content, a string will be substituted with the text
     * specified with this methis.
     *
     * @param [in] d Offset days the birthday of the subject.
     *
     */
    void setSubjectHis(const QString& h);

    //=========================================================================================================
    /**
     * @brief Set state of all user interface elements.
     *
     * @details Set UI elements to be cleared and disabled.
     *
     */
    void setDefaultStateExtraInfo();

    //=========================================================================================================
    /**
     * @brief Show message in the status bar.
     *
     * @param [in] s String to show.
     * @param [in] to time to show the message for.
     *
     */
    void statusMsg(const QString s, int to = 0);

signals:
    //=========================================================================================================
    /**
     * @brief Signal triggered when the text for the input file is changed.
     *
     * @param [in] s Absolute path of the input file.
     *
     */
    void fileInChanged(const QString& s) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the text for the output file is changed.
     *
     * @param [in] s Absolute path of the output file.
     *
     */
    void fileOutChanged(const QString& s) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the checkbox specifying Brute mode on.
     *
     * @param [in] s State of the checkbox Brute Mode.
     *
     */
    void bruteModeChanged(bool b) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the value of the measurement date has changed.
     *
     * @param [in] t Date specified.
     *
     */
    void measurementDateChanged(const QDateTime& t) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the checkbox for using measurement days of offset instead of a new measurement
     * date.
     *
     * @param [in] f State of the checkbox.
     *
     */
    void useMeasurementOffset(bool f) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the number of days of offset for the measurement date is changed.
     *
     * @param [in] o Number of days to offset the measurement.
     *
     */
    void measurementDateOffsetChanged(int o) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the number of days of offset for the birthday date is changed.
     *
     * @param [in] o Number of days to offset the measurement.
     *
     */
    void birthdayDateChanged(const QDateTime& d) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the checkbox specifying to use number days to offset the birthday.
     *
     * @param [in] o Number of days to offset the measurement.
     *
     */
    void useBirthdayOffset(bool f) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the number of days of offset for the birthday of the subject date is changed.
     *
     * @param [in] o Number of days to offset the measurement.
     *
     */
    void birthdayOffsetChanged(int o) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the text to substitute the HisId tag info with, has changed.
     *
     * @param [in] text New text to substitute HisId with.
     *
     */
    void subjectHisIdChanged(const QString& text) const;

    //=========================================================================================================
    /**
     * @brief Signal triggered when the checkbox to show options and input file info changed.
     *
     * @param [in] b state of the checkbox.
     *
     */
    void showOptionsChanged(bool b);

    //=========================================================================================================
    /**
     * @brief Signal triggered when the read input data button has been pressed.
     *
     */
    void readInputDataButtonClicked();

public slots:
    //=========================================================================================================
    /**
     * @brief Generate a window popup in order to show a message and make the user confirm acknowledging it by pressing
     * an OK button.
     *
     * @param [in] s Text to show in the popup window.
     *
     */
    void winPopup(QString s);

    //=========================================================================================================
    /**
     * @brief Specify the file version number found in the input file after reading it.
     *
     * @details The fileID tag has stored the file version.
     *
     * @param [in] v Version number.
     *
     */
    void setLineEditIdFileVersion(double v);

    //=========================================================================================================
    /**
     * @brief Specify the measurement date to show in the file infomration menu.
     *
     * @details Measurement date read from the Id Tag. It is shown in a QLineEdit element.
     *
     * @param [in] d Measurement date.
     *
     */
    void setLineEditIdMeasurementDate(QDateTime d);

    //=========================================================================================================
    /**
     * @brief Specify the MAC address text in the input file information menu.
     *
     * @param [in] mac MAC address as text.
     *
     */
    void setLineEditIdMacAddress(QString mac);

    //=========================================================================================================
    /**
     * @brief Specify the Measurement date of the file to be shown in the information menu.
     *
     * @details Measurmenet date specified int he measurement date tag, not in the id tag which also contains
     * measurement information.
     *
     * @param [in] d Date and time to show.
     *
     */
    void setLineEditFileMeasurementDate(QDateTime d);

    //=========================================================================================================
    /**
     * @brief Specify the text to show as File comment in the input file information menu.
     *
     * @param [in] s File comment.
     *
     */
    void setLineEditFileComment(QString c);

    //=========================================================================================================
    /**
     * @brief Specify the text to show as File experimenter in the input file information menu.
     *
     * @param [in] e Experimenter text.
     *
     */
    void setLineEditFileExperimenter(QString e);

    //=========================================================================================================
    /**
     * @brief Specify the integer to show as subject id in the input file information menu.
     *
     * @param [in] i Subject id.
     *
     */
    void setLineEditSubjectId(int i);

    //=========================================================================================================
    /**
     * @brief Specify the string to show as subject's first name in the input file information menu.
     *
     * @param [in] fn First name of the subject.
     *
     */
    void setLineEditSubjectFirstName(QString fn);

    //=========================================================================================================
    /**
     * @brief Specify the string to show as subject's middle name in the input file information menu.
     *
     * @param [in] mn Subject's middle name.
     *
     */
    void setLineEditSubjectMiddleName(QString mn);

    //=========================================================================================================
    /**
     * @brief Specify the string to show as subject's last name in the input file information menu.
     *
     * @param [in] ln Subject's last name.
     *
     */
    void setLineEditSubjectLastName(QString ln);

    //=========================================================================================================
    /**
     * @brief Specify the subject's birthday to show in the input file information menu.
     *
     * @param [in] b Date and time for the birthday of the subject.
     *
     */
    void setLineEditSubjectBirthday(QDateTime b);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setComboBoxSubjectSex(int s);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditSubjectHand(int h);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditSubjectWeight(float w);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditSubjectHeight(float h);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditSubjectComment(QString c);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditSubjectHisId(QString his);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditProjectId(int);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditProjectName(QString);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditProjectAim(QString);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditProjectPersons(QString);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditProjectComment(QString);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLabelMriDataFoundVisible(bool);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditMNEWorkingDir(QString s);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setLineEditMNECommand(QString s);

    //=========================================================================================================

protected:
    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void closeEvent(QCloseEvent *event) override;

    //=========================================================================================================
private slots:

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void checkBoxShowOptionsChanged();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void checkBoxBruteModeChanged();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void checkBoxMeasurementDateOffsetStateChanged(int arg);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void checkBoxBirthdayDateOffsetStateChanged(int arg);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void inFileEditingFinished();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void outFileEditingFinished();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void dateTimeMeasurementDateDateTimeChanged(const QDateTime &dateTime);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void spinBoxMeasurementDateOffsetValueChanged(int arg1);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void dateTimeBirthdayDateDateTimeChanged(const QDateTime &dateTime);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void spinBoxBirthdayDateOffsetValueChanged(int arg1);

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void lineEditSubjectHisIdEditingFinished();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void openInFileDialog();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void openOutFileDialog();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void helpButtonClicked();

    //=========================================================================================================

private:

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    bool confirmClose();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setDefautlStateUi();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void setupConnections();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void idMeasurementDateChanged();

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void resizeEvent(QResizeEvent* event) override;

    //=========================================================================================================
    /**
     * @brief Specify the
     *
     * @details
     *
     * @param [in]
     *
     */
    void checkSmallGui();

    //=========================================================================================================

    bool m_bOptionsVisibility;                  /**< Options and input file info is shown.*/
    const int m_iDefaultHeight;
    const int m_iDefaultHeightLarge;

    QFileInfo m_fiInFile;                       /**< Input file QFileInfo object.*/
    QFileInfo m_fiOutFile;                      /**< Output file QFileInfo object.*/

    Ui::MainWindow* m_pUi;                      /**< Pointer to the user interface form.*/
    SettingsControllerGui* m_pController;       /**< Pointer to the controller object.*/
};

}
#endif // MNEANONYMIZE_MAINWINDOW_H
