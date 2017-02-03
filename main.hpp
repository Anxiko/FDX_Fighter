/*
 * main.hpp
 *
 * Copyright 2015 Joaquín Monteagudo Gómez <kindos7@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

/*
    FDX_Fighter - main.hpp
    Little spaceship game made with C++ and SFML
    Main game
*/

/*
    Version 0.1 (dd/mm/yy, 21/03/2015 -> )
*/

/*
    Preprocessor
*/

/*Header guard*/
#ifndef _FDX_MAIN_H_
#define _FDX_MAIN_H_

/* Includes */

//Input/output
#include <iostream>
#include <sstream>

//Spaceships
#include "Ships.hpp"

//Graphics
#include "Graphics.hpp"

//System
#include "System.hpp"

//Modes
#include "Modes.hpp"

//Controls
#include "System.hpp"

//Dictionary
#include "ClassMenu.hpp"

//Jokes
#include "Joke.hpp"

#include "JokeManager.hpp"
namespace fdx{ namespace fighter
{
    //Offline mode
    void launch_single_player();

    void launch_multi_player();
}}

#endif // _FDX_MAIN_H_
