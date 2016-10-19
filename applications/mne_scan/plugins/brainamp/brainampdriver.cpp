//=============================================================================================================
/**
* @file     brainampdriver.cpp
* @author   Lorenz Esch <lorenz.esch@tu-ilmenau.de>;
*           Viktor Klüber <Viktor.Klüber@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2016
*
* @section  LICENSE
*
* Copyright (C) 2106, Lorenz Esch, Viktor Klüber and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the implementation of the BrainAmpDriver class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "brainampdriver.h"
#include "brainampproducer.h"

#include <conio.h>
#include <stdio.h>
#include <io.h>
#include <vector>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace BRAINAMPPLUGIN;
using namespace std;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

BrainAmpDriver::BrainAmpDriver(BrainAmpProducer* pBrainAmpProducer)
: m_pBrainAmpProducer(pBrainAmpProducer)
, m_bInitDeviceSuccess(false)
, m_uiNumberOfChannels(32)
, m_uiSamplingFrequency(1000)
, m_uiSamplesPerBlock(200)
, m_bWriteDriverDebugToFile(false)
, m_sOutputFilePath("/mne_scan_plugins/resources/brainamp")
, m_bMeasureImpedances(false)
{
}


//*************************************************************************************************************

BrainAmpDriver::~BrainAmpDriver()
{
}


//*************************************************************************************************************

bool BrainAmpDriver::initDevice(int iNumberOfChannels,
                            int iSamplesPerBlock,
                            int iSamplingFrequency,
                            bool bWriteDriverDebugToFile,
                            QString sOutpuFilePath,
                            bool bMeasureImpedance)
{
    Q_UNUSED(iNumberOfChannels);
    Q_UNUSED(iSamplesPerBlock);
    Q_UNUSED(iSamplingFrequency);
    Q_UNUSED(bWriteDriverDebugToFile);
    Q_UNUSED(sOutpuFilePath);
    Q_UNUSED(bMeasureImpedance);

    printf("*** BrainAmp Control Example Program ***\n\n");

    // Open device
    if (!OpenDevice())
    {
        printf("No BrainAmp USB adapter and no ISA/PCI adapter found!\n");
        return -1;
    }

    // Show version
    unsigned nModule = DriverVersion % 10000,
             nMinor = (DriverVersion % 1000000) / 10000,
             nMajor = DriverVersion / 1000000;

    printf("%s Driver Found, Version %u.%02u.%04u\n",
            UsbDevice ? "USB" : "ISA/PCI", nMajor, nMinor, nModule);

    // Send simplest setup, 32 channels, one amp. AC, 1000Hz, 100nV
    Setup.nChannels = 32;
    Setup.nHoldValue = 0x0;		// Value without trigger
    Setup.nPoints = 40 * 5;		// 5 kHz = 5 points per ms -> 40 ms data block

    for (int i = 0; i < Setup.nChannels; i++) {
        Setup.nChannelList[i] = i;
    }

    DWORD dwBytesReturned = 0;
    if (!DeviceIoControl(DeviceAmp, IOCTL_BA_SETUP, &Setup,
        sizeof(Setup), NULL, 0, &dwBytesReturned, NULL))
    {
        printf("Setup failed, error code: %u\n", ::GetLastError());
    }

//    //Start acqusition process
//    // Pulldown input resistors for trigger input, (active high)
//    unsigned short pullup = 0;
//    DWORD dwBytesReturned = 0;
//    if (!DeviceIoControl(DeviceAmp, IOCTL_BA_DIGITALINPUT_PULL_UP, &pullup,
//        sizeof(pullup), NULL, 0, &dwBytesReturned, NULL))
//    {
//        printf("Can't set pull up/down resistors, error code: %u\n",
//            ::GetLastError());
//    }

//    // Make sure that amps exist, otherwise a long timeout will occur.
//    int nHighestChannel = 0;
//    for (int i = 0; i < Setup.nChannels; i++)
//    {
//        nHighestChannel = max(Setup.nChannelList[i], nHighestChannel);
//    }
//    int nRequiredAmps = (nHighestChannel + 1) / 32;
//    int nAmps = FindAmplifiers();
//    if (nAmps < nRequiredAmps)
//    {
//        printf("Required Amplifiers: %d, Connected Amplifiers: %d\n",
//            nRequiredAmps, nAmps);
//        return;
//    }

//    // Start acquisition
//    long acquisitionType = 1;
//    if (!DeviceIoControl(DeviceAmp, IOCTL_BA_START, &acquisitionType,
//        sizeof(acquisitionType), NULL, 0, &dwBytesReturned, NULL))
//    {
//        printf("Start failed, error code: %u\n", ::GetLastError());
//    }

    // Set flag for successfull initialisation true
    m_bInitDeviceSuccess = true;

    return true;
}


//*************************************************************************************************************

bool BrainAmpDriver::openDevice()
{
    DWORD dwFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH;
    // First try USB box
    DeviceAmp = CreateFile(DEVICE_USB, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                                OPEN_EXISTING, dwFlags, NULL);
    if (DeviceAmp != INVALID_HANDLE_VALUE)
    {
        UsbDevice = true;
    }
    else
    {
        // USB box not found, try PCI host adapter
        UsbDevice = false;;
        DeviceAmp = CreateFile(DEVICE_PCI, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                                OPEN_EXISTING, dwFlags, NULL);
    }

    // Retrieve driver version
    if (DeviceAmp != INVALID_HANDLE_VALUE)
    {
        DriverVersion = 0;
        DWORD dwBytesReturned;
        DeviceIoControl(DeviceAmp, IOCTL_BA_DRIVERVERSION, NULL, 0, &DriverVersion,
                        sizeof(DriverVersion), &dwBytesReturned, NULL);
    }

    return DeviceAmp != INVALID_HANDLE_VALUE;
}


//*************************************************************************************************************

bool BrainAmpDriver::uninitDevice()
{
    //Check if the device was initialised
    if(!m_bInitDeviceSuccess)
    {
        std::cout << "Plugin EEGoSports - ERROR - uninitDevice() - Device was not initialised - therefore can not be uninitialised" << std::endl;
        return false;
    }

    // Stop acquisition
    if (!DeviceIoControl(DeviceAmp, IOCTL_BA_STOP, NULL, 0, NULL, 0,
        &dwBytesReturned, NULL))
    {
        printf("Stop failed, error code: %u\n", ::GetLastError());
    }

    return true;
}


//*************************************************************************************************************

bool BrainAmpDriver::getSampleMatrixValue(Eigen::MatrixXd &sampleMatrix)
{
//    //Check if device was initialised and connected correctly
//    if(!m_bInitDeviceSuccess)
//    {
//        std::cout << "Plugin EEGoSports - ERROR - getSampleMatrixValue() - Cannot start to get samples from device because device was not initialised correctly" << std::endl;
//        return false;
//    }

//    sampleMatrix = MatrixXd(Setup.nChannels + 1, Setup.nPoints);
//    sampleMatrix.setZero(); // Clear matrix - set all elements to zero

//    // Get the data
//    // Data including marker channel
//    vector<short>pnData((Setup.nChannels + 1) * Setup.nPoints);

//    // Pure data
//    vector<short>pnPureData(Setup.nChannels * Setup.nPoints);

//    unsigned short nLastMarkerValue = 0;
//    unsigned nDataOffset = 0;
//    unsigned int nMarkerNumber = 0;

//    printf("Press any key to stop the recording...\n");

//    // Check for error
//    int nTemp = 0;
//    DWORD dwBytesReturned;
//    if (!DeviceIoControl(DeviceAmp, IOCTL_BA_ERROR_STATE, NULL, 0,
//            &nTemp, sizeof(nTemp), &dwBytesReturned, NULL))
//    {
//        printf("Acquisition Error, GetLastError(): %d\n", ::GetLastError());
//        return false;
//    }
//    if (nTemp != 0)
//    {
//        printf("Acquisition Error %d\n", nTemp);
//        return false;
//    }

//    // Receive data
//    int nTransferSize = (int)pnData.size() * sizeof(short);
//    if (!ReadFile(DeviceAmp, &pnData[0], nTransferSize, &dwBytesReturned, NULL))
//    {
//        printf("Acquisition Error, GetLastError(): %d\n", ::GetLastError());
//        return false;
//    }

//    //Transform into matrix structure
//    for (int i = 0; i < Setup.nPoints; ++i) {
//        for (int n = 0; n < Setup.nChannels; ++n) {
//            sampleMatrix(n,i) = *(pSrc++);
//        }
//    }

//    Sleep(100);

    return true;
}
