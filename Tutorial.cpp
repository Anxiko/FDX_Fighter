/*
 * Tutorial.cpp
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
    C++ library (source file)
    Modes
    Gamemodes
*/

/*
    Version 0.1 (dd/mm/yy, 22/11/2015 ->)
*/

/*
    Preprocessor
*/

/* Includes */

//Header file
#include "Tutorial.hpp"

namespace fdx{ namespace fighter
{
    /*Init*/

    std::uniform_real_distribution<double> Tutorial::RNG_D(0,1);//Generates random doubles
    std::default_random_engine Tutorial::RE;//Engine for the random numbers

    /*
        Functions
    */

    /*Tutorial*/

    //Population control
    void Tutorial::add_enemy()//Add enemy
    {
        //Update the counters
        ++now_cur;
        --now_tot;

        //Insert the enemy
        ais.emplace_front(get_rand_cnt(),txture,cfg,Team(player?player->team().get_swapped():Team().get_swapped()));

        //Configure it
        AI_spaceship& new_ai=ais.front();
        new_ai.set_enemy(player);
        new_ai.put_lasers(&al);
        new_ai.put_cannon(can);
        new_ai.put_cannon_lasers(&al);
    }

    void Tutorial::clean()//Delete dead ships
    {
        for (auto it=ais.begin();it!=ais.end();)//Circle through the ships
        {
            if (it->alive())//Ignore the alive ones
                ++it;
            else//Dead, notice and delete
            {
                //Notice
                sf::Text new_text;
                new_text.setString(fdx::menu::Language::getText(fdx::menu::Language::ENEMY_DIED));
                new_text.setColor(teams.get_friendly());
                texts.push_back(new_text);

                //Delete
                it=ais.erase(it);
                now_cur--;
            }
        }
    }

    void Tutorial::update(unsigned int ticks)
    {
        if (end_game()) return;

        clean();

        al.mov(ticks);

        if (player&&need_rev())
        {
            //Revive the player only if it has any lifes left
            if (--now_lif)
                player->revive_at(arrow::Vct(0,0));

            sf::Text new_text;
            new_text.setString(fdx::menu::Language::getText(fdx::menu::Language::YOU_DIED));
            new_text.setColor(teams.get_enemy());
            texts.push_back(new_text);
        }

        for (auto it=ais.begin();it!=ais.end();++it)
        {
            it->think();
            it->play();
            it->mov(ticks);
        }
    }
}}//End of namespace
