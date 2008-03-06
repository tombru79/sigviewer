#ifndef CNT_READER_H_
#define CNT_READER_H_
#include "../file_signal_reader.h"
#include "cnt_setup_header.h"

#include <biosig.h>

#include <QFile>
#include <QMutex>

namespace BioSig_
{

// CNT reader
class CNTReader : public FileSignalReader
{
public:
    CNTReader();
    virtual ~CNTReader();
    virtual FileSignalReader* clone();

    virtual bool open(const QString& file_name);
    virtual bool isOpen() {return biosig_header_ ? true : false;}
    virtual void close() {delete biosig_header_; biosig_header_ = 0;}

    virtual void loadSignals(SignalDataBlockPtrIterator begin, 
                             SignalDataBlockPtrIterator end,
                             uint32 start_record);
    //-------------------------------------------------------------------------
    /// there are no events in BKR-files so this method does nothing
    virtual void loadEvents(SignalEventVector& event_vector) {}
    
    virtual QPointer<BasicHeader> getBasicHeader ();
    
    //-------------------------------------------------------------------------
    /// 
    /// @return false if an error occured
    ///         true if everything is fine
    virtual bool loadRawRecords(float64** record_data, uint32 start_record,
                                uint32 records);

private:
    // not allowed
    CNTReader(const CNTReader& src);
    const CNTReader& operator=(const CNTReader& src);

    bool loadFixedHeader(const QString& file_name);

    int8* buffer_;
    QPointer<BasicHeader> basic_header_;
    CNTSetupHeader header_;
    mutable QMutex mutex_;
    mutable QMutex biosig_access_lock_;
    HDRTYPE* biosig_header_;
};

} // namespace BioSig_

#endif /*CNT_READER_H_*/
