/*
 * Teams.cpp
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
    FDX_Fighter - Teams.cpp
    Little spaceship game made with C++ and SFML
    Teams data - code file
*/

/*
    Version Nº nversion (dd/mm/yy, 26/10/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

//Header file
#include "Teams.hpp"

namespace fdx{ namespace fighter
{
    /* Initializations */

    //Default colors
    const sf::Color Team_Color::DEF_FRIENDLY=sf::Color::Blue;
    const sf::Color Team_Color::DEF_NEUTRAL=sf::Color::White;
    const sf::Color Team_Color::DEF_ENEMY=sf::Color::Red;

    //Teams
    Team_Color teams;

    /*
        Functions
    */

}}//End of namespace
