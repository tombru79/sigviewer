// signal_browser_model.cpp

#include "signal_browser_model_4.h"
#include "signal_browser_view.h"
#include "signal_graphics_item.h"
#include "y_axis_graphics_item.h"

#include "../base/file_signal_reader.h"
#include "../base/math_utils.h"

/*#include "main_window_model.h"
#include "signal_browser.h"
#include "smart_canvas/smart_canvas.h"
#include "smart_canvas/smart_canvas_view.h"
#include "signal_canvas_item.h"
#include "x_grid_canvas_item.h"
#include "channel_separator_canvas_item.h"
#include "navigation_canvas_item.h"
#include "x_axis_widget.h"
#include "y_axis_widget.h"
#include "label_widget.h"
#include "event_canvas_item.h"
#include "copy_event_dialog.h"
#include "base/event_table_file_reader.h"

#include <math.h>

#include <QDialog>
#include <QEvent>
#include <QCursor>
#include <QApplication>
#include <QInputDialog>
#include <QResizeEvent>

#include <QSettings>

#define max(a,b) ((a) > (b) ? (a) : (b))

*/

#include <QTextStream>
#include <QApplication>
#include <QDialog>

#include <cmath>
#include <iostream>

namespace BioSig_
{

namespace PortingToQT4_
{

//-----------------------------------------------------------------------------
// TODO! constructor
SignalBrowserModel::SignalBrowserModel(FileSignalReader& reader,
                                       MainWindowModel& main_window_model)
: signal_browser_view_ (0),
  log_stream_(0),
  main_window_model_(main_window_model),
  state_(STATE_READY),
  mode_(MODE_POINTER),
  signal_buffer_ (reader),
  basic_header_(reader.getBasicHeader()),
  release_buffer_(false),
  pixel_per_sec_(100),
  signal_height_(75),
  signal_spacing_(0),
  prefered_x_grid_pixel_intervall_(100),
  prefered_y_grid_pixel_intervall_(25),
  x_grid_pixel_intervall_(0),
  show_y_grid_(true)
/*,
  x_grid_item_(0),
  channel_separator_item_(0),
  navigation_item_(0),
  selected_event_item_(0),
  pixel_per_sec_(100),
  signal_height_(75),
  signal_spacing_(1),
  actual_event_creation_type_ (-1),
  show_x_grid_(true),
  show_channel_labels_(true),
  show_y_scales_(true),
  show_x_scales_(true),
  all_event_types_selected_(false)*/
{
    // nothing
}

//-----------------------------------------------------------------------------
// destructor
SignalBrowserModel::~SignalBrowserModel()
{
    // nothing to do
}


//-----------------------------------------------------------------------------
// get main window modell
MainWindowModel& SignalBrowserModel::getMainWindowModel()
{
    return main_window_model_;
}

//-----------------------------------------------------------------------------
// TODO! set signal browser view
void SignalBrowserModel::setSignalBrowserView(SignalBrowserView* signal_browser_view)
{
    signal_browser_view_ = signal_browser_view;

    /*
    if (!x_grid_item_)
    {
        x_grid_item_ = new XGridCanvasItem(*this, signal_browser_view_);
    }

    if (!channel_separator_item_)
    {
        channel_separator_item_
            = new ChannelSeparatorCanvasItem(*this, signal_browser_view_);
    }

    if (!navigation_item_)
    {
        navigation_item_ = new NavigationCanvasItem(*this, signal_browser_view_);
    }
    */
}

//-----------------------------------------------------------------------------
// TODO! set log stream
void SignalBrowserModel::setLogStream(QTextStream* log_stream)
{
    log_stream_ = log_stream;
    // signal_buffer_.setLogStream(log_stream);
}

//-----------------------------------------------------------------------------
// get log stream
QTextStream& SignalBrowserModel::getLogStream()
{
    return *log_stream_;
}

//-----------------------------------------------------------------------------
// TODO! load settings
void SignalBrowserModel::loadSettings()
{
/*
    QSettings settings("SigViewer");

    settings.beginGroup("SignalBrowserModel");

    release_buffer_ = settings.value("release_buffer", release_buffer_).toBool();
    pixel_per_sec_ = settings.value("pixel_per_sec", pixel_per_sec_).toDouble();
    signal_height_ = settings.value("signal_height", signal_height_).toInt();
    signal_spacing_ = settings.value("signal_spacing", signal_spacing_).toInt();
    prefered_x_grid_pixel_intervall_ = settings.value("prefered_x_grid_pixel_intervall",
                                                      prefered_x_grid_pixel_intervall_).toInt();
    prefered_y_grid_pixel_intervall_ = settings.value("prefered_y_grid_pixel_intervall",
                                                      prefered_y_grid_pixel_intervall_).toInt();
    show_x_grid_ = settings.value("show_x_grid", show_x_grid_).toBool();
    show_channel_labels_ = settings.value("show_channel_labels", show_channel_labels_).toBool();
    show_y_scales_ = settings.value("show_y_scales", show_y_scales_).toBool();
    show_x_scales_ = settings.value("show_x_scales", show_x_scales_).toBool();
    show_y_grid_ = settings.value("show_y_grid", show_y_grid_).toBool();

    all_event_types_selected_ = settings.value("all_event_types_selected", all_event_types_selected_).toBool();
    if (all_event_types_selected_)
    {
		int size = settings.beginReadArray("shown_event_types");
		for (int i = 0; i < size; i++)
		{
			settings.setArrayIndex(i);
			bool ok = false;
			uint16 event_type = settings.value("shown_event_type").toInt(&ok);
			if (ok)
				shown_event_types_.append(event_type);
		}

		settings.endArray();
    }

    settings.endGroup();

    SignalCanvasItem::loadSettings();
    XAxisWidget::loadSettings();
    YAxisWidget::loadSettings();
    LabelWidget::loadSettings();
    EventCanvasItem::loadSettings();
*/
}

//-----------------------------------------------------------------------------
// TODO! save settings
void SignalBrowserModel::saveSettings()
{
/*
    QSettings settings("SigViewer");

    settings.beginGroup("SignalBrowserModel");

    settings.setValue("release_buffer", release_buffer_);
    settings.setValue("pixel_per_sec", pixel_per_sec_);
    settings.setValue("signal_height", signal_height_);
    settings.setValue("signal_spacing", signal_spacing_);
    settings.setValue("prefered_x_grid_pixel_intervall", prefered_x_grid_pixel_intervall_);
    settings.setValue("prefered_y_grid_pixel_intervall", prefered_y_grid_pixel_intervall_);
    settings.setValue("show_x_grid", show_x_grid_);
    settings.setValue("show_channel_labels", show_channel_labels_);
    settings.setValue("show_y_scales", show_y_scales_);
    settings.setValue("show_x_scales", show_x_scales_);
    settings.setValue("show_y_grid", show_y_grid_);

    settings.setValue("all_event_types_selected", all_event_types_selected_);
    if (all_event_types_selected_)
    {
		settings.beginWriteArray("shown_event_types", shown_event_types_.size());
		for (int i = 0; i < shown_event_types_.size(); i++)
		{
			settings.setArrayIndex(i);
			settings.setValue("shown_event_type", shown_event_types_.at(i));
		}
		settings.endArray();
    }

    settings.endGroup();

    SignalCanvasItem::saveSettings();
    XAxisWidget::saveSettings();
    YAxisWidget::saveSettings();
    LabelWidget::saveSettings();
    EventCanvasItem::saveSettings();
*/
}

//-----------------------------------------------------------------------------
// TODO! set mode
void SignalBrowserModel::setMode(Mode mode)
{
    if (!checkSignalBrowserPtr("setMode") ||
        !checkReadyState("setMode"))
    {
        return;
    }

    mode_ = mode;
    //signal_browser_->getCanvasView()
    //                    ->setToolTipsEnabled(mode == MODE_POINTER ||
    //                                         mode == MODE_NEW);
}

//-----------------------------------------------------------------------------
// get mode
SignalBrowserModel::Mode SignalBrowserModel::getMode()
{
    return mode_;
}

//-----------------------------------------------------------------------------
void SignalBrowserModel::setWholeDataBuffer(SignalBuffer::WHOLE_BUFFER
                                                whole_buffer)
{
    signal_buffer_.setWholeDataBuffer(whole_buffer);
}

//-----------------------------------------------------------------------------
void SignalBrowserModel::enableInitDownsampling(bool enabled)
{
    signal_buffer_.enableInitDownsampling(enabled);
}

//-----------------------------------------------------------------------------
void SignalBrowserModel::enableInitMinMaxSearch(bool enabled)
{
    signal_buffer_.enableInitMinMaxSearch(enabled);
}

//-----------------------------------------------------------------------------
void SignalBrowserModel::setDefaultRange(float32 min, float32 max)
{
    signal_buffer_.setDefaultRange(min, max);
}



/*
// set release buffer
void SignalBrowserModel::setReleaseBuffer(bool release)
{
    release_buffer_ = release;
}

// get release buffer
bool SignalBrowserModel::getReleaseBuffer()
{
    return release_buffer_;
}
*/
// add signal view
void SignalBrowserModel::addChannel(uint32 channel_nr)
{
    if (!checkSignalBrowserPtr("addChannel") ||
        !checkReadyState("addChannel"))
    {
        return;
    }

    if (channel_nr >= basic_header_->getNumberChannels())
    {
        return; // illegal channel number
    }

    if(channel2signal_item_.find(channel_nr) != channel2signal_item_.end())
    {
        std::cout << "already added channel " << channel_nr << std::endl;
         return; // already added
    }

    const SignalChannel& signal_channel = basic_header_->getChannel(channel_nr);

    // generate signal canvas item
    SignalGraphicsItem* signal_item
        = new SignalGraphicsItem(signal_buffer_, basic_header_->getChannel(channel_nr), *this,
                               signal_browser_view_);

    channel2signal_item_[channel_nr] = signal_item;

    // add label to label widget
/*    signal_browser_->getLabelWidget()->addChannel(channel_nr,
                                                  signal_channel.getLabel());
*/
    // add label to y axis widget
    signal_browser_view_->getYAxisWidget().addChannel(channel_nr, signal_item);
    //signal_browser_view_->addSignalGraphicsItem(signal_item);//

    // add channel to buffer
    signal_buffer_.addChannel(channel_nr);
}

// remove signal view
void SignalBrowserModel::removeChannel(uint32 channel_nr)
{
    if (!checkSignalBrowserPtr("removeChannel") ||
        !checkReadyState("removeChannel"))
    {
        return;
    }

    Int2SignalGraphicsItemPtrMap::iterator sig_iter;

    sig_iter = channel2signal_item_.find(channel_nr);
    if(sig_iter == channel2signal_item_.end())
    {
         return; // not added
    }

    // remove signal canvas item
    channel2signal_item_.erase(sig_iter);
    delete sig_iter->second;

/*  TODO QT4: Implement!!!
    // remove label from label widget
    signal_browser_->getLabelWidget()->removeChannel(channel_nr);
*/

    signal_browser_view_->getYAxisWidget().removeChannel(channel_nr);

    // remove channel from buffer
    if (release_buffer_)
    {
        signal_buffer_.removeChannel(channel_nr);
    }
    else
    {
        signal_buffer_.setChannelActive(channel_nr, false);
    }
}

// is signal shown
bool SignalBrowserModel::isChannelShown(uint32 channel_nr)
{
    return channel2signal_item_.find(channel_nr) != channel2signal_item_.end();
}


// get number shown channels
uint32 SignalBrowserModel::getNumberShownChannels()
{
    return channel2signal_item_.size();
}


// init buffer
void SignalBrowserModel::initBuffer()
{
    if (!checkSignalBrowserPtr("initBuffer") ||
        !checkReadyState("initBuffer"))
    {
        return;
    }

    // set all channel active
    Int2SignalGraphicsItemPtrMap::iterator iter;

    for (iter = channel2signal_item_.begin();
         iter != channel2signal_item_.end();
         iter++)
    {
        signal_buffer_.setChannelActive(iter->first, true);
    }

    // init buffer
    state_ = STATE_INIT_BUFFER;
    signal_browser_view_->hide();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    signal_buffer_.init();
    QApplication::restoreOverrideCursor();
    state_ = STATE_READY;

    // get range from buffer
    for (iter = channel2signal_item_.begin();
         iter != channel2signal_item_.end();
         iter++)
    {
        iter->second->getRangeFromBuffer(2.0);
    }

    /*
    // gernerate event items
    uint32 number_events = signal_buffer_.getNumberEvents();

    if (id2event_item_.size() == 0)
    {
        for (uint32 event_nr = 0; event_nr < number_events; event_nr++)
        {
            int32 event_id = signal_buffer_.eventNumber2ID(event_nr);
            id2event_item_[event_id] = new EventCanvasItem(event_id,
                                                           signal_buffer_,
                                                           *this,
                                                           signal_browser_);
        }
    }*/
}


// TODO QT4: IMPLEMENT!!!! zoom in all
void SignalBrowserModel::zoomInAll()
{/*
    if (!checkSignalBrowserPtr("zoomInAll") ||
        !checkReadyState("zoomInAll"))
    {
        return;
    }

    Int2SignalCanvasItemPtrMap::iterator iter;
    for (iter = channel2signal_item_.begin();
         iter != channel2signal_item_.end();
         iter++)
    {
        iter.value()->zoomIn(false);
    }

    signal_browser_->getCanvas()->setAllChanged();
    signal_browser_->getCanvas()->update();
    signal_browser_->getYAxisWidget()->repaint();*/
}

// TODO QT4: IMPLEMENT !!!! zoom out all
void SignalBrowserModel::zoomOutAll()
{/*
    if (!checkSignalBrowserPtr("zoomOutAll") ||
        !checkReadyState("zoomOutAll"))
    {
        return;
    }

    Int2SignalCanvasItemPtrMap::iterator iter;

    for (iter = channel2signal_item_.begin();
         iter != channel2signal_item_.end();
         iter++)
    {
        iter.value()->zoomOut(false);
    }

    signal_browser_->getCanvas()->setAllChanged();
    signal_browser_->getCanvas()->update();
    signal_browser_->getYAxisWidget()->repaint();*/
}

// TODO QT4 IMPLEMENT!!! auto scale all
void SignalBrowserModel::autoScaleAll()
{
    // TODO QT4: implement!!
//    if (!checkSignalBrowserPtr("autoScaleAll") ||
//        !checkReadyState("autoScaleAll"))
//    {
//        return;
//    }
//
//    Int2SignalCanvasItemPtrMap::iterator iter;
//
//    for (iter = channel2signal_item_.begin();
//         iter != channel2signal_item_.end();
//         iter++)
//    {
//        iter.value()->autoScale(false, auto_zoom_type_);
//    }
//
//    signal_browser_->getCanvas()->setAllChanged();
//    signal_browser_->getCanvas()->update();
//    signal_browser_->getYAxisWidget()->repaint();
}

// set pixel per sec
void SignalBrowserModel::setPixelPerSec(float64 pixel_per_sec)
{
    if (!checkSignalBrowserPtr("setPixelPerSec") ||
        !checkReadyState("setPixelPerSec"))
    {
        return;
    }
    // TODO: implement!

    //int32 y = signal_browser_->getCanvasView()->contentsY();
    //int32 x  = (int32)(signal_browser_->getCanvasView()->contentsX() *
    //                   pixel_per_sec / pixel_per_sec_);
    pixel_per_sec_ = pixel_per_sec;
    updateLayout();
    //signal_browser_->getCanvasView()->setContentsPos(x, y);
}

// get pixel per sec
float64 SignalBrowserModel::getPixelPerSec()
{
    return pixel_per_sec_;
}



// set signal height
void SignalBrowserModel::setSignalHeight(int32 height)
{
    signal_height_ = height;
    updateLayout();
    // TODO: reimplement!
//    if (!checkSignalBrowserPtr("setSignalHeight") ||
//        !checkReadyState("setSignalHeight"))
//    {
//        return;
//    }
//
//    int32 x = signal_browser_->getCanvasView()->contentsX();
//    int32 y  = (int32)(signal_browser_->getCanvasView()->contentsY() *
//                       (height + signal_spacing_) /
//                       (signal_height_ + signal_spacing_));
//
//    signal_height_ = height;
//    updateLayout();
//    signal_browser_->getCanvasView()->setContentsPos(x, y);
}

// get signal height
int32 SignalBrowserModel::getSignalHeight()
{
    return signal_height_;
}

// update layout
void SignalBrowserModel::updateLayout()
{
    int32 width = (int32)(signal_buffer_.getBlockDuration() *
                          signal_buffer_.getNumberBlocks() * pixel_per_sec_);

    int32 height = (signal_height_  + signal_spacing_) *
                   channel2signal_item_.size();

    QDialog* dialog = new QDialog;
    dialog->setCaption("Initialize Chunk Matrix");
    dialog->resize(250, 50);
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    dialog->show();
    signal_browser_view_->resizeScene(width, height);
    QApplication::restoreOverrideCursor();
    dialog->hide();
    delete dialog;

    // TODO: implement other items!!!
    /*
    // navigation item
    navigation_item_->setSize(width, height);
    navigation_item_->setX(0);
    navigation_item_->setY(0);
    navigation_item_->setZ(NAVIGATION_Z);
    navigation_item_->show();

    // x grid
    x_grid_item_->setSize(width, height);
    x_grid_item_->setX(0);
    x_grid_item_->setY(0);
    x_grid_item_->setZ(X_GRID_Z);

    x_grid_item_->setVisible(show_x_grid_);

    // channel separator
    channel_separator_item_->setSize(width, height);
    channel_separator_item_->setX(0);
    channel_separator_item_->setY(0);
    channel_separator_item_->setZ(CHANNEL_SEPARATOR_Z);
    channel_separator_item_->show();
    */
    // singanls position
    channel2y_pos_.clear();
    channel2y_pos_[SignalEvent::UNDEFINED_CHANNEL] = 0;
    int32 y_pos = 0;
    Int2SignalGraphicsItemPtrMap::iterator signal_iter;

    std::cout << "signal_height_ = "<< signal_height_ << std::endl;

    for (signal_iter = channel2signal_item_.begin();
         signal_iter != channel2signal_item_.end();
         signal_iter++, y_pos += signal_height_ + signal_spacing_)
    {
        channel2y_pos_[signal_iter->first] = y_pos;

        std::cout << "y_pos = "<< y_pos << std::endl;
        //signal_iter->second->setRect(0, y_pos, width, signal_height_);
        signal_iter->second->setHeight (signal_height_);
        signal_iter->second->setPos (0, y_pos); // FIXME: why "/2" ????
        signal_iter->second->setZValue(SIGNAL_Z);
        signal_iter->second->updateYGridIntervall();

        signal_iter->second->enableYGrid(show_y_grid_);
        signal_browser_view_->addSignalGraphicsItem(signal_iter->first, signal_iter->second);

        signal_iter->second->show();
    }
    // TODO: implement events!!
    /*

    // event items
    Int2EventCanvasItemPtrMap::iterator event_iter;
    for (event_iter = id2event_item_.begin();
         event_iter != id2event_item_.end();
         event_iter++)
    {
        SignalEvent* event = signal_buffer_.getEvent(event_iter.key());
        if (!event)
        {
            *log_stream_ << "SignalBrowserModel::updateLayout Error: "
                         << "inconsistant events\n";
            continue;
        }

        Int2IntMap::iterator y_pos_iter;
        y_pos_iter = channel2y_pos_.find(event->getChannel());

        if (!shown_event_types_.contains(event->getType()) ||
            y_pos_iter == channel2y_pos_.end())
        {
            event_iter.value()->hide();
            if (event_iter.value() == selected_event_item_)
            {
                selected_event_item_->setSelected(false);
                selected_event_item_ = 0;
                main_window_model_.setSelectionState(
                                        MainWindowModel::SELECTION_STATE_NONE);
            }

            continue; // event type or channel not shown
        }

        int32 event_width = (int32)(pixel_per_sec_ *
                                    (float64)event->getDuration() /
                                    signal_buffer_.getEventSamplerate() + 0.5);

        event_width = max(event_width, 1);
        event_iter.value()->setY(y_pos_iter.value());

        if (event->getChannel() == SignalEvent::UNDEFINED_CHANNEL)
        {
            event_iter.value()->setSize(event_width, height);
        }
        else
        {
            event_iter.value()->setSize(event_width, signal_height_);
        }

        event_iter.value()->setZ(EVENT_Z + event->getType() / 100000.0);
        int32 event_x = (int32)(pixel_per_sec_ * (float64)event->getPosition() /
                                signal_buffer_.getEventSamplerate() + 0.5);

        event_iter.value()->setX(event_x);
        event_iter.value()->updateColor();
        event_iter.value()->show();
    }
    */
    // update x grid intervall
    float64 x_grid_intervall = round125(prefered_x_grid_pixel_intervall_ /
                                        pixel_per_sec_);

    x_grid_pixel_intervall_ = pixel_per_sec_ * x_grid_intervall;

    /* TODO: IMPLEMENT!!!
  	signal_browser_->getLabelWidget()->setVisible(show_channel_labels_);
  	signal_browser_->getXAxisWidget()->setVisible(show_x_scales_);
  	signal_browser_->getYAxisWidget()->setVisible(show_y_scales_);
*/
    signal_browser_view_->update();
    /* TODO: IMPLEMENT!!!!
    signal_browser_->getXAxisWidget()->update();
    signal_browser_->getLabelWidget()->update();
    signal_browser_->getYAxisWidget()->update();
    */
}
/*
// set signal spacing
void SignalBrowserModel::setSignalSpacing(int32 spacing)
{
    if (!checkSignalBrowserPtr("setSignalSpacing") ||
        !checkReadyState("setSignalSpacing"))
    {
        return;
    }

    signal_spacing_ = spacing;
    updateLayout();
}
*/
// get signal spacing
int32 SignalBrowserModel::getSignalSpacing()
{
    return signal_spacing_;
}
/*
// canvas view content moving
void SignalBrowserModel::canvasViewContentMoving(int content_x, int content_y)
{
    if (!checkSignalBrowserPtr("canvasViewContentMoving") ||
        !checkReadyState("canvasViewContentMoving"))
    {
        return;
    }

    SmartCanvasView* canvas_view = signal_browser_->getCanvasView();
    int32 visible_height = canvas_view->visibleHeight();
    int32 signal_view_y = 0;
    Int2SignalCanvasItemPtrMap::iterator iter;

    for (iter = channel2signal_item_.begin();
         iter != channel2signal_item_.end();
         iter++)
    {
        if (signal_view_y + signal_height_ > content_y &&
            signal_view_y < content_y + visible_height)
        {
            signal_buffer_.setChannelActive(iter.key(), true);
        }
        else
        {
            signal_buffer_.setChannelActive(iter.key(), false); // prediction
        }

        signal_view_y += signal_height_ + signal_spacing_;
    }
}

// set prefered x grid pixel intervall
void SignalBrowserModel::setPreferedXGirdPixelIntervall(int32 pixel_intervall)
{
    if (!checkSignalBrowserPtr("setPreferedXGirdPixelIntervall") ||
        !checkReadyState("setPreferedXGirdPixelIntervall"))
    {
        return;
    }

    prefered_x_grid_pixel_intervall_ = pixel_intervall;
    updateLayout();
}

// set prefered y grid pixel intervall
void SignalBrowserModel::setPreferedYGirdPixelIntervall(int32 pixel_intervall)
{
    if (!checkSignalBrowserPtr("setPreferedYGirdPixelIntervall") ||
        !checkReadyState("setPreferedYGirdPixelIntervall"))
    {
        return;
    }

    prefered_y_grid_pixel_intervall_ = pixel_intervall;
    updateLayout();
}

// get prefered x grid pixel intervall
int32 SignalBrowserModel::getPreferedXGirdPixelIntervall()
{
    return prefered_x_grid_pixel_intervall_;
}
*/
// get prefered y grid pixel intervall
int32 SignalBrowserModel::getPreferedYGirdPixelIntervall()
{
    return prefered_y_grid_pixel_intervall_;
}

// get x grid pixel intervall
float64 SignalBrowserModel::getXGridPixelIntervall()
{
    return x_grid_pixel_intervall_;
}


// check ready state
inline bool SignalBrowserModel::checkReadyState(const QString& function)
{
    if (state_ != STATE_READY)
    {
        *log_stream_ << "SignalBrowserModel::" << function << " "
                     << "Error: illegal state\n";
        return false;
    }

    return true;
}

// check signal browser ptr
bool SignalBrowserModel::checkSignalBrowserPtr(const QString function)
{
    if (!signal_browser_view_)
    {
        *log_stream_ << "SignalBrowserModel::" << function << " "
                     << "Error: SignalBrowser not set\n";
        return false;
    }

    return true;
}

// TODO QT4: IMPLEMENT!!! go to
void SignalBrowserModel::goTo(int32 sec, int32 channel_index)
{/*
    int32 x = (int32)(sec * pixel_per_sec_);
    int32 y = channel_index < 0 ? signal_browser_->getCanvasView()->contentsY()
                                : channel_index *
                                  (signal_height_ + signal_spacing_);

    signal_browser_->getCanvasView()->setContentsPos(x, y);*/
}
/*
// zoom rect
void SignalBrowserModel::zoomRect(const QRect& rect)
{
    SmartCanvasView* canvas_view = signal_browser_->getCanvasView();
    int32 visible_width = canvas_view->visibleWidth();
    int32 visible_height = canvas_view->visibleHeight();
    float64 sec = rect.x() / pixel_per_sec_;
    int32 y_signals = (int32)(rect.y() / (float64)(signal_height_ +
                                          signal_spacing_));

    pixel_per_sec_ *= visible_width / (float64)rect.width();
    pixel_per_sec_ = pixel_per_sec_ > visible_width ? visible_width
                                                   : pixel_per_sec_;

    float64 signals_per_page = visible_height / (signal_height_ +
                                                 signal_spacing_);

    signals_per_page *= rect.height() / (float64)visible_height;
    signal_height_ = (int32)(visible_height / ceil(signals_per_page));
    signal_height_ = signal_height_ > visible_height ? visible_height
                                                     : signal_height_;
    signal_height_ -= signal_spacing_;
    updateLayout();

    int32 x = (int32)(sec * pixel_per_sec_);
    int32 y = (int32)(y_signals * (signal_height_ + signal_spacing_));

    signal_browser_->getCanvasView()->setContentsPos(x, y);
    main_window_model_.pixelPerSecChanged(pixel_per_sec_);
    main_window_model_.signalHeightChanged(signal_height_);
}
*/
// TODO QT4: IMPLEMENT!!! get shown event types
void SignalBrowserModel::getShownEventTypes(IntList& event_type)
{/*
    event_type = shown_event_types_;*/
}

// TODO QT4: IMPLEMENT!!! get shown event types
void SignalBrowserModel::setShownEventTypes(const IntList& event_type, const bool all)
{/*
    shown_event_types_ = event_type;
    all_event_types_selected_ = all;*/
}
/*
// set event changed
void SignalBrowserModel::setEventChanged(uint32 id, bool update)
{
    if (!checkSignalBrowserPtr("setEventChanged") ||
        !checkReadyState("setEventChanged"))
    {
        return;
    }

    SignalEvent* event = signal_buffer_.getEvent(id);
    if (!event)
    {
        *log_stream_ << "SignalBrowserModel::setEventChanged "
                     << "Error: invalid event-id\n";
        return;
    }

    EventCanvasItem* event_item = id2event_item_[id];
    Int2IntMap::iterator y_pos_iter;

    y_pos_iter = channel2y_pos_.find(event->getChannel());

    if (!shown_event_types_.contains(event->getType()) ||
        y_pos_iter == channel2y_pos_.end())
    {
        event_item->hide(); // event type or channel not shown
    }
    else
    {
        int32 height = (signal_height_  + signal_spacing_) *
                        channel2signal_item_.size();
        int32 event_width = (int32)(pixel_per_sec_ *
                                    (float64)event->getDuration() /
                                    signal_buffer_.getEventSamplerate() + 0.5);

        event_width = max(event_width, 1);
        event_item->setY(y_pos_iter.value());

        if (event->getChannel() == SignalEvent::UNDEFINED_CHANNEL)
        {
            event_item->setSize(event_width, height);
        }
        else
        {
            event_item->setSize(event_width, signal_height_);
        }

        event_item->setZ(EVENT_Z + event->getType() / 100000.0);
        int32 event_x = (int32)(pixel_per_sec_ * (float64)event->getPosition() /
                                signal_buffer_.getEventSamplerate() + 0.5);

        event_item->setX(event_x);
        event_item->updateColor();
        event_item->show();
    }

    if (update)
    {
        signal_browser_->getCanvas()->update();
    }

    main_window_model_.setChanged();
}
*/
// TODO! : remove event
void SignalBrowserModel::removeEvent(uint32 id, bool update)
{
    /// QT4 TODO: IMPLEMENT!!!
/*    Int2EventCanvasItemPtrMap::iterator it = id2event_item_.find(id);

    if (it == id2event_item_.end())
    {
        return;
    }

    EventCanvasItem* event_item = it.value();

    id2event_item_.erase(it);
    delete event_item;
    signal_buffer_.removeEvent(id);

    if (update)
    {
        signal_browser_->getCanvas()->update();
    }

    main_window_model_.setChanged();*/
}

// TODO! add event
void SignalBrowserModel::addEvent(const SignalEvent& event,
                                              bool update)
{
    /// TODO QT4: implement
    /*int32 event_id = signal_buffer_.addEvent(event);
    EventCanvasItem* event_item = new EventCanvasItem(event_id,
                                                      signal_buffer_,
                                                      *this, signal_browser_);

    id2event_item_[event_id] = event_item;
    setEventChanged(event_id, update);

    return event_item;*/
}

// TODO QT4: IMPLEMENT!!! set selected event item
void SignalBrowserModel::setSelectedEventItem(EventGraphicsItem* item)
{/*
    if (selected_event_item_)
    {
        selected_event_item_->setSelected(false);
    }

    selected_event_item_ = item;

    if (!selected_event_item_)
    {
        main_window_model_.setSelectionState(
                                MainWindowModel::SELECTION_STATE_NONE);
    }
    else
    {
        selected_event_item_->setSelected(true);
        signal_browser_->getCanvas()->update();

        if (signal_buffer_.getEvent(item->getId())->getChannel() ==
            SignalEvent::UNDEFINED_CHANNEL)
        {
            main_window_model_.setSelectionState(
                                MainWindowModel::SELECTION_STATE_ALL_CHANNELS);
        }
        else
        {
            main_window_model_.setSelectionState(
                                MainWindowModel::SELECTION_STATE_ONE_CHANNEL);
        }
    }*/
}
/*
// get selected event item
EventCanvasItem* SignalBrowserModel::getSelectedEventItem()
{
    return selected_event_item_;
}
*/
// QT4 TODO:!!!! set selected channel to all channels
void SignalBrowserModel::setSelectedEventToAllChannels()
{
    /* QT4 TODO: IMPLEMENT!!
    if (!selected_event_item_)
    {
        return;
    }

    uint32 id = selected_event_item_->getId();
    SignalEvent* event = signal_buffer_.getEvent(id);

    event->setChannel(SignalEvent::UNDEFINED_CHANNEL);
    main_window_model_
        .setSelectionState(MainWindowModel::SELECTION_STATE_ALL_CHANNELS);
    setEventChanged(id);*/
}

// TODO QT4: IMPLEMENT!!! change selected event channel
void SignalBrowserModel::changeSelectedEventChannel()
{
    /*
    if (!selected_event_item_)
    {
        return;
    }

    uint32 id = selected_event_item_->getId();
    SignalEvent* event = signal_buffer_.getEvent(id);

    // generate channel list
    int32 current_item = 0;
    QStringList channel_list;

    channel_list.append(tr("All Channels"));

    for (uint32 channel_nr = 0;
         channel_nr < basic_header_->getNumberChannels();
         channel_nr++)
    {
        if (isChannelShown(channel_nr))
        {
            if (event->getChannel() == (int32)channel_nr)
            {
                current_item = channel_list.size();
            }

            channel_list.append(
                QString("(%1) ").arg(channel_nr + 1) +
                basic_header_->getChannel(channel_nr).getLabel());
        }
    }

    // dialog
    bool ok = false;

    QString res = QInputDialog::getItem(tr("Change Channel"),
                                        tr("Select new Channel:"),
                                        channel_list, current_item,
                                        false, &ok, signal_browser_);

    int32 new_channel = res.replace(')', '(').section('(', 1, 1).toInt() - 1;

    if (ok && new_channel != event->getChannel())
    {
        event->setChannel(new_channel);
        setEventChanged(id);

        if (event->getChannel() == SignalEvent::UNDEFINED_CHANNEL)
        {
            main_window_model_
              .setSelectionState(MainWindowModel::SELECTION_STATE_ALL_CHANNELS);
        }
        else
        {
            main_window_model_
               .setSelectionState(MainWindowModel::SELECTION_STATE_ONE_CHANNEL);
        }
    }*/
}

// TODO QT4: IMPLEMENT!!! copy selected event to channels
void SignalBrowserModel::copySelectedEventToChannels()
{
    /*
    if (!selected_event_item_)
    {
        return;
    }

    uint32 id = selected_event_item_->getId();
    SignalEvent* event = signal_buffer_.getEvent(id);
    CopyEventDialog copy_event_dialog(basic_header_, signal_browser_);

    for (uint32 channel_nr = 0;
         channel_nr < basic_header_->getNumberChannels();
         channel_nr++)
    {
        if ((int32)channel_nr != event->getChannel() &&
            isChannelShown(channel_nr))
        {
            copy_event_dialog.addSelectableChannel(channel_nr);
        }
    }

    // show dialog
    copy_event_dialog.loadSettings();
    copy_event_dialog.exec();
    copy_event_dialog.saveSettings();

    if (copy_event_dialog.result() == QDialog::Rejected)
    {
        return; // user cancel
    }

    // generate copies
    for (uint32 channel_nr = 0;
         channel_nr < basic_header_->getNumberChannels();
         channel_nr++)
    {
        if (copy_event_dialog.isSelected(channel_nr))
        {
            SignalEvent new_event(*event);
            new_event.setChannel(channel_nr);
            addEvent(new_event, false);
        }
    }

    signal_browser_->getCanvas()->update();*/
}

// TODO QT4: IMPLEMENT!!! change selected event type
void SignalBrowserModel::changeSelectedEventType()
{
    /*
    SignalEvent* event = 0;

    if (selected_event_item_)
        event = signal_buffer_.getEvent(selected_event_item_->getId());

    // generate list show all shown types
    QStringList event_type_list;
    int32 current_item = 0;

    for (IntList::const_iterator it = shown_event_types_.begin();
         it != shown_event_types_.end();
         it++)
    {
        if (event)
        {
            if (event->getType() == *it)
                current_item = event_type_list.size();
        }
        else
            if (actual_event_creation_type_ == *it)
                current_item = event_type_list.size();

        QString event_name
             = main_window_model_.getEventTableFileReader().getEventName(*it);

            event_type_list.append(event_name + " " + QString("(%1)")
                                                        .arg(*it,4, 16)
                                                        .replace(' ', '0'));
    }

    // dialog
    bool ok = false;

    QString res = QInputDialog::getItem(tr("Change Type"),
                                        tr("Select new Type:"),
                                        event_type_list, current_item,
                                        false, &ok, signal_browser_);

    uint16 new_type = res.right(5).left(4).toUShort(0, 16);

    if (event)
    {
        if (ok && new_type != event->getType())
        {
            event->setType(new_type);
            setEventChanged(selected_event_item_->getId());
        }
    }

    else
        if (ok)
            actual_event_creation_type_ = new_type;*/
}
/*
uint16 SignalBrowserModel::getActualEventCreationType () const
{
    return actual_event_creation_type_;
}
*/
// TODO QT4: IMPLEMENT!!! remove selected event
void SignalBrowserModel::removeSelectedEvent()
{/*
    if (!selected_event_item_)
    {
        return;
    }

    uint32 id = selected_event_item_->getId();

    selected_event_item_ = 0;
    main_window_model_.setSelectionState(MainWindowModel::SELECTION_STATE_NONE);
    removeEvent(id);*/
}
/*
// get channel nr
int32 SignalBrowserModel::getChannelNr(uint32 shown_nr)
{
    if (static_cast<int32>(shown_nr) >= channel2signal_item_.size())
    {
        return -1;
    }

    Int2SignalCanvasItemPtrMap::iterator it = channel2signal_item_.begin();

    while (shown_nr > 0)
    {
        it++;
        shown_nr--;
    }

    return it.key();
}
*/
// get events
void SignalBrowserModel::getEvents(SignalEventVector& event_vector)
{
    int32 number_events = signal_buffer_.getNumberEvents();

    for (int event_nr = 0; event_nr < number_events; event_nr++)
    {
        int32 event_id = signal_buffer_.eventNumber2ID(event_nr);
        event_vector.push_back(*signal_buffer_.getEvent(event_id));
    }
}

//-------------------------------------------------------------------------
void SignalBrowserModel::showXScales(bool enabled)
{
    // TODO QT4: IMPLEMENT!!!
    /*show_x_scales_ = enabled;*/
}

//-------------------------------------------------------------------------
void SignalBrowserModel::showYScales(bool enabled)
{
    // TODO QT4: IMPLEMENT!!!
    /*show_y_scales_ = enabled;*/
}


//-------------------------------------------------------------------------
void SignalBrowserModel::showChannelLabels(bool enabled)
{
    // TODO QT4: IMPLEMENT!!!
 /*   show_channel_labels_ = enabled;*/
}

//-------------------------------------------------------------------------
void SignalBrowserModel::setXGridVisible(bool visible)
{
    // TODO QT4: IMPLEMENT!!!
    /*show_x_grid_ = visible;*/
}

//-------------------------------------------------------------------------
void SignalBrowserModel::setYGridVisible(bool visible)
{
    // TODO QT4: IMPLEMENT!!!
    /*show_y_grid_ = visible;*/
}

//-------------------------------------------------------------------------
void SignalBrowserModel::setAutoZoomBehaviour(ScaleMode auto_zoom_type)
{
    // TODO QT4: IMPLEMENT!!!
    /*uto_zoom_type_ = auto_zoom_type;*/
}


// TODO QT4: IMPLEMENT!!!!
bool SignalBrowserModel::isShowAllEventTypes() const
{
 /*   return all_event_types_selected_;*/
    return false; /// TODO QT4 : RETURN all_event_types_selected_
}


} // namespace PortingToQT4_

} // namespace Biosig_
