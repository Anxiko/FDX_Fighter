/*
 * Laser.hpp
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
    FDX_Fighter - Laser.hpp
    Little spaceship game made with C++ and SFML
    Laser shot implementation - header file
*/

/*
    Version Nº nversion (dd/mm/yy, 29/04/2015 -> )
*/

/*
    Preprocessor
*/

/*Header guard*/
#ifndef _FDX_LASER_H_
#define _FDX_LASER_H_


/* Includes */

//Physics resources
#include "Physics.hpp"

//SFML graphics
#include "SFML/Graphics.hpp"

//Teams
#include "Teams.hpp"

//STL lists
#include <list>

/* Defines */

/*Constants*/

/*Macros*/

namespace fdx { namespace fighter
{
    /*
        Class definitions
    */

    //Laser configuration
    class Laser_Model;

    //Laser
    class Laser;

    //Energy bar
    class Energy;

    //Laser array
    class Array_Laser;

    //Target that will get hit
    class Target;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    /* Classes */

    //Laser configuration
    class Laser_Model
    {
        /*Default config*/
        private:

            //Default TTL
            static constexpr long unsigned int DEF_TTL=1000000;//Default number of ticks that this laser will live before dissapearing
            //Default laser model
            static constexpr unsigned int DEF_DMG=10;//Default damage
            static constexpr unsigned int DEF_CST=3;//Default laser cost
            static constexpr float DEF_SPEED=1e-3;//Defaul speed
            //Default drawing
            static constexpr unsigned int DEF_QUAL=10;//Quality of the particle (sides of the fake circle)
            static constexpr unsigned int DEF_SZ=5;//Size of the particle


        /*Attributes*/
        private:

            //TTL
            long unsigned int TTL;//Ticks that this laser will live before dissapearing

            //Drawing
            mutable sf::CircleShape circle;//Circle to be drawn

            //Config

            arrow::Vct::Mod s;//Speed of the laser
            //Energy
            unsigned int cst,dmg;//Cost to fire the laser and damage at reciving it
            //Team of the laser
            Team team;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Laser_Model(long unsigned int iTTL, unsigned int isz, unsigned int iqual, arrow::Vct::Mod is, unsigned int icst, int idmg, const Team& iteam)
            :TTL(iTTL),circle(isz,iqual),s(is),cst(icst),dmg(idmg),team(iteam)
            {
                circle.setFillColor(teams.get_team_color(team));
                circle.setOrigin(isz,isz);
            }

            //Default constructor
            Laser_Model()
            :Laser_Model(DEF_TTL,DEF_SZ,DEF_QUAL,DEF_SZ,DEF_CST,DEF_DMG,Team())
            {}

        /*Get/set*/
        public:

            long unsigned int get_TTL() const
            {
                return TTL;
            }

            arrow::Vct::Mod get_s() const
            {
                return s;
            }

            unsigned int get_cst() const
            {
                return cst;
            }

            unsigned int get_dmg() const
            {
                return dmg;
            }

            const Team& get_team() const
            {
                return team;
            }

        /*Laser methods*/
        public:

            //Laser factory
            Laser create_laser (arrow::Vct::Mod dir, const arrow::Vct& ir) const;

            //Draw the given laser
            void draw(sf::RenderTarget& target, sf::RenderStates states, const Laser& l) const;
    };

    //Laser
    class Laser : public sf::Drawable
    {

        /*Configuration*/
        private:

            const Laser_Model& lm;

        /*Attributes*/
        private:

            //Speed of the laser
            arrow::Vct v;

            //Position of the laser
            Pnt r;

            //Time that this laser has been alive (updated when moving the laser)
            long unsigned int talive=0;

            //Laser is active
            bool alive=true;

        /*Constructors, copy control*/
        public:

            //Copy constructor
            Laser(const Laser&) = default;

            //Complete constructor
            Laser(const arrow::Vct& iv, const arrow::Vct& ir, const Laser_Model& ilm)
            :lm(ilm),v(iv),r(ir)
            {}

            //Laser model constructor
            Laser(arrow::Vct::Mod dir,const arrow::Vct &ir, const Laser_Model& ilm)
            :Laser(ilm.create_laser(dir,ir))
            {}

            //Assignment operator
            Laser& operator= (const Laser &nl) = default;

        /*Movement*/
        public:

            //Move the laser by its speed
            void mov(unsigned int tick=1);

            //Set and get

            //Get the position
            arrow::Vct get_r() const
            {
                return r.get_center();
            }

        /*Collision, damage*/
        public:

            //Time that will take to this laser to hit the circle (negative if the laser will miss)
            arrow::Vct::Mod tth (const Crl &ic, arrow::Vct::Mod scale=1.0) const
            {
                return r.tth(ic,v*scale);
            }

            //Damage that this laser will do to the circle in the given time value. Set the dmg to zero at hit if specified, return true/false if hit/not hit
            bool hit (Target& tar, unsigned int tick, bool zero_dmg);

            //Check if the laser is active
            bool active() const
            {
                return alive;
            }

            //Check the damage of this laser
            int get_dmg() const
            {
                return lm.get_dmg();
            }

        /*Draw*/
        public:

            //Draw the particle
            void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                if (alive)
                    lm.draw(target,states,*this);
            }
    };

    //Array of lasers
    class Array_Laser : public sf::Drawable
    {
        /*Attributes*/
        public:

            //Array of lasers Lasers
            std::list<Laser> lasers;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Array_Laser ()
            :lasers()
            {}

        /*Draw*/
        public:

            //Draw the laser
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;


        /*Array management*/

        public:

            //Insert a laser using the laser model
            void insert_laser(const Laser_Model &nmodel, arrow::Vct &nr, const arrow::Vct &ndir)
            {
                lasers.emplace_back(ndir.angle(),nr,nmodel);
            }

            //Insert a given laser
            void insert_laser(const Laser &nlaser)
            {
                lasers.push_back(nlaser);
            }

            //Delete used lasers
            void delete_laser();

        /*Movement*/
        public:

            //Move the lasers the given time in microseconds
            void mov(unsigned int tick=1);

        /*Collision*/
        public:

            //Damage that this laser will do to the circle in the given time value. Set the dmg to zero at hit if specified
            int hit (Target &it, unsigned int tick, bool zero_dmg);

    };

    //Energy bar
    class Energy
    {
        /*Default values*/
        public:

            static constexpr unsigned int DEF_MAX=100u;//Default maximum value

        private:

            static constexpr int VALUE_AT_ZERO=0;//When the bar reaches zero after a decrease, this value will substitute the old value
            static constexpr int VALUE_WHEN_DEAD=-1;//Value the bar will get when it's dead

        /*Attributes*/
        private:

            //Values
            unsigned int max_value;//Maximum value
            int value;//Current value

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Energy(unsigned int nmax_value, int nvalue)
            :max_value(nmax_value),value(nvalue>static_cast<int>(nmax_value)?nmax_value:nvalue)
            {}

            //Full bar constructor
            Energy(unsigned int nmax_value)
            :Energy(nmax_value,static_cast<int>(nmax_value))
            {}

            //Default constructor
            Energy()
            :Energy(DEF_MAX)
            {}

            //Porcentage complete constructor
            Energy(unsigned int nmax_value, float p)
            :Energy(nmax_value,static_cast<int>(nmax_value*p))
            {}

            //Porcentage default constructor
            Energy(float p)
            :Energy(DEF_MAX,p)
            {}

            //Copy constructor
            Energy(const Energy &ne)
            :Energy(ne.max_value,ne.value)
            {}

            //Assignment operator
            Energy& operator= (const Energy& ne)
            {
                max_value=ne.max_value;
                value=ne.value;
                return *this;
            }

        /*Alive/dead*/
        public:

            //Check if the bar is alive
            bool alive() const
            {
                return value>=0;
            }

            //Check if the bar is full
            bool full() const
            {
                return value==static_cast<int>(max_value);
            }

            //Revive

            //Revive the bar if it's dead (at the given value)
            void revive(int nvalue)
            {
                if (!alive())
                    value=nvalue;
            }

            //Revive the bar (at full health)
            void revive()
            {
                revive(max_value);
            }

            //Fill the bar
            void fill_bar()
            {
                value=max_value;
            }

            //Empty the bar
            void empty_bar()
            {
                value=0;
            }

        /*Get*/
        public:

            //Get the value
            int get_value() const
            {
                return value;
            }

            //Get the percentage
            float get_perc() const
            {
                return alive()?(static_cast<float>(value)/max_value):(0.0f);
            }

            //Get the max. value
            unsigned int get_max_value() const
            {
                return max_value;
            }

        /*Set*/
        private:

            //Check if the value is adequate
            void chk_value();

        public:

            //Set the value
            void set_value(int nvalue)
            {
                if (alive())
                {
                    value=nvalue;
                    chk_value();
                }
            }

            //Set the percentage
            void set_perc(float nperc)
            {
                set_value(max_value*nperc);
            }

            //Set the max. value
            void set_max_value(unsigned int nmax_value)
            {
                max_value=nmax_value;
            }

        //Add
        public:

            //Add value
            void add_value(int nadd)
            {
                set_value(get_value()+nadd);
            }

            //Add percentage
            void add_perc(float nadd_perc)
            {
                add_value(get_value()*nadd_perc);
            }

        //Safe add: if the bar overflows, return how much of it was not needed to full the bar
        public:

            //Value necessary to fill the bar
            unsigned int need_fill() const
            {
                return get_max_value()-get_value();
            }

            //Safe add value
            unsigned int safe_add_value (int nsadd);

            //Safe add percentage
            float safe_add_perc (float nsadd_perc)
            {
                return static_cast<float>(safe_add_value(nsadd_perc*get_max_value()))/get_max_value();
            }
    };

    //Target that will get hit
    class Target
    {
        /*Constructors, copy control*/

        protected:

            //Virtual destructor to be inherited
            virtual ~Target()
            {}

        /*Methods to implement*/

        public:

            //Add damage to the target
            virtual void dmg(int d) = 0;

            //Return the shape
            virtual const Crl& crl() const = 0;

            //Return the team
            virtual Team team() const = 0;

            //Check if this target is alive
            virtual bool alive() const = 0;

    };

}}//End of namespace

//End of library
#endif
