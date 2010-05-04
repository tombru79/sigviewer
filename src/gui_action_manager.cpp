#include "gui_action_manager.h"
#include "application_context.h"
#include "main_window_model_impl.h"
#include "gui/gui_action_factory.h"

#include <QApplication>
#include <QStyle>

namespace BioSig_
{

//-----------------------------------------------------------------------------
GUIActionManager::GUIActionManager ()
    : main_window_model_ (0),
      application_state_ (APP_STATE_NO_FILE_OPEN),
      file_state_ (FILE_STATE_UNCHANGED),
      tab_edit_state_ (TAB_STATE_NO_REDO_NO_UNDO)
{
    action_map_[ACTION_SEPARATOR] = new QAction (this);
    action_map_[ACTION_SEPARATOR]->setSeparator (true);
}

//-----------------------------------------------------------------------------
GUIActionManager::~GUIActionManager ()
{
    // QActions are deleted automatically because they are children of this
    // GUIActionManager object!
}

//-----------------------------------------------------------------------------
void GUIActionManager::init (MainWindowModelImpl* main_window_model)
{
    connect (ApplicationContext::getInstance().data(), SIGNAL(stateChanged(ApplicationState)), SLOT(setApplicationState(ApplicationState)));
    main_window_model_ = main_window_model;
    initViewActions();
    initOptionsActions();
    initToolsActions();
    initShortcuts ();
    initGroups ();
    initDisabledStates ();
    setApplicationState (application_state_);
    setFileState (file_state_);
}

//-------------------------------------------------------------------------
void GUIActionManager::setApplicationState (ApplicationState
                                            application_state)
{
    ActionAppStateMap::iterator state_iter =
            app_state_action_map_.find (application_state_);
    if (state_iter != app_state_action_map_.end ())
        setActionsEnabled (state_iter->second, true);

    application_state_ = application_state;

    updateAllActionsDisabling ();
}

//-------------------------------------------------------------------------
void GUIActionManager::setFileState (FileState file_state)
{
    ActionFileStateMap::iterator state_iter =
            file_state_action_map_.find (file_state_);
    if (state_iter != file_state_action_map_.end ())
        setActionsEnabled (state_iter->second, true);

    file_state_ = file_state;

    updateAllActionsDisabling ();
}

//-----------------------------------------------------------------------------
void GUIActionManager::setTabSelectionState(TabSelectionState tab_state)
{
    ActionTabSelectionStateMap::iterator state_iter =
            tab_selection_state_action_map_.find (tab_selection_state_);
    if (state_iter != tab_selection_state_action_map_.end ())
        setActionsEnabled (state_iter->second, true);

    tab_selection_state_ = tab_state;

    updateAllActionsDisabling ();
}

//-----------------------------------------------------------------------------
void GUIActionManager::setTabEditState(TabEditState tab_state)
{
    ActionTabEditStateMap::iterator state_iter =
            tab_edit_state_action_map_.find (tab_edit_state_);
    if (state_iter != tab_edit_state_action_map_.end ())
        setActionsEnabled (state_iter->second, true);

    tab_edit_state_ = tab_state;

    updateAllActionsDisabling ();
}

//-----------------------------------------------------------------------------
QMenu* GUIActionManager::getGroupAsMenu (ActionGroup group, QString const& title,
                                         QWidget* parent)
{
    QMenu* menu = new QMenu (title, parent);
    ActionGroupMap::iterator group_iter = action_group_map_.find (group);
    if (group_iter != action_group_map_.end())
    {
        for (ActionList::iterator action_iter = group_iter->second.begin();
             action_iter != group_iter->second.end ();
             ++action_iter)
        {
            ActionMap::iterator action = action_map_.find (*action_iter);
            if (action == action_map_.end())
                menu->addAction (tr("ACTION NOT INITIALIZED!!!!"));
            else
            {
                if (action->second->isSeparator ())
                    menu->addSeparator ();
                else
                    menu->addAction (action->second);
            }
        }
    }
    return menu;
}

//-----------------------------------------------------------------------------
QList<QAction*> GUIActionManager::getActionsOfGroup (ActionGroup group)
{
    QList<QAction*> list;
    ActionGroupMap::iterator group_iter = action_group_map_.find (group);
    if (group_iter != action_group_map_.end())
    {
        for (ActionList::iterator action_iter = group_iter->second.begin();
             action_iter != group_iter->second.end ();
             ++action_iter)
        {
            ActionMap::iterator action = action_map_.find (*action_iter);
            if (action == action_map_.end())
                list.append (new QAction (tr("ACTION NOT INITIALIZED!!!!"), this));
            else
            {
                if (action->second->isSeparator ())
                    list.append (createSeparator());
                else
                    list.append (action->second);
            }
        }
    }
    return list;
}

//-----------------------------------------------------------------------------
void GUIActionManager::initViewActions ()
{
    createAction (ACTION_VIEW_ZOOM_IN, tr("Zoom &In"),
                  SLOT(viewZoomInAction()),
                  tr("Zoom in all channels"),
                  QIcon(":/images/zoom_in_22x22.png"));

    createAction (ACTION_VIEW_ZOOM_OUT, tr("Zoom &Out"),
                  SLOT(viewZoomOutAction()),
                  tr("Zoom out all channels"),
                  QIcon(":/images/zoom_out_22x22.png"));

    createAction (ACTION_VIEW_AUTO_SCALE, tr("&Auto Scale"),
                  SLOT(viewAutoScaleAction()),
                  tr("Autoscale all channels"),
                  QIcon(":/images/auto_scale_22x22.png"));
}

//-------------------------------------------------------------------------
void GUIActionManager::initOptionsActions ()
{
    createAction (ACTION_OPTIONS_CHANGE_EVENT_CREATION_TYPE,
                  tr("Set Event Creation Type"),
                  SLOT(optionsChangeCreationType()),
                  tr("Change the type of newly created or inserted events"));

    createAction (ACTION_SHOW_SETTINGS,
                  tr("Settings"),
                  SLOT(optionsShowSettingsAction()),
                  tr("Shows the settings dialog/"),
                  QIcon(":/images/icons/configure.png"));
}

//-------------------------------------------------------------------------
void GUIActionManager::initToolsActions ()
{
    createAction (ACTION_CALCULATE_MEANS,
                  tr("Calculate Mean (alpha version)"),
                  SLOT(calculateMeanAction()),
                  tr("Calculates mean of selected event type"));
}

//-----------------------------------------------------------------------------
void GUIActionManager::initShortcuts ()
{
    setShortCut (ACTION_VIEW_ZOOM_IN, QKeySequence::ZoomIn);
    setShortCut (ACTION_VIEW_ZOOM_OUT, QKeySequence::ZoomOut);
}

//-----------------------------------------------------------------------------
void GUIActionManager::initGroups ()
{
    // FILE_MENU_ACTIONS
    action_group_map_[FILE_MENU_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[FILE_MENU_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[FILE_MENU_ACTIONS].push_back (ACTION_SEPARATOR);

    // EDIT_MENU_ACTIONS
    action_group_map_[EDIT_MENU_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[EDIT_MENU_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[EDIT_MENU_ACTIONS].push_back (ACTION_SEPARATOR);

    // EVENT_CONTEXT_ACTIONS
    action_group_map_[EVENT_CONTEXT_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[EVENT_CONTEXT_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[EVENT_CONTEXT_ACTIONS].push_back (ACTION_SEPARATOR);

    // VIEW_MENU_ACTIONS
    action_group_map_[VIEW_MENU_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[VIEW_MENU_ACTIONS].push_back (ACTION_VIEW_ZOOM_IN);
    action_group_map_[VIEW_MENU_ACTIONS].push_back (ACTION_VIEW_ZOOM_OUT);
    action_group_map_[VIEW_MENU_ACTIONS].push_back (ACTION_VIEW_AUTO_SCALE);
    action_group_map_[VIEW_MENU_ACTIONS].push_back (ACTION_SEPARATOR);
    action_group_map_[VIEW_MENU_ACTIONS].push_back (ACTION_SEPARATOR);


    // VIEW_TOOLBAR_ACTIONS
    action_group_map_[VIEW_TOOLBAR_ACTIONS].push_back (ACTION_VIEW_ZOOM_IN);
    action_group_map_[VIEW_TOOLBAR_ACTIONS].push_back (ACTION_VIEW_ZOOM_OUT);
    action_group_map_[VIEW_TOOLBAR_ACTIONS].push_back (ACTION_VIEW_AUTO_SCALE);

    // OPTIONS_MENU_ACTIONS
    action_group_map_[OPTIONS_MENU_ACTIONS].push_back (ACTION_SHOW_SETTINGS);
    action_group_map_[OPTIONS_MENU_ACTIONS].push_back (ACTION_OPTIONS_CHANGE_EVENT_CREATION_TYPE);

    // TOOLS_MENU_ACTIONS
    action_group_map_[TOOLS_MENU_ACTIONS].push_back (ACTION_CALCULATE_MEANS);
}

//-----------------------------------------------------------------------------
void GUIActionManager::initDisabledStates ()
{
    app_state_action_map_[APP_STATE_NO_FILE_OPEN].push_back (ACTION_VIEW_ZOOM_IN);
    app_state_action_map_[APP_STATE_NO_FILE_OPEN].push_back (ACTION_VIEW_ZOOM_OUT);
    app_state_action_map_[APP_STATE_NO_FILE_OPEN].push_back (ACTION_VIEW_AUTO_SCALE);
    app_state_action_map_[APP_STATE_NO_FILE_OPEN].push_back (ACTION_OPTIONS_CHANGE_EVENT_CREATION_TYPE);
    app_state_action_map_[APP_STATE_NO_FILE_OPEN].push_back (ACTION_SHOW_SETTINGS);
    app_state_action_map_[APP_STATE_NO_FILE_OPEN].push_back (ACTION_CALCULATE_MEANS);
}


//-----------------------------------------------------------------------------
QAction* GUIActionManager::createAction (GUIAction action_id,
                                         QString const& text,
                                         char const * const slot,
                                         QString const& status_tip,
                                         QIcon const& icon)
{
    QAction* action = new QAction (text, this);
    if (!icon.isNull())
        action->setIcon (icon);
    if (!status_tip.isEmpty())
        action->setStatusTip (status_tip);
    main_window_model_->connect (action, SIGNAL(triggered()), slot);
    action_map_[action_id] = action;
    return action;
}

//-------------------------------------------------------------------------
QAction* GUIActionManager::createSeparator ()
{
    QAction* separator = new QAction (this);
    separator->setSeparator (true);
    return separator;
}


//-------------------------------------------------------------------------
void GUIActionManager::setShortCut (GUIAction const&  gui_action,
                                    QKeySequence const& key_sequence)
{
    ActionMap::iterator action_iter = action_map_.find (gui_action);
    if (action_iter != action_map_.end())
        action_map_[gui_action]->setShortcut (key_sequence);
}

//-------------------------------------------------------------------------
void GUIActionManager::updateAllActionsDisabling ()
{
    ActionTabSelectionStateMap::iterator tab_state_iter =
            tab_selection_state_action_map_.find (tab_selection_state_);
    if (tab_state_iter != tab_selection_state_action_map_.end ())
        setActionsEnabled (tab_state_iter->second, false);

    ActionTabEditStateMap::iterator tab_edit_state_iter =
            tab_edit_state_action_map_.find (tab_edit_state_);
    if (tab_edit_state_iter != tab_edit_state_action_map_.end ())
        setActionsEnabled (tab_edit_state_iter->second, false);

    ActionAppStateMap::iterator app_state_iter =
            app_state_action_map_.find (application_state_);
    if (app_state_iter != app_state_action_map_.end ())
        setActionsEnabled (app_state_iter->second, false);

    ActionFileStateMap::iterator file_state_iter =
            file_state_action_map_.find (file_state_);
    if (file_state_iter != file_state_action_map_.end ())
        setActionsEnabled (file_state_iter->second, false);

}


//-------------------------------------------------------------------------
void GUIActionManager::setActionsEnabled (ActionList& action_list, bool enabled)
{
    for (ActionList::iterator action_iter = action_list.begin();
         action_iter != action_list.end ();
         ++action_iter)
    {
        ActionMap::iterator action = action_map_.find (*action_iter);
        if (action != action_map_.end())
            action->second->setEnabled (enabled);
    }
}



} // namespace BioSig_
