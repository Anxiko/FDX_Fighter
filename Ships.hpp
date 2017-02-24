/*
 * Ships.hpp
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
* FDX_Fighter - Ships.hpp
* Little spaceship game made with C++ and SFML
* Ship resources - Header file
*/

/*
    Version 0.1 (dd/mm/yy, 23/03/2015 -> )
*/

/*
    Preprocessor
*/

//Header guard
#ifndef _FDX_SHIPS_H_
#define _FDX_SHIPS_H_

/* Includes */

//Physics resources
#include "FDX_Geo/FDX_Geo.hpp"

//Lasers
#include "Laser.hpp"

//SFML graphics
#include "SFML/Graphics.hpp"

#include "SFML/Audio.hpp"

//Team colors
#include "Teams.hpp"

//Controllers
#include "System.hpp"

//RNG
#include <random>

//Double ended queue
#include <deque>

/* Defines */

/*Constants*/

/*Macros*/

//Angle conversions

//Convert from radians to degrees
#define RAD_TO_DEG(rad) (rad*(180/PI))

//Convert from degrees to radians
#define DEG_TO_RAD(deg) (deg*(PI/180))

namespace fdx { namespace fighter
{
    /*
        Class definitions
    */

    //Spacehip configuration
    class Spshp_config;

    //Spaceship
    class Spaceship;

    //Propeller
    class Propeller;

    //Laser cannon
    class Cannon;

    //Damage indicator
    class Damage_ind;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    //Coefficient type
    typedef arrow::Vct::Mod Coef;

    /* Classes */

    //Spacehip configuration
    class Spshp_config
    {

        /*Default values*/
        private:

            //Default coefficient
            static constexpr Coef DEF_MAX_A=5e-10,DEF_MAX_S=5e-4;//Default maximum acceleration and speed
            static constexpr Coef DEF_DELTA_A=1e-15;//Default acceleration increase
            static constexpr Coef DEF_MIN_TURN_S=3e-6,DEF_DELTA_TURN_S=2e-6;//Default turn speeds
            static constexpr Coef DEF_FRICTION_COEF=(1-1e-6),DEF_BRAKE_COEF=(1-3e-6);//Default speed reduction coefficients

        /*Attributes*/
        public:

            //Movement parameters
            Coef max_a,max_s;//Max acceleration and maximum speed
            Coef delta_a;//Acceleration increse
            Coef min_turn_s,delta_turn_s;//Minumum turn speed and increment on turn speed when the ship is not moving
            Coef friction_coef,brake_coef;//Speed reduction coefficientes for normal and brake conditions

        /*Constructor, copy control*/
        public:

            //Contructor
            Spshp_config(Coef nmax_a, Coef nmax_s, Coef ndelta_a, Coef nmin_turn_s, Coef ndelta_turn_s, Coef nfriction_coef, Coef nbrake_coef)
            :max_a(nmax_a),max_s(nmax_s),delta_a(ndelta_a),min_turn_s(nmin_turn_s), delta_turn_s(ndelta_turn_s),friction_coef(nfriction_coef),brake_coef(nbrake_coef)
            {}

            //Default constructor
            Spshp_config()
            :Spshp_config(DEF_MAX_A,DEF_MAX_S,DEF_DELTA_A,DEF_MIN_TURN_S,DEF_DELTA_TURN_S,DEF_FRICTION_COEF,DEF_BRAKE_COEF)
            {}

            //Copy constructor
            Spshp_config(const Spshp_config &nc)
            :Spshp_config(nc.max_a,nc.max_s,nc.delta_a,nc.min_turn_s,nc.delta_turn_s,nc.friction_coef,nc.brake_coef)
            {}

            //Assignment operator
            Spshp_config& operator= (const Spshp_config &nc);

        /*Turn*/
        public:

            //Returns the maximum turn speed that can perform the spaceship at the given speed
            Coef turn(Coef speed) const
            {
                return min_turn_s+delta_turn_s*(1-(speed/max_s));
            }

        /*Limit*/
        public:

            //Get the limit

            //Speed limit
            Coef get_s_limit() const
            {
                return max_s;
            }

            //Acceleration limit
            Coef get_a_limit() const
            {
                return max_a;
            }

        /*Acceleration*/
        public:

            //Get the acceleration increase
            Coef get_delta_a() const
            {
                return delta_a;
            }

        /*Reduce speed*/
        public:

            //Friction decceleration
            Coef red_frict() const
            {
                return friction_coef;
            }

            //Brake decceleration
            Coef red_brake() const
            {
                return brake_coef;
            }
    };

    //Spaceship's propeller
    class Propeller: public sf::Drawable
    {
        /*Default configuration*/
        private:

            //Default full oscilation time
            static constexpr unsigned int DEF_TIME=100u;

            //Default full oscilation scale
            static constexpr float DEF_FULL_SCALE=0.8f;

        /*Attributes*/
        private:

            //Fire's sprite
            sf::Sprite sp;

            //Relative position
            arrow::Vct rel_pos;

            //Full acceleration oscilation timer
            Energy timer;

            //Full oscilation scale
            float scale;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Propeller(const sf::Texture &txture, const arrow::Vct &nrel_pos, unsigned int t=DEF_TIME, float nscale=DEF_FULL_SCALE)
            :sp(txture), rel_pos(nrel_pos), timer(t,0), scale(nscale)
            {
                sf::Vector2u sz=txture.getSize();
                sp.setOrigin(sz.x,sz.y*0.5);
            }

            //Copy constructor
            Propeller(const Propeller &nprop)
            :Propeller(*nprop.sp.getTexture(),nprop.rel_pos,nprop.timer.get_max_value(),nprop.scale)
            {}

        /*Sprite*/
        public:

            //Set the position of the propeller relative to the spaceship's position
            void set_position(const arrow::Vct &ncenter, const arrow::Vct &ndir, arrow::Vct::Mod nsize);

            //Draw the sprite
            void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                target.draw(sp,states);
            }

            //Set the size of the fire according to the acceleration's percentage
            void set_size(float a_perc, unsigned int tick);
    };

    //Laser cannon
    class Cannon
    {
        /*Default configuration*/
        private:

            static constexpr unsigned int DEF_REFT=100000u;//Default refresh time


        /*Attributes*/
        private:

            const Laser_Model &lm;
            arrow::Vct rel_pos;//Position from where the lasers are fired (relative to the center, size and rotation of the spaceship)
            Array_Laser *lasers;//Array where to insert the lasers

        public:

            Energy *power;//Power bar for the laser
            Energy refresh;//Refresh bar (on microseconds)
            sf::Sound snd;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Cannon(const Laser_Model &nlm, const arrow::Vct &nrel_pos, Array_Laser *nlasers=nullptr, Energy *npower=nullptr,unsigned int nrefresh=DEF_REFT)
            :lm(nlm),rel_pos(nrel_pos),lasers(nlasers),power(npower),refresh(nrefresh)
            {}

            //Copy constructor
            Cannon(const Cannon &nc)
            :Cannon(nc.lm,nc.rel_pos,nc.lasers,nullptr,nc.refresh.get_max_value())
            {
                snd.setBuffer(*nc.snd.getBuffer());
            }

        /*Set*/
        public:

            //Set the Array_Laser to fire lasers at (delete it if called without argument)
            void set_lasers(Array_Laser *nlasers=nullptr)
            {
                lasers=nlasers;
            }

            //Set the power bar (delete it if called without argument)
            void set_power(Energy *npower=nullptr)
            {
                power=npower;
            }

            void put_sound(sf::SoundBuffer *sb)
            {
                snd.setBuffer(*sb);
            }

        /*Refresh*/
        public:

            //Check if the cannon is ready to fire
            bool ready() const
            {
                return ((lasers)&&(power)&&(power->get_value()>(static_cast<int>(lm.get_cst())))&&(refresh.full()));
            }

            //Update the refresh with the given ticks
            void upd_refresh(int mil)
            {
                refresh.add_value(mil);
            }

            //Get the refresh bar
            const Energy* get_refresh() const
            {
                return &refresh;
            }

        /*Fire*/
        public:

            //Fire the laser
            void fire (const arrow::Vct &ncenter, const arrow::Vct &ndir, arrow::Vct::Mod nsize);
    };

    //Damage indicator
    class Damage_ind : public sf::Drawable
    {
        /*Settings*/
        private:

            static constexpr unsigned int DEF_MAX_IND=3u;//Max number of damage indicators
            static constexpr unsigned int DEF_MAX_TIME=500000u;//Time to remove a damage indicator
            static constexpr unsigned int DEF_MAX_RADIUS=20u;//Radius to draw the damage
            static constexpr unsigned int DEF_MIN_RADIUS=15u;//Second min radius
            static constexpr unsigned int DEF_DESPL=20u;//Lateral movement

        /*Attributes*/
        private:

            std::deque<arrow::Vct::Mod> damages;//Damage indicators
            unsigned int t;//Time to remove
            arrow::Vct c;//Center of the ship

        /*Constructors, copy control*/
        public:

            //Default const
            Damage_ind()
            :damages(),t(0)
            {}

        /*Damage*/
        public:

            void update (unsigned int ticks)
            {
                t+=ticks;
                while (t>=DEF_MAX_TIME)
                {
                    take_damage();
                    t-=DEF_MAX_TIME;
                }
            }

            void put_damage(const arrow::Vct& point, const arrow::Vct& center)
            {
                if (damages.size()==DEF_MAX_IND)
                    take_damage();

                arrow::Vct::Mod angle=(point-center).angle();

                damages.push_front(angle);
            }

        private:

            void take_damage()
            {
                if (!damages.empty())
                    damages.pop_back();
            }

        /*Draw*/
        public:

            void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                sf::ConvexShape shape(3);
                shape.setFillColor(teams.get_enemy());

                for (auto it=damages.begin();it!=damages.end();++it)
                {
                    arrow::Vct t=arrow::Vct::mk_ang_mod(*it,1);
                    arrow::Vct n=arrow::Vct::mk_ang_mod(*it+3.14/2,1);

                    arrow::Vct p(t*static_cast<arrow::Vct::Mod>(DEF_MAX_RADIUS));

                    arrow::Vct  p1(t*static_cast<arrow::Vct::Mod>(DEF_MAX_RADIUS)),
                                p2(t*static_cast<arrow::Vct::Mod>(DEF_MIN_RADIUS)+n*static_cast<arrow::Vct::Mod>(DEF_DESPL)),
                                p3(t*static_cast<arrow::Vct::Mod>(DEF_MIN_RADIUS)-n*static_cast<arrow::Vct::Mod>(DEF_DESPL));

                    shape.setPoint(0,sf::Vector2f(p1.x,p1.y));
                    shape.setPoint(1,sf::Vector2f(p2.x,p2.y));
                    shape.setPoint(2,sf::Vector2f(p3.x,p3.y));
                    shape.setOrigin(-c.x,-c.y);

                    target.draw(shape,states);
                }
            }

        /*Center*/
        public:

            void set_center(const arrow::Vct& nc)
            {
                c=nc;
            }
    };

    //Spaceship
    class Spaceship : public sf::Drawable, public Target
    {
        /*Configuration*/
        private:

            static constexpr arrow::Vct::Mod DEF_DIR=0.0;//Default direction in which the spaceship points
            static bool reset_ctrl;//The controls are resetted after every movement (true) or not
            static constexpr unsigned int DEF_ENERGY=250u;//Default spaceship's energy
            static constexpr unsigned int DEF_RECHARGE=10000u;//Default ticks needed to regen a unit of energy
            const static sf::Font *ft;

        /*Attributes*/
        private:

            //Spaceship
            arrow::Crl shield;//Circular shield of the ship
            sf::Sprite sp;//Spaceship's sprite
            sf::Sprite fr;//Fire's sprite

            //Physics
            Coef a;//Acceleration (only its module is needed, the direction is hold in dir)
            arrow::Vct s;//Speed
            arrow::Vct dir;//Direction in which the spaceship is pointing right now
            arrow::Vct crosshair;//Point at which the spaceship must point

            //Controls
            bool ctrl_accelerate=false;//Accelerate the spaceship
            bool ctrl_brake=false;//Brake the spaceship
            bool ctrl_fire=false;//Fire the laser

            //Spaceship configuration
            Spshp_config cfg;

            //Team
            Team t;

            //Energy
            Energy e;

            //Laser cannon
            Cannon *weapon=nullptr;

            //Propeller
            Propeller *prop=nullptr;

            //Array of enemy lasers
            Array_Laser *lasers=nullptr;

            //Damage indicator
            Damage_ind *ind=nullptr;

            //Controller
            Inter_Controller *controller=nullptr;

            //Recharge energy
            Energy r;

            //Name
            std::string name;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Spaceship(const arrow::Vct &ncenter, const sf::Texture &txture, const Spshp_config &ncfg, const Team &nt, unsigned int ne=DEF_ENERGY, unsigned int nr=DEF_RECHARGE)
            :shield(ncenter,arrow::Vct(txture.getSize().x,txture.getSize().y).mod()/2)
            ,a(0.0),s(0.0,0.0),dir(arrow::Vct::mk_ang_mod(DEF_DIR,1.0)),crosshair(ncenter),cfg(ncfg),t(nt),e(ne),r(nr,0),name("")
            {
                create_sprite(txture);
            }

            //Constructor with default parameters
            Spaceship(const arrow::Vct &ncenter, const sf::Texture &txture)
            :Spaceship(ncenter,txture,Spshp_config(),Team())
            {}

            //Copy constructor
            Spaceship(const Spaceship& ispshp)
            :Spaceship(ispshp.get_center(),*ispshp.sp.getTexture())
            {}

            //Destructor
            virtual ~Spaceship()
            {
                del_cannon();
                del_propeller();
                del_ind();
            }

            //Assignment operator
            Spaceship& operator= (const Spaceship &nspaceship);

        /*Sprite*/
        private:

            //Sprite creation, create the sprite and set its position
            void create_sprite(const sf::Texture &txture)
            {
                sf::Vector2u txture_sz=txture.getSize();
                sp.setTexture(txture);
                sp.setOrigin(txture_sz.x*0.5,txture_sz.y*0.5);
                sp.setPosition(shield.get_pos_center().x,shield.get_pos_center().y);
            }

        public:

            //Draw the sprite
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        /*Controls*/
        public:

            //Accelerate the ship
            void set_acc(bool na=true)
            {
                ctrl_accelerate=na;
            }

            //Brake the ship
            void set_brake(bool nb=true)
            {
                ctrl_brake=nb;
            }

            //Fire the laser
            void set_fire(bool nf=true)
            {
                ctrl_fire=nf;
            }

            //Get the control type
            bool get_ctrl_type() const
            {
                return reset_ctrl;
            }

            //Set the control type
            void set_ctrl_type(bool nc)
            {
                reset_ctrl=nc;
            }

        /*Position*/
        public:

            //Get the center of the spaceship
            arrow::Vct get_center() const
            {
                return shield.get_pos_center();
            }

            //Get the tip of the laser
            arrow::Vct get_tip() const
            {
                return get_center()+dir*shield.get_size();
            }

        /*Movement*/
        private:

            //Accelerate the ship with its thrusters
            void acc_thrusters(unsigned int tick=1);

            //Decelerate the ship due to friction
            void dec_friction(unsigned int tick=1)
            {
                s*=std::pow(cfg.red_frict(),tick);
            }

            //Decelerate the ship due to brakes
            void dec_brake(unsigned int tick=1)
            {
                s*=std::pow(cfg.red_brake(),tick);
            }

        public:

            //Moves the ship

            //Move the ship
            void mov(unsigned int key=1);

            //Make the spaceship point at the given point
            void point(const arrow::Vct &target);

            //Get the percentage values

            //Get the percentage of acceleration
            float get_a_perc() const
            {
                return a/cfg.get_a_limit();
            }

            //Get the percentage of velocity
            float get_v_perc() const
            {
                return s.mod()/cfg.get_s_limit();
            }

        /*Energy, lasers*/
        private:

            //Recharge energy
            void recharge(unsigned int tick);

        public:

            //Revive the spaceship and change its position
            void revive_at (const arrow::Vct &nr)
            {
                e.revive();
                shield.set_pos_center(nr);
                sp.setPosition(shield.get_pos_center().x,shield.get_pos_center().y);
                mov(0);
            }

            //Revive the spaceship
            void revive()
            {
                e.revive();
                mov(0);
            }

            //Get the energy
            const Energy* get_energy() const
            {
                return &e;
            }

            //Get the refresh
            const Energy* get_refresh() const
            {
                if (weapon)
                    return weapon->get_refresh();
                return nullptr;
            }

            //Put a cannon on the ship (replaces the old cannon if there is one)
            void put_cannon(const Cannon &nc)
            {
                del_cannon();
                weapon=new Cannon(nc);
                weapon->set_power(&e);
            }

            //Deletes the cannon (if it exists)
            void del_cannon()
            {
                delete weapon;
                weapon=nullptr;
            }

            //Put the laser on the cannons
            void put_cannon_lasers (Array_Laser *al)
            {
                if (weapon)
                    weapon->set_lasers(al);
            }

            //Fire the cannon
            void fire()
            {
                if ((weapon)&&(weapon->ready()))
                    weapon->fire(shield.get_pos_center(),dir,shield.get_size());
            }

            //Take damage
            void take_damage(unsigned int tick)
            {
                if (lasers)
                {
                    if (!ind)
                        lasers->hit(*this,tick,true);
                    else
                    {
                        for (Laser &that : lasers->lasers)
                            if (that.hit(*this ,tick,true))
                                ind->put_damage(that.get_r(),get_center());
                    }
                }
            }

            //Put laser array
            void put_lasers (Array_Laser* nlasers)
            {
                lasers=nlasers;
            }

            //Delete the array of lasers
            void del_lasers()
            {
                lasers=nullptr;
            }

        /*Propeller*/
        public:

            //Put a propeller on the spaceship
            void put_propeller (const Propeller &nprop)
            {
                del_propeller();
                prop=new Propeller(nprop);
            }

            //Delete the propeller (if it exists)
            void del_propeller()
            {
                delete prop;
                prop=nullptr;
            }

        private:

            //Update the propeller
            void upd_propeller(unsigned int tick)
            {
                if (prop)
                {
                    prop->set_position(shield.get_pos_center(),dir,shield.get_size());
                    prop->set_size(get_a_perc(),tick);
                }
            }

        /*Damage indicator*/
        public:

            //Put the indicator
            void put_ind()
            {
                del_ind();
                ind=new Damage_ind();
            }

            //Delete the indicator
            void del_ind()
            {
                delete ind;
                ind=nullptr;
            }

        /*Target implementation*/
        public:

            //Add damage to the target
            void dmg(int d)
            {
                e.add_value(-d);
            }

            //Return the shape
            const arrow::Shp& shp() const
            {
                return shield;
            }

            //Return the team
            Team team() const
            {
                return t;
            }

            //Check if this target is alive
            bool alive() const
            {
                return e.alive();
            }

        /*Controller*/
        public:

            void set_controller (Inter_Controller *icontroller=nullptr)
            {
                controller=icontroller;
            }

        /*Naming*/
        public:

            void set_name(const std::string & iname)
            {
                name=iname;
            }

            static void set_font(const sf::Font * ift)
            {
                ft=ift;
            }

    };


    //AI for the spaceship
    class AI_spaceship : public Spaceship
    {
        /*States*/
        public:

            enum struct AI_state : unsigned char
            {
                NONE=0,//Enemy does nothing
                SEARCHING,//Search for an enemy (none found yet)
                MOVING,//Move towards an enemy (until reaching a distance close enough)
                FIGHTING,//Fight the enemy
                ESCAPING,//Low on energy, escape from the enemy
                REPAIRING,//Escaped from the enemy, get enough energy to fight him again
            };

        /*RNG*/
        private:

            static std::uniform_real_distribution<double> RNG_D;//Generates random doubles
            static std::default_random_engine RE;//Engine for the random numbers

        /*Default config*/
        private:

            static constexpr unsigned int DEF_DIST_DETECT=500u;//Default distance to detect the enemy
            static constexpr unsigned int DEF_DIST_CLOSE=150u;//Default distance to engage the enemy
            static constexpr float DEF_PERC_ENER_ESC=0.25f;//Default perc at which the spaceship will try to escape
            static constexpr float DEF_PERC_ENER_FIGHT=0.75f;//Percentage at which the spaceship will fight again after escaping

        /*Attributes*/
        private:

            //Distances

            unsigned int dist_detect;//Distance to detect the enemy
            unsigned int dist_close;//Distance at which the enemy is close enough to the target

            //Energy

            float perc_ener_esc;//Perc at which the spaceship will try to escape
            float perc_ener_fight;//Percentage at which the spaceship will fight again after escaping

            //Spacehips
            const Spaceship* enemy_spshp;//Enemy spaceship

            //State machine
            AI_state state;

            //Random target
            arrow::Vct rnd_target;

        /*Constructors, copy control*/
        public:

            //Default constructor
            AI_spaceship(const arrow::Vct& nr,const sf::Texture& txture, const Spshp_config& cfg, const Team& iteam, unsigned int idist_detect=DEF_DIST_DETECT, unsigned int idist_close=DEF_DIST_CLOSE, float iperc_ener_esc=DEF_PERC_ENER_ESC,float iperc_ener_fight=DEF_PERC_ENER_FIGHT)
            :Spaceship(nr,txture,cfg,iteam)
            ,dist_detect(idist_detect),dist_close(idist_close),perc_ener_esc(iperc_ener_esc),perc_ener_fight(iperc_ener_fight),enemy_spshp(nullptr),state(AI_state::SEARCHING),rnd_target()
            {}

            //Destructor
            ~AI_spaceship(){}

        /*AI interface*/
        public:

            //Go to the next AI state
            void think();

            //Move the spaceship
            void play();

            void set_enemy(const Spaceship* ienemy)
            {

                if (!ienemy) return;
                if ((!enemy_spshp)||(enemy_spshp->get_center()-get_center()).sq_mod()>(ienemy->get_center()-get_center()).sq_mod())
                    enemy_spshp=ienemy;
            }

            void delete_enemy()
            {
                enemy_spshp=nullptr;
            }
    };

}}//End of namespace

#endif // _FDX_SHIPS_H_

