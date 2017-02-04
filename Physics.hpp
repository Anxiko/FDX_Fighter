/*
 * Physics.hpp
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
* FDX_Fighter - Physics.hpp
* Little spaceship game made with C++ and SFML
* Physic resources - header file
*/

/*
    Version 0.1 (dd/mm/yy, 20/3/2015 -> )
*/

/*
    Preprocessor
*/

//Header guard
#ifndef _FDX_PHYSICS_H_
#define _FDX_PHYSICS_H_

/* Includes */

//FDX 2D vectors
#include "FDX_Vct/FDX_Vct.hpp"

/* Defines */

/*Constants*/

/*Macros*/

//Angle conversions

//Convert from radians to degrees
#define RAD_TO_DEG(rad) (rad*(180/PI))

//Convert from degrees to radians
#define DEG_TO_RAD(deg) (deg*(PI/180))


//Namespace
namespace fdx { namespace fighter
{
    /*
        Class definitions
    */

    //Circle
    class Crl;

    //Point
    class Pnt;

    /*
        Function prototypes
    */

    //Contact between a circle and a point
    bool collide (const Crl &c, const Pnt &p);

    //Time that will take the point with a given speed to hit a circle
    arrow::Vct::Mod tth (const Pnt &p, const arrow::Vct &pv, const Crl &c);

    /*
        Data types
     */

    /* Typedefs */

    /* Classes */

    //Circle
    class Crl
    {
        /*Constants*/
        private:

            //Default circle
            static constexpr arrow::Vct::Mod DEF_S=0.0;//Default size
            static constexpr arrow::Vct::Coord DEF_X=0.0,DEF_Y=0.0;//Default X and Y coordinates

        /*Attributes*/
        private:

            arrow::Vct r;//Center of the circle
            arrow::Vct::Mod s;//Radius of the circle

        /*Constructors, copy control*/
        public:

            //Default constructor
            Crl()
            :r(DEF_X,DEF_Y),s(DEF_S)
            {}

            //Coordinate constructor
            Crl(arrow::Vct::Coord ix,arrow::Vct::Coord iy,arrow::Vct::Mod is)
            :r(ix,iy),s(is)
            {}

            //Center and size constructor
            Crl(arrow::Vct ir,arrow::Vct::Mod is)
            :r(ir),s(is)
            {}

            //Copy constructor
            Crl(const Crl &ic)
            :r(ic.r),s(ic.s)
            {}

            //Assignment operator
            Crl& operator= (const Crl &ic)
            {
                r=ic.r;
                s=ic.s;
                return *this;
            }

            //Destructor
            ~Crl(){}

        /*Position and size methods*/
        public:

            //Get the center
            arrow::Vct get_center() const
            {
                return r;
            }

            //Set the center
            void set_center(const arrow::Vct &ir)
            {
                r=ir;
            }

            //Move the center
            void mov_center(const arrow::Vct &im)
            {
                r+=im;
            }

            //Get the radius
            arrow::Vct::Mod get_size() const
            {
                return s;
            }

            //Set the radius
            void set_size(arrow::Vct::Mod is)
            {
                s=is;
            }

        /*Contact methods*/
        public:

            //Contact with a point
            bool contact (const Pnt &ip) const
            {
                return collide(*this,ip);
            }

            //Time to hit a point at when the circle moves at the given speed
            arrow::Vct::Mod tth (const Pnt &ip, const arrow::Vct &iv) const
            {
                return fighter::tth(ip,iv,*this);
            }

    };

    //Point
    class Pnt
    {
        /*Constants*/
        private:

            static constexpr arrow::Vct::Coord DEF_X=0.0,DEF_Y=0.0;//Default X and Y coordinates

        /*Attributes*/
        private:

            arrow::Vct r;//Position

        /*Constructors, copy control*/
        public:

            //Default constructor
            Pnt()
            :r(DEF_X,DEF_Y)
            {}

            //Coordinate constructor
            Pnt(arrow::Vct::Coord ix,arrow::Vct::Coord iy)
            :r(ix,iy)
            {}

            //Vct constructor
            Pnt(arrow::Vct ir)
            :r(ir)
            {}

            //Copy constructor
            Pnt(const Pnt &ip)
            :r(ip.r)
            {}

            //Assignment operator
            Pnt& operator= (const Pnt &ip)
            {
                r=ip.r;
                return *this;
            }

            //Destructor
            ~Pnt(){}

        /*Position and size methods*/
        public:

            //Get the center
            arrow::Vct get_center() const
            {
                return r;
            }

            //Set the center
            void set_center(const arrow::Vct &ir)
            {
                r=ir;
            }

            //Move the center
            void mov_center(const arrow::Vct &im)
            {
                r+=im;
            }

        /*Contact methods*/
        public:

            //Contact with a circle
            bool contact (const Crl &ic) const
            {
                return collide(ic,*this);
            }

            //Time to hit a circle when the point moves at the given speed
            arrow::Vct::Mod tth (const Crl &ic, const arrow::Vct &iv) const
            {
                return fighter::tth(*this,iv,ic);
            }
    };

}}//End of namespace

#endif // _FDX_PHYSICS_H_
