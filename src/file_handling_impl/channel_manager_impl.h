// Copyright (c) 2016 The SigViewer Development Team
// Licensed under the GNU General Public License (GPL)
// https://www.gnu.org/licenses/gpl


#ifndef CHANNEL_MANAGER_IMPL_H
#define CHANNEL_MANAGER_IMPL_H

#include "file_handling/channel_manager.h"
#include "file_handling/file_signal_reader.h"

namespace sigviewer
{

//-----------------------------------------------------------------------------
/// ChannelManagerImpl
///
/// implementation of ChannelManager
///
class ChannelManagerImpl : public ChannelManager
{
public:
    //---------------------------------------------------------------------------------------------
    /// @param file_signal_reader the ChannelManagerImpl takes ownership of the given FileSignalReader
    ///                           and destroys it if the ChannelManagerImpl is destructed
    ChannelManagerImpl (FileSignalReader* file_signal_reader);

    //---------------------------------------------------------------------------------------------
    virtual ~ChannelManagerImpl ();

    //-------------------------------------------------------------------------
    virtual std::set<ChannelID> getChannels () const;

    //-------------------------------------------------------------------------
    virtual uint32 getNumberChannels () const;

    //-------------------------------------------------------------------------
    virtual QString getChannelLabel (ChannelID id) const;

    //-------------------------------------------------------------------------
    virtual QString getChannelLabel (ChannelID id, int streamNumber) const;

    //-------------------------------------------------------------------------
    virtual QString getChannelYUnitString (ChannelID id) const;

    //-------------------------------------------------------------------------
    virtual QSharedPointer<DataBlock const> getDataOld_ (ChannelID id,
                                                        unsigned start_pos,
                                                        unsigned length) const;


    //-------------------------------------------------------------------------
    virtual QSharedPointer<DataBlock const> getDataNew (ChannelID id,
                                                        unsigned start_pos,
                                                        unsigned length) const;

    //-------------------------------------------------------------------------
    virtual float64 getDurationInSec() const;

    //-------------------------------------------------------------------------
    virtual size_t getNumberSamplesOld_() const;

    //-------------------------------------------------------------------------
    virtual size_t getChannelNumberSamplesNew (ChannelID id) const;

    //-------------------------------------------------------------------------
    virtual float64 getSampleRateOld_() const;

    //-------------------------------------------------------------------------
    virtual float64 getChannelSampleRateNew (ChannelID id) const;


private:
    FileSignalReader* reader_;
};

}

#endif
