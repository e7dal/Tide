/*********************************************************************/
/* Copyright (c) 2015, EPFL/Blue Brain Project                       */
/*                     Raphael Dumusc <raphael.dumusc@epfl.ch>       */
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

#include "MovieSynchronizer.h"

#include "ContentWindow.h"
#include "MovieContent.h"
#include "MovieUpdater.h"
#include "Tile.h"
#include "WallToWallChannel.h"

MovieSynchronizer::MovieSynchronizer( const QString& uri )
    : ContentSynchronizer()
    , _updater( new MovieUpdater( uri ))
    , _tileAdded( false )
    , _swapReady( false )
    , _visible( false )
{
}

void MovieSynchronizer::update( const ContentWindow& window,
                                const QRectF& visibleArea )
{
    // we only have one tile, which we add once, and we don't update it in
    // synchronize() if it's not visible.
    if( !_tileAdded )
    {
        emit addTile( std::make_shared<Tile>( 0, QRect( QPoint( 0, 0 ),
                                                        getTilesArea( ))));
        emit tilesAreaChanged();

        _tileAdded = true;
    }

    _visible = !visibleArea.isEmpty();
    _updater->update( static_cast< const MovieContent& >( *window.getContent()),
                      _visible );
}

void MovieSynchronizer::synchronize( WallToWallChannel& channel )
{
    if( channel.allReady( _swapReady ))
    {
        if( _tile )
            _tile->swapImage();
        _tile.reset();
        _swapReady = false;

        _updater->lastFrameDone();
        emit statisticsChanged();
    }

    if( _updater->advanceToNextFrame( channel ))
    {
        if( _updater->canRequestNewFrame( ))
            emit updateTile( 0, _updater->getTileRect( 0 ));
        else
            _swapReady = true;
    }
}

QSize MovieSynchronizer::getTilesArea() const
{
    return _updater->getTilesArea( 0 );
}

QString MovieSynchronizer::getStatistics() const
{
    return _updater->getStatistics();
}

ImagePtr MovieSynchronizer::getTileImage( const uint tileIndex ) const
{
    return _updater->getTileImage( tileIndex );
}

void MovieSynchronizer::onSwapReady( TilePtr tile )
{
    _tile = tile;
    _swapReady = true;
}