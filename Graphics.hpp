/*
 * Graphics.hpp
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
* FDX_Fighter - Graphics.hpp
* Little spaceship game made with C++ and SFML
* SFML layer - header file
*/

/*
    Version 0.1 (dd/mm/yy, 20/3/2015 -> )
*/

/*
    Preprocessor
*/

//Header guard
#ifndef _FDX_GRAPHICS_H_
#define _FDX_GRAPHICS_H_

/* Includes */

//Physics resources
#include "Physics.hpp"

//Spaceships
#include "Ships.hpp"

//SFML graphics
#include "SFML/Graphics.hpp"

//Game modes
#include "Modes.hpp"

//RNG
#include <random>

//Double-ended queue
#include <deque>

//String stream
#include <sstream>


//Team colours
#include "Teams.hpp"

/* Defines */

/*Constants*/

/*Macros*/


//Namespace
namespace fdx { namespace fighter
{
    /*
        Class definitions
    */

    //Camera
    class Camera;

    //Background
    class Background;

    //Little shiny particle
    class Particle;

    //Particle with a TTL
    class LiveParticle;

    //Background particles
    class Bg_Particles;

    //Smoke particles
    class Sm_Particle;

    //Energy displayer
    class Energy_displayer;

    //HUD for the player's spaceship
    class HUD_player;

    //Spaceship's propeller
    class Propeller;

    //Text displayer
    class Text_displayer;

    //Game mode HUD
    class HUD_mode;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    /* Classes */

    //Camera
    class Camera : public sf::RenderWindow
    {
        /*Enums*/
        public:

            enum Players : int
            {
                SINGLE_PLAYER=0,
                PLAYER_1,
                PLAYER_2,
                MAX_PLAYERS//Count, leave this at the end
            };

        /*Attributes*/
        private:

            //View when the camera is translated
            sf::View vw;

            //Center of the screen
            arrow::Vct center;

            //Original center of the screen
            const arrow::Vct old_center;

            //Mouse position (without translation)
            arrow::Vct ms;

            //Distance to the center before scrolling
            unsigned int dist;

            //Number of selected players
            int players;

        /*Contructors, copy control*/
        public:

            //Complete constructor
            Camera(int nsx, int nsy, const char * ntitle, unsigned char nAA, unsigned int ndist)
            :RenderWindow(sf::VideoMode(nsx,nsy),ntitle,sf::Style::Default,sf::ContextSettings(0,0,nAA)),vw(sf::FloatRect(0, 0, nsx, nsy)),
             center(nsx/2,nsy/2),old_center(center),ms(center),dist(ndist),players(SINGLE_PLAYER)
            {}

            //Virtual destructor
            virtual ~Camera() {}

        /*Focus on target*/
        public:

            //Keep a target withing the focus
            void focus_target(const arrow::Vct& target);

            //Center a target on screen
            void center_target(const arrow::Vct& target);

        private:

            //Set the given center as center of the view
            void set_center(const arrow::Vct& ncenter);

        /*Set current view*/
        public:

            //Set the camera to its original state
            void view_default()
            {
                setView(getDefaultView());
            }

            //Set the camera to its translated state
            void view_translated()
            {
                setView(vw);
            }

        /*Translation*/
        public:

            //Get the current translation
            arrow::Vct get_translation() const
            {
                return center-old_center;
            }

            //Apply the translation to a given Vct
            arrow::Vct apply_translation(const arrow::Vct& vct) const
            {
                return vct+get_translation();
            }

            //Get the center of the camera
            arrow::Vct get_center() const
            {
                return center;
            }

        /*Player view*/
        public:

            //Singleplayer view
            void set_player_single()
            {
                players=SINGLE_PLAYER;
                sf::View current_view = this->getView();
                current_view.setViewport(sf::FloatRect(0.0f,0.0f,1.0f,1.0f));
                setView(current_view);
            }

            //Multiplayer view
            void set_player_multi(int player)
            {
                if (player>=PLAYER_1&&player<MAX_PLAYERS)
                {
                    players=player;
                    sf::View vw;
                    vw.reset(sf::FloatRect(0.0f,0.0f,getSize().x/2,getSize().y));
                    vw.setCenter(getView().getCenter());
                    switch(players)
                    {
                        case PLAYER_1:
                        {
                            vw.setViewport(sf::FloatRect(0.0f,0.0f,0.5f,1.0f));
                            break;
                        }

                        case PLAYER_2:
                        {
                            vw.setViewport(sf::FloatRect(0.5f,0.0f,0.5f,1.0f));
                        }
                    }
                    setView(vw);
                }
            }

        /*Mouse*/
        public:

            //Update the raw position of the mouse
            void update_mouse(const arrow::Vct& nms)
            {
                ms=nms;
            }

            //Get the raw mouse
            arrow::Vct get_raw_ms() const
            {
                return ms;
            }

            //Get the translated mouse
            arrow::Vct get_tra_ms() const
            {
                arrow::Vct rv (apply_translation(get_raw_ms()));
                switch (players)
                {
                    case PLAYER_1:
                    {
                        rv.x+=getSize().x/4;
                        break;
                    }

                    case PLAYER_2:
                    {
                        rv.x-=getSize().x/4;
                        break;
                    }
                }

                return rv;
            }
    };

    class Background : public sf::Drawable
    {
        /*Attributes*/
        private:

            sf::Sprite sprt;//Sprite of the background
            arrow::Vct::Mod scale;//Scale using on the translation of the sprite
            const arrow::Vct center;//Center of the background

        /*Constructors, copy control*/
        public:

            //Constructor
            Background(const sf::Texture& txture, arrow::Vct::Mod nscale, const arrow::Vct& ncenter)
            :scale(nscale),center(ncenter)
            {
                create_sprite(txture);
            }

        /*Sprite*/
        private:

            //Sprite creation, create the sprite and set its position
            void create_sprite(const sf::Texture &ntxture)
            {
                sf::Vector2u txture_sz=ntxture.getSize();
                sprt.setTexture(ntxture);
                sprt.setOrigin(txture_sz.x*0.5,txture_sz.y*0.5);
                sprt.setPosition(center.x,center.y);
            }

        public:

            //Draw the background
            void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                target.draw(sprt,states);
            }

        /*Translation*/
        public:

            //Apply the translation to the background
            void mov(arrow::Vct t)
            {
                //Scale the translation
                t*=-scale;

                //Apply the translation to the center
                t+=center;

                //Set the sprite's position
                sprt.setPosition(t.x,t.y);
            }
    };

    //Little shiny particle
    class Particle : public sf::Drawable
    {
        /*Attributes*/
        private:

            arrow::Vct r;//Position of the particle

            //Attributes shared between particles
            static sf::CircleShape circle;//Circle to be drawn
            static constexpr unsigned int DEF_QUAL=10;//Quality of the particle (sides of the fake circle)

        /*Constructors, copy control*/
        public:

            //Constructor
            Particle(const arrow::Vct& nr)
            :r(nr)
            {}

            //Default constructor
            Particle()
            :Particle(arrow::Vct())
            {}

            //Virtual destructor
            virtual ~Particle(){}

        /*Draw*/
        public:

            //Draw the particle
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

            //Set the drawing configuration

            //Set the size
            static void set_size(unsigned int nsz);

            //Set the color

            //Copy from another color
            static void set_color(const sf::Color& ncolor);

            //Set the color by its components
            static void set_color(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na);

        /*Position*/
        public:

            //Movement

            //Move the particle with the given speed
            void mov(const arrow::Vct& s)
            {
                r+=s;
            }

            //Position

            //Get position
            arrow::Vct get_pos() const
            {
                return r;
            }

            //Set position
            void set_pos(const arrow::Vct &nr)
            {
                r=nr;
            }

    };

    //Particle with a TTL
    class LiveParticle : public Particle
    {
        /*Attributes*/
        private:

            unsigned int ttl;//Time to live in microseconds

        /*Constructors, copy control*/
        public:

            //Constructor
            LiveParticle(unsigned int nttl, arrow::Vct nr)
            :Particle(nr),ttl(nttl)
            {}

            //Destructor
            virtual ~LiveParticle()
            {}

        /*TTL*/
        public:

            //Set and get

            unsigned int get_ttl() const
            {
                return ttl;
            }

            void set_ttl(unsigned int nttl)
            {
                ttl=nttl;
            }

            //Decrease the ttl by the given amount
            void decrease_ttl (unsigned int tick)
            {
                if (tick>ttl)
                    ttl=0;
                else
                    ttl-=tick;
            }

            //Kill the particle by putting its ttl to zero
            void kill()
            {
                set_ttl(0);
            }

            //Check if the particle is alive (ttl is not zero)
            bool alive()
            {
                return ttl;
            }
    };

    //Background particles
    class Bg_Particles : public sf::Drawable
    {
        /*Static RNG*/
        private:

            static std::uniform_int_distribution<unsigned> RNG_UI;//Generates random integers
            static std::uniform_real_distribution<double> RNG_D;//Generates random doubles
            static std::default_random_engine RE;//Engine for the random numbers

        /*Default configuration*/
        private:

            static constexpr unsigned int NPART=100;//Number of particles
            static constexpr unsigned int MAX_RAD_PART=5;//Maximum radius of the particle when drawn

        private:

            Particle particles[NPART];//Array of particles
            arrow::Vct center;//Center of the area
            unsigned int sq_radius;//Squared area of focus

        /*Construcors, copy control*/
        public:

            //Constructor
            Bg_Particles(const arrow::Vct &ncenter, unsigned int nradius)
            :center(ncenter),sq_radius(nradius*nradius)
            {
                for (Particle& p: particles)
                    set_particle(p);
            }

        /*Particle management*/
        private:

            //Checks if the given particle is in a valid position (true if the position is valid, false otherwise)
            bool check_particle (const Particle& p) const;

            //Set position of the given particle to a valid random state
            void set_particle (Particle& p) const;

            //Set position of the given particle to its opposite position
            void reset_particle (Particle& p) const;

            //Check the state of all particles within the array, and change the position of those in an invalid state
            void mng_particles();

        /*Center of the particles*/
        public:

            //Set the center of the particles
            void set_center(const arrow::Vct& ncenter)
            {
                center=ncenter;
                mng_particles();
            }

            //Get the radius of focus
            arrow::Vct::Mod get_radius() const
            {
                return std::sqrt(sq_radius);
            }

        /*Draw*/
        public:

            //Draw the particles
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:

            //Set the drawing options of the particles
            static void draw_config();
    };

    //Energy displayer
    class Energy_displayer : public sf::Drawable
    {
        /*Default configuration*/
        public:

            //Axes
            static constexpr bool HORIZONTAL_AXE=false;//Horizontal axe
            static constexpr bool VERTICAL_AXE=true;//Vertical axe

        private:
            static constexpr bool DEF_AXE=HORIZONTAL_AXE;//Default axe

            //Position and size
            static const arrow::Vct DEF_POS;//Position
            static const arrow::Vct DEF_SZ;//Size

            //Color
            static const sf::Color DEF_COL;

        /*Attributes*/
        private:

            const Energy *pbar;//Pointer to the bar to be displayed
            float perc;//Percentage to be displayed (automatically updated from the *pbar if not null)

        public:
            arrow::Vct r,s;//Position and size of the bar
            bool axe;//Horizontal or vertical movement of the bar
            sf::Color col;//Color of the bar

        /*Constructors, copy control*/
        private:

            //Complete constructor
            Energy_displayer   (const Energy *npbar, float nperc, const arrow::Vct &nr=DEF_POS, const arrow::Vct &ns=DEF_SZ
                                ,bool naxe=DEF_AXE,const sf::Color &ncol=DEF_COL)
            :pbar(npbar),perc(nperc),r(nr),s(ns),axe(naxe),col(ncol)
            {}

        public:

            //Energy pointer constructor
            Energy_displayer    (const Energy *npbar, const arrow::Vct &nr=DEF_POS, const arrow::Vct &ns=DEF_SZ
                                ,bool naxe=DEF_AXE,const sf::Color &ncol=DEF_COL)
            :Energy_displayer(npbar,0.0f, nr, ns, naxe, ncol)
            {}

            //Energy percentage constructor
            Energy_displayer    (float nperc=0.0f, const arrow::Vct &nr=DEF_POS, const arrow::Vct &ns=DEF_SZ
                                ,bool naxe=DEF_AXE,const sf::Color &ncol=DEF_COL)
            :Energy_displayer(nullptr,nperc, nr, ns, naxe, ncol)
            {}

        /*Energy*/
        public:

            //Set the pointer to the energy bar, set it to null if no energy is provided
            void set_ener (const Energy *npbar=nullptr)
            {
                pbar=npbar;
            }

            //Update the percentage

            //Set the percentage to be displayed with the given value
            void set_perc (float nperc)
            {
                perc=nperc;
            }

            //Set the percentage using the energy bar in the pointer
            void set_perc ()
            {
                if (pbar)
                    set_perc(pbar->get_perc());
            }

            //Get the energy
            float get_perc() const
            {
                return perc;
            }

        /*Draw*/
        public:

            //Draw the bar
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

    //HUD for the player's spaceship
    class HUD_player : public sf::Drawable
    {
        /*Configuration*/
        private:

            //Degradation of percentage of energy
            static constexpr float DEG_ENER=1.0f-0.01;

            //Tick update
            static constexpr unsigned int TICK_UPD=10000u;

            //Positive delta init coef
            static constexpr float COEF_POS_INIT=0.4;

            //Percentage to reset
            static constexpr float PERC_RESET=0.01f;

            //Max percentage of change
            static constexpr float PERC_MAX=0.07f;

            //Percentage of full/half bar
            static constexpr float PERC_BAR_FULL=0.5f;

            //Percentage of half/low bar
            static constexpr float PERC_BAR_HALF=0.25f;

        /*Attributes*/
        private:

            //Spaceship to be displayed
            const Spaceship& spshp;

            //Polygon
            sf::ConvexShape background,cross,speed[2];

            //Bars
            Energy_displayer ener_bar,speed_bar;

            //Value cache

            //Energy
            float prev_ener;//Previous energy percentage
            float delta_ener;//Change on the energy level

        /*Copy control, constructors*/
        public:

            //Complete constructor
            HUD_player  (
                        const Spaceship& nspshp,
                        int rdcornerx, int rdcornery,
                        int tamx1, int tamx2, int tamy,
                        int xpos1, int xpos2, int ypos1, int ypos2,
                        int sziconx, int szbarx, int szy
                        );

        /*Icons, bars, background*/

        //Icons
        private:

            //Put the points on the cross
            void put_cross_points(int tamx, int tamy);

            //Put the points on the arrow
            void put_speed_points(int tamx, int tamy);

        //Background
        private:

            //Put the points on the backgrond
            void put_background_points(int tamx1,int tamx2,int tamy);

        /*Update*/
        public:

            //Update the HUD bars and icons automatically
            void update(unsigned int tick);

        /*Draw*/
        public:

            //Draw the HUD
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

    class Text_displayer : public sf::Drawable
    {
        /*Configuration*/
        private:

            static constexpr unsigned int DEF_LINES=5u;//Default number of lines
            static constexpr unsigned int DEF_TIME=1000000u;//Time to delete 1 lines
            static constexpr unsigned int DEF_SEP_LINES=10u;//Separation between lines

        /*Attributes*/
        private:

            std::deque<sf::Text> texts;//Texts being displayed
            unsigned int t;//Time to delete a text
            const sf::Font &font;//Font for the text
            int x, y;//Position on the screen

        /*Constructors, copy control*/
        public:

            Text_displayer(const sf::Font &ifont, int ix, int iy)
            :texts(),t(0),font(ifont),x(ix),y(iy)
            {}

        /*Text*/
        private:

            void delete_text()
            {
                if (!texts.empty())
                    texts.pop_back();
            }

            //Update the y position
            void upd_y()
            {
                unsigned int sep=0;
                for (unsigned int cnt=0;cnt<texts.size();++cnt)
                {
                    sf::Text& that=texts.at(cnt);
                    that.setPosition(that.getPosition().x,that.getPosition().y+sep);
                    sep=DEF_SEP_LINES+that.getLocalBounds().height;
                }
            }

        /*Interface*/
        public:

            //Update
            void update(unsigned int tick)
            {
                t+=tick;
                while (t>=DEF_TIME)
                {
                    delete_text();
                    t-=DEF_TIME;
                }
            }

            void add_text(const sf::Text& text);

        /*Draw*/
        public:

            //Draw the texts
            void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                for (const sf::Text& that : texts)
                    target.draw(that,states);
            }
    };

    class HUD_mode : public sf::Drawable
    {
        /*Setting*/
        private:

            static constexpr unsigned int DEF_RAD_RANGE=2500u;

        /*Attributes*/
        private:

            Mode& m;

            sf::CircleShape rad;
            sf::ConvexShape ally_icon,ene_icon;
            sf::ConvexShape background;
            sf::Sprite clock_icon;
            const sf::Font& ft;

            arrow::Vct center;
            int t_x,t_y;
            int jump_y;
            int txt_sz;
        /*Constructors, copy control*/
        public:

            HUD_mode(Mode& im, const sf::Texture& clock_txture, const sf::Font &ift, int ldcornerx, int ldcornery, int tamx1, int tamx2, int tamy, int radx, int rady, int radsz, int posx, int posy, int sepy, int sz)
            :m(im),rad(radsz),ally_icon(4),ene_icon(4),background(4),clock_icon(),ft(ift),center(ldcornerx+radx+radsz,ldcornery-tamy+rady+radsz),t_x(posx+2*sz+ldcornerx),t_y(posy+ldcornery-tamy),jump_y(sz+sepy),txt_sz(sz)
            {
                sf::Color cr=teams.get_neutral();
                cr.a=127;

                rad.setFillColor(cr);
                rad.setPosition(ldcornerx+radx,ldcornery-tamy+rady);

                background.setPoint(0,sf::Vector2f(ldcornerx,ldcornery));
                background.setPoint(1,sf::Vector2f(ldcornerx+tamx2,ldcornery));
                background.setPoint(2,sf::Vector2f(ldcornerx+tamx1,ldcornery-tamy));
                background.setPoint(3,sf::Vector2f(ldcornerx,ldcornery-tamy));

                sf::Color c=teams.get_friendly();
                c.a=31;
                background.setFillColor(c);

                put_icons(sz/2,sz/3);
                ally_icon.setPosition(ldcornerx+posx,ldcornery-tamy+posy+sepy+sz);
                ene_icon.setPosition(ldcornerx+posx,ldcornery-tamy+posy+2*(sepy+sz));
                ally_icon.setFillColor(teams.get_friendly());
                ene_icon.setFillColor(teams.get_enemy());

                clock_icon.setTexture(clock_txture);
                clock_icon.setPosition(ldcornerx+posx,ldcornery-tamy+posy);
            }

        private:

            void put_icons(int tamx, int tamy)
            {
                ally_icon.setPoint(0,sf::Vector2f(1*tamx,0*tamy));
                ally_icon.setPoint(1,sf::Vector2f(2*tamx,3*tamy));
                ally_icon.setPoint(2,sf::Vector2f(1*tamx,2*tamy));
                ally_icon.setPoint(3,sf::Vector2f(0*tamx,3*tamy));

                ene_icon.setPoint(0,sf::Vector2f(1*tamx,0*tamy));
                ene_icon.setPoint(1,sf::Vector2f(2*tamx,3*tamy));
                ene_icon.setPoint(2,sf::Vector2f(1*tamx,2*tamy));
                ene_icon.setPoint(3,sf::Vector2f(0*tamx,3*tamy));
            }

        /*Draw*/
        public:

            //Draw the HUD
            void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                target.draw(background,states);

                if (m.has_radar()&&m.get_player())
                {
                    target.draw(rad,states);

                    arrow::Vct c(m.get_player()->get_center());

                    std::list<const Target *> l=m.get_targets();

                    sf::CircleShape cir(4);
                    cir.setOrigin(4,4);
                    for (auto it=l.begin();it!=l.end();++it)
                    {
                        const Target* t=*it;
                        cir.setFillColor(teams.get_team_color(t->team()));
                        arrow::Vct r(t->crl().get_center()-c);
                        if (r.mod()>DEF_RAD_RANGE)
                            r.limmod(DEF_RAD_RANGE);

                        r*=(1.0f/DEF_RAD_RANGE);
                        r*=rad.getRadius();

                        r+=center;
                        cir.setPosition(r.x,r.y);
                        target.draw(cir,states);
                    }
                }

                int y=t_y;
                if (m.has_time())
                {

                    target.draw(clock_icon);
                    std::ostringstream ss;
                    ss<<m.get_minutes()<<':';
                    if (m.get_seconds()<10)
                        ss<<'0';
                    ss<<m.get_seconds();
                    sf::Text text(ss.str(),ft,txt_sz);
                    text.setPosition(t_x,y);
                    text.setFillColor(teams.get_neutral());
                    text.setOutlineColor(teams.get_neutral());
                    target.draw(text);
                    y+=jump_y;
                }

                if (m.has_friendly())
                {
                    target.draw(ally_icon);
                    std::ostringstream ss;
                    ss<<m.get_current_friend()<<"/"<<m.get_max_friend();
                    sf::Text text(ss.str(),ft,txt_sz);
                    text.setPosition(t_x,y);
                    text.setFillColor(teams.get_friendly());
                    text.setOutlineColor(teams.get_friendly());
                    target.draw(text);
                    y+=jump_y;
                }

                if (m.has_enemies())
                {
                    target.draw(ene_icon);
                    std::ostringstream ss;
                    ss<<m.get_current_enemy()<<"/"<<m.get_max_enemy();
                    sf::Text text(ss.str(),ft,txt_sz);
                    text.setPosition(t_x,y);
                    text.setFillColor(teams.get_enemy());
                    text.setOutlineColor(teams.get_enemy());
                    target.draw(text);
                    y+=jump_y;
                }

                if (m.has_score())
                {
                    std::ostringstream ss;
                    ss<<"Score: "<<m.get_score();
                    sf::Text text(ss.str(),ft,txt_sz);
                    text.setPosition(t_x+400,50);
                    text.setFillColor(teams.get_neutral());
                    text.setOutlineColor(teams.get_neutral());
                    target.draw(text);
                }
            }
    };
}}//End of namespace

#endif // _FDX_GRAPHICS_H_
