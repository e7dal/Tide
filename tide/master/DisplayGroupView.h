/*********************************************************************/
/* Copyright (c) 2011 - 2012, The University of Texas at Austin.     */
/* Copyright (c) 2013-2016, EPFL/Blue Brain Project                  */
/*                     Raphael.Dumusc@epfl.ch                        */
/*                     Daniel.Nachbaur@epfl.ch                       */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of Ecole polytechnique federale de Lausanne.          */
/*********************************************************************/

#ifndef DISPLAY_GROUP_VIEW_H
#define DISPLAY_GROUP_VIEW_H

#include "types.h"

#include "QmlControlPanel.h"

#include <QUuid>

#include <QGesture>
#include <QGestureEvent>

#include <QQuickView>

/**
 * An interactive graphical view of a DisplayGroup's ContentWindows.
 */
class DisplayGroupView : public QQuickView
{
    Q_OBJECT

public:
    /** Constructor. */
    DisplayGroupView( OptionsPtr options, const MasterConfiguration& config );

    /** Destructor */
    virtual ~DisplayGroupView();

    /** Set the DisplayGroup model that this view should present. */
    void setDataModel( DisplayGroupPtr displayGroup );

    /** Get the control panel in this view. */
    QmlControlPanel& getControlPanel();

    /** Map a normalized touch event on the wall to this view's coordinates. */
    QPointF mapToWallPos( const QPointF& normalizedPos ) const;

signals:
    /** Emitted when a user taps the background. */
    void backgroundTap( QPointF pos );

    /** Emitted when a user taps and holds the background. */
    void backgroundTapAndHold( QPointF pos );

protected:
    /** Re-implement QWindow event to capture tab key. */
    bool event( QEvent* event ) override;

private slots:
    void add( ContentWindowPtr contentWindow );
    void remove( ContentWindowPtr contentWindow );
    void moveToFront( ContentWindowPtr contentWindow );

private:
    void clearScene();
    QPointF getScenePos( const QPointF& pos ) const;

    DisplayGroupPtr displayGroup_;

    QQuickItem* displayGroupItem_;
    QObject* wallObject_;
    QmlControlPanel controlPanel_;

    typedef QMap<QUuid, QQuickItem*> UuidToWindowMap;
    UuidToWindowMap uuidToWindowMap_;
};

#endif