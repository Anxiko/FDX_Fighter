/*
 * Laser.cpp
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
    FDX_Fighter - Laser.cpp
    Little spaceship game made with C++ and SFML
    Laser shot implementation - code file
*/

/*
    Version Nº nversion (dd/mm/yy, 29/04/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

//Header file
#include "Laser.hpp"

namespace fdx{ namespace fighter
{
    /*
        Functions
    */

    /*Laser_Model*/

    //Laser methods

    //Laser factory
    Laser Laser_Model::create_laser (arrow::Vct::Mod dir, const arrow::Vct& ir) const
    {
        return Laser(arrow::Vct::mk_ang_mod(dir,get_s()),ir,*this);
    }

    //Draw the given laser
    void Laser_Model::draw(sf::RenderTarget& target, sf::RenderStates states, const Laser& l) const
    {
        //Put the circle's center on the laser's position
        circle.setPosition(l.get_r().x,l.get_r().y);
        //Draw the laser
        target.draw(circle,states);
    }

    /* Laser */

    //Movement

   //Move the laser by its speed
    void Laser::mov(unsigned int tick)
    {
        if (!alive)
            return;

        r.mov(v*static_cast<arrow::Vct::Mod>(tick));
        talive+=tick;
        if (talive>=lm.get_TTL())
            alive=false;
    }

    //Collision

    //Damage that this laser will do to the circle in the given time value. Set the dmg to zero at hit if specified, return true/false if hit/not hit
    bool Laser::hit (Target& tar, unsigned int tick, bool zero_dmg)
    {
        //Check if the laser can do damage
        if (!((alive)&&(tar.alive()&&(tar.team().enemy(lm.get_team())))))
            return false;

        //Get the time of collision
        arrow::Vct::Mod t=tth(tar.shp(),tick);

        //Check if it will hit
        if (t<0||t>1)//No hit in this movement
            return false;

        //The laser will hit. Set its damage to zero if needed, and hit the target
        tar.dmg(lm.get_dmg());
        if (zero_dmg) alive=false;
        return true;
    }

    /*Array_Laser*/

    //Draw

    //Draw the particle
    void Array_Laser::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        //Draw the lasers
        for (const Laser &that : lasers)
            that.draw(target,states);
    }

    //Array management

    //Delete used lasers
    void Array_Laser::delete_laser()
    {
        for (auto it=lasers.begin();it!=lasers.end();)
        {
            if (it->active())//Laser still is active
                ++it;
            else//Used laser, delete
                it=lasers.erase(it);
        }
    }

    //Movement

    //Move the lasers the given time in microseconds
    void Array_Laser::mov(unsigned int t)
    {
        for (auto it=lasers.begin();it!=lasers.end();)
        {
            it->mov(t);//Move the laser
            if (it->get_dmg())//If the laser is active, move to the next laser
                ++it;
            else//If the laser is inactive, delete it and continue
                it=lasers.erase(it);
        }
    }


    //Collision

    //Damage that this laser will do to the target in the given time value. Set the dmg to zero at hit if specified
    int Array_Laser::hit (Target &it, unsigned int tick, bool zero_dmg)
    {
        int total_dmg=0;
        for (Laser &that : lasers)
            total_dmg+=that.hit(it ,tick,zero_dmg);
        return total_dmg;
    }

    /*Energy*/

    //Set

    //Check if the value is adequate
    void Energy::chk_value()
    {
        if (value>static_cast<int>(max_value))
            value=max_value;
        else
        {
            if (value==0)
                value=VALUE_AT_ZERO;
            else
                if (value<0)
                    value=VALUE_WHEN_DEAD;
        }
    }

    //Safe add

    //Safe add value
    unsigned int Energy::safe_add_value (int nsadd)
    {
        int rv;//Return value
        if (nsadd>static_cast<int>(get_max_value()))//Bar will overflow
            rv=nsadd-get_max_value();
        else//Bar won't overflow
            rv=0;
        add_value(nsadd);
        return rv;
    }

}}//End of namespace
