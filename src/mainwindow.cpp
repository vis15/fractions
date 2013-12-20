/*
 * mainwindow.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#include <iostream>
#include <fstream>

#include "mainwindow.h"
#include "parser.h"
#include "util.h"
#include "menu.h"
#include "window_icon.h"

namespace Math
{

namespace Gui
{

/////////////////////////////////////////////////////////////////////////
//class MainWindow
/////////////////////////////////////////////////////////////////////////
constr MainWindow::kexp_marker_ = ">";
constr MainWindow::ktab_label_ = "Math ";

MainWindow::MainWindow(int argc, char* argv[])
{
	processArgs(argc, argv);
	
	Gtk::Window::set_title(Info::kProgName());
	Glib::RefPtr<Gdk::Pixbuf> pix = Gdk::Pixbuf::create_from_data(math_icon.pixel_data, Gdk::COLORSPACE_RGB, false, 8, math_icon.width, math_icon.height, math_icon.width*3);
	this->set_icon(pix);
	Gtk::Window::set_default_size(500, 500);
	//this->set_border_width(5);
	
	//box_main
	box_main_.set_orientation(Gtk::ORIENTATION_VERTICAL);
	
	//Setup MainWindow
	Gtk::Window::add(box_main_);
	
	setupMenu();
	
	//Main Area
	//Tabs
	box_main_.add(tabs_);
	
	newTab();

	//setup statusbar
	
	box_main_.add(sep);
	statusbar_.set_border_width(5);
	box_main_.add(statusbar_);
	
	MainWindow::show_all_children();
	
	txt_view_i_->grab_focus();
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::setupMenu()
{
	menu_fractions_ = Gtk::ToggleAction::create("CalcFractions", "Fractions: Enabled", "When checked fractions will be enabled");
	menu_fractions_->set_active(true);
	menu_fractions_->signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::fractionsToggle));
	
	actiongrp_ = Gtk::ActionGroup::create();
	actiongrp_->add(Gtk::Action::create("FileMenu", "File"));
	actiongrp_->add(Gtk::Action::create_with_icon_name("FileNewWindow", "window-new", "_New Window", ""), Gtk::AccelKey("<control>N"));
	actiongrp_->add(Gtk::Action::create("FileNewTab", Gtk::Stock::NEW, "New _Tab"), Gtk::AccelKey("<control>T"), sigc::mem_fun(*this, &MainWindow::newTab));
	actiongrp_->add(Gtk::Action::create("FileSave", Gtk::Stock::SAVE, "_Save Output"), sigc::mem_fun(*this, &MainWindow::save));
	actiongrp_->add(Gtk::Action::create("FileSaveAs", Gtk::Stock::SAVE_AS, "Save Output As..."), Gtk::AccelKey("<control><shift>S"), sigc::mem_fun(*this, &MainWindow::saveOutputDialog));
	actiongrp_->add(Gtk::Action::create("FileCloseTab", Gtk::Stock::CLOSE, "Close Tab"), Gtk::AccelKey("<control>W"), sigc::mem_fun(*this, &MainWindow::removeCurrentTab));
	actiongrp_->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT));
	actiongrp_->add(Gtk::Action::create("EditMenu", "Edit"));
	actiongrp_->add(Gtk::Action::create("EditClearHistory", Gtk::Stock::CLEAR, "Clear History"));
	actiongrp_->add(Gtk::Action::create("EditClearAll", Gtk::Stock::CLEAR, "Clear All"));
	actiongrp_->add(Gtk::Action::create("EditPref", Gtk::Stock::PREFERENCES));
	actiongrp_->add(Gtk::Action::create("CalcMenu", "Calculate"));
	actiongrp_->add(Gtk::Action::create_with_icon_name("CalcCalc", "system-run", "Calculate", ""), Gtk::AccelKey("<release>Enter"));
	actiongrp_->add(menu_fractions_);
	actiongrp_->add(Gtk::Action::create("HelpMenu", "Help"));
	actiongrp_->add(Gtk::Action::create("HelpHelp", Gtk::Stock::HELP));
	actiongrp_->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT));
	
	uimanager_ = Gtk::UIManager::create();
	uimanager_->insert_action_group(actiongrp_);
	
	this->add_accel_group(uimanager_->get_accel_group());
	
	try
	{
		uimanager_->add_ui_from_string(getMenu());
	}
	catch(const Glib::Error& error)
	{
		say("uimanager failed: " + error.what(), MessageState::kError);
		return;
	}
	
	Gtk::Widget* menu = uimanager_->get_widget("/menu-main");
	if(!menu)
		say("menu get widget failed", MessageState::kError);
	else
		box_main_.add(*menu);
}

void MainWindow::fractionsToggle()
{
	str label = "Fractions: ";
	if(menu_fractions_->get_active())
		label += "Enabled";
	else
		label += "Disabled";
	
	menu_fractions_->set_label(label);
}

//void MainWindow::btnOK()
//{
	
//	Parser::Parser p(txt_exp_.get_text(), debug_);
//	p.signal_say.connect(sigc::mem_fun(*this, &MainWindow::say));
//	try
//	{
//		txt_ans_.set_text(p.parse());
//	}
//	catch(constr& err_msg)
//	{
//		say(err_msg);
//	}
//}

void MainWindow::newTab()
{	
	tab_count_++;
	cint tab_num = nextTabNum();
	updateTabNum(true, tab_num);
	
	Gtk::Box* lbl_box = Gtk::manage(new Gtk::Box);
	event_box_ = Gtk::manage(new Gtk::EventBox);
	Gtk::Label* lbl = Gtk::manage(new Gtk::Label);
	lbl->set_text(ktab_label_ + Util::toString(tab_num));
	event_box_->add(*lbl);
	lbl_box->pack_start(*event_box_);
	Gtk::Button* button = Gtk::manage(new Gtk::Button);
	Gtk::Image* image = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_SMALL_TOOLBAR));
	button->add(*image);
	lbl_box->pack_start(*button, Gtk::PACK_SHRINK);

	tabs_.set_scrollable(true);
	
	box_tabs_ = Gtk::manage(new Gtk::Box);
	setupTabMenu();
	tabs_.append_page(*box_tabs_, *lbl_box);
	tabs_.set_tab_reorderable(*box_tabs_, true);
	tabs_.set_tab_detachable(*box_tabs_, true);
	box_tabs_->set_orientation(Gtk::ORIENTATION_VERTICAL);
	
	scroll_win_ = Gtk::manage(new Gtk::ScrolledWindow);
	box_tabs_->add(*scroll_win_);
	
	auto box_mark = Gtk::manage(new Gtk::Box);
	box_tabs_->add(*box_mark);
	txt_view_mark_ = Gtk::manage(new Gtk::TextView);
	txt_view_mark_->set_margin_left(2);
	txt_view_mark_->set_border_width(2);
	txt_view_mark_->set_pixels_above_lines(2);
	txt_view_mark_->set_editable(false);
	txt_view_mark_->set_state_flags(Gtk::STATE_FLAG_FOCUSED);
	txt_view_mark_->set_can_focus(false);
	
	setMarkerTag();
	buffer_mark_ = Gtk::TextBuffer::create(tag_table_);
	txt_view_mark_->set_buffer(buffer_mark_);
	buffer_mark_->insert_with_tag(buffer_mark_->begin(), kexp_marker_, tag_);
	
	box_mark->pack_start(*txt_view_mark_, Gtk::PACK_SHRINK);
	txt_view_mark_->set_size_request(0);
	scroll_win_i_ = Gtk::manage(new Gtk::ScrolledWindow);
	box_mark->pack_start(*scroll_win_i_);
	
	//Output box
	txt_view_ = Gtk::manage(new Gtk::TextView);
	txt_view_->set_can_focus(false);
	txt_view_->set_border_width(4);
	txt_view_->set_editable(false);
	txt_view_->set_state_flags(Gtk::STATE_FLAG_FOCUSED);
	buffer_output_ = txt_view_->get_buffer();
	txt_view_->set_buffer(buffer_output_);
	scroll_win_->set_vexpand(true);
	scroll_win_->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	scroll_win_->add(*txt_view_);
	
	//Input box
	txt_view_i_ = Gtk::manage(new Gtk::TextView);
	txt_view_i_->set_border_width(2);
	txt_view_i_->set_margin_right(2);
	txt_view_i_->set_pixels_above_lines(2);
	
	buffer_input_ = txt_view_i_->get_buffer();
	txt_view_i_->set_buffer(buffer_input_);
	scroll_win_i_->add(*txt_view_i_);
	
	//keeps the input box from going grey.
	txt_view_i_->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::inputEvent), txt_view_i_));
	
	//connects buffer_input_ with buffer_output_ and checks for return/enter
	txt_view_i_->signal_key_release_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::keyPress), buffer_input_, buffer_output_), false);
	
	button->signal_clicked().connect(sigc::bind<Gtk::Box&>(sigc::mem_fun(*this, &MainWindow::removeTab), *box_tabs_));

	event_box_->signal_button_press_event().connect(sigc::bind<Glib::RefPtr<Gtk::TextBuffer>, Gtk::Box&>(sigc::mem_fun(*this, &MainWindow::tabRightClick), buffer_output_, *box_tabs_), false);

	tabs_.signal_page_reordered().connect(sigc::mem_fun(*this, &MainWindow::tabReorder));
	tabs_.signal_switch_page().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::tabSwitch), buffer_output_));
	
	lbl_box->show_all();
	tabs_.show_all();
	
	say("Created New Tab: " + Util::toString(tab_num), MessageState::kInfo, Verbosity::kDebug);
	
	tabs_.set_current_page(tabs_.page_num(*box_tabs_));
	
	txt_view_i_->grab_focus();
	
	//setup saves
	setFirstSave(buffer_output_);
	
	updateStatus("");
}

bool MainWindow::keyPress(GdkEventKey* eventkey, Glib::RefPtr<Gtk::TextBuffer> buffer_input, Glib::RefPtr<Gtk::TextBuffer> buffer_output)
{
	if(eventkey->keyval != GDK_KEY_Return && eventkey->keyval != GDK_KEY_KP_Enter)
		return false;
	
	//todo: this whole function can be a thread
	say("Calculating...", MessageState::kInfo, Verbosity::kNone);
	
	Parser::Parser p(buffer_input->get_text(), debug_);
	//p.signal_say.connect(sigc::mem_fun(*this, &MainWindow::say));

	try
	{
		buffer_output->set_text(p.parse());
		buffer_input->set_text("");
	}
	catch(constr& err_msg)
	{
		say(err_msg, MessageState::kError, Verbosity::kError);
		return false;
	}
	
	say("Done", MessageState::kInfo, Verbosity::kNone);
	
	return false;
}

void MainWindow::tabReorder(Widget* widget, guint page_num)
{
	tab_reorder_call_++;
	if(tab_reorder_call_ == tab_count_) //signal calls this function for each page including removed pages, we only need the first call
		tab_reorder_call_ = 0;
	
	if(tab_reorder_call_ != 1)
		return;
	
	cint tab_num = tab_numbers_.at(current_tab_);
	tab_numbers_.erase(tab_numbers_.begin()+current_tab_);
	tab_numbers_.insert(tab_numbers_.begin()+page_num, tab_num);
	current_tab_ = page_num;
	
	//std::cout << "new order: " << Util::vecToString(tab_numbers_) << std::endl;
}

void MainWindow::tabSwitch(Widget*,guint page_num, Glib::RefPtr<Gtk::TextBuffer> buffer)
{
//	tab_switch_call_++;
//	if(tab_switch_call_ == tab_count_) //same as tabReorder
//		tab_switch_call_ = 0;
//	
//	if(tab_switch_call_ != 1)
//		return;
	
	current_tab_ = page_num;
	//TabData td = tabdata_map_.at(getTabNum());
	
}

void MainWindow::setMarkerTag()
{
	tag_ = Gtk::TextBuffer::Tag::create();
	tag_->property_editable() = false;
	Gdk::RGBA color_purple(ColorsHTML::kDPurple());
	tag_->property_foreground_rgba() = color_purple;
	tag_table_ = Gtk::TextBuffer::TagTable::create();
	tag_table_->add(tag_);
}

void MainWindow::setupTabMenu()
{
	tab_menu_ = Gtk::manage(new Gtk::Menu);
	Gtk::MenuItem* item = Gtk::manage(new Gtk::MenuItem("Save"));
	tab_menu_->append(*item);
	item = Gtk::manage(new Gtk::MenuItem("Save As..."));
	tab_menu_->append(*item);
	Gtk::SeparatorMenuItem* separator = Gtk::manage(new Gtk::SeparatorMenuItem);
	tab_menu_->append(*separator);
	item = Gtk::manage(new Gtk::MenuItem("Close"));
	item->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::removeCurrentTab));
	tab_menu_->append(*item);
	tab_menu_->show_all();
}

bool MainWindow::tabRightClick(GdkEventButton* event, Glib::RefPtr<Gtk::TextBuffer> buffer_output, Gtk::Box& close_tab)
{
	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) ) //right click
	{
		const int a = tabs_.page_num(close_tab);
		tabs_.set_current_page(a);
		tab_menu_->popup(event->button, event->time);
	}
	
	return false;
}

bool MainWindow::inputEvent(GdkEvent* event, Gtk::TextView* txt_view)
{
	txt_view->set_state_flags(Gtk::STATE_FLAG_FOCUSED);
		
	return false;
}

void MainWindow::updateStatus(constr& message, const MessageState& msg_state /* = MessageState::kNone */)
{	
	Gdk::RGBA color;
	
	switch(msg_state)
	{
	case MessageState::kNone:
	case MessageState::kInfo:
		color.set(ColorsHTML::kPurple()); //purple
		break;
	case MessageState::kWarning:
	case MessageState::kError:
		color.set("red");
		break;
	}
	
	statusbar_.override_color(color); //if not set each time color will stay the same
	statusbar_.push(message, 1);
	
	TabData td = tabdata_map_.at(getTabNum());
	td.status_msg = message;
	updateMap(getTabNum(), td);
}

void MainWindow::processArgs(int argc, char* argv[])
{
	//set default values
	verbosity_ = Verbosity::kError;
	
	for(int i=0; i<argc; i++)
	{
		constr var = argv[i];
		if(var == "--debug")
		{
			debug_ = true;
			verbosity_ = Verbosity::kDebug;
		}
		else if(var == "-v")
		{
			verbosity_ = Verbosity::kInfo;
		}
		else if(var == "-vv")
		{
			verbosity_ = Verbosity::kDebug;
		}
		else if(var == "-nv")
		{
			verbosity_ = Verbosity::kNone;
		}
	}
}

void MainWindow::say(constr& message, const MessageState& msg_state /* = MessageState::kNone */, const Verbosity& verbosity /* = Verbosity::kError */)
{
	if(verbosity > verbosity_)
		return;
	
	Util::Say s(debug_);
	s.termDisplay(message, msg_state);
	
	if(verbosity != Verbosity::kDebug)
		updateStatus(message, msg_state);
}

void MainWindow::removeCurrentTab() //right click close and menu close/ctrl+w
{
	removeFirstSave();
	say("Closing Current Tab: " + updateTabNum(false, tabs_.get_current_page()), MessageState::kInfo, Verbosity::kDebug);
	tabs_.remove_page(tabs_.get_current_page());
}

void MainWindow::removeTab(Gtk::Box& box) //tab button close
{
	cint page_num = tabs_.page_num(box);
	removeFirstSave(page_num);
	say("Closing Tab: " + updateTabNum(false, page_num), MessageState::kInfo, Verbosity::kDebug);
	tabs_.remove_page(box);
}

int MainWindow::nextTabNum()
{
	uint next_num = 1;
	bool found = false;
	
	for(uint i = 1; i < tab_numbers_.size()+2; i++) // +2 b/c we start at 1 and we need n+1 numbers
	{
		for(uint j=0; j < tab_numbers_.size(); j++)
		{
			if(i == tab_numbers_.at(j))
			{
				found = true;
				break;
			}
			else
				found = false;
		}
		
		if(found == false)
		{
			next_num = i;
			break;
		}
	}
	
	return next_num;
}

constr MainWindow::updateTabNum(bool add, cuint num)
{
	str r = "";
	if(add == true)
	{
		tab_numbers_.push_back(num);
		return Util::toString(num);
	}
	else
	{
		r = Util::toString(getTabNum(num));
		tab_numbers_.erase(tab_numbers_.begin()+num);
	}
	
	return r;
}

void MainWindow::setFirstSave(Glib::RefPtr<Gtk::TextBuffer> output_buffer)
{
	TabData sda;
	sda.file = "";
	sda.uri = current_dir_;
	sda.output_buffer = output_buffer;
			
	tabdata_map_.insert(pairis(getTabNum(), sda));
}

void MainWindow::updateSave()
{
	if(save_file_ == "none")
		return;
	getFirstSave();
	
	TabData sd;
	sd.file = save_file_;
	sd.uri = current_dir_;
	sd.output_buffer = current_output_buffer_;
	
	updateMap(getTabNum(), sd);
}

bool MainWindow::getFirstSave()
{
	TabData sd = tabdata_map_.at(getTabNum());
	current_output_buffer_ = sd.output_buffer;
	//std::cout << "sd.file: " << sd.file << std::endl;
	if(sd.file == "")
		return false;
	
	save_file_ = sd.file;
	current_dir_ = sd.uri;
	
	return true;
}

void MainWindow::removeFirstSave()
{
	tabdata_map_.erase(getTabNum());
}

void MainWindow::removeFirstSave(cint page_num)
{
	tabdata_map_.erase(getTabNum(page_num));
}

void MainWindow::save()
{
	if(! getFirstSave())
	{
		saveOutputDialog();
		updateSave();
	}
	else
		saveOutputWrite(save_file_);
}

void MainWindow::saveOutputDialog()
{
	Gtk::FileChooserDialog save("Save Output", Gtk::FILE_CHOOSER_ACTION_SAVE);
	save.set_transient_for(*this);
	
	save.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	save.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	
	getFirstSave();
	if(current_dir_ != "")
		save.set_uri(current_dir_);
	
	bool done = false;
	
	while(! done)
	{
		cint response = save.run();
		save_file_ = save.get_filename();
		current_dir_ = save.get_uri();
		
		switch(response)
		{
		case Gtk::RESPONSE_OK:
			done = saveOutput(save_file_);
			break;
		case Gtk::RESPONSE_CANCEL:
			done = true;
			save_file_ = "none";
			break;
		default:
			std::cerr << "Unknown error in save dialog window response" << std::endl;
			save_file_ = "none";
			done = true;
		}
	}
}

bool MainWindow::saveOutput(constr file)
{
	std::ifstream checkfile(file);
	
	if(checkfile)
	{
		if(! saveFileExistDialog(file))
			return false;
	}
	
	checkfile.close();
	
	saveOutputWrite(file);
	
	return true;
}

void MainWindow::saveOutputWrite(constr file)
{
	cint rtn = Util::saveFile(file, current_output_buffer_->get_text());
	
	if(rtn != 0)
		say(Util::Error::getMessage(Util::Error::ErrorName::NoFileOpen, file), MessageState::kError, Verbosity::kError);
	else
		say("Saved file " + Util::getFilename(save_file_) + " done", MessageState::kInfo, Verbosity::kNone);
}

bool MainWindow::saveFileExistDialog(constr file)
{
	constr filename = Util::getFilename(file);
	constr str = "The file named \"" + filename + "\" already exist";
	Gtk::MessageDialog exist(*this, str, false, Gtk::MessageType::MESSAGE_QUESTION, Gtk::ButtonsType::BUTTONS_YES_NO);
	
	exist.set_secondary_text("Do you want to replace it?");
	
	cint response = exist.run();
	
	switch(response)
	{
	case Gtk::RESPONSE_YES:
		return true;
	case Gtk::RESPONSE_NO:
	default:
		return false;
	}
}

inline cint MainWindow::getTabNum() const
{
	return tab_numbers_.at(tabs_.get_current_page());
}

inline cint MainWindow::getTabNum(cint page_num) const
{
	return tab_numbers_.at(page_num);
}

void MainWindow::updateMap(cint num, TabData sd)
{
	tabdata_map_.erase(num);
	tabdata_map_.insert(pairis(num, sd));
}

} //namespace Gui
} //namespace Math
