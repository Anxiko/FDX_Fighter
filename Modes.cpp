/*
 * Modes.cpp
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
#include "Modes.hpp"

namespace fdx{ namespace fighter
{
    /*Init*/

    std::uniform_real_distribution<double> Offline_Survival::RNG_D(0,1);//Generates random doubles
    std::default_random_engine Offline_Survival::RE;//Engine for the random numbers

    std::uniform_real_distribution<double> Multi_DM::RNG_D(0,1);//Generates random doubles
    std::default_random_engine Multi_DM::RE;//Engine for the random numbers

    /*
        Functions
    */

    /*Offline survival*/

    //Population control
    void Offline_Survival::add_enemy()//Add enemy
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

    void Offline_Survival::clean()//Delete dead ships
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
                new_text.setFillColor(teams.get_friendly());
                new_text.setOutlineColor(teams.get_friendly());
                texts.push_back(new_text);

                //Delete
                it=ais.erase(it);
                now_cur--;
            }
        }
    }

    void Offline_Survival::update(unsigned int ticks)
    {
        if (end_game()) return;

        while (need_add())
            add_enemy();

        clean();

        al.mov(ticks);

        if (player&&need_rev())
        {
            //Revive the player only if it has any lifes left
            if (--now_lif)
                player->revive_at(arrow::Vct(0,0));

            sf::Text new_text;
            new_text.setString(fdx::menu::Language::getText(fdx::menu::Language::YOU_DIED));
            new_text.setFillColor(teams.get_enemy());
            new_text.setOutlineColor(teams.get_enemy());
            texts.push_back(new_text);
        }

        for (auto it=ais.begin();it!=ais.end();++it)
        {
            it->think();
            it->play();
            it->mov(ticks);
        }
    }

    void Multi_DM::update(unsigned int ticks)
    {
        if (delay1)
        {
            if (static_cast<int>(ticks)>delay1)
                delay1=0;
            else
            delay1-=ticks;
        }
        if (delay2)
        {
            if (static_cast<int>(ticks)>delay2)
                delay2=0;
            else
                delay2-=ticks;
        }

        if (end_game())
        {
            if (!ended)
            {
                ended=true;
                if (get_winner().friendly(player->team()))
                {
                    sf::Text new_text;
                    new_text.setString(fdx::menu::Language::getText(fdx::menu::Language::WON));
                    new_text.setFillColor(teams.get_friendly());
                    new_text.setOutlineColor(teams.get_friendly());
                    texts.push_back(new_text);

                    sf::Text new_text2;
                    new_text2.setString(fdx::menu::Language::getText(fdx::menu::Language::LOSE));
                    new_text2.setFillColor(teams.get_enemy());
                    new_text2.setOutlineColor(teams.get_enemy());
                    texts2.push_back(new_text2);
                }
                else
                {
                    sf::Text new_text;
                    new_text.setString(fdx::menu::Language::getText(fdx::menu::Language::LOSE));
                    new_text.setFillColor(teams.get_enemy());
                    new_text.setOutlineColor(teams.get_enemy());
                    texts.push_back(new_text);

                    sf::Text new_text2;
                    new_text2.setString(fdx::menu::Language::getText(fdx::menu::Language::WON));
                    new_text2.setFillColor(teams.get_friendly());
                    new_text2.setOutlineColor(teams.get_friendly());
                    texts2.push_back(new_text2);
                }
            }
        }

        al.mov(ticks);

        rev_ships();
    }
}}//End of namespace
