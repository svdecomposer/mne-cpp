//=============================================================================================================
/**
* @file     cshsensordatatreeitem.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     October, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief     CshSensorDataTreeItem class declaration.
*
*/

#ifndef DISP3DLIB_CSHSENSORDATATREEITEM_H
#define DISP3DLIB_CSHSENSORDATATREEITEM_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../../../../disp3D_global.h"
#include "../common/abstracttreeitem.h"
#include "../measurement/measurementtreeitem.h"
#include "../../workers/rtSensorData/rtcshsensordataworker.h"
#include <mne/mne_bem_surface.h>

//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE DISP3DLIB
//=============================================================================================================

namespace DISP3DLIB {


//*************************************************************************************************************
//=============================================================================================================
// DISP3DLIB FORWARD DECLARATIONS
//=============================================================================================================

class CshInterpolationItem;

//=============================================================================================================
/**
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/

class DISP3DSHARED_EXPORT CshSensorDataTreeItem : public AbstractTreeItem
{
    Q_OBJECT

public:
    typedef QSharedPointer<CshSensorDataTreeItem> SPtr;            /**< Shared pointer type for CshSensorDataTreeItem. */
    typedef QSharedPointer<const CshSensorDataTreeItem> ConstSPtr; /**< Const shared pointer type for CshSensorDataTreeItem. */

    //=========================================================================================================
    /**
    * Constructs a CshSensorDataTreeItem object.
    */
    explicit CshSensorDataTreeItem(int iType = Data3DTreeModelItemTypes::CshSensorDataItem, const QString& text = "Csh Sensor Data");

    //=========================================================================================================
    /**
    * Destructor, stops and deletes rtsensordata worker
    */
    ~CshSensorDataTreeItem();

    //=========================================================================================================
    /**
     * Initializes the sensor data item with neccessary information for visualization computations.
     * Constructs and initalizes the worker for this item.
     *
     * @param[in] tBemSurface               MNEBemSurface that holds the mesh that should be visualized
     * @param[in] tFiffInfo                 FiffInfo that holds the sensors information
     * @param[in] tSensorType               The sensor type that is later used for live interpolation
     * @param[in] tCancelDist               Distances higher than this are ignored for the interpolation
     * @param[in] tInterpolationFunction    Function that computes interpolation coefficients using the distance values
     * @param[in] t3DEntityParent           The Qt3D entity parent of the new item.
     */
    void init(const MNELIB::MNEBemSurface& tBemSurface,
              const FIFFLIB::FiffInfo &tFiffInfo,
              const QString& tSensorType,
              const double tCancelDist,
              const QString &tInterpolationFunction,
              Qt3DCore::QEntity *t3DEntityParent);

    //=========================================================================================================
    /**
    * Adds a block actual rt data which is streamed by this item's worker thread item. In order for this function to worker, you must call init(...) beforehand.
    *
    * @param[in] tSensorData                The matrix that holds rt measurement data.
    */
    void addData(const MatrixXd& tSensorData);

    //=========================================================================================================
    /**
    * Returns true if this item is initialized.
    *
    * @return                               Returns true if this item is initialized.
    */
    inline bool isDataInit() const;

    //=========================================================================================================
    /**
    * This function sets the loop flag.
    *
    * @param[in] bState                      Whether to loop the data or not.
    */
    void setLoopState(bool bState);

    //=========================================================================================================
    /**
    * This function sets the data streaming.
    *
    * @param[in] bState                      Whether to stream the data to the display or not.
    */
    void setStreamingActive(bool bState);

    //=========================================================================================================
    /**
    * This function sets the time interval for streaming.
    *
    * @param[in] iMSec                      The waiting time inbetween samples.
    */
    void setTimeInterval(int iMSec);

    //=========================================================================================================
    /**
    * This function sets the number of averages.
    *
    * @param[in] iNumberAverages            The new number of averages.
    */
    void setNumberAverages(int iNumberAverages);

    //=========================================================================================================
    /**
    * This function sets the colortable type.
    *
    * @param[in] sColortable                The new colortable ("Hot Negative 1" etc.).
    */
    void setColortable(const QString& sColortable);

    //=========================================================================================================
    /**
    * This function set the normalization value.
    *
    * @param[in] vecThresholds              The new threshold values used for normalizing the data.
    */
    void setNormalization(const QVector3D& vecThresholds);

    //=========================================================================================================
    /**
     * This function sets the cancel distance used in distance calculations for the interpolation.
     * Distances higher than this are ignored, i.e. the respective coefficients are set to zero.
     *
     * @param[in] dCancelDist               The new cancel distance value in meters.
     */
    void setCancelDistance(double dCancelDist);

    //=========================================================================================================
    /**
     * This function sets the function that is used in the interpolation process.
     *
     * @param sInterpolationFunction         Function that computes interpolation coefficients using the distance values.
     */
    void setInterpolationFunction(const QString &sInterpolationFunction);

    //=========================================================================================================
    /**
    * Set the sampling frequency.
    *
    * @param[in] dSFreq                 The new sampling frequency.
    */
    void setSFreq(const double dSFreq);

    //=========================================================================================================
    /**
    * Update bad channels and recalculate interpolation matrix.
    *
    * @param[in] info                 The fiff info including the new bad channels.
    */
    void updateBadChannels(const FIFFLIB::FiffInfo& info);

protected:

    //=========================================================================================================
    /**
    * This adds all meta tree items and connects them fittingly
    */
    void initItem();

    //=========================================================================================================
    /**
     * Calculates a weight matrix which is based on surfaced constrained distances.
     * Surface data member needs to be initialized first
     */
    QSharedPointer<SparseMatrix<double> > calculateWeigtMatrix();

    //=========================================================================================================
    /**
    * This function gets called whenever the check/activation state of the rt data worker changed.
    *
    * @param[in] checkState                 The check state of the worker.
    */
    void onCheckStateWorkerChanged(const Qt::CheckState& checkState);

    //=========================================================================================================
    /**
    * This function gets called whenever this item receives sensor values for each estimated source.
    *
    * @param[in] sensorData         The senor values for each estimated source.
    */
    void onNewRtData(const Eigen::VectorXf &sensorData);

    //=========================================================================================================
    /**
    * This function gets called whenever the used colormap type changed.
    *
    * @param[in] sColormapType              The name of the new colormap type.
    */
    void onColormapTypeChanged(const QVariant& sColormapType);

    //=========================================================================================================
    /**
    * This function gets called whenever the time interval in between the streamed samples changed.
    *
    * @param[in] iMSec                      The new time in milliseconds waited in between each streamed sample.
    */
    void onTimeIntervalChanged(const QVariant &iMSec);

    //=========================================================================================================
    /**
    * This function gets called whenever the normaization value changed. The normalization value is used to normalize the estimated source activation.
    *
    * @param[in] vecThresholds              The new threshold values used for normalizing the data.
    */
    void onDataNormalizationValueChanged(const QVariant &vecThresholds);

    //=========================================================================================================
    /**
    * This function gets called whenever the check/activation state of the looped streaming state changed.
    *
    * @param[in] checkState                 The check state of the looped streaming state.
    */
    void onCheckStateLoopedStateChanged(const Qt::CheckState& checkState);

    //=========================================================================================================
    /**
    * This function gets called whenever the number of averages of the streamed samples changed.
    *
    * @param[in] iNumAvr                    The new number of averages.
    */
    void onNumberAveragesChanged(const QVariant& iNumAvr);

    //=========================================================================================================
    /**
    * This function gets called whenever the cancel distance of the interpolation changed.
    *
    * @param[in] dCancelDist     The new cancel distance.
    */
    void onCancelDistanceChanged(const QVariant& dCancelDist);

    //=========================================================================================================
    /**
    * This function gets called whenever the function of the interpolation changed.
    *
    * @param[in] sInterpolationFunction     The new function name.
    */
    void onInterpolationFunctionChanged(const QVariant& sInterpolationFunction);



    bool                                    m_bIsDataInit;                    /**< The init flag. */

    QPointer<RtCshSensorDataWorker>         m_pSensorRtDataWorker;            /**< The source data worker. This worker streams the rt data to this item.*/
    QVector<int>                            m_iUsedSensors;                   /**< Stores the indices of channels inside the passed fiff evoked that are used for interpolation. */
    QVector<int>                            m_iSensorsBad;                    /**< Store bad channel indexes.*/

    QPointer<CshInterpolationItem>          m_pInterpolationItem;

    //Surface data
    int                                     m_iSensorType;                      /**< Type of the sensor: FIFFV_EEG_CH or FIFFV_MEG_CH. */
    double                                  m_dCancelDistance;                  /**< Cancel distance for the interpolaion in meters. */
    QSharedPointer<QVector<qint32>>         m_pVecMappedSubset;                 /**< Vector index position represents the id of the sensor and the qint in each cell is the vertex it is mapped to. */
    MNELIB::MNEBemSurface                   m_bemSurface;                       /**< Holds all vertex information that is needed (public member rr). */
    FIFFLIB::FiffInfo                       m_fiffInfo;                         /**< Contains all information about the sensors. */
    double (*m_interpolationFunction) (double);                                 /**< Function that computes interpolation coefficients using the distance values. */

    //@TODO interpolation function and canceldistance members?

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline bool CshSensorDataTreeItem::isDataInit() const
{
    return m_bIsDataInit;
}

} // namespace DISP3DLIB

#endif // DISP3DLIB_CSHSENSORDATATREEITEM_H
