/*
 * prefswindow.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: dis
 */

#include "prefswindow.h"
#include "util.h"

namespace Math
{

namespace Gui
{

PrefsWindow::PrefsWindow()
{
	this->set_title(Info::kProgName() + " Preferences");
	this->set_default_size(200,100);
	
	this->add(box_main_);
	
	lblfractions_.set_label("<b>Tabs</b>");
	lblfractions_.set_use_markup(true);
	frmfractions_.set_label_widget(lblfractions_);
	frmfractions_.set_shadow_type(Gtk::SHADOW_NONE);
	
	chkfractions_.set_label("Fractions Enabled");
	chkfractions_.set_tooltip_text("If checked fractions will be enabled by default");
	chkfractions_.set_margin_left(20);
	
	frmfractions_.add(box_fractions_);
	box_fractions_.pack_start(chkfractions_);
	
	lbllog_.set_label("<b>Log</b>");
	lbllog_.set_use_markup(true);
	frmlog_.set_label_widget(lbllog_);
	frmlog_.set_shadow_type(Gtk::SHADOW_NONE);
	frmlog_.add(box_log_);
	
	chklog_.set_label("Log Enabled");
	chklog_.set_margin_left(20);
	box_log_.pack_start(chklog_);
	
	lblcolor_.set_label("<b>Color</b>");
	lblcolor_.set_use_markup(true);
	frmcolor_.set_label_widget(lblcolor_);
	frmcolor_.set_shadow_type(Gtk::SHADOW_NONE);
	
	chkcolor_.set_label("Show Color");
	chkcolor_.set_tooltip_text("If checked input and output will be colored");
	chkcolor_.set_margin_left(20);
	
	frmcolor_.add(box_color_);
	box_color_.pack_start(chkcolor_);
	
	box_main_.pack_start(frmfractions_, Gtk::PACK_SHRINK, 5);
	box_main_.pack_start(frmlog_);
	box_main_.pack_start(frmcolor_);
	box_main_.pack_start(btnbox_, Gtk::PACK_SHRINK, 5);
	btnbox_.pack_start(*btncancel_);
	btnbox_.pack_start(*btnok_);
	btnbox_.pack_start(*btnapply_);
	
	btnapply_->signal_clicked().connect(sigc::mem_fun(*this, &PrefsWindow::applySettings));
	btnok_->signal_clicked().connect(sigc::mem_fun(*this, &PrefsWindow::buttonOK));
	btncancel_->signal_clicked().connect(sigc::mem_fun(*this, &PrefsWindow::buttonCancel));
	
	this->show_all_children();
}

PrefsWindow::~PrefsWindow()
{
	
}

void PrefsWindow::setSettings(PrefsWindowSettings prefswinsettings)
{
	prefswinsettings_ = prefswinsettings;
	
	chkfractions_.set_active(prefswinsettings_.fractions);
	chklog_.set_active(prefswinsettings_.log);
	chkcolor_.set_active(prefswinsettings.color);
}

void PrefsWindow::applySettings()
{
	prefswinsettings_.fractions = chkfractions_.get_active();
	prefswinsettings_.log = chklog_.get_active();
	prefswinsettings_.color = chkcolor_.get_active();
	
	signal_apply.emit(prefswinsettings_);
}

void PrefsWindow::buttonOK()
{
	applySettings();
	this->hide();
}

void PrefsWindow::buttonCancel()
{
	this->hide();
}

} // namespace Gui

} // namespace Math
