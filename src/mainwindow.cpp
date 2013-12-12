/*
 * mainwindow.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#include <iostream>

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
constr MainWindow::kexp_marker_ = "> ";
constr MainWindow::ktab_label_ = "Math ";

MainWindow::MainWindow(int argc, char* argv[])
{
	processArgs(argc, argv);
	
	Gtk::Window::set_title("Fraction Program");
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
	actiongrp_ = Gtk::ActionGroup::create();
	actiongrp_->add(Gtk::Action::create("FileMenu", "File"));
	actiongrp_->add(Gtk::Action::create_with_icon_name("FileNewWindow", "window-new", "_New Window", ""), Gtk::AccelKey("<control>N"));
	actiongrp_->add(Gtk::Action::create("FileNewTab", Gtk::Stock::NEW, "New _Tab"), Gtk::AccelKey("<control>T"), sigc::mem_fun(*this, &MainWindow::newTab));
	actiongrp_->add(Gtk::Action::create("FileSave", Gtk::Stock::SAVE));
	actiongrp_->add(Gtk::Action::create("FileSaveAs", Gtk::Stock::SAVE_AS), Gtk::AccelKey("<control><shift>S"));
	actiongrp_->add(Gtk::Action::create("FileCloseTab", Gtk::Stock::CLOSE, "Close Tab"), Gtk::AccelKey("<control>W"), sigc::mem_fun(*this, &MainWindow::removeCurrentTab));
	actiongrp_->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT));
	actiongrp_->add(Gtk::Action::create("EditMenu", "Edit"));
	actiongrp_->add(Gtk::Action::create("EditClearHistory", Gtk::Stock::CLEAR, "Clear History"));
	actiongrp_->add(Gtk::Action::create("EditClearAll", Gtk::Stock::CLEAR, "Clear All"));
	actiongrp_->add(Gtk::Action::create("EditPref", Gtk::Stock::PREFERENCES));
	actiongrp_->add(Gtk::Action::create("CalcMenu", "Calculate"));
	actiongrp_->add(Gtk::Action::create_with_icon_name("CalcCalc", "system-run", "Calculate", ""));
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
	setupTabMenu(*lbl);
	tabs_.append_page(*box_tabs_, *lbl_box);
	tabs_.set_tab_reorderable(*box_tabs_, true);
	tabs_.set_tab_detachable(*box_tabs_, true);
	box_tabs_->set_orientation(Gtk::ORIENTATION_VERTICAL);
	
	scroll_win_ = Gtk::manage(new Gtk::ScrolledWindow);
	box_tabs_->add(*scroll_win_);
	scroll_win_i_ = Gtk::manage(new Gtk::ScrolledWindow);
	box_tabs_->add(*scroll_win_i_);
	
	//Output box
	txt_view_ = Gtk::manage(new Gtk::TextView);
	txt_view_->set_can_focus(false);
	txt_view_->set_border_width(3);
	txt_view_->set_editable(false);
	txt_view_->set_state_flags(Gtk::STATE_FLAG_FOCUSED);
	buffer_output_ = txt_view_->get_buffer();
	txt_view_->set_buffer(buffer_output_);
	scroll_win_->set_vexpand(true);
	scroll_win_->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	scroll_win_->add(*txt_view_);
	
	//Input box
	txt_view_i_ = Gtk::manage(new Gtk::TextView);
	txt_view_i_->set_border_width(3);
	
	setMarkerTag();
	buffer_input_ = txt_view_i_->get_buffer();
	buffer_input_ = Gtk::TextBuffer::create(tag_table_);
	txt_view_i_->set_buffer(buffer_input_);
	scroll_win_i_->add(*txt_view_i_);
	scroll_win_i_->set_shadow_type(Gtk::SHADOW_OUT);
	
	buffer_it_ = buffer_input_->begin();
	buffer_mark_ = buffer_input_->create_mark(buffer_it_);
	buffer_input_->insert_with_tag(buffer_mark_->get_iter(), kexp_marker_, tag_);
	
	txt_view_i_->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::inputEvent), txt_view_i_));
	
	buffer_input_->signal_insert().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::updateInputBox), buffer_input_, buffer_output_));
	buffer_input_->signal_mark_set().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::setMark), txt_view_i_));
	
	button->signal_clicked().connect(sigc::bind<Gtk::Box&>(sigc::mem_fun(*this, &MainWindow::removeTab), *box_tabs_));

	event_box_->signal_button_press_event().connect(sigc::bind<Glib::RefPtr<Gtk::TextBuffer>, Gtk::Box&>(sigc::mem_fun(*this, &MainWindow::tabRightClick), buffer_output_, *box_tabs_), false);

	tabs_.signal_page_reordered().connect(sigc::mem_fun(*this, &MainWindow::tabReorder));
	tabs_.signal_switch_page().connect(sigc::mem_fun(*this, &MainWindow::tabSwitch));
	
	lbl_box->show_all();
	tabs_.show_all();
	
	say("Created New Tab: " + Util::toString(tab_num), MessageState::kInfo, Verbosity::kDebug);
}

void MainWindow::tabReorder(Widget* widget, guint page_num)
{
	tab_reorder_call++;
	//std::cout << "tab reorder call: " << tab_reorder_call << std::endl;
	if(tab_reorder_call == tabs_.get_n_pages()) //signal calls this function for each page, we only need the first call
		tab_reorder_call = 0;
	
	if(tab_reorder_call != 1)
		return;
	
	cint tab_num = tab_numbers_.at(current_tab_);
//	std::cout << "current tab: " << current_tab_ << std::endl;
//	std::cout << "Math: " << tab_num << std::endl;
	tab_numbers_.erase(tab_numbers_.begin()+current_tab_);
	tab_numbers_.insert(tab_numbers_.begin()+page_num, tab_num);
	current_tab_ = page_num;
	
	str s = "";
	for(uint i = 0; i<tab_numbers_.size(); i++)
	{
		s += " " + Util::toString(tab_numbers_.at(i));
	}
	std::cout << "new order: " << s << std::endl;
}

void MainWindow::tabSwitch(Widget*,guint page_num)
{
	current_tab_ = page_num;
	std::cout << "tab switch current tab: " << page_num << std::endl;
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

void MainWindow::setupTabMenu(Gtk::Label& label)
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

void MainWindow::updateInputBox(const Gtk::TextBuffer::iterator& it, const Glib::ustring& str, int len, Glib::RefPtr<Gtk::TextBuffer> buffer_input, Glib::RefPtr<Gtk::TextBuffer> buffer_output)
{
	if(str != "\n")
		return;
	
	say("Calculating...", MessageState::kInfo, Verbosity::kNone);
	buffer_output->set_text(buffer_input->get_text(true));
	say("Done", MessageState::kInfo, Verbosity::kNone);
	
}

bool MainWindow::inputEvent(GdkEvent* event, Gtk::TextView* txt_view)
{
	txt_view->set_state_flags(Gtk::STATE_FLAG_FOCUSED);
		
	return false;
}

void MainWindow::setMark(const Gtk::TextBuffer::iterator& it, const Glib::RefPtr<Gtk::TextBuffer::Mark>& mark, Gtk::TextView* txt_view)
{
	if(it.get_offset() < 2)
		txt_view->set_editable(false);
	else
		txt_view->set_editable(true);
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
	updateStatus(message, msg_state);
}

void MainWindow::removeCurrentTab() //right click close and menu close/ctrl+w
{
	say("Closing Current Tab: " + updateTabNum(false, tabs_.get_current_page()), MessageState::kInfo, Verbosity::kDebug);
	tabs_.remove_page(tabs_.get_current_page());
}

void MainWindow::removeTab(Gtk::Box& box) //tab button close
{
	say("Closing Tab: " + updateTabNum(false, tabs_.page_num(box)), MessageState::kInfo, Verbosity::kDebug);
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

constr MainWindow::updateTabNum(bool add, uint num)
{
	str r = "";
	if(add == true)
	{
		tab_numbers_.push_back(num);
		return Util::toString(num);
	}
	else
	{
		r = Util::toString(tab_numbers_.at(num));
		tab_numbers_.erase(tab_numbers_.begin()+num);
	}
	
	return r;
}

} //namespace Gui
} //namespace Math
