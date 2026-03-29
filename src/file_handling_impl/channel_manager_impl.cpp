// Copyright (c) 2016 The SigViewer Development Team
// Licensed under the GNU General Public License (GPL)
// https://www.gnu.org/licenses/gpl


#include "channel_manager_impl.h"


namespace sigviewer
{

//-----------------------------------------------------------------------------
ChannelManagerImpl::ChannelManagerImpl (FileSignalReader* file_signal_reader)
    : reader_ (file_signal_reader)
{
    setXAxisUnitLabel ("s");
}


//-----------------------------------------------------------------------------
ChannelManagerImpl::~ChannelManagerImpl ()
{
    delete reader_;
}

//-------------------------------------------------------------------------
std::set<ChannelID> ChannelManagerImpl::getChannels () const
{
    std::set<ChannelID> channels;
    for (ChannelID id = 0; id < static_cast<int>(reader_->getBasicHeader()->getNumberChannels()); ++id)
        channels.insert(id);
    return channels;
}

//-----------------------------------------------------------------------------
uint32 ChannelManagerImpl::getNumberChannels () const
{
    return reader_->getBasicHeader()->getNumberChannels();
}

//-------------------------------------------------------------------------
QString ChannelManagerImpl::getChannelLabel (ChannelID id) const
{
    if (id == UNDEFINED_CHANNEL)
        return QObject::tr("All Channels");
    else if (!reader_->getBasicHeader()->getChannel (id).isNull())
        return reader_->getBasicHeader()->getChannel (id)->getLabel();
    else
        return QObject::tr("Invalid Channel");
}

//-------------------------------------------------------------------------
QString ChannelManagerImpl::getChannelLabel (ChannelID id, int streamNumber) const
{
    if (id == UNDEFINED_CHANNEL)
    {
        if (streamNumber == -1)
            return QObject::tr("All Channels");
        else
            return QObject::tr("Stream %1").arg(QString::number(streamNumber+1));//+1 for user's convenience (1 based instead 0 based)
    }
    else if (!reader_->getBasicHeader()->getChannel (id).isNull())
        return reader_->getBasicHeader()->getChannel (id)->getLabel();
    else
        return QObject::tr("Invalid Channel");
}

//-------------------------------------------------------------------------
QString ChannelManagerImpl::getChannelYUnitString (ChannelID id) const
{
    if (id != UNDEFINED_CHANNEL)
        return reader_->getBasicHeader()->getChannel(id)->getPhysicalDim();
    else
        return "";
   // reader_->getBasicHeader()->get
}


//-----------------------------------------------------------------------------
QSharedPointer<DataBlock const> ChannelManagerImpl::getDataOld_ (ChannelID id, unsigned start_pos, unsigned length) const
{
    if (((start_pos + length) > getNumberSamplesOld_()) || length == 0)
        return QSharedPointer<DataBlock const> (0);
    else
        return reader_->getSignalDataOld_ (id, start_pos, length);
}

//-----------------------------------------------------------------------------
QSharedPointer<DataBlock const> ChannelManagerImpl::getDataNew (ChannelID id, unsigned start_pos, unsigned length) const
{
    if (((start_pos + length) > getChannelNumberSamplesNew(id)) || length == 0)
        return QSharedPointer<DataBlock const> (0);
    else
        return reader_->getSignalDataNew (id, start_pos, length);
}

//-----------------------------------------------------------------------------
float64 ChannelManagerImpl::getDurationInSec () const
{
    return reader_->getBasicHeader()->getNumberOfSamplesOld_() /
           reader_->getBasicHeader()->getSampleRateOld_();
}


//-----------------------------------------------------------------------------
size_t ChannelManagerImpl::getNumberSamplesOld_ () const
{
    return reader_->getBasicHeader()->getNumberOfSamplesOld_();
}

//-----------------------------------------------------------------------------
size_t ChannelManagerImpl::getChannelNumberSamplesNew (ChannelID id) const
{
    return reader_->getBasicHeader()->getChannelNumberOfSamplesNew(id);
}

//-----------------------------------------------------------------------------
float64 ChannelManagerImpl::getSampleRateOld_ () const
{
    return reader_->getBasicHeader()->getSampleRateOld_();
}

//-----------------------------------------------------------------------------
float64 ChannelManagerImpl::getChannelSampleRateNew (ChannelID id) const
{
    return reader_->getBasicHeader()->getChannelSampleRateNew(id);
}

}
