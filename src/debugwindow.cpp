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
	btnrpn_.set_tooltip_text("To Reverse Polish Notation");
	btnrpn_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnRPNClicked));
	
	btncalc_.set_label("Calc");
	btncalc_.set_tooltip_text(kCalc_);
	btncalc_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::btnCalcClicked));
	
	btnclear_.set_label("Clear");
	
	chkparrep_.set_label(kProcedure_);
	chkparrep_.set_sensitive(false);
	chkparrep_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::chkParRepClicked));
	
	chkaddmul_.set_label(kProcedure_);
	chkaddmul_.set_sensitive(false);
	chkaddmul_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::chkAddMulClicked));
	
	chksubtoadd_.set_label(kProcedure_);
	chksubtoadd_.set_sensitive(false);
	chksubtoadd_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::chkSubToAddClicked));
	
	chktokenize_.set_label(kProcedure_);
	chktokenize_.set_sensitive(false);
	chktokenize_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::chkTokenizeClicked));
	
	chkrpn_.set_label(kProcedure_);
	chkrpn_.set_sensitive(false);
	chkrpn_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::chkRPNClicked));
	
	chkcalc_.set_label(kProcedure_);
	chkcalc_.set_sensitive(false);
	chkcalc_.signal_clicked().connect(sigc::mem_fun(*this, &DebugWindow::chkCalcClicked));
	
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

void DebugWindow::addFunctionOutput(constr& text, FunctionDebug function, bool procedure)
{	
	bool check = false;
	str functionstr = "";
	bool chkpro = false; //check procedure
	
	switch(function)
	{
	case FunctionDebug::kParRep:
		check = btnparrep_.get_active();
		functionstr = kParRep_;
		chkpro = chkparrep_.get_active();
		break;
	case FunctionDebug::kAddMul:
		check = btnaddmul_.get_active();
		functionstr = kAddMul_;
		chkpro = chkaddmul_.get_active();
		break;
	case FunctionDebug::kSubToAdd:
		check = btnsubtoadd_.get_active();
		functionstr = kSubToAdd_;
		chkpro = chksubtoadd_.get_active();
		break;
	case FunctionDebug::kTokenize:
		check = btntokenize_.get_active();
		functionstr = kTokenize_;
		chkpro = chktokenize_.get_active();
		break;
	case FunctionDebug::kRPN:
		check = btnrpn_.get_active();
		functionstr = kRPN_;
		chkpro = chkrpn_.get_active();
		break;
	case FunctionDebug::kCalc:
		check = btncalc_.get_active();
		functionstr = kCalc_;
		chkpro = chkcalc_.get_active();
		break;
	}
	
	if(chkpro && procedure && check)
	{
		appendOutputText(getProcedureBeginText(functionstr));
		appendOutputText(text);
	}
	
	if(check && ! procedure)
	{
		appendOutputText(getFunctionBeginText(functionstr));
		appendOutputText(text + "\n");
	}
}

constr DebugWindow::getFunctionBeginText(constr function)
{
	constr sd = "=========";
	str output = sd;
	
	output += function + " Output" + sd;
	
	return output;
}

constr DebugWindow::getProcedureBeginText(constr function)
{
	constr sd = "---------------------";
	str output = sd;
	
	output += function + " Procedure" + sd;
	
	return output;
}

void DebugWindow::setExpression(constr& expression)
{
	appendOutputText("Parsing Expression: " + expression + "\n");
}

void DebugWindow::btnParRepClicked()
{
	chkparrep_.set_sensitive(btnparrep_.get_active());
	dwsettings_.parrep = btnparrep_.get_active();
}

void DebugWindow::btnAddMulClicked()
{
	chkaddmul_.set_sensitive(btnaddmul_.get_active());
	dwsettings_.addmul = btnaddmul_.get_active();
}

void DebugWindow::btnSubToAddClicked()
{
	chksubtoadd_.set_sensitive(btnsubtoadd_.get_active());
	dwsettings_.subtoadd = btnsubtoadd_.get_active();
}

void DebugWindow::btnTokenizeClicked()
{
	chktokenize_.set_sensitive(btntokenize_.get_active());
	dwsettings_.tokenize = btntokenize_.get_active();
}

void DebugWindow::btnRPNClicked()
{
	chkrpn_.set_sensitive(btnrpn_.get_active());
	dwsettings_.rpn = btnrpn_.get_active();
}

void DebugWindow::btnCalcClicked()
{
	chkcalc_.set_sensitive(btncalc_.get_active());
	dwsettings_.calc = btncalc_.get_active();
}

void DebugWindow::chkParRepClicked()
{
	dwsettings_.parrep_p = chkparrep_.get_active();
}

void DebugWindow::chkAddMulClicked()
{
	dwsettings_.addmul_p = chkaddmul_.get_active();
}

void DebugWindow::chkSubToAddClicked()
{
	dwsettings_.subtoadd_p = chksubtoadd_.get_active();
}

void DebugWindow::chkTokenizeClicked()
{
	dwsettings_.tokenize_p = chktokenize_.get_active();
}

void DebugWindow::chkRPNClicked()
{
	dwsettings_.rpn_p = chkrpn_.get_active();
}

void DebugWindow::chkCalcClicked()
{
	dwsettings_.calc_p = chkcalc_.get_active();
}

DebugWindowSettings DebugWindow::getDWSettings()
{
	return dwsettings_;
}

void DebugWindow::setSettigns(const DebugWindowSettings& dwsettings)
{
	dwsettings_ = dwsettings;
	
	btnparrep_.set_active(dwsettings.parrep);
	chkparrep_.set_active(dwsettings.parrep_p);
	btnaddmul_.set_active(dwsettings.addmul);
	chkaddmul_.set_active(dwsettings.addmul_p);
	btnsubtoadd_.set_active(dwsettings.subtoadd);
	chksubtoadd_.set_active(dwsettings.subtoadd_p);
	btntokenize_.set_active(dwsettings.tokenize);
	chktokenize_.set_active(dwsettings.tokenize_p);
	btnrpn_.set_active(dwsettings.rpn);
	chkrpn_.set_active(dwsettings.rpn_p);
	btncalc_.set_active(dwsettings.calc);
	chkcalc_.set_active(dwsettings.calc_p);
}

} // namespace Gui

} // namespace Math


