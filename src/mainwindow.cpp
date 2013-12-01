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

namespace Math
{

namespace Gui
{
/////////////////////////////////////////////////////////////////////////
//class MainWindow
/////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(int argc, char* argv[])
{
	processArgs(argc, argv);
	
	Gtk::Window::set_title("Fraction Program");
	Gtk::Window::set_default_size(400, 320);
	
	//box_main
	box_main_.set_orientation(Gtk::ORIENTATION_VERTICAL);
	
	//box_split
	box_split_.set_orientation(Gtk::ORIENTATION_VERTICAL);
	//box_split_.set_
	
	//box_exp
	box_exp_.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_exp_.set_halign(Gtk::ALIGN_CENTER);
	box_exp_.set_margin_top(30);
	box_exp_.set_margin_bottom(30);
	box_exp_.set_margin_right(30);
	
	//box_ans
	box_ans_.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_ans_.set_halign(Gtk::ALIGN_CENTER);
	box_ans_.set_margin_bottom(30);
	box_ans_.set_margin_right(30);
	
	//box_btn
	box_btn_.set_layout(Gtk::BUTTONBOX_CENTER);
	box_btn_.set_spacing(20);
	
	//lbl_main
	lbl_main_.set_text("Fraction Program");
	
	//lbl_exp
	lbl_exp_.set_text("Expression: ");
	lbl_exp_.set_margin_right(5);
	lbl_exp_.set_margin_left(30);
	lbl_exp_.set_halign(Gtk::ALIGN_END);
	
	//lbl_ans
	lbl_ans_.set_text("Answer: ");
	lbl_ans_.set_margin_right(5);
	lbl_ans_.set_margin_left(30);
	lbl_ans_.set_halign(Gtk::ALIGN_END);
	
	//lbl_info_.set_text("test");
	
	//Setup MainWindow
	Gtk::Window::add(box_main_);
	
	box_main_.pack_start(lbl_main_);
	box_main_.pack_start(box_split_);
	box_main_.pack_start(box_btn_);
	box_main_.pack_start(statusbar_);
	
	box_split_.pack_start(box_exp_);
	box_split_.pack_start(box_ans_);
	
	box_btn_.pack_start(*btn_clear_);
	box_btn_.pack_start(*btn_ok_);
	
	box_exp_.pack_start(lbl_exp_);
	box_exp_.pack_start(txt_exp_);
	
	box_ans_.pack_start(lbl_ans_);
	box_ans_.pack_start(txt_ans_);
	
	//connect signals to buttons
	btn_clear_->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::btnClear));
	btn_ok_->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::btnOK));
	
	//when enter is pressed click OK button
	txt_exp_.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::btnOK));
	
	//update text 
	txt_exp_.signal_changed().connect(sigc::bind<Gtk::Entry&>(sigc::mem_fun(*this, &MainWindow::setWidthChars), txt_exp_));
	txt_ans_.signal_changed().connect(sigc::bind<Gtk::Entry&>(sigc::mem_fun(*this, &MainWindow::setWidthChars), txt_ans_));
	
	//increases txt box size when window size is increased
	signal_size_allocate().connect(sigc::mem_fun(*this, &MainWindow::boxMainChange));
	
	//setup statusbar
	statusbar_.set_border_width(10);
	
	MainWindow::show_all_children();
}

MainWindow::~MainWindow()
{
	delete btn_ok_;
	delete btn_clear_;
}

void MainWindow::btnOK()
{
	
	Parser::Parser p(txt_exp_.get_text(), debug_);
	p.signal_say.connect(sigc::mem_fun(*this, &MainWindow::say));
	try
	{
		txt_ans_.set_text(p.parse());
	}
	catch(constr& err_msg)
	{
		say(err_msg);
	}
}

void MainWindow::btnClear()
{
	txt_exp_.set_text("");
	txt_ans_.set_text("");
}

void MainWindow::updateStatus(constr& message, const MessageState& msg_state)
{	
	Gdk::RGBA color;
	
	switch(msg_state)
	{
	case MessageState::kNone:
	case MessageState::kInfo:
		color.set("#703D8F"); //purple
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
	for(int i=0; i<argc; i++)
	{
		constr var = argv[i];
		if(var == "--debug")
			debug_ = true;
	}
}

void MainWindow::setWidthChars(Gtk::Entry& entry)
{
	if(entry.get_text_length() < kchar_limit_)
	{
		entry.set_width_chars(kchar_limit_);
		return;
	}
	
	//Todo: bug: plus char doesn't work well
	entry.set_width_chars(entry.get_text_length());
	
	update_text_ = true;
}

inline cint MainWindow::getSize(int diff) const
{
	if(diff == 1)
		return 1;
	else if(diff < 6)
		return 2;
	else if(diff < 11)
		return 3;
	else if(diff < 21)
		return 10;
	else if(diff < 31)
		return 25;
	else if(diff < 41)
		return 35;
	else if(diff < 51)
		return 40;
	else
		return 60;
	
}

inline cint MainWindow::txt_length_to_pix(cint txt_length) const
{
	return txt_length * 12;
}

void MainWindow::boxMainChange(Gtk::Allocation& a)
{
//	if(txt_length_to_pix(txt_exp_.get_text_length()) > txt_exp_.get_width())
//		return;
	
	if(update_text_ == true)
	{
		update_text_ = false;
		return;
	}
	
	if(win_width_ < get_width())
	{
		//todo: with a loop?
		txt_exp_.set_size_request(txt_exp_.get_width() + getSize(a.get_width() - win_width_), -1);
		txt_ans_.set_size_request(txt_ans_.get_width() + getSize(a.get_width() - win_width_), -1);
		win_width_ = a.get_width();
	}
	else if (win_width_ > get_width())
	{
		txt_exp_.set_size_request(txt_exp_.get_width() - getSize(win_width_ - a.get_width()), -1);
		txt_ans_.set_size_request(txt_ans_.get_width() - getSize(win_width_ - a.get_width()), -1);
		win_width_ = a.get_width();
	}

}

void MainWindow::say(constr& message, const MessageState& msg_state)
{
	Util::Say s(debug_);
	s.termDisplay(message, msg_state);
	updateStatus(message, msg_state);
}

} //namespace Gui
} //namespace Math
