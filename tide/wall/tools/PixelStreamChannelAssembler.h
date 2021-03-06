/*********************************************************************/
/* Copyright (c) 2017-2018, EPFL/Blue Brain Project                  */
/*                          Raphael Dumusc <raphael.dumusc@epfl.ch>  */
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
/*    THIS  SOFTWARE  IS  PROVIDED  BY  THE  ECOLE  POLYTECHNIQUE    */
/*    FEDERALE DE LAUSANNE  ''AS IS''  AND ANY EXPRESS OR IMPLIED    */
/*    WARRANTIES, INCLUDING, BUT  NOT  LIMITED  TO,  THE  IMPLIED    */
/*    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR  A PARTICULAR    */
/*    PURPOSE  ARE  DISCLAIMED.  IN  NO  EVENT  SHALL  THE  ECOLE    */
/*    POLYTECHNIQUE  FEDERALE  DE  LAUSANNE  OR  CONTRIBUTORS  BE    */
/*    LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,    */
/*    EXEMPLARY,  OR  CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT NOT    */
/*    LIMITED TO,  PROCUREMENT  OF  SUBSTITUTE GOODS OR SERVICES;    */
/*    LOSS OF USE, DATA, OR  PROFITS;  OR  BUSINESS INTERRUPTION)    */
/*    HOWEVER CAUSED AND  ON ANY THEORY OF LIABILITY,  WHETHER IN    */
/*    CONTRACT, STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE    */
/*    OR OTHERWISE) ARISING  IN ANY WAY  OUT OF  THE USE OF  THIS    */
/*    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.   */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of Ecole polytechnique federale de Lausanne.          */
/*********************************************************************/

#ifndef PIXELSTREAMCHANNELASSEMBLER_H
#define PIXELSTREAMCHANNELASSEMBLER_H

#include "types.h"

#include "PixelStreamProcessor.h"

#include <deflect/server/Frame.h>

/**
 * Assemble small frame tiles (e.g. 64x64) into 512x512 ones.
 *
 * This class operates on a single channel of a frame.
 *
 * The tile indices returned by computeVisibleSet() and taken by getTileRect() /
 * getTileImage() are specific to the current channel, i.e. they are in the
 * range [0; getTilesCount()-1].
 */
class PixelStreamChannelAssembler : public PixelStreamProcessor
{
public:
    /**
     * Create an assembler for one channel of the frame.
     * @param frame to assemble with tiles sorted by channel and in left-right +
     *        top-bottom order.
     * @param channel target channel to assemble.
     * @throw std::runtime_error if the frame's channel cannot be assembled.
     */
    PixelStreamChannelAssembler(deflect::server::FramePtr frame, uint channel);

    /** @copydoc PixelStreamProcessor::getTileImage */
    ImagePtr getTileImage(uint tileIndex,
                          deflect::server::TileDecoder& decoder) final;

    /** @copydoc PixelStreamProcessor::getTileRect */
    QRect getTileRect(uint tileIndex) const final;

    /** @copydoc PixelStreamProcessor::computeVisibleSet */
    Indices computeVisibleSet(const QRectF& visibleArea,
                              uint channel) const final;

    /** @copydoc PixelStreamProcessor::getTilesCount */
    size_t getTilesCount() const final;

private:
    deflect::server::FramePtr _frame;
    QSize _frameSize;
    uint _channel;
    size_t _begin, _end;
    deflect::server::FramePtr _assembledFrame;

    bool _canAssemble() const;

    uint _getTilesX() const;
    uint _getTilesY() const;

    void _initTargetFrame();

    Indices _findSourceTiles(uint tileIndex) const;
    void _decodeSourceTiles(const Indices& indices,
                            deflect::server::TileDecoder& decoder);
    void _assembleTargetTile(uint tileIndex, const Indices& indices);
};

#endif
