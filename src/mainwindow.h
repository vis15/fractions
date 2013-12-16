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

namespace Math
{

namespace Gui
{

class MainWindow : public Gtk::Window
{
private:
	struct TabData
	{
		str file;
		str uri;
		Glib::RefPtr<Gtk::TextBuffer> output_buffer;
	};
	
public:
	MainWindow(int argc, char* argv[]);
	virtual ~MainWindow();
	void updateStatus(constr& message, const MessageState& msg_state = MessageState::kNone);
	void say(constr& message, const MessageState& msg_state = MessageState::kNone, const Verbosity& verbosity = Verbosity::kError); //Verbose error is lowest besides no verbosity
	void newTab();
	bool saveOutput(constr file);
	
protected:
	void updateInputBox(const Gtk::TextBuffer::iterator&, const Glib::ustring&, int, Glib::RefPtr<Gtk::TextBuffer>, Glib::RefPtr<Gtk::TextBuffer>);
	bool outputEvent(GdkEvent*);
	bool inputEvent(GdkEvent*, Gtk::TextView*);
	void setMark(const Gtk::TextBuffer::iterator&,const Glib::RefPtr<Gtk::TextBuffer::Mark>&, Gtk::TextView*);
	void removeCurrentTab();
	void removeTab(Gtk::Box&);
	bool tabRightClick(GdkEventButton*, Glib::RefPtr<Gtk::TextBuffer>, Gtk::Box&);
	void tabReorder(Widget*,guint);
	void tabSwitch(Widget*,guint, Glib::RefPtr<Gtk::TextBuffer>);
	void saveOutputDialog();
	bool saveFileExistDialog(constr file);
	void fractionsToggle();
	
	Gtk::Box box_main_;
	Gtk::Box* box_tabs_;
	Glib::RefPtr<Gtk::UIManager> uimanager_;
	Glib::RefPtr<Gtk::ActionGroup> actiongrp_;
	Gtk::Notebook tabs_;
	Gtk::ScrolledWindow* scroll_win_;
	Gtk::ScrolledWindow* scroll_win_i_;
	Gtk::TextView* txt_view_;
	Gtk::TextView* txt_view_i_; //i = input
	Glib::RefPtr<Gtk::TextBuffer> buffer_input_;
	Glib::RefPtr<Gtk::TextBuffer> buffer_output_;
	Gtk::TextBuffer::iterator buffer_it_;
	Glib::RefPtr<Gtk::TextBuffer::Mark> buffer_mark_;
	Gtk::Statusbar statusbar_;
	Gtk::Separator sep;
	Glib::RefPtr<Gtk::TextBuffer::Tag> tag_;
	Glib::RefPtr<Gtk::TextBuffer::TagTable> tag_table_;
	Gtk::EventBox* event_box_;
	Gtk::Menu* tab_menu_;
	Glib::RefPtr<Gtk::ToggleAction> menu_fractions_;
	
private:
	void processArgs(int argc, char* argv[]);
	void setupMenu();
	void setMarkerTag();
	void setupTabMenu();
	int nextTabNum();
	constr updateTabNum(bool add, cuint num);
	void save();
	void saveOutputWrite(constr file);
	bool getFirstSave();
	void setFirstSave(Glib::RefPtr<Gtk::TextBuffer> output_buffer);
	void removeFirstSave();
	void removeFirstSave(cint);
	inline cint getTabNum() const;
	inline cint getTabNum(cint) const;
	void updateMap(cint, TabData);
	void updateSave();
	
	static constexpr cint kchar_limit_ = 19;
	static constexpr cint kavg_char_size = 2;
	static constexpr cint kchars_width_ = 4;
	static constr kexp_marker_ ;
	static constr ktab_label_;
	
	bool debug_ = false;
	bool update_text_ = false;
	Verbosity verbosity_;
	int win_width_;
	vuint tab_numbers_;
	uint current_tab_ = 0;
	uint tab_reorder_call_ = 0;
	uint tab_count_ = 0;
	str save_file_;
	std::map<cuint, TabData> save_map_;
	str current_dir_;
	Glib::RefPtr<Gtk::TextBuffer> current_output_buffer_;
	
private:
	typedef std::pair<cuint, TabData> pairis;
};

} //namespace Gui
} //namespace Math

#endif /* MAINWINDOW_H_ */
