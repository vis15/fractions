/*
 * mainwindow.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: Timothy O'Connor
 */

#include <iostream>
#include <fstream>
#include <algorithm> //std::remove

#include "mainwindow.h"
#include "Calculate.h"
#include "util.h"
#include "menu.h"
#include "window_icon.h"
#include "css.h"
#include "version.h"

namespace Math
{

namespace Gui
{
int showWindow(int argc, char** argv, int);
/////////////////////////////////////////////////////////////////////////
//class MainWindow
/////////////////////////////////////////////////////////////////////////
constr MainWindow::kexp_marker_ = ">";
constr MainWindow::ktab_label_ = "Math ";
constr MainWindow::kfractions_ = "Fractions: ";
constr MainWindow::kenabled_ = "Enabled";
constr MainWindow::kdisabled_ = "Disabled";
constr MainWindow::kgcd_ = "GCD: ";

MainWindow::MainWindow(cint argc, char** argv) : kargc_(argc), kargv_(argv)
{	
	processArgs(argc, argv);
	
	Gtk::Window::set_title(Info::kProgName());
	window_icon_ = Gdk::Pixbuf::create_from_data(math_icon.pixel_data, Gdk::COLORSPACE_RGB, false, 8, math_icon.width, math_icon.height, math_icon.width*3);
	this->set_default_icon(window_icon_);
	
	config_.init(argv[0], debug_);
	configsettings_ = config_.parseConfig();
	setSettings();
	
	//setup CSS
	auto css = Gtk::CssProvider::create();
	css->load_from_data(getCss());
	auto screen = Gdk::Screen::get_default();
	auto ctx = this->get_style_context();
	ctx->add_provider_for_screen(screen, css, GTK_STYLE_PROVIDER_PRIORITY_USER);
	
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
	Gtk::Separator sep;
	box_main_.add(sep);
	
	statusbar_.set_border_width(4);
	
	Gdk::RGBA color_purple(ColorsHTML::kDPurple());
	fractions_enabled_.override_color(color_purple);
	evtb_fractions_.add(fractions_enabled_);
	evtb_fractions_.signal_button_press_event().connect(sigc::mem_fun(*this, &MainWindow::fractionsEnalbedClick));
	evtb_fractions_.set_tooltip_text("Double click to enable and disable");
	//when mouse enters event box/label
	evtb_fractions_.signal_enter_notify_event().connect(sigc::mem_fun(*this, &MainWindow::fractionsEnabledEventEnter));
	evtb_fractions_.signal_leave_notify_event().connect(sigc::mem_fun(*this, &MainWindow::fractionsEnabledEventLeave));
	status_box_.pack_start(statusbar_);
	
	statusbar_fractions_.set_margin_right(15);
	statusbar_fractions_.add(evtb_fractions_);
	status_box_.pack_start(statusbar_fractions_, Gtk::PACK_SHRINK);
	box_main_.pack_start(status_box_, Gtk::PACK_SHRINK);
	
	this->signal_focus_in_event().connect(sigc::mem_fun(*this, &MainWindow::mainWindowDrag));
	
	MainWindow::show_all_children();
	
	debugwin_ = 0; //when 0 debug window is not active
	
	txt_view_i_->grab_focus();
}

MainWindow::~MainWindow()
{
	//Util::debugSay("closing...");
	shutingdown_ = true;
	saveSettings();
}

void MainWindow::setSettings()
{
	using namespace DefaultConfigVars;
	
	this->set_default_size(configsettings_.winsizew, configsettings_.winsizel);
	
	if(configsettings_.winposx != kwinposx || configsettings_.winposy != kwinposy)
		this->move(configsettings_.winposx, configsettings_.winposy);
	
	dwsettings_ = configsettings_.dwsettings;
}

void MainWindow::saveSettings()
{
	int l, w;
	this->get_size(w, l);
	configsettings_.winsizel = l;
	configsettings_.winsizew = w;
	
	config_.updateConfig(configsettings_);
}

bool MainWindow::mainWindowDrag(GdkEventFocus*)
{
	int x, y;
	get_position(x,y);
	configsettings_.winposx = x;
	configsettings_.winposy = y;
	
	return false;
}

void MainWindow::setupMenu()
{
	menuitem_fractions_ = Gtk::Action::create_with_icon_name("CalcFractions", "system-run", kfractions_ + kenabled_, "When checked fractions will be enabled");
	
	fractions_menu_ = Gtk::Action::create("FractionsMenu", "Fractions");
	menuitem_fractions_gcd_ = Gtk::Action::create("FractionsGCD", kgcd_ + kenabled_);
	
	menuitem_save_ = Gtk::Action::create("FileSave", Gtk::Stock::SAVE, "_Save Output");
	menuitem_save_as_ = Gtk::Action::create("FileSaveAs", Gtk::Stock::SAVE_AS, "Save Output As...");
	menuitem_close_tab_ = Gtk::Action::create("FileCloseTab", Gtk::Stock::CLOSE, "Close Tab");
	menuitem_clear_all_tabs_ = Gtk::Action::create("EditClearAll", Gtk::Stock::CANCEL, "Clear All Tabs");
	menuitem_clear_tab_ = Gtk::Action::create("EditClearHistory", Gtk::Stock::CLEAR, "Clear Tab History");
	menuitem_calculate_ = Gtk::Action::create("CalcCalc", Gtk::Stock::APPLY, "Calculate", "");
	
	actiongrp_ = Gtk::ActionGroup::create();
	actiongrp_->add(Gtk::Action::create("FileMenu", "File"));
	actiongrp_->add(Gtk::Action::create_with_icon_name("FileNewWindow", "window-new", "_New Window", ""), Gtk::AccelKey("<control>N"), sigc::mem_fun(*this, &MainWindow::newMainWindow));
	actiongrp_->add(Gtk::Action::create("FileNewTab", Gtk::Stock::NEW, "New _Tab"), Gtk::AccelKey("<control>T"), sigc::mem_fun(*this, &MainWindow::newTab));
	actiongrp_->add(menuitem_save_, sigc::mem_fun(*this, &MainWindow::save));
	actiongrp_->add(menuitem_save_as_, Gtk::AccelKey("<control><shift>S"), sigc::mem_fun(*this, &MainWindow::saveOutputDialog));
	actiongrp_->add(menuitem_close_tab_, Gtk::AccelKey("<control>W"), sigc::mem_fun(*this, &MainWindow::removeCurrentTab));
	actiongrp_->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &MainWindow::closeWindow));
	actiongrp_->add(Gtk::Action::create("EditMenu", "Edit"));
	actiongrp_->add(menuitem_clear_tab_, Gtk::AccelKey("<control><alt>C"), sigc::mem_fun(*this, &MainWindow::clearTabHistory));
	actiongrp_->add(menuitem_clear_all_tabs_);
	actiongrp_->add(Gtk::Action::create("EditPref", Gtk::Stock::PREFERENCES));
	actiongrp_->add(Gtk::Action::create("ViewMenu", "View"));
	actiongrp_->add(Gtk::Action::create("ViewDebugWin", Gtk::Stock::PROPERTIES, "Debug Window"), Gtk::AccelKey("<control><shift>D"), sigc::mem_fun(*this, &MainWindow::showDebugWin));
	actiongrp_->add(fractions_menu_);
	actiongrp_->add(menuitem_fractions_gcd_, sigc::mem_fun(*this, &MainWindow::gcdToggle));
	actiongrp_->add(Gtk::Action::create("CalcMenu", "Calculate"));
	actiongrp_->add(menuitem_calculate_, Gtk::AccelKey("<release>Enter"), sigc::mem_fun(*this, &MainWindow::onEnter));
	actiongrp_->add(menuitem_fractions_, Gtk::AccelKey("<control>F"), sigc::mem_fun(*this, &MainWindow::fractionsToggle));
	actiongrp_->add(Gtk::Action::create("HelpMenu", "Help"));
	actiongrp_->add(Gtk::Action::create("HelpHelp", Gtk::Stock::HELP));
	actiongrp_->add(Gtk::Action::create("HelpUpdate", "Check for Updates"));
	actiongrp_->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT), sigc::mem_fun(*this, &MainWindow::showAboutWindow));
	
	uimanager_ = Gtk::UIManager::create();
	uimanager_->insert_action_group(actiongrp_);
	
	this->add_accel_group(uimanager_->get_accel_group());
	
	try
	{
		uimanager_->add_ui_from_string(getMainMenu());
	}
	catch(const Glib::Error& error)
	{
		say("Error: Menu: uimanager failed: " + error.what(), MessageState::kError);
		return;
	}
	
	Gtk::Widget* menu = uimanager_->get_widget("/menu-main");
	if(!menu)
		say("Error: Menu: get_widget failed", MessageState::kError);
	else
		box_main_.add(*menu);
}

void MainWindow::newMainWindow()
{
	mainwindow_ = new MainWindow(kargc_, kargv_);
	mainwindow_->show();
//	int argc = kargc_;
//	char** argv = kargv_;
//	showWindow(argc, argv, wincount++);
}

void MainWindow::fractionsToggle()
{	
	TabData td = getCurrentTabData();
	
	setFractionsEnabled(true);
	
	td.fractions_enabled = !(td.fractions_enabled);
	updateMap(getTabNum(), td);
}

void MainWindow::setFractionsEnabled(bool toggle /* = false */)
{
	bool enabled = true;
	
	if(getFractionsEnabled())
		enabled = true;
	else
		enabled = false;
	
	if(toggle)
		enabled = !enabled;
	
	setFractionsMenuEnabled(enabled);
}

void MainWindow::setFractionsMenuEnabled(bool enabled)
{
	str label = kfractions_;
	label += getEnabled(enabled);
	
	menuitem_fractions_->set_label(label);
	fractions_enabled_.set_label(label);
	setFractionsMenu(enabled);
}

bool MainWindow::getFractionsEnabled() const
{
	//Util::debugSay("get fractions en");
	TabData td = getCurrentTabData();
	return td.fractions_enabled;
}

bool MainWindow::fractionsEnalbedClick(GdkEventButton* event)
{
	if(event->type == GDK_2BUTTON_PRESS)
		fractionsToggle();
	
	return false;
}

void MainWindow::setFractionsMenu(bool enabled)
{
	//fractions_menu_->set_sensitive(enabled);
	menuitem_fractions_gcd_->set_sensitive(enabled);
}

void MainWindow::gcdToggle()
{
	gcd_enabled_ = !gcd_enabled_;
	menuitem_fractions_gcd_->set_label(kgcd_ + getEnabled(gcd_enabled_));
}

constr MainWindow::getEnabled(bool enabled)
{
	if(enabled)
		return kenabled_;
	else
		return kdisabled_;
}

bool MainWindow::fractionsEnabledEventEnter(GdkEventCrossing*)
{
	fractions_enabled_.set_use_markup();
	fractions_enabled_.set_label("<u>" + fractions_enabled_.get_label() + "</u>");
	return false;
}

bool MainWindow::fractionsEnabledEventLeave(GdkEventCrossing*)
{
	fractions_enabled_.set_use_markup();
	fractions_enabled_.set_label(kfractions_ + getEnabled(getFractionsEnabled()));
	return false;
}

void MainWindow::clearTabHistory()
{
	TabData td = getCurrentTabData();
	td.output_buffer->set_text("");
	
	TabData ntd;
	td.ans_history = ntd.ans_history;
	td.exp_history = ntd.exp_history;
	
	updateMap(getTabNum(), td);
	initTabData();
}

void MainWindow::newTab()
{	
	creating_tab_ = true;
	tab_count_++;
	cint tab_num = nextTabNum();
	updateTabNum(true, tab_num);
	
	Gtk::Box* lbl_box = Gtk::manage(new Gtk::Box);
	event_box_ = Gtk::manage(new Gtk::EventBox);
	Gtk::Label* lbl = Gtk::manage(new Gtk::Label);
	lbl->set_text(ktab_label_ + Util::toString(tab_num));
	event_box_->add(*lbl);
	lbl_box->pack_start(*event_box_);
	Gtk::Button* button = Gtk::manage(new Gtk::Button());
	Gtk::Image* image = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU));
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
	auto mark_buffer = txt_view_mark_->get_buffer();
	mark_buffer->set_text(kexp_marker_);
	
	box_mark->pack_start(*txt_view_mark_, Gtk::PACK_SHRINK);
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
	Pango::FontDescription font("monospace 10");
	txt_view_->override_font(font);
	
	//Input box
	txt_view_i_ = Gtk::manage(new Gtk::TextView);
	txt_view_i_->set_border_width(2);
	txt_view_i_->set_margin_right(1);
	txt_view_i_->set_pixels_above_lines(2);
	
	buffer_input_ = txt_view_i_->get_buffer();
	txt_view_i_->set_buffer(buffer_input_);
	scroll_win_i_->add(*txt_view_i_);
	
	//keeps the input box from going grey.
	txt_view_i_->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::inputEvent), txt_view_i_));
	
	//connects buffer_input_ with buffer_output_ and checks for return/enter then "deletes" return/enter, return/enter does not need to be displayed
	txt_view_i_->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::inputKeyPress), false);
	//same as above but does not delete enter/return but calls function to calculate
	txt_view_i_->signal_key_release_event().connect(sigc::bind<txtbuff&, txtbuff&>(sigc::mem_fun(*this, &MainWindow::keyPress), buffer_input_, buffer_output_), false);
	
	//auto-scroll
	scroll_win_->get_vadjustment()->signal_changed().connect(sigc::bind<Gtk::ScrolledWindow&>(sigc::mem_fun(*this, &MainWindow::buffOutChanged), *scroll_win_));
	
	//button for the close tab
	button->signal_clicked().connect(sigc::bind<Gtk::Box&>(sigc::mem_fun(*this, &MainWindow::removeTab), *box_tabs_));

	event_box_->signal_button_press_event().connect(sigc::bind<Gtk::Box&>(sigc::mem_fun(*this, &MainWindow::tabRightClick), *box_tabs_), false);

	tabs_.signal_page_reordered().connect(sigc::mem_fun(*this, &MainWindow::tabReorder));
	tabs_.signal_switch_page().connect(sigc::mem_fun(*this, &MainWindow::tabSwitch));
	//tabs_.signal_event().connect(sigc::mem_fun(*this, &MainWindow::tabRemoved));
	
	//sets input box to have focus
	event_box_->signal_button_release_event().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::txtInputFocus), txt_view_i_));
	event_box_->signal_event_after().connect(sigc::bind(sigc::mem_fun(*this, &MainWindow::dragInputFocus), txt_view_i_));
	
	lbl_box->show_all();
	tabs_.show_all();
	
	say("Created New Tab: " + Util::toString(tab_num), MessageState::kInfo, Verbosity::kDebug);
	
	tabs_.set_current_page(tabs_.page_num(*box_tabs_));
	
	txt_view_i_->grab_focus();
	
	setFractionsMenuEnabled(true);
	
	//setup saves
	setFirstSave(buffer_output_, buffer_input_);
	
	updateStatus("");
	initTabData();
	creating_tab_ = false;
}

bool MainWindow::tabRemoved(GdkEvent* event)
{
	if(event->type == 3)
		return false;
	
	//Util::debugSay("tabRemvoed" + Util::toString(event->type));
	return false;
}

void MainWindow::dragInputFocus(GdkEvent* event, Gtk::TextView* txt_view_i)
{
	if(event->type == GDK_LEAVE_NOTIFY || event->type == GDK_ENTER_NOTIFY)
		txt_view_i->grab_focus();
}

bool MainWindow::txtInputFocus(GdkEventButton*, Gtk::TextView* txt_view_i)
{
	//Util::debugSay("txtInputFocus");
	txt_view_i->grab_focus();
	return false;
}

void MainWindow::buffOutChanged(Gtk::ScrolledWindow& scroll_win)
{
	auto adj = scroll_win.get_vadjustment();
	adj->set_value(adj->get_upper());
	scroll_win.set_vadjustment(adj);
}

void MainWindow::setupTabMenu()
{
	tab_menu_ = Gtk::manage(new Gtk::Menu);
	Gtk::MenuItem* item = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::SAVE));
	tab_menu_->append(*item);
	item = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::SAVE_AS));
	tab_menu_->append(*item);
	Gtk::SeparatorMenuItem* separator = Gtk::manage(new Gtk::SeparatorMenuItem);
	tab_menu_->append(*separator);
	item = Gtk::manage(new Gtk::ImageMenuItem(Gtk::Stock::CLOSE));
	item->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::removeCurrentTab));
	tab_menu_->append(*item);
	tab_menu_->show_all();
}

bool MainWindow::inputKeyPress(GdkEventKey* eventkey)
{
	if(eventkey->keyval == GDK_KEY_Return || eventkey->keyval == GDK_KEY_KP_Enter)
	{
		//Util::debugSay("input key press");
		return true;
	}
	
	return false;
}

bool MainWindow::keyPress(GdkEventKey* eventkey, txtbuff& buffer_input, txtbuff& buffer_output)
{
	if(eventkey->keyval == GDK_KEY_Return || eventkey->keyval == GDK_KEY_KP_Enter)
	{
		onEnter();
	}
	else if(eventkey->keyval == GDK_KEY_Up && (eventkey->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK) //MOD mask 3 is always on for some reason
	{
		//Util::debugSay("ctrl up");
		onCtrlUp(buffer_input);
	}
	else if(eventkey->keyval == GDK_KEY_Down && (eventkey->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK)
	{
		//Util::debugSay("ctrl down");
		onCtrlDown(buffer_input);
	}
	else if(eventkey->keyval == GDK_KEY_Up)
	{
		//Util::debugSay("up arrow");
		onUpArrow(buffer_input);
	}
	else if(eventkey->keyval == GDK_KEY_Down)
	{
		onDownArrow(buffer_input);
	}
	
	return false;
}

void MainWindow::onEnter()
{
	//todo: this whole function can be a thread
	say("Calculating...", MessageState::kInfo, Verbosity::kNone);
	
	str input_text = buffer_input_->get_text(); 
	//input_text.erase(std::remove(input_text.begin(), input_text.end(), '\n'), input_text.end()); //remove newline
	constr output_text = buffer_output_->get_text();
	
	if(debugwin_ != 0)
		debugwin_->setExpression(input_text);
	
	ClassVars classvars;
	classvars.debug = debug_;
	classvars.debugwin = debugwin_enabled_;
	classvars.exp = input_text;
	classvars.fractions = getFractionsEnabled();
	Math::Calculate exp(classvars);
	exp.signal_say.connect(sigc::mem_fun(*this, &MainWindow::say));
	exp.signal_debugoutput.connect(sigc::mem_fun(*this, &MainWindow::displayDebug));

	try
	{
		constr ans = exp.calculate();
		buffer_output_->set_text(formatOutput(output_text, ans, input_text));
		buffer_input_->set_text("");
		addHistory(input_text, ans);
	}
	catch(constr& err_msg)
	{
		say(err_msg, MessageState::kError, Verbosity::kError);
		return;
	}
	
	say("Done", MessageState::kInfo, Verbosity::kNone);
}

constr MainWindow::formatOutput(constr& current_output, constr& new_output, constr& exp)
{
	str newline = "";
	if(current_output != "")
		newline = "\n";
	
	str output = current_output + newline;
	output += kexp_marker_ + " " + exp + "\n";
	output += kexp_marker_ + kexp_marker_ + " " + new_output + "\n";
	
	return output;
}

void MainWindow::onUpArrow(txtbuff& buffer_input)
{
	arrowPressed(buffer_input, 1, false);
}

void MainWindow::onDownArrow(txtbuff& buffer_input)
{
	arrowPressed(buffer_input, -1, false);
}

void MainWindow::onCtrlUp(txtbuff& buffer_input)
{
	arrowPressed(buffer_input, 1, true);
}

void MainWindow::onCtrlDown(txtbuff& buffer_input)
{
	arrowPressed(buffer_input, -1, true);
}

void MainWindow::arrowPressed(txtbuff& buffer_input, cint num, bool exp) //true for exp; false for ans
{
	if(history_position_ == 0)
		addCurrentHistory(buffer_input->get_text());
	
	increaseHistoryPosition(num);
	setHistory(buffer_input, exp);
}

void MainWindow::increaseHistoryPosition(cint num)
{
	TabData td = getCurrentTabData();
	cint history_position = history_position_;
	cint td_history_size = td.exp_history.size();

	if(history_position+num > td_history_size-1) // -1 because first position is current data
		history_position_ = td.exp_history.size()-1; // -1 because vector starts at 0
	else if(history_position+num < 0)
		history_position_ = 0;
	else
		history_position_ += num;
}

void MainWindow::addHistory(constr& exp, constr& ans)
{
	TabData td = getCurrentTabData();
	td.exp_history.push_back(exp);
	td.ans_history.push_back(ans);
	updateMap(getTabNum(), td);
	//Util::debugSay("add his");
}

constr MainWindow::getHistory(bool exp, uint position) const
{
	//Util::debugSay("get his");
	TabData td = getCurrentTabData();
	
	if(position > td.exp_history.size()-1)
		position = td.exp_history.size()-1;
	
	uint np = td.exp_history.size() - position;
	if(np == td.exp_history.size()) //current buffer data is at position 0
		np = 0;
	
	if(exp || np == 0)
		return td.exp_history.at(np);
	else
		return td.ans_history.at(np);
}

void MainWindow::setHistory(txtbuff& buffer_input, bool exp)
{
	buffer_input->set_text(getHistory(exp, history_position_));
}

void MainWindow::initTabData()
{
	addHistory("", "");
}

void MainWindow::addCurrentHistory(constr& text)
{
	TabData td = getCurrentTabData();
	td.exp_history.erase(td.exp_history.begin());
	td.exp_history.insert(td.exp_history.begin(), text);
	updateMap(getTabNum(), td);
}

void MainWindow::tabReorder(Widget*, guint page_num)
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
	
	//Util::debugSay("tabReorder");
}

void MainWindow::tabSwitch(Widget*,guint page_num)
{
	if(shutingdown_) //weird bug in gtk+: when the program is closing it switches tabs, only sometimes though
		return;
	
	tab_switch_call_++;
	if(tab_switch_call_ == tab_count_) //same as tabReorder
		tab_switch_call_ = 0;
	
	if(tab_switch_call_ != 1)
		return;
	
	current_tab_ = page_num;
	
	if(creating_tab_ == true)
		return;
	
	buffer_input_ = getCurrentTabData().input_buffer;
	buffer_output_ = getCurrentTabData().output_buffer;
	
	//Util::debugSay("tab sw");
	changeStatus(getStatus());
	setFractionsEnabled();
}

bool MainWindow::tabRightClick(GdkEventButton* event, const Gtk::Box& close_tab)
{
	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) ) //right click
	{
		const int a = tabs_.page_num(close_tab);
		tabs_.set_current_page(a);
		tab_menu_->popup(event->button, event->time);
	}
	
	return false;
}

bool MainWindow::inputEvent(GdkEvent*, Gtk::TextView* txt_view)
{
	txt_view->set_state_flags(Gtk::STATE_FLAG_FOCUSED);
		
	return false;
}

void MainWindow::updateStatus(constr& message, const MessageState& msg_state /* = MessageState::kNone */)
{
	changeStatus(message, msg_state);
	//Util::debugSay("update status");
	
	TabData td = getCurrentTabData();
	td.status_msg = message;
	updateMap(getTabNum(), td);
}

constr MainWindow::getStatus() const
{
	//Util::debugSay("get status");
	TabData td = getCurrentTabData();
	return td.status_msg;
}

inline const MainWindow::TabData MainWindow::getCurrentTabData() const
{
	return tabdata_map_.at(getTabNum());
}

void MainWindow::closeWindow()
{
	this->hide();
}

void MainWindow::changeStatus(constr& message, const MessageState& msg_state /* = MessageState::kNone */)
{	
	//Util::debugSay("change status: " + message);
	Gdk::RGBA color;
	
	switch(msg_state)
	{
	case MessageState::kNone:
	case MessageState::kInfo:
		color.set(ColorsHTML::kPurple()); //purple
		break;
	case MessageState::kWarning:
		color.set(ColorsHTML::kBlue());
		break;
	case MessageState::kError:
		color.set("red");
		break;
	}
	
	statusbar_.override_color(color); //if not set each time color will stay the same
	statusbar_.push(message);
}

void MainWindow::processArgs(cint argc, const char* const* argv)
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
	
	//Util::debugSay("debug: " + Util::toString(debug_));
}

void MainWindow::say(constr& message, const MessageState& msg_state /* = MessageState::kNone */, const Verbosity& verbosity /* = Verbosity::kError */)
{
	if(verbosity > verbosity_)
		return;
	
	Util::Say s(debug_);
	s.display(message, msg_state);
	
	if(verbosity != Verbosity::kDebug)
		updateStatus(message, msg_state);
}

void MainWindow::removeCurrentTab() //right click close and menu close/ctrl+w
{
	removeTabData();
	say("Closing Current Tab: " + updateTabNum(false, tabs_.get_current_page()), MessageState::kInfo, Verbosity::kDebug);
	tabs_.remove_page(tabs_.get_current_page());
}

void MainWindow::removeTab(Gtk::Box& box) //tab button close
{
	cint page_num = tabs_.page_num(box);
	removeTabData(page_num);
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
	if(add)
	{
		tab_numbers_.push_back(num);
	}
	else
	{
		r = Util::toString(getTabNum(num));
		tab_numbers_.erase(tab_numbers_.begin()+num);
		return r;
	}
	
	if(tab_numbers_.size() == 1)
		enableTabItems(true);
	
	return Util::toString(num);
}

void MainWindow::setFirstSave(const txtbuff& output_buffer, const txtbuff& input_buffer)
{
	TabData sda;
	sda.file = "";
	sda.uri = current_dir_;
	sda.output_buffer = output_buffer;
	sda.input_buffer = input_buffer;
			
	updateMap(getTabNum(), sda);
}

void MainWindow::updateSave()
{
	if(save_file_ == "none")
		return;
	
	getFirstSave();
	
	TabData sd = getCurrentTabData();
	sd.file = save_file_;
	sd.uri = current_dir_;
	
	updateMap(getTabNum(), sd);
}

bool MainWindow::getFirstSave()
{
	TabData sd = getCurrentTabData();
	
	if(sd.file == "")
		return false;
	
	save_file_ = sd.file;
	current_dir_ = sd.uri;
	
	//Util::debugSay("get first save");
	return true;
}

void MainWindow::removeTabData(cint page_num /* = kdefault_value_ */)
{
	int page = 0;
	
	if(page_num == kdefault_value_)
		page = getTabNum();
	else
		page = getTabNum(page_num);
	
	tabdata_map_.erase(page);
	
	if(tabdata_map_.empty())
		enableTabItems(false);
}

void MainWindow::enableTabItems(const bool enable)
{
	fractions_enabled_.set_visible(enable);
	menuitem_save_->set_sensitive(enable);
	menuitem_save_as_->set_sensitive(enable);
	menuitem_close_tab_->set_sensitive(enable);
	menuitem_clear_all_tabs_->set_sensitive(enable);
	menuitem_clear_tab_->set_sensitive(enable);
	menuitem_calculate_->set_sensitive(enable);
	menuitem_fractions_->set_sensitive(enable);
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

bool MainWindow::saveOutput(constr& file)
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
	cint rtn = Util::saveFile(file, buffer_output_->get_text());
	
	if(rtn != 0)
		say(Util::Error::getMessage(Util::Error::ErrorName::NoFileOpen, file, __func__), MessageState::kError, Verbosity::kError);
	else
		say("Saved file " + Util::getFilenameFromPath(save_file_) + " done", MessageState::kInfo, Verbosity::kNone);
}

bool MainWindow::saveFileExistDialog(constr& file)
{
	constr filename = Util::getFilenameFromPath(file);
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

inline cint MainWindow::getTabNum(cint page_num /* = kdefault_value_ */) const
{
	int page = 0;
	
	if(page_num == kdefault_value_)
		page = tabs_.get_current_page();
	else
		page = page_num;
	
	return tab_numbers_.at(page);
}

void MainWindow::updateMap(cint num, const TabData& sd)
{
	tabdata_map_.erase(num);
	tabdata_map_.insert(pairis(num, sd));
	//Util::debugSay("update map: message: " + sd.status_msg);
}

void MainWindow::showAboutWindow()
{
	about_window_.set_program_name(Info::kProgName());
	about_window_.set_version(getVersion());
	about_window_.set_copyright("Timothy O'Connor");
	about_window_.set_comments("A calculator that incorporates fractions");
	about_window_.set_license_type(Gtk::LICENSE_GPL_3_0);
	about_window_.set_website("https://sourceforge.net/projects/math-fractions/");
	about_window_.set_website_label("Fraction's Website");
	std::vector<Glib::ustring> authors = {"Timothy O'Connor"};
	about_window_.set_authors(authors);
	about_window_.set_logo_default();
	about_window_.signal_response().connect(sigc::mem_fun(*this, &MainWindow::onAboutResponse));
	
	about_window_.show();
}

constr MainWindow::getVersion()
{
	return Version::kMajor() + "." + Version::kMinor() + "." + Version::kRevision() + " build: " + Version::kBuild() + "\nBuild Release: " + Version::kGetBuildType();
}

void MainWindow::onAboutResponse(int response)
{
	//Util::debugSay("about response: " + Util::toString(response));
	if(response == Gtk::RESPONSE_CANCEL || response == Gtk::RESPONSE_CLOSE)
		about_window_.hide();
}

void MainWindow::showDebugWin()
{
	if(debugwin_ != 0)
		return;
	
	debugwin_ = new DebugWindow;
	debugwin_enabled_ = true;
	debugwin_->signal_hide().connect(sigc::mem_fun(*this, &MainWindow::debugWindowHide));
	debugwin_->setSettigns(dwsettings_);
	WinPos ws = getDebugWinPos();
	debugwin_->move(ws.x, ws.y);
	debugwin_->show();
}

void MainWindow::debugWindowHide()
{
	dwsettings_ = debugwin_->getDWSettings();
	configsettings_.dwsettings = dwsettings_;
	debugwin_ = 0;
	delete debugwin_;
	debugwin_enabled_ = false;
}

MainWindow::WinPos MainWindow::getDebugWinPos()
{
	int posx = 0;
	int posy = 0;
	int width = 0;
	int height = 0;
	
	this->get_position(posx, posy);
	this->get_size(width, height);
	
	WinPos ws;
	ws.x = posx + width + kwinbuffer_;
	ws.y = posy;
	
	return ws;
}

void MainWindow::displayDebug(constr& txt, FunctionDebug function, bool procedure)
{
	if(debugwin_ == 0)
		return;
	
	debugwin_->addFunctionOutput(txt, function, procedure);
}

int showWindow(int argc, char** argv, cint wincount)
{
	int fargc = 1; //fake argc //Gtk::Application will error if extra command line args
	//Gtk::Application::create MUST be done with argc and argv, or application will crash
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(fargc, argv, "Math.Calculator.Fractions" + Util::toString(wincount), Gio::APPLICATION_NON_UNIQUE);
	Util::debugSay("wincount: " + Util::toString(wincount));
	Math::Gui::MainWindow mw(argc, argv);
	
	return app->run(mw);
}

} // namespace Gui

} // namespace Math
