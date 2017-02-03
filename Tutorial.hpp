/*
 * Tutorial.hpp
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
    C++ library (header file)
    Tutorial
    Gamemodes
*/

/*
    Version 0.1 (dd/mm/yy, 22/11/2015 ->)
*/

/*
    Preprocessor
*/

/*Header guard*/
#ifndef _FDX_TUTORIAL_H_
#define _FDX_TUTORIAL_H_

/* Includes */

//Spaceships
#include "Ships.hpp"

//Lasers
#include "Laser.hpp"

//Graphics
#include "SFML/Graphics.hpp"

//Modes
#include "Modes.hpp"

//Dictionary
#include "ClassMenu.hpp"

/* Defines */

/*Constants*/

/*Macros*/

namespace fdx { namespace fighter
{
    /*
        Class definitions
    */

    //Game mode
    class Tutorial;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    /* Classes */

    class Tutorial : public Mode
    {
        /*Config*/
        private:

            static constexpr unsigned int DEF_SAME_ENEM=0u;//Max number of concurrennt enemies
            static constexpr unsigned int DEF_TOTAL_ENEM=1u;//Total number of enemies
            static constexpr unsigned int DEF_LIFES=10u;//Number of lifes
            static constexpr unsigned int DEF_RADIUS=500u;//Radius when creating an enemy

        /*RNG*/
        private:

            static std::uniform_real_distribution<double> RNG_D;//Generates random doubles
            static std::default_random_engine RE;//Engine for the random numbers

        /*Attributes*/
        private:

            //Vars
            std::list<AI_spaceship> ais;//AIs of the ship
            std::deque<sf::Text> texts;//Text to output
            Spaceship* player;//Player spaceship
            sf::Clock ck;//Clock for the time

            //Spaceship
            const sf::Texture& txture;
            const Spshp_config& cfg;
            const Cannon& can;
            Array_Laser al;


            //Counters
            int now_cur,max_cur;//Numbers of enemy ships at the moment
            int now_tot,max_tot;//Numbers of enemy ships for the whole level
            int now_lif,max_lif;//Lifes of the player's ship

        /*Constructors, copy control*/
        public:

            //Default constructor
            Tutorial(const sf::Texture& itxture,const Spshp_config &icfg,const Cannon& ican,unsigned int icur=DEF_SAME_ENEM, unsigned int itot=DEF_TOTAL_ENEM, unsigned int ilifes=DEF_LIFES)
            :ais(),texts(),player(nullptr),ck(),txture(itxture),cfg(icfg),can(ican),now_cur(0),max_cur(icur),now_tot(0),max_tot(itot),now_lif(0),max_lif(ilifes)
            {}

            //Destructor
            virtual ~Tutorial() {}

        public:

            void add_enemy();//Add enemy
        private:
            void clean();//Delete dead ships

            bool need_add()//Need to add enemies?
            {
                return ((now_cur<max_cur)&&(now_tot<max_tot));
            }

            arrow::Vct get_rand_cnt()//Get a random new center
            {
                return arrow::Vct::mk_ang_mod(RNG_D(RE)*6.28,RNG_D(RE)*DEF_RADIUS)+(player?player->get_center():arrow::Vct(0,0));
            }

            bool need_rev()//Need to revive?
            {
                return (player&&(!player->alive())&&((now_lif)<max_lif));
            }

        /*Implementation*/
        public:

            void put_player(Spaceship* ispshp)//Put the main player
            {
                player=ispshp;
                if (player)
                {
                    player->put_lasers(&al);
                    player->put_cannon_lasers(&al);
                }

                for (auto it=ais.begin();it!=ais.end();++it)
                    it->set_enemy(ispshp);
            }

            const Spaceship* get_player() const//Get the player
            {
                return player;
            }

            bool has_time() const//Has time
            {
                return true;
            }

            int get_minutes() const //Get the minutes
            {
                return static_cast<int>(ck.getElapsedTime().asSeconds())/60;
            }
            int get_seconds() const//Get the seconds
            {
                return static_cast<int>(ck.getElapsedTime().asSeconds())%60;
            }

            bool has_friendly() const//Has number of ally ships
            {
                return true;
            }

            int get_current_friend() const//Get current allies
            {
                return max_lif-now_lif;
            }

            int get_max_friend() const//Get max allies
            {
                return max_lif;
            }

            bool has_enemies() const//Has number of enemy ships
            {
                return true;
            }

            int get_current_enemy() const//Get current enemies
            {
                return now_cur+(max_tot-now_tot);
            }

            int get_max_enemy() const//Get max enemies
            {
                return max_tot;
            }

            bool has_text() const//Displays text
            {
                return !texts.empty();
            }

            sf::Text get_text()//Text to be displayed
            {
                if (!has_text())
                    return sf::Text();

                sf::Text that=texts.front();
                texts.pop_front();
                return that;
            }

            bool has_radar() const//Radar of enemies
            {
                return true;
            }

            bool has_score() const
            {
                return false;
            }

            int get_score() const
            {
                return 0;
            }

            std::list<const Target *> get_targets() const//Get radar targets
            {
                std::list<const Target *> rv;
                for (const Target& it : ais)
                    rv.push_front(&it);
                return rv;
            }

            void draw(sf::RenderTarget& target, sf::RenderStates states) const//Draw the ships
            {
                for (const Spaceship& that : ais)
                    target.draw(that,states);
                target.draw(al,states);
            }

            void update(unsigned int ticks);//Move the ships

            bool end_game() const//End of the game?
            {
                return (now_lif);
            }

            bool enemy_dead() const
            {
                return (now_cur);
            }

            Team get_winner() const//Winner of the game
            {
                Team rv(player?player->team():Team());
                if (end_game())
                {
                    if ((now_cur+now_tot)==0)//All enemies died
                        return rv;
                    else//Player died
                        return rv.get_swapped();

                }
                return rv;
            }
    };

}}//End of namespace

#endif // _FDX_TUTORIAL_H_
