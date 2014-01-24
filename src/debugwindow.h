/*
 * debugwindow.h
 *
 *  Created on: Jan 11, 2014
 *      Author: dis
 */

#ifndef DEBUGWINDOW_H_
#define DEBUGWINDOW_H_

#include <gtkmm.h>

#include "vars.h"

namespace Math
{

namespace Gui
{

class DebugWindow : public Gtk::Window
{
private:
	typedef Glib::RefPtr<Gtk::TextBuffer> txtbuff;
	
public:
	DebugWindow();
	~DebugWindow();
	void setOutputText(constr& text);
	void appendOutputText(constr& text);
	void addFunctionOutput(constr& text, FunctionDebug function, bool procedure);
	void setExpression(constr& expression);
	DebugWindowSettings getDWSettings();
	void setSettigns(const DebugWindowSettings& dwsettings);
	
protected:
	void onClear();
	constr getFunctionBeginText(constr function);
	void btnParRepClicked();
	void btnAddMulClicked();
	void btnSubToAddClicked();
	void btnTokenizeClicked();
	void btnRPNClicked();
	void btnCalcClicked();
	constr getProcedureBeginText(constr function);
	void chkParRepClicked();
	void chkAddMulClicked();
	void chkSubToAddClicked();
	void chkTokenizeClicked();
	void chkRPNClicked();
	void chkCalcClicked();
	
	Gtk::VBox boxmain_;
	Gtk::ToggleButton btnrpn_;
	Gtk::ToggleButton btntokenize_;
	Gtk::ToggleButton btnaddmul_;
	Gtk::ToggleButton btnparrep_;
	Gtk::ToggleButton btnsubtoadd_;
	Gtk::ToggleButton btncalc_;
	Gtk::Button btnclear_;
	Gtk::TextView txtoutput_;
	txtbuff buffoutput_;
	Gtk::ScrolledWindow scrollwin_;
	Glib::RefPtr<Gdk::Pixbuf> piximgparrep_;
	Gtk::Image imgparrep_;
	Gtk::ButtonBox btnbox_;
	Gtk::Image imgaddmul_;
	Glib::RefPtr<Gdk::Pixbuf> piximgsubtoadd_;
	Gtk::Image imgsubtoadd_;
	Gtk::Statusbar statusbar_;
	Gtk::HBox hbox_middle_;
	Gtk::CheckButton chkparrep_;
	Gtk::VBox vbox_parrep_;
	Gtk::CheckButton chkaddmul_;
	Gtk::VBox vbox_addmul_;
	Gtk::CheckButton chksubtoadd_;
	Gtk::VBox vbox_subtoadd_;
	Gtk::CheckButton chktokenize_;
	Gtk::VBox vbox_tokenize_;
	Gtk::CheckButton chkrpn_;
	Gtk::VBox vbox_rpn_;
	Gtk::CheckButton chkcalc_;
	Gtk::VBox vbox_calc_;
	
private:
	static constr kAddMul_;
	static constr kParRep_;
	static constr kSubToAdd_;
	static constr kTokenize_;
	static constr kRPN_;
	static constr kCalc_;
	static constr kProcedure_;
	
	DebugWindowSettings dwsettings_;
};

} // namespace Gui

} // namedapce Math



#endif /* DEBUGWINDOW_H_ */
