/*
 * prefswindow.h
 *
 *  Created on: Jan 30, 2014
 *      Author: dis
 */

#ifndef PREFSWINDOW_H_
#define PREFSWINDOW_H_

#include <gtkmm.h>
#include <sigc++-2.0/sigc++/sigc++.h>

#include "vars.h"
#include "config.h"

namespace Math
{

namespace Gui
{

class PrefsWindow : public Gtk::Window
{
public:
	PrefsWindow();
	~PrefsWindow();
	void setSettings(PrefsWindowSettings prefswinsettings);
	
	sigc::signal<void, PrefsWindowSettings> signal_apply;
	
protected:
	Gtk::VBox box_main_;
	Gtk::CheckButton chkfractions_;
	Gtk::Button* btnok_ = new Gtk::Button(Gtk::Stock::OK);
	Gtk::Button* btncancel_ = new Gtk::Button(Gtk::Stock::CANCEL);
	Gtk::Button* btnapply_ = new Gtk::Button(Gtk::Stock::APPLY);
	Gtk::ButtonBox btnbox_;
	
	//fractions
	Gtk::Frame frmfractions_;
	Gtk::Label lblfractions_;
	Gtk::VBox box_fractions_;
	
	//log
	Gtk::Frame frmlog_;
	Gtk::VBox box_log_;
	Gtk::Label lbllog_;
	Gtk::CheckButton chklog_;
	
	//color
	Gtk::Frame frmcolor_;
	Gtk::Label lblcolor_;
	Gtk::CheckButton chkcolor_;
	Gtk::VBox box_color_;
	
private:
	void applySettings();
	void buttonOK();
	void buttonCancel();
	
	PrefsWindowSettings prefswinsettings_;
	Config config_;
};

} // namespace Gui

} // namespace Math

#endif /* PREFSWINDOW_H_ */
