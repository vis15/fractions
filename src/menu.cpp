/*
 * menu.cpp
 *
 *  Created on: Dec 2, 2013
 *      Author: dis
 */

#include "menu.h"

namespace Math
{

namespace Gui
{

constr getMainMenu()
{
	return R"(
    <ui>
      <menubar name="menu-main">
      <menu action="FileMenu">
        <menuitem action="FileNewWindow" />
        <menuitem action="FileNewTab" />
        <separator/>
        <menuitem action="FileSave" />
        <menuitem action="FileSaveAs" />
        <separator/>
        <menuitem action="FileCloseTab" />
        <separator/>
        <menuitem action="FileQuit" />
      </menu>
      <menu action="EditMenu">
        <menuitem action="EditClearHistory" />
        <menuitem action="EditClearAll" />
        <separator/>
        <menuitem action="EditPref" />
      </menu>
      <menu action="ViewMenu">
        <menuitem action="ViewDebugWin" />
        <menuitem action="ViewLog" />
        <menuitem action="ViewMessages" />
      </menu>
      <menu action="FractionsMenu">
        <menuitem action="FractionsGCD" />
      </menu>
      <menu action="CalcMenu">
        <menuitem action="CalcCalc" />
        <menuitem action="CalcFractions" />
      </menu>
      <menu action="HelpMenu">
        <menuitem action="HelpHelp" />
        <menuitem action="HelpUpdate" />
        <separator/>
        <menuitem action="HelpAbout" />
      </menu>
      </menubar>
    </ui>
    )";
}

} //namespace Gui

} //namespace Math
