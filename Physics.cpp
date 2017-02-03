/*
 * Physics.cpp
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
* FDX_Fighter - Physics.cpp
* Little spaceship game made with C++ and SFML
* Physic resources - code file
*/

/*
    Version 0.1 (dd/mm/yy, 20/3/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

//Headers file
#include "Physics.hpp"

/* Defines */

/*Constants*/

/*Macros*/


//Namespace
namespace fdx { namespace fighter
{
    /*
        Code
    */

    /* Functions */

    //Contact methods

    //Contact between a point and a circle
    bool collide (const Crl &c, const Pnt &p)
    {
        return (c.get_center()-p.get_center()).sq_mod()<=c.get_size()*c.get_size();
    }

    //Time that will take the point with a given speed to hit a circle
    arrow::Vct::Mod tth (const Pnt &p, const arrow::Vct &pv, const Crl &c)
    {
        //If they are alredy at contact, the tth is 0
        if (p.contact(c))
            return 0;
        else
            if(!(pv))//If they are not at contact and the speed is null, they will never meet
                return -1;

        //Get the distance and the componets of the speed
        arrow::Vct d(p.get_center()-c.get_center());
        arrow::Vct::Mod dx(d.mod_tan_part(pv));

        //Get the a, b and c coefficientes of the second degree ecuation
        double
                ac=pv.sq_mod(),
                bc=2*dx*pv.mod(),
                cc=d.sq_mod()-c.get_size()*c.get_size();

        //Get the value of the discriminant
        double disc=bc*bc-4*ac*cc;

        //Check that the 2nd degree equation has solution
        if (disc<0)//No solution
            return -1;//No contact
        else//Has solution
        {
            //Find the two solutions
            double
                    sol1=(-bc-std::sqrt(disc))/(2*ac),
                    sol2=(-bc+std::sqrt(disc))/(2*ac);
            double sol=std::min(sol1,sol2);//Find the final solution
            if (sol<0) return -1;//No contact return value
            return sol;
        }
    }
}}//End of namespace
