/*
 * Modes.hpp
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
    Modes
    Gamemodes
*/

/*
    Version 0.1 (dd/mm/yy, 22/11/2015 ->)
*/

/*
    Preprocessor
*/

/*Header guard*/
#ifndef _FDX_MODES_H_
#define _FDX_MODES_H_


/* Includes */

//Spaceships
#include "Ships.hpp"

//Lasers
#include "Laser.hpp"

//Graphics
#include "SFML/Graphics.hpp"

//Linked lists
#include <list>

//Double ending queue
#include <deque>

//Strings
#include <string>

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
    class Mode;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    /* Classes */

    class Mode : public sf::Drawable
    {
        /*Interface*/
        public:

            virtual void put_player(Spaceship* ispshp) = 0;//Put the main player

            virtual const Spaceship* get_player() const = 0;//Get the player

            virtual bool has_time() const = 0;//Has time

            virtual int get_minutes() const = 0;//Get the minutes

            virtual int get_seconds() const = 0;//Get the seconds

            virtual bool has_friendly() const = 0;//Has number of ally ships

            virtual int get_current_friend() const = 0;//Get current allies

            virtual int get_max_friend() const = 0;//Get max allies

            virtual bool has_enemies() const = 0;//Has number of enemy ships

            virtual int get_current_enemy() const = 0;//Get current enemies

            virtual int get_max_enemy() const= 0;//Get max enemies

            virtual bool has_text() const = 0;//Displays text

            virtual sf::Text get_text() = 0;//Text to be displayed

            virtual bool has_radar() const = 0;//Radar of enemies

            virtual std::list<const Target *> get_targets() const= 0;//Get radar targets

            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;//Draw the ships

            virtual void update(unsigned int ticks) = 0;//Move the ships

            virtual bool end_game() const = 0;//End of the game?

            virtual Team get_winner() const = 0;//Winner of the game

            virtual bool has_score() const = 0;//Has score?

            virtual int get_score() const = 0;//Get the score
    };

    class Offline_Survival : public Mode
    {
        /*Config*/
        private:

            static constexpr unsigned int DEF_SAME_ENEM=4u;//Max number of concurrennt enemies
            static constexpr unsigned int DEF_TOTAL_ENEM=20u;//Total number of enemies
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
            Offline_Survival(const sf::Texture& itxture,const Spshp_config &icfg,const Cannon& ican,unsigned int icur=DEF_SAME_ENEM, unsigned int itot=DEF_TOTAL_ENEM, unsigned int ilifes=DEF_LIFES)
            :ais(),texts(),player(nullptr),ck(),txture(itxture),cfg(icfg),can(ican),now_cur(0),max_cur(icur),now_tot(0),max_tot(itot),now_lif(0),max_lif(ilifes)
            {}

            //Destructor
            virtual ~Offline_Survival() {}

        /*Population control*/
        private:

            //Objectives

            bool need_add()//Need to add enemies?
            {
                return ((now_cur<max_cur)&&(now_tot<max_tot));
            }

            bool need_rev()//Need to revive?
            {
                return (player&&(!player->alive())&&((now_lif)<max_lif));
            }

            arrow::Vct get_rand_cnt()//Get a random new center
            {
                return arrow::Vct::mk_ang_mod(RNG_D(RE)*6.28,RNG_D(RE)*DEF_RADIUS)+(player?player->get_center():arrow::Vct(0,0));
            }

            void add_enemy();//Add enemy

            void clean();//Delete dead ships

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

            virtual bool has_score() const
            {
                return false;
            }

            virtual int get_score() const
            {
                return 0;
            }
    };


    class Multi_DM : public Mode
    {
        /*Config*/
        public:

            //Enums
            enum Players : int
            {
                PLAYER_1=1,
                PLAYER_2,
                MAX_PLAYERS
            };

            static constexpr int DEF_MAX_LIF=5;

        /*RNG*/
        private:

            static std::uniform_real_distribution<double> RNG_D;//Generates random doubles
            static std::default_random_engine RE;//Engine for the random numbers

            static constexpr int PUNT_DEATH=-100,PUNT_KILL=200;
            static constexpr int DEF_DEL=1000000;

        /*Attributes*/
        private:

            //Vars
            std::deque<sf::Text> texts,texts2;//Text to output
            Spaceship *player,*player2;//Player spaceship
            sf::Clock ck;//Clock for the time
            Array_Laser al;
            sf::Sound snd;
            sf::Sprite ded;
            sf::Sprite ded2;

            int max_player_lif;
            int cur_player,cur_player2;
            int sel_play;
            bool ended=false;
            int delay1=0,delay2=0;

            int score1=0,score2=0;

        /*Constructors, copy control*/
        public:

            //Default constructor
            Multi_DM(unsigned int itot=DEF_MAX_LIF)
            :texts(),texts2(),player(nullptr),player2(nullptr),ck(),al(),max_player_lif(itot),cur_player(0),cur_player2(0)
            {}

            //Destructor
            virtual ~Multi_DM() {}

        /*Population control*/
        private:


            arrow::Vct get_rand_cnt(arrow::Vct r)//Get a random new center
            {
                return arrow::Vct::mk_ang_mod(RNG_D(RE)*6.28,RNG_D(RE)*300+200)+(r);
            }

            void rev_ships()
            {
                if (player&&(!player->alive())&&cur_player<max_player_lif)
                {
                    if (!delay1) {delay1=DEF_DEL;
                    ded.setPosition(player->get_center().x, player->get_center().y);}

                    cur_player++;
                    score2+=PUNT_KILL;
                    score1+=PUNT_DEATH;

                    snd.play();
                    if (cur_player!=max_player_lif)
                    {
                        player->revive_at(get_rand_cnt(player->get_center()));
                    }

                }

                if (player2&&(!player2->alive())&&cur_player2<max_player_lif)
                {
                    if (!delay2){delay2=DEF_DEL;
                    ded2.setPosition(player2->get_center().x, player2->get_center().y);}

                    score1+=PUNT_KILL;
                    score2+=PUNT_DEATH;
                    cur_player2++;
                    snd.play();
                    if (cur_player2!=max_player_lif)
                    {
                        player2->revive_at(get_rand_cnt(player2->get_center()));
                    }
                }
            }

        /*Implementation*/
        public:

            void sel_player(int iplayer)
            {
                sel_play=iplayer;
            }

            void put_player(Spaceship* ispshp)//Put the main player
            {
                switch(sel_play)
                {
                    case PLAYER_1:
                    {
                        player=ispshp;
                        if (player)
                        {
                            player->put_lasers(&al);
                            player->put_cannon_lasers(&al);
                        }

                        break;
                    }

                    case PLAYER_2:
                    {
                        player2=ispshp;
                        if (player2)
                        {
                            player2->put_lasers(&al);
                            player2->put_cannon_lasers(&al);
                        }

                        break;
                    }
                }
            }

            const Spaceship* get_player() const//Get the player
            {
                switch (sel_play)
                {
                    case PLAYER_1:
                        return player;

                    case PLAYER_2:
                        return player2;
                }
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
                switch (sel_play)
                {
                    case PLAYER_1:
                        return max_player_lif-cur_player;
                    case PLAYER_2:
                        return max_player_lif-cur_player2;
                }
            }

            int get_max_friend() const//Get max allies
            {
                return max_player_lif;
            }

            bool has_enemies() const//Has number of enemy ships
            {
                return true;
            }

            int get_current_enemy() const//Get current enemies
            {
                switch (sel_play)
                {
                    case PLAYER_1:
                        return max_player_lif-cur_player2;

                    case PLAYER_2:
                        return max_player_lif-cur_player;
                }
            }

            int get_max_enemy() const//Get max enemies
            {
                return max_player_lif;
            }

            bool has_text() const//Displays text
            {
                switch (sel_play)
                {
                    case PLAYER_1:
                        return !texts.empty();
                    case PLAYER_2:
                        return !texts2.empty();
                    default:
                        return false;
                }
            }

            sf::Text get_text()//Text to be displayed
            {
                if (!has_text())
                    return sf::Text();

                sf::Text that;

                switch (sel_play)
                {
                    case PLAYER_1:
                        that=texts.front();
                        texts.pop_front();
                        break;

                    case PLAYER_2:
                        that=texts2.front();
                        texts2.pop_front();
                }
                return that;
            }

            bool has_radar() const//Radar of enemies
            {
                return true;
            }

            std::list<const Target *> get_targets() const//Get radar targets
            {
                std::list<const Target *> rv;

                switch (sel_play)
                {
                    case PLAYER_1:
                        rv.push_back(player2);
                        break;
                    case PLAYER_2:
                        rv.push_back(player);
                        break;
                }
                return rv;
            }

            void draw(sf::RenderTarget& target, sf::RenderStates states) const//Draw the ships
            {
                target.draw(al,states);
                if (delay1)
                    target.draw(ded);
                if (delay2)
                    target.draw(ded2);
            }

            void put_sound (sf::SoundBuffer *nsnd)
            {
                snd.setBuffer(*nsnd);
            }

            void put_image(sf::Texture *img)
            {
                ded.setTexture(*img);
                ded2.setTexture(*img);
                ded.setOrigin(25.f, 23.f);
                ded2.setOrigin(25.f, 23.f);
            }

            void update(unsigned int ticks);//Move the ships

            bool end_game() const//End of the game?
            {
                return cur_player==max_player_lif||cur_player2==max_player_lif;
            }

            Team get_winner() const//Winner of the game
            {
                Team rv(player?player->team():Team());
                if (end_game())
                {
                    if (score1>score2)//All enemies died
                        return rv;
                    else//Player died
                        return rv.get_swapped();

                }
                return rv;
            }

            virtual bool has_score() const
            {
                return true;
            }

            virtual int get_score() const
            {
                switch(sel_play)
                {
                    case PLAYER_1:
                        return score1;
                    case PLAYER_2:
                        return score2;
                    default:
                        return 0;
                }
            }
    };
}}//End of namespace

//End of library
#endif
