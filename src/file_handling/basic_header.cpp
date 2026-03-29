// Copyright (c) 2016 The SigViewer Development Team
// Licensed under the GNU General Public License (GPL)
// https://www.gnu.org/licenses/gpl


#include "basic_header.h"

namespace sigviewer
{

//-----------------------------------------------------------------------------
QString BasicHeader::getFileTypeString() const
{
    return file_type_string_;
}

//-----------------------------------------------------------------------------
float64 BasicHeader::getSampleRateOld_() const
{
    return sample_rate_old_;
}

//-------------------------------------------------------------------------
float64 BasicHeader::getChannelSampleRateNew (ChannelID id) const
{
    if (channels_.contains(id))
        return channels_[id]->getSampleRate();

    return sample_rate_old_;
}

//-----------------------------------------------------------------------------
uint32 BasicHeader::getNumberEvents() const
{
    return number_events_;
}

void BasicHeader::setNumberEvents(uint32 number_events)
{
    number_events_ = number_events;
}

// get event samplerate
double BasicHeader::getEventSamplerate() const
{
    return sample_rate_old_;
}

void BasicHeader::setEventSamplerate(double event_sample_rate)
{
    event_sample_rate_ = event_sample_rate;
}

//-------------------------------------------------------------------------
QSharedPointer<SignalChannel const> BasicHeader::getChannel(ChannelID id) const
{
    if (channels_.contains(id))
        return channels_[id];
    return QSharedPointer<SignalChannel const> (0);
}


//-------------------------------------------------------------------------
unsigned BasicHeader::getNumberChannels() const
{
    return channels_.size();
}

//-------------------------------------------------------------------------
void BasicHeader::setFileTypeString(QString const& file_type_string)
{
    file_type_string_ = file_type_string;
}

//-------------------------------------------------------------------------
void BasicHeader::setSampleRateOld_(float64 sample_rate)
{
    sample_rate_old_ = sample_rate;
}

//-------------------------------------------------------------------------
void BasicHeader::addChannel(ChannelID id, QSharedPointer<SignalChannel const> channel)
{
    channels_[id] = channel;
}

}
