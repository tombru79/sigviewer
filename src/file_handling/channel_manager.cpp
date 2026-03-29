// Copyright (c) 2016 The SigViewer Development Team
// Licensed under the GNU General Public License (GPL)
// https://www.gnu.org/licenses/gpl


#include "channel_manager.h"
#include "base/fixed_data_block.h"

#include "gui/progress_bar.h"

#include <limits>

namespace sigviewer
{

////-------------------------------------------------------------------------
//QString ChannelManager::getChannelLabel(ChannelID id, int streamNumber) const
//{

//}

void ChannelManager::addDownsampledMinMaxVersion (ChannelID id, QSharedPointer<DataBlock const> min,
                                                  QSharedPointer<DataBlock const> max, unsigned factor)
{
    downsampled_max_map_[id][factor] = max;
    downsampled_min_map_[id][factor] = min;
}

//-------------------------------------------------------------------------
unsigned ChannelManager::getNearestDownsamplingFactor (ChannelID id, unsigned factor) const
{
    if (!downsampled_min_map_.contains (id))
        return 0;

    unsigned nearest_factor = 1;
    bool search = true;
    for (nearest_factor = factor + 1; search && (nearest_factor > 1); --nearest_factor)
        if (downsampled_min_map_[id].contains (nearest_factor - 1))
            search = false;

    return nearest_factor;
}

//-------------------------------------------------------------------------
QSharedPointer<DataBlock const> ChannelManager::getDownsampledMin (ChannelID id, unsigned factor) const
{
    return downsampled_min_map_[id][factor];
}

//-------------------------------------------------------------------------
QSharedPointer<DataBlock const> ChannelManager::getDownsampledMax (ChannelID id, unsigned factor) const
{
    return downsampled_max_map_[id][factor];
}

//-------------------------------------------------------------------------
QSharedPointer<DataBlock const> ChannelManager::getDownSamplesMinDataNew (ChannelID id, unsigned factor, unsigned start_pos, unsigned length) const
{
    if (!downsampled_min_map_.contains(id) || !downsampled_min_map_[id].contains(factor))
        return QSharedPointer<DataBlock const> (0);

    size_t number_of_samples = downsampled_min_map_[id][factor]->size();

    if (((start_pos + length) > number_of_samples) || length == 0)
        return QSharedPointer<DataBlock const> (0);

    if (length == number_of_samples && start_pos == 0)
        return downsampled_min_map_[id][factor];
    else
        return downsampled_min_map_[id][factor]->createSubBlock (start_pos, length);
}

//-------------------------------------------------------------------------
QSharedPointer<DataBlock const> ChannelManager::getDownSamplesMaxDataNew (ChannelID id, unsigned factor, unsigned start_pos, unsigned length) const
{
    if (!downsampled_max_map_.contains(id) || !downsampled_max_map_[id].contains(factor))
        return QSharedPointer<DataBlock const> (0);

    size_t number_of_samples = downsampled_max_map_[id][factor]->size();

    if (((start_pos + length) > number_of_samples) || length == 0)
        return QSharedPointer<DataBlock const> (0);

    if (length == number_of_samples && start_pos == 0)
        return downsampled_max_map_[id][factor];
    else
        return downsampled_max_map_[id][factor]->createSubBlock (start_pos, length);
}

//-------------------------------------------------------------------------
float64 ChannelManager::getValueRange (std::set<ChannelID> const& channels) const
{
    return getMaxValue (channels) - getMinValue (channels);
}

//-------------------------------------------------------------------------
float64 ChannelManager::getMinValue (std::set<ChannelID> const& channels) const
{
    if (!min_max_initialized_)
        initMinMax();

    float64 min = std::numeric_limits<float64>::max();
    for (const auto channel : channels)
        min = std::min (min, min_values_[channel]);
    return min;
}

//-------------------------------------------------------------------------
float64 ChannelManager::getMaxValue (std::set<ChannelID> const& channels) const
{
    if (!min_max_initialized_)
        initMinMax();

    float64 max = std::numeric_limits<float64>::min();
    for (const auto channel : channels)
        max = std::max (max, max_values_[channel]);
    return max;
}

//-------------------------------------------------------------------------
float64 ChannelManager::getMinValue (ChannelID channel_id) const
{
    if (!min_max_initialized_)
        initMinMax();

    if (min_values_.count (channel_id))
        return min_values_[channel_id];
    else
        return std::numeric_limits<float64>::min();
}

//-------------------------------------------------------------------------
float64 ChannelManager::getMaxValue (ChannelID channel_id) const
{
    if (!min_max_initialized_)
        initMinMax();

    if (max_values_.count (channel_id))
        return max_values_[channel_id];
    else
        return std::numeric_limits<float64>::max();
}

void ChannelManager::ChannelManager::initDownsamplesMinMax(ChannelID channel_id, unsigned factor_multiplier, unsigned up_to_factor, unsigned prev_factor) const
{
    unsigned factor = prev_factor * factor_multiplier;
    if (factor > up_to_factor || prev_factor < 1)
        return;

    QSharedPointer<DataBlock const> prev_min_data;
    QSharedPointer<DataBlock const> prev_max_data;
    if (prev_factor == 1)
    {
        prev_min_data = getDataNew (channel_id, 0, getChannelNumberSamplesNew(channel_id));
        prev_max_data = getDataNew (channel_id, 0, getChannelNumberSamplesNew(channel_id));
    }
    else
    {
        prev_min_data = downsampled_min_map_[channel_id][prev_factor];
        prev_max_data = downsampled_max_map_[channel_id][prev_factor];
    }

    size_t prev_number_of_samples = prev_min_data->size();
    size_t number_of_samples =  std::ceil(prev_number_of_samples / (float32)factor_multiplier);

    QSharedPointer<QVector<float32> > raw_min(new QVector<float32> (number_of_samples, NAN));
    QSharedPointer<QVector<float32> > raw_max(new QVector<float32> (number_of_samples, NAN));

    float32 tmp_min = NAN;
    float32 tmp_max = NAN;

    size_t data_index = 0;
    size_t factor_counter = 0;

    for (size_t prev_data_index = 0; prev_data_index < prev_number_of_samples; prev_data_index++)
    {
        float32 prev_min_value = prev_min_data->operator [](prev_data_index);
        if (factor_counter == 0 || prev_min_value < tmp_min)
            tmp_min = prev_min_value;

        float32 prev_max_value = prev_max_data->operator [](prev_data_index);
        if (factor_counter == 0 || prev_max_value > tmp_max)
            tmp_max = prev_max_value;

        if (++factor_counter == factor_multiplier)
        {
            raw_min->operator [](data_index) = tmp_min;
            raw_max->operator [](data_index) = tmp_max;

            factor_counter = 0;
            data_index++;
        }
    }
    if (factor_counter > 0)
    {
        raw_min->operator [](data_index) = tmp_min;
        raw_max->operator [](data_index) = tmp_max;
    }

    QSharedPointer<DataBlock const> min_data(new FixedDataBlock(raw_min, prev_min_data->getSampleRatePerUnit()/factor_multiplier));
    QSharedPointer<DataBlock const> max_data(new FixedDataBlock(raw_max, prev_max_data->getSampleRatePerUnit()/factor_multiplier));

    downsampled_min_map_[channel_id][factor] = min_data;
    downsampled_max_map_[channel_id][factor] = max_data;

    initDownsamplesMinMax(channel_id, factor_multiplier, up_to_factor, factor);
}

//-------------------------------------------------------------------------
void ChannelManager::initMinMax () const
{
    if (min_max_initialized_)
        return;
    for (const auto id : getChannels())
    {
        // old
        {
            QSharedPointer<DataBlock const> data = getDataOld_ (id, 0, getNumberSamplesOld_ ());
            max_values_[id] = data->getMax ();
            min_values_[id] = data->getMin ();
        }

        // new
        {
            QSharedPointer<DataBlock const> data = getDataNew (id, 0, getChannelNumberSamplesNew(id));
            max_values_[id] = data->getMax ();
            min_values_[id] = data->getMin ();

            initDownsamplesMinMax(id, 4, 256, 1);
        }

        ProgressBar::instance().increaseValue (1, QObject::tr("Searching for Min-Max"));
    }
    min_max_initialized_ = true;
}

}
