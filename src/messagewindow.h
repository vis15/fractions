/*
 * messagewindow.h
 *
 *  Created on: Apr 2, 2014
 *      Author: dis
 */

#ifndef MESSAGEWINDOW_H_
#define MESSAGEWINDOW_H_

#include <gtkmm.h>

#include "vars.h"

namespace Math
{

namespace Gui
{

class MessageWindow : public Gtk::Window
{
public:
	typedef Glib::RefPtr<Gtk::TextBuffer> txtbuff;
	
public:
	MessageWindow();
	virtual ~MessageWindow();
	void updateMessages(messages_t messages);
	
protected:
	void onClose();
	void onVerboseChange();
	
	Gtk::VBox main_box_;
	Gtk::ScrolledWindow scroll_win_;
	Gtk::TextView text_view_;
	txtbuff txtbuffer_;
	Gtk::HBox box_vc_; //vc = verbose and close
	Gtk::Label lblverbosity_;
	Gtk::ComboBoxText comboverbose_;
	Gtk::HBox box_close_;
	Gtk::Button* btnclose_ = new Gtk::Button(Gtk::Stock::CLOSE);
	
private:
	void displayMessages();
	Verbosity getVerbosity();
	
	static constr knone_;
	static constr kerror_;
	static constr kinfo_;
	static constr kdebug_;
	
	messages_t messages_;
};

} // namespace Gui

} /* namespace Math */

#endif /* MESSAGEWINDOW_H_ */
