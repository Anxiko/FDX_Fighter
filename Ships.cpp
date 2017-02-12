/*
 * Ships.cpp
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
* FDX_Fighter - Ships.cpp
* Little spaceship game made with C++ and SFML
* Ship resources - code file
*/

/*
    Version 0.1 (dd/mm/yy, 23/03/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

//Header file
#include "Ships.hpp"

/* Defines */

/*Constants*/

/*Macros*/

namespace fdx { namespace fighter
{
	/*
		Code
	*/

	/* Initializations */

	//Default control type
	bool Spaceship::reset_ctrl=false;
	const sf::Font* Spaceship::ft=nullptr;

    //RNG of AI_spaceship
    std::uniform_real_distribution<double> AI_spaceship::RNG_D(0,1);//Generates random doubles
    std::default_random_engine AI_spaceship::RE;//Engine for the random numbers

	/* Functions */

	/* Methods */

	/*Spacehip configuration*/

	//Copy control, constructors

	//Assignment operator
    Spshp_config& Spshp_config::operator= (const Spshp_config &nc)
    {
        max_a=nc.max_a;
        max_s=nc.max_s;
        delta_a=nc.delta_a;
        min_turn_s=nc.min_turn_s;
        delta_turn_s=nc.delta_turn_s;
        return *this;
    }

	/*Spaceship*/

	//Copy control, constructors

	//Assignment operator
    Spaceship& Spaceship::operator= (const Spaceship &nspaceship)
    {
        shield=nspaceship.shield;
        dir=nspaceship.dir;
        a=nspaceship.a;
        s=nspaceship.s;
        cfg=nspaceship.cfg;
        const sf::Texture *pointer;
        if ((pointer=nspaceship.sp.getTexture()))
            create_sprite(*pointer);
        else
            sp=sf::Sprite();
        return *this;
    }

    //Sprite

    //Draw the sprite
    void Spaceship::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (!alive())
            return;

        sf::CircleShape circle;
        circle.setOrigin(shield.get_size(),shield.get_size());
        circle.setPosition(shield.get_pos_center().x,shield.get_pos_center().y);
        circle.setRadius(shield.get_size());
        sf::Color tc=teams.get_team_color(t);
        tc.a=127;
        circle.setOutlineColor(tc);
        circle.setOutlineThickness(5);
        tc.a*=e.get_perc();
        circle.setFillColor(tc);
        target.draw(circle,states);
        if (prop)
            target.draw(*prop,states);
        target.draw(sp,states);
        if (ind)
            target.draw(*ind,states);
        if (ft&&(!name.empty()))
        {
            sf::Text text(name,*ft,20);
            text.setFillColor(teams.get_team_color(t));
            text.setOutlineColor(teams.get_team_color(t));
            text.setPosition(get_center().x,get_center().y-shield.get_size()-15);

            //Center the text
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

            target.draw(text,states);
        }
    }

	//Movement

	//Make the spaceship point at the given point
    void Spaceship::point(const arrow::Vct &target)
    {
        crosshair=target;
    }

    //Accelerate the ship with its thrusters
    void Spaceship::acc_thrusters(unsigned int tick)
    {
        a+=cfg.get_delta_a()*tick;
        if (a>cfg.get_a_limit())
            a=cfg.get_a_limit();
    }

    //Moves the ship

    //Move the ship without modifying the speed
    void Spaceship::mov(unsigned int tick)
    {
        //Turn the ship to its objective

        //Get the new direction
        arrow::Vct new_dir(crosshair-shield.get_pos_center());//New direction that the spaceship should have
        if (!new_dir)//Give a value to new_dir if it's null
            new_dir=arrow::Vct::mk_ang_mod(DEF_DIR,1.0);
        else//If the new_dir is not null, make it unitary
            new_dir.unitary();

        //Apply the turn
        dir+=new_dir*(cfg.turn(s.mod())*tick);//Turn the spacehip
        dir.unitary();//The vector must be unitary
        sp.setRotation(-RAD_TO_DEG(dir.angle()));//Set the rotation of the vector

        //Automatic controls
        if (controller)
        {
            set_acc(controller->ctrl_acc());
            set_brake(controller->ctrl_brake());
            set_fire(controller->ctrl_fire());
            point(arrow::Vct(controller->point_x(),controller->point_y()));
        }

        //Respond to controls
        if (ctrl_accelerate)//Accelerate
            acc_thrusters(tick);
        else
        {
            a=0;
            if (ctrl_brake)//Brake
                dec_brake(tick);
        }
        //Refresh or fire the laser
        if (weapon)
        {
            if (ctrl_fire)
                fire();
            else
                weapon->upd_refresh(tick);
        }

        //Increase the speed
        if (a)
            s+=arrow::Vct(dir,a)*(static_cast<arrow::Vct::Mod>(tick));

        //Limit the speed
        if (s.mod()>cfg.get_s_limit())
            s.limmod(cfg.get_s_limit());

        //Move the ship
        if (s)
        {
            shield.mov(s*(static_cast<arrow::Vct::Mod>(tick)));
            sp.setPosition(shield.get_pos_center().x,shield.get_pos_center().y);
        }

        //Friction
        if (!(a)||(ctrl_brake))
            dec_friction(tick);

        //Refresh or fire the laser
        if (weapon)
        {
            if (ctrl_fire)
                fire();
            weapon->upd_refresh(tick);
        }

        //Recharge energy
        recharge(tick);

        //Position the propeller's fire on its place
        upd_propeller(tick);

        //Take damage
        take_damage(tick);

        //Update the damage indicator
        if (ind)
        {
            ind->update(tick);
            ind->set_center(get_center());
        }

        //Reset controls if needed
        if (reset_ctrl)
            ctrl_accelerate=ctrl_brake=ctrl_fire=false;
    }

    //Energy, lasers

    //Recharge energy
    void Spaceship::recharge(unsigned int tick)
    {
        unsigned int add_energy=0;//Energy to add
        tick=r.safe_add_value(tick);//Fill the recharge bar
        if (r.full())//If the reload bar is full
        {
            ++add_energy;//One unit more to add
            r.empty_bar();//Empty the bar
        }
        if (tick)//Check how many more full energy units can be generated
        {
            add_energy+=tick/r.get_max_value();
            tick=tick%r.get_max_value();
            r.add_value(tick);
        }

        //Add the energy
        e.add_value(add_energy);
    }

    /*Spaceship's propeller*/

    //Sprite

    //Set the position of the propeller relative to the spaceship's position
    void Propeller::set_position(const arrow::Vct &ncenter, const arrow::Vct &ndir, arrow::Vct::Mod nsize)
    {
        //Calculate the position of the propeller
        arrow::Vct p(rel_pos);//Copy the relative position

        p.rot(ndir.angle());//Position relative to the direction the spaceship is pointing at
        p*=nsize;//Position relative to the spaceship's size
        p+=ncenter;//Position relative to the spaceship's position (its center)

        sp.setPosition(p.x,p.y);//Set the position of the sprite
        sp.setRotation(-RAD_TO_DEG(ndir.angle()));//Set the rotation of the vector
    }

    //Set the size of the fire according to the acceleration's percentage and tick
    void Propeller::set_size(float a_perc, unsigned int tick)
    {
        if (a_perc==1)
        {
            tick=timer.safe_add_value(tick);
            if (tick)
            {
                timer.empty_bar();
                timer.add_value(tick%timer.get_max_value());
            }
            sp.setScale((1-scale)*timer.get_perc()+scale,1);
        }
        else
            sp.setScale(a_perc*scale,1);
    }

    /*Cannon*/

    //Fire

    //Fire the laser
    void Cannon::fire(const arrow::Vct &ncenter, const arrow::Vct &ndir, arrow::Vct::Mod nsize)
    {
        if (ready())//Fire if the cannon is ready
        {
            snd.play();

            //Calculate the position at which the laser will be fired
            arrow::Vct p(rel_pos);//Copy the relative position

            p.rot(ndir.angle());//Position relative to the direction the spaceship is pointing at
            p*=nsize;//Position relative to the spaceship's size
            p+=ncenter;//Position relative to the spaceship's position (its center)

            //Insert the laser in the array
            lasers->insert_laser(lm,p,ndir);

            //Consume energy
            power->add_value(-lm.get_cst());

            //Overheat the weapon
            refresh.empty_bar();
        }
    }

    /*AI_spaceship*/

    //AI interface

    //Go to the next AI state
    void AI_spaceship::think()
    {
        if (!enemy_spshp)
        {
            state=AI_state::SEARCHING;
            return;
        }

        float dist=(enemy_spshp->get_center()-get_center()).mod();
        float ener=get_energy()->get_perc();

        switch(state)
        {
            case AI_state::NONE:
                break;

            case AI_state::SEARCHING:
            {
                if (enemy_spshp)
                {
                    if(dist<dist_detect)
                        state=AI_state::MOVING;
                }
                break;
            }

            case AI_state::MOVING:
            {
                if (dist<dist_close)
                    state=AI_state::FIGHTING;
                else if (dist>dist_detect)
                    state=AI_state::SEARCHING;
                break;
            }

            case AI_state::FIGHTING:
            {
                if (ener<perc_ener_fight)
                    state=AI_state::ESCAPING;
                else if (dist>dist_close)
                    state=AI_state::MOVING;
                break;
            }

            case AI_state::ESCAPING:
            {
                if (ener>perc_ener_esc)
                    state=AI_state::SEARCHING;
                else if (dist>dist_detect)
                    state=AI_state::REPAIRING;
                break;
            }

            case AI_state::REPAIRING:
            {
                if (dist<dist_detect)
                    state=AI_state::ESCAPING;
                else if (ener>perc_ener_esc)
                    state=AI_state::SEARCHING;
                break;
            }

            default:
                break;
        }
    }

    //Move the spaceship
    void AI_spaceship::play()
    {
        set_acc(false);
        set_brake(false);
        set_fire(false);

        switch(state)
        {
            case AI_state::NONE:
                break;

            case AI_state::SEARCHING:
            {
                if ((rnd_target-get_center()).mod()<dist_close)
                    rnd_target=arrow::Vct::mk_ang_mod(RNG_D(RE)*6.28,RNG_D(RE)*(dist_detect-dist_close));
                point(rnd_target);
                set_acc();
                break;
            }

            case AI_state::MOVING:
            {
                point(enemy_spshp->get_center());
                set_acc();
                break;
            }

            case AI_state::FIGHTING:
            {
                set_brake(false);
                set_fire();
                break;
            }

            case AI_state::ESCAPING:
            {
                arrow::Vct d(enemy_spshp->get_center()-get_center());
                if (d)
                    d.inv_dir();
                else
                    d=arrow::Vct();

                point(get_center()+d);
                set_acc();
                break;
            }

            case AI_state::REPAIRING:
            {
                break;
            }

            default:
                break;
        }
    }
}}//End of namespace

