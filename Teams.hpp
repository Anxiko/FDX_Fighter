/*
 * Teams.hpp
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
    FDX_Fighter - Teams.hpp
    Little spaceship game made with C++ and SFML
    Teams data - header file
*/

/*
    Version Nº nversion (dd/mm/yy, 26/10/2015 -> )
*/

/*
    Preprocessor
*/

/*Header guard*/
#ifndef _FDX_TEAMS_H_
#define _FDX_TEAMS_H_


/* Includes */

#include "SFML/Graphics.hpp"

/* Defines */

/*Constants*/

/*Macros*/

namespace fdx { namespace fighter
{
    /*
        Data types
     */

    /* Classes */

    class Team_Color;//Color of the different teams
    class Team;//Team class

    /* Variables */

    extern Team_Color teams;

    /* Typedefs */

    /*Constants*/

    /*
        Class definitions
    */

    //Team class
    class Team
    {
        /*Settings*/
        public:

            static constexpr bool TEAM_GOOD=true, TEAM_BAD=false;

        /*Attributes*/
        private:

            bool t;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Team(bool it)
            :t(it)
            {}

            //Default constructor
            Team()
            :t(TEAM_GOOD)
            {}

            //Copy constructor
            Team(const Team& t)
            :Team(t.get_t())
            {}

            //Asignment operator
            Team& operator= (const Team& t)
            {
                set_t(t.get_t());
                return *this;
            }

        /*Get/set*/
        public:

            bool get_t() const
            {
                return t;
            }

            void set_t(bool it)
            {
                t=it;
            }

        /*Check teams*/
        public:

            //Check if they are friendly, neutral or enemy

            bool friendly (const Team& team) const
            {
                return team.get_t()==get_t();
            }

            bool neutral (const Team&) const
            {
                return false;
            }

            bool enemy (const Team& team) const
            {
                return team.get_t()!=get_t();
            }

        /*Swap*/
        public:

            //Swap this team
            void swap_team()
            {
                set_t(!get_t());
            }

            //Get the swap of this team
            Team get_swapped()
            {
                Team rv(*this);
                rv.swap_team();
                return rv;
            }
    };

    class Team_Color;

    /*
        Data types
     */

    /* Classes */

    class Team_Color
    {

        /*Default values*/
        private:

            static const sf::Color DEF_FRIENDLY,DEF_NEUTRAL,DEF_ENEMY;

        /*Attributes*/
        private:

            //Colors
            sf::Color friendly,neutral,enemy;

        /*Constructors, copy control*/

        /*Constructors*/
        public:

            //Complete  constructor
            Team_Color(const sf::Color& ifriendly, const sf::Color& ineutral, const sf::Color& ienemy)
            :friendly(ifriendly),neutral(ineutral),enemy(ienemy)
            {}

            //Default constructor
            Team_Color()
            :Team_Color(DEF_FRIENDLY,DEF_NEUTRAL,DEF_ENEMY)
            {}

            //Copy constructor
            Team_Color(const Team_Color& tc) = default;

        /*Get/set*/
        public:

            sf::Color get_friendly() const
            {
                return friendly;
            }

            void set_friendly(const sf::Color& ifriendly)
            {
                friendly=ifriendly;
            }

            sf::Color get_neutral() const
            {
                return neutral;
            }

            void set_neutral(const sf::Color& ineutral)
            {
                neutral=ineutral;
            }

            sf::Color get_enemy() const
            {
                return enemy;
            }

            void set_enemy(const sf::Color& ienemy)
            {
                enemy=ienemy;
            }

        //Get color of a team
        sf::Color get_team_color(Team t) const
        {
            switch (t.get_t())
            {
                case Team::TEAM_GOOD:
                    return get_friendly();

                case Team::TEAM_BAD:
                    return get_enemy();

                default:
                    return get_neutral();
            }
        }
    };

    /*
        Function prototypes
    */

}}//End of namespace

//End of library
#endif
