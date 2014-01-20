/*
 * css.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: dis
 */

#include "css.h"

namespace Math
{

namespace Gui
{

constr getCss()
{
	return R"FractionsCSS(

GtkNotebook
{
    padding: 4px;
}

.notebook tab
{
    padding: 5px;
}

.notebook .button
{
    padding: 0;
    background-image: none;
    border-style: none;
    box-shadow: none;
}
 
.notebook .button:hover 
{
    border-color: shade(#800080, 0.1); 
}

GtkLabel:focus
{
    
}

)FractionsCSS";
	
}

} //namespace Gui

} //namespace Math
