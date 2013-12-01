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
public:
	MainWindow(int argc, char* argv[]);
	virtual ~MainWindow();
	void updateStatus(constr& message, const MessageState& msg_state = MessageState::kNone);
	void say(constr& message, const MessageState& msg_state = MessageState::kNone);
	
protected:
	void btnOK();
	void btnClear();
	void boxMainChange(Gtk::Allocation&);
	void setWidthChars(Gtk::Entry& entry);
	
	Gtk::Box box_main_, box_split_, box_ans_, box_exp_;
	Gtk::ButtonBox box_btn_;
	Gtk::Button* btn_ok_ = new Gtk::Button(Gtk::Stock::OK);
	Gtk::Button* btn_clear_ = new Gtk::Button(Gtk::Stock::CLEAR);
	Gtk::Label lbl_main_, lbl_exp_, lbl_ans_;
	Gtk::Entry txt_exp_, txt_ans_;
	Gtk::Statusbar statusbar_;
	
private:
	void processArgs(int argc, char* argv[]);
	inline cint getSize(int diff) const;
	inline cint txt_length_to_pix(cint txt_length) const;
	
	static constexpr cint kchar_limit_ = 19;
	static constexpr cint kavg_char_size = 2;
	bool debug_ = false;
	static constexpr cint kchars_width_ = 4;
	bool update_text_ = false;
	
	int win_width_;
};

} //namespace Gui
} //namespace Math

#endif /* MAINWINDOW_H_ */
