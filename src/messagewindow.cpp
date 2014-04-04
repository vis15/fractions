/*
 * messagewindow.cpp
 *
 *  Created on: Apr 2, 2014
 *      Author: dis
 */

#include "messagewindow.h"
#include "util.h"

namespace Math
{

namespace Gui
{

constr MessageWindow::kerror_ = "Error";
constr MessageWindow::kinfo_ = "Info";
constr MessageWindow::kdebug_ = "Debug";
constr MessageWindow::knone_ = "None";

MessageWindow::MessageWindow()
{
	this->set_title("Messages");
	this->set_default_size(400, 400);
	
	this->add(main_box_);
	main_box_.pack_start(scroll_win_, true, true);
	scroll_win_.add(text_view_);
	text_view_.set_editable(false);
	txtbuffer_ = Gtk::TextBuffer::create();
	text_view_.set_buffer(txtbuffer_);
	
	comboverbose_.append(knone_);
	comboverbose_.append(kerror_);
	comboverbose_.append(kinfo_);
	comboverbose_.append(kdebug_);
	comboverbose_.set_active(0);
	
	main_box_.pack_start(box_vc_, Gtk::PACK_SHRINK);
	lblverbosity_.set_text("Verbosity: ");
	box_vc_.set_border_width(10);
	box_vc_.pack_start(lblverbosity_, Gtk::PACK_SHRINK);
	box_vc_.pack_start(comboverbose_, Gtk::PACK_SHRINK);
	
	box_vc_.pack_start(box_close_);
	box_close_.set_halign(Gtk::ALIGN_END);
	box_close_.pack_start(*btnclose_, Gtk::PACK_SHRINK);
	btnclose_->set_halign(Gtk::ALIGN_END);
	
	btnclose_->signal_clicked().connect(sigc::mem_fun(*this, &MessageWindow::onClose));
	comboverbose_.signal_changed().connect(sigc::mem_fun(*this, &MessageWindow::onVerboseChange));

	this->show_all_children();
}

MessageWindow::~MessageWindow()
{
	
}

void MessageWindow::onClose()
{
	this->hide();
}

void MessageWindow::updateMessages(messages_t messages)
{
	messages_ = messages;
	displayMessages();
}
void MessageWindow::displayMessages()
{
	Message msg;
	str messages = "Messages: \n";
	Verbosity thisverbose = getVerbosity();
	
	for(uint i = 0; i< messages_.size(); i++)
	{
		msg = messages_.at(i);
		if(msg.verbosity <= thisverbose)
			messages += msg.message + "\n";
	}
	
	txtbuffer_->set_text(messages);
}

Verbosity MessageWindow::getVerbosity()
{
	constr id = comboverbose_.get_active_text();
	if(id == knone_)
		return Verbosity::kNone;
	else if(id == kerror_)
		return Verbosity::kError;
	else if(id == kinfo_)
		return Verbosity::kInfo;
	else if(id == kdebug_)
		return Verbosity::kDebug;
	else
		return Verbosity::kError;
}

void MessageWindow::onVerboseChange()
{
	displayMessages();
}

} //namespace Gui

} /* namespace Math */
