/*
 * mainwindow.h
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtkmm.h>

#include "vars.h"
#include "debugwindow.h"
#include "config.h"
#include "prefswindow.h"
#include "Log.h"
#include "messagewindow.h"

namespace Math
{

namespace Gui
{

class MainWindow : public Gtk::Window
{
private:
	struct TabData;
	typedef std::pair<cuint, TabData> pairis;
	typedef Glib::RefPtr<Gtk::TextBuffer> txtbuff;
	
private:
	struct TabData
	{
		str file;
		str uri;
		txtbuff output_buffer;
		txtbuff input_buffer;
		str status_msg;
		vstr exp_history;
		vstr ans_history;
		bool fractions_enabled = true;
	};
	
	struct WinPos
	{
		int x, y;
	};
	
public:
	MainWindow(cint argc, char** argv);
	virtual ~MainWindow();
	void changeStatus(constr& message, const MessageState& msg_state = MessageState::kNone);
	void say(constr& message, const Verbosity& verbosity = Verbosity::kError, const MessageState& msg_state = MessageState::kNone); //Verbose error is lowest besides no verbosity
	void newTab();
	bool saveOutput(constr& file);
	void setWinCount(cint wincount);
	
protected:
	bool outputEvent(GdkEvent*);
	bool inputEvent(GdkEvent*, Gtk::TextView*);
	void removeCurrentTab();
	void removeTab(Gtk::Box&);
	bool tabRightClick(GdkEventButton*, const Gtk::Box&);
	void tabReorder(Widget*, guint);
	void tabSwitch(Widget*,guint);
	void saveOutputDialog();
	bool saveFileExistDialog(constr& file);
	void fractionsToggle();
	bool keyPress(GdkEventKey*, txtbuff&, txtbuff&);
	bool fractionsEnalbedClick(GdkEventButton*);
	void buffOutChanged(Gtk::ScrolledWindow&);
	void closeWindow();
	bool inputKeyPress(GdkEventKey*);
	void clearTabHistory();
	bool txtInputFocus(GdkEventButton*, Gtk::TextView*);
	void dragInputFocus(GdkEvent*, Gtk::TextView*);
	void showAboutWindow();
	void onAboutResponse(int);
	void showDebugWin();
	void debugWindowHide();
	void newMainWindow();
	bool fractionsEnabledEventEnter(GdkEventCrossing*);
	bool fractionsEnabledEventLeave(GdkEventCrossing*);
	bool tabRemoved(GdkEvent*);
	bool mainWindowDrag(GdkEventFocus*);
	void onInsertMain();
	void onPaste(const Glib::RefPtr<Gtk::Clipboard>&);
	void showMessageWindow();
	
	Gtk::Box box_main_;
	Gtk::Box* box_tabs_;
	Glib::RefPtr<Gtk::UIManager> uimanager_;
	Glib::RefPtr<Gtk::ActionGroup> actiongrp_;
	Gtk::Notebook tabs_;
	Gtk::ScrolledWindow* scroll_win_;
	Gtk::ScrolledWindow* scroll_win_i_;
	Gtk::TextView* txt_view_;
	Gtk::TextView* txt_view_i_; //i = input
	txtbuff buffer_input_;
	txtbuff buffer_output_;
	txtbuff buffer_mark_;
	Gtk::Statusbar statusbar_;
	Gtk::EventBox* event_box_;
	Gtk::Menu* tab_menu_;
	Glib::RefPtr<Gtk::Action> menuitem_fractions_;
	Gtk::TextView* txt_view_mark_;
	Gtk::Label fractions_enabled_;
	Gtk::EventBox evtb_fractions_;
	Glib::RefPtr<Gtk::Action> fractions_menu_;
	Gtk::Box status_box_; //box to hold two (main, and fractions) statusbars
	Gtk::Statusbar statusbar_fractions_;
	Glib::RefPtr<Gtk::Action> menuitem_fractions_gcd_;
	Glib::RefPtr<Gtk::Action> menuitem_save_;
	Glib::RefPtr<Gtk::Action> menuitem_save_as_;
	Glib::RefPtr<Gtk::Action> menuitem_close_tab_;
	Glib::RefPtr<Gtk::Action> menuitem_clear_all_tabs_;
	Glib::RefPtr<Gtk::Action> menuitem_clear_tab_;
	Glib::RefPtr<Gtk::Action> menuitem_calculate_;
	Gtk::AboutDialog about_window_;
	Glib::RefPtr<Gdk::Pixbuf> window_icon_;
	DebugWindow debugwin_;
	MainWindow* mainwindow_;
	Glib::RefPtr<Gtk::TextBuffer::Tag> tag_num_ = Gtk::TextBuffer::Tag::create();
	Glib::RefPtr<Gtk::TextBuffer::Tag> tag_sym_ = Gtk::TextBuffer::Tag::create();
	Glib::RefPtr<Gtk::TextBuffer::TagTable> tagtable_ = Gtk::TextBuffer::TagTable::create();
	MessageWindow msgwindow_;
	
private:
	void processArgs(cint argc, const char* const* argv);
	void setupMenu();
	void setMarkerTag();
	void setupTabMenu();
	int nextTabNum();
	constr updateTabNum(bool add, cuint num);
	void save();
	void saveOutputWrite(constr file);
	bool getFirstSave();
	void setFirstSave(const txtbuff& output_buffer, const txtbuff& input_buffer);
	void removeTabData(cint = kdefault_value_);
	inline cint getTabNum(cint = kdefault_value_) const;
	void updateMap(cint, const TabData&);
	void updateSave();
	void updateStatus(constr& message, const MessageState& msg_state = MessageState::kNone);
	constr getStatus() const;
	inline const TabData getCurrentTabData() const;
	void onEnter();
	void onUpArrow(txtbuff& buffer_input);
	void onDownArrow(txtbuff& buffer_input);
	void addHistory(constr& exp, constr& ans);
	constr getHistory(bool exp, uint position) const;
	void setHistory(txtbuff& buffer_input, bool exp);
	void increaseHistoryPosition(cint num);
	void initTabData();
	void addCurrentHistory(constr& text);
	constr formatOutput(constr& current_output, constr& new_output, constr& exp);
	void setFractionsEnabled(bool toggle = false);
	bool getFractionsEnabled() const;
	void onCtrlUp(txtbuff& buffer_input);
	void onCtrlDown(txtbuff& buffer_input);
	void arrowPressed(txtbuff& buffer_input, cint num, bool exp);
	void gcdToggle();
	constr getEnabled(bool enabled);
	void setFractionsMenu(bool enabled);
	void setFractionsMenuEnabled(bool enabled);
	constr getVersion();
	WinPos getDebugWinPos();
	void displayDebug(constr& txt, FunctionDebug function, bool procedure);
	void setSettings();
	void enableTabItems(const bool enable);
	void saveSettings();
	void showPrefsWindow();
	void prefsWindowApply(PrefsWindowSettings prefswinsettings);
	WinPos getCenterPos(const Gtk::Window& window);
	void colorText(txtbuff buffer);
	void applyTag(txtbuff buffer, const Glib::RefPtr<Gtk::TextBuffer::Tag>&, cint pos);
	void viewLog();
	
	static constr kexp_marker_ ;
	static constr ktab_label_;
	static constr kfractions_;
	static constr kenabled_;
	static constr kdisabled_;
	static constr kgcd_;
	static constexpr cint kwinbuffer_ = 7;
	cint kargc_; //used for new window arguments
	char** kargv_;
	static constexpr cint kdefault_value_ = -1;
	static constr klog_;
	str working_dir_;
	
	
	bool debug_ = false;
	bool update_text_ = false;
	Verbosity verbosity_;
	int win_width_;
	vuint tab_numbers_;
	uint current_tab_ = 0;
	uint tab_reorder_call_ = 0;
	uint tab_count_ = 0;
	str save_file_;
	std::map<cuint, TabData> tabdata_map_;
	str current_dir_;
	uint tab_switch_call_ = 0;
	bool creating_tab_ = false;
	uint history_position_ = 0;
	bool gcd_enabled_ = true;
	bool shutingdown_ = false;
	ConfigSettings configsettings_;
	Config config_;
	bool debugwin_enabled_ = false;
	DebugWindowSettings dwsettings_;
	int wincount_ = 0;
	Glib::RefPtr<Gtk::Application> app_;
	PrefsWindow prefswindow_;
	Log log_;
	bool color_ = DefaultConfigVars::kcolor;
};

} //namespace Gui
} //namespace Math

#endif /* MAINWINDOW_H_ */
