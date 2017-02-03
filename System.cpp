/*
 * System.cpp
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
* FDX_Fighter - System.cpp
* Little spaceship game made with C++ and SFML
* System resources - code file
*/

/*
    Version 0.1 (dd/mm/yy, 8/4/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

//Headers file
#include "System.hpp"

/* Defines */

/*Constants*/

/*Macros*/


//Namespace
namespace fdx { namespace fighter
{
    /*
        Code
    */

    /* Init */

    /*KM_Controller*/

    const Event_KM KM_Controller::DEF_ACC(Event_KM::CTRL_KB,sf::Keyboard::A);
    const Event_KM KM_Controller::DEF_BRAKE(Event_KM::CTRL_KB,sf::Keyboard::D);
    const Event_KM KM_Controller::DEF_FIRE(Event_KM::CTRL_M,sf::Mouse::Left);

    /*GP_Controller*/
    const Event_GPB GP_Controller::DEF_ACC(fdx::fighter::Event_GPB::A);
    const Event_GPB GP_Controller::DEF_BRAKE(fdx::fighter::Event_GPB::B);
    const Event_GPB GP_Controller::DEF_FIRE(fdx::fighter::Event_GPB::X);

    /*Variables*/

    //Controllers
    Selected_Controller controllers;

    const Event_GPS GP_Controller::DEF_STICK(fdx::fighter::Event_GPS::LSX,fdx::arrow::Vct(0,0));

    /* Functions */

    /*Fps_manager*/

    //FPS counter

    //Pass to the next frame
    void Fps_manager::upd_frame()
    {
        ++frames;
        tm_tick tsu=(tm.getElapsedTime()-counter).asMilliseconds();//Time since last update
        if(tsu>=TM_UPD_TXT)
        {
            counter=tm.getElapsedTime();
            std::ostringstream ss;
            ss<<"FPS: "<<(frames/(tsu*0.001));//Converse milliseconds to seconds
            frames=0;
            txt.setString(ss.str());
        }
    }

}}//End of namespace
