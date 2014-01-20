/*
 * debugwindow.cpp
 *
 *  Created on: Jan 11, 2014
 *      Author: dis
 */

#include "debugwindow.h"
#include "css.h"

namespace Math
{

namespace Gui
{

constr DebugWindow::kAddMul_ = "Add Multiply";
constr DebugWindow::kParRep_ = "Parenthesis Replace";
constr DebugWindow::kSubToAdd_ = "Subtraction to Addition";
constr DebugWindow::kTokenize_ = "Tokenize";
constr DebugWindow::kRPN_ = "RPN";
constr DebugWindow::kCalc_ = "Calculation";
constr DebugWindow::kProcedure_ = "Procedure";

DebugWindow::DebugWindow()
{
	this->set_title("Debug Window");
	this->set_default_size(690, 550);
	
	//setup CSS
	auto css = Gtk::CssProvider::create();
	css->load_from_data(getCss());
	auto screen = Gdk::Screen::get_default();
	auto ctx = this->get_style_context();
	ctx->add_provider_for_screen(screen, css, GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	this->add(boxmain_);
	btnbox_.set_border_width(3);
	boxmain_.pack_start(hbox_middle_);
	
//	piximgparrep_ = Gdk::Pixbuf::create_from_data(img_parrep22.pixel_data, Gdk::COLORSPACE_RGB, true, 8, img_parrep22.width, img_parrep22.height, img_parrep22.width*4);
//	imgparrep_.set(piximgparrep_);
//	btnparrep_.set_image(imgparrep_);
	btnparrep_.set_label("ParRep");
	btnparrep_.set_tooltip_text(kParRep_);
	btnparrep_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnParRepClicked));
	
	btnaddmul_.set_label("AddMul");
	btnaddmul_.set_tooltip_text(kAddMul_);
	btnaddmul_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnAddMulClicked));
	
	btnsubtoadd_.set_label("SubToAdd");
	btnsubtoadd_.set_tooltip_text(kSubToAdd_);
	btnsubtoadd_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnSubToAddClicked));
	
	btntokenize_.set_label(kTokenize_);
	btntokenize_.set_tooltip_text(kTokenize_);
	btntokenize_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnTokenizeClicked));
	
	btnrpn_.set_label(kRPN_);
	btnrpn_.set_tooltip_text(kRPN_);
	btnrpn_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnRPNClicked));
	
	btncalc_.set_label("Calc");
	btncalc_.set_tooltip_text(kCalc_);
	btncalc_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnCalcClicked));
	
	btnclear_.set_label("Clear");
	
	chkparrep_.set_label(kProcedure_);
	chkparrep_.set_sensitive(false);
	chkaddmul_.set_label(kProcedure_);
	chkaddmul_.set_sensitive(false);
	chksubtoadd_.set_label(kProcedure_);
	chksubtoadd_.set_sensitive(false);
	chktokenize_.set_label(kProcedure_);
	chktokenize_.set_sensitive(false);
	chkrpn_.set_label(kProcedure_);
	chkrpn_.set_sensitive(false);
	chkcalc_.set_label(kProcedure_);
	chkcalc_.set_sensitive(false);
	
	btnbox_.set_orientation(Gtk::ORIENTATION_VERTICAL);
	btnbox_.pack_start(vbox_parrep_, Gtk::PACK_SHRINK);
	vbox_parrep_.pack_start(btnparrep_);
	vbox_parrep_.pack_start(chkparrep_, Gtk::PACK_SHRINK);
	btnbox_.pack_start(vbox_addmul_, Gtk::PACK_SHRINK);
	vbox_addmul_.pack_start(btnaddmul_);
	vbox_addmul_.pack_start(chkaddmul_);
	btnbox_.pack_start(vbox_subtoadd_, Gtk::PACK_SHRINK);
	vbox_subtoadd_.pack_start(btnsubtoadd_);
	vbox_subtoadd_.pack_start(chksubtoadd_);
	btnbox_.pack_start(vbox_tokenize_, Gtk::PACK_SHRINK);
	vbox_tokenize_.pack_start(btntokenize_);
	vbox_tokenize_.pack_start(chktokenize_);
	btnbox_.pack_start(vbox_rpn_, Gtk::PACK_SHRINK);
	vbox_rpn_.pack_start(btnrpn_);
	vbox_rpn_.pack_start(chkrpn_);
	btnbox_.pack_start(vbox_calc_, Gtk::PACK_SHRINK);
	vbox_calc_.pack_start(btncalc_);
	vbox_calc_.pack_start(chkcalc_);
	btnbox_.pack_start(btnclear_, Gtk::PACK_SHRINK);
	btnclear_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::onClear));
	
	hbox_middle_.pack_start(btnbox_, Gtk::PACK_SHRINK);
	
	buffoutput_ = txtoutput_.get_buffer();
	txtoutput_.set_border_width(4);
	hbox_middle_.pack_start(scrollwin_);
	scrollwin_.add(txtoutput_);
	boxmain_.pack_start(statusbar_, Gtk::PACK_SHRINK);
	
	this->show_all_children();
}

DebugWindow::~DebugWindow()
{
	
}

void DebugWindow::onClear()
{
	buffoutput_->set_text("");
}

void DebugWindow::setOutputText(constr& text)
{
	buffoutput_->set_text(text);
}

void DebugWindow::appendOutputText(constr& text)
{
	buffoutput_->set_text(buffoutput_->get_text() + text + "\n");
}

void DebugWindow::addFunctionOutput(constr& text, FunctionDebug function)
{	
	bool check = false;
	
	switch(function)
	{
	case FunctionDebug::kParRep:
		check = checkParRep();
		break;
	case FunctionDebug::kAddMul:
		check = checkAddMul();
		break;
	case FunctionDebug::kSubToAdd:
		check = checkSubToAdd();
		break;
	case FunctionDebug::kTokenize:
		check = checkTokenizebtn();
		break;
	case FunctionDebug::kRPN:
		check = checkRPNbtn();
		break;
	case FunctionDebug::kCalc:
		check = checkCalc();
		break;
	}
	
	if(check)
	{
		appendOutputText(getFunctionBeginText(function));
		appendOutputText(text + "\n");
	}
}

bool DebugWindow::checkRPNbtn()
{
	if(btnrpn_.get_active())
		return true;
	else
		return false;
}

bool DebugWindow::checkTokenizebtn()
{
	if(btntokenize_.get_active())
		return true;
	else
		return false;
}

bool DebugWindow::checkParRep()
{
	if(btnparrep_.get_active())
		return true;
	else
		return false;
}

bool DebugWindow::checkSubToAdd()
{
	if(btnsubtoadd_.get_active())
		return true;
	else
		return false;
}

bool DebugWindow::checkAddMul()
{
	if(btnaddmul_.get_active())
		return true;
	else
		return false;
}

bool DebugWindow::checkCalc()
{
	if(btncalc_.get_active())
		return true;
	else
		return false;
}

constr DebugWindow::getFunctionBeginText(FunctionDebug function)
{
	constr sd = "=========";
	str output = sd;
	
	switch(function)
	{
	case FunctionDebug::kParRep:
		output += kParRep_;
		break;
	case FunctionDebug::kAddMul:
		output += kAddMul_;
		break;
	case FunctionDebug::kSubToAdd:
		output += kSubToAdd_;
		break;
	case FunctionDebug::kTokenize:
		output += kTokenize_;
		break;
	case FunctionDebug::kRPN:
		output += kRPN_;
		break;
	case FunctionDebug::kCalc:
		output += kCalc_;
		break;
	}
	
	output += " Output" + sd;
	
	return output;
}

void DebugWindow::setExpression(constr& expression)
{
	appendOutputText("Parsing Expression: " + expression);
}

void DebugWindow::btnParRepClicked()
{
	if(checkParRep())
		chkparrep_.set_sensitive(true);
	else
		chkparrep_.set_sensitive(false);
}

void DebugWindow::btnAddMulClicked()
{
	if(checkAddMul())
		chkaddmul_.set_sensitive(true);
	else
		chkaddmul_.set_sensitive(false);
}

void DebugWindow::btnSubToAddClicked()
{
	if(checkSubToAdd())
		chksubtoadd_.set_sensitive(true);
	else
		chksubtoadd_.set_sensitive(false);
}

void DebugWindow::btnTokenizeClicked()
{
	if(checkTokenizebtn())
		chktokenize_.set_sensitive(true);
	else
		chktokenize_.set_sensitive(false);
}

void DebugWindow::btnRPNClicked()
{
	if(checkRPNbtn())
		chkrpn_.set_sensitive(true);
	else
		chkrpn_.set_sensitive(false);
}

void DebugWindow::btnCalcClicked()
{
	if(checkCalc())
		chkcalc_.set_sensitive(true);
	else
		chkcalc_.set_sensitive(false);
}

} // namespace Gui

} // namespace Math


