//============================================================================
// Name        : Math.cpp
// Author      : Timothy O'Connor
// Version     :
// Copyright   : Copyright 2013-2014
// Description : Fraction Calculator
//============================================================================

#include "mainwindow.h"

int main(int argc, char* argv[])
{
	int fargc = 1; //fake argc //Gtk::Application will error if extra command line args
	
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(fargc, argv, "Math.Calculator.Fractions", Gio::APPLICATION_NON_UNIQUE);
	
	Math::Gui::MainWindow mw(argc, argv);
	
	return app->run(mw);
}
