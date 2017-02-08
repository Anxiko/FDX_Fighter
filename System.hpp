/*
 * System.hpp
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
* FDX_Fighter - System.hpp
* Little spaceship game made with C++ and SFML
* System resources - header file
*/

/*
    Version 0.1 (dd/mm/yy, 8/4/2015 -> )
*/

/*
    Preprocessor
*/

//Header guard
#ifndef _FDX_SYSTEM_H_
#define _FDX_SYSTEM_H_

/* Includes */

//SFML
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

//String stream
#include <sstream>

//2D Vector
#include "FDX_Vct/FDX_Vct.hpp"

#include <iostream>

/* Defines */

/*Macros*/


//Namespace
namespace fdx { namespace fighter
{
    /*
        Class definitions
    */

    //Counts fps and provides ticks between frames, can draw FPS counter
    class Fps_manager;

    //Control selected
    class Selected_Controller;

    //Holds a keyboard or mouse event
    class Event_KM;

    //Holds a gamepad button event
    class Event_GPB;

    //Holds a gamepad stick event
    class Event_GPS;

    //General interface for ship controller
    class Inter_Controller;

    //Keyboard and mouse controllers
    class KM_Controller;

    //Gamepad controller
    class GP_Controller;

    /* Variables */

    extern Selected_Controller controllers;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    typedef long long int tm_tick;//Tick time

    /* Classes */

    //Counts fps and provides ticks between frames, can draw FPS counter
    class Fps_manager : public sf::Drawable
    {
        /*Static configuration*/
        private:

            static constexpr tm_tick TM_UPD_TXT = 500;//Miliseconds that will pass between two FPS counter updates

        /*Attributes*/
        private:

            sf::Clock tm;//Clock
            unsigned int frames;//Frames since last update
            sf::Text txt;//Text to draw with FPS counter
            sf::Time tick,counter;//Time from last tick update and FPS counter update

        /*Construction, copy control*/
        public:

            //Complete constructor
            Fps_manager(int ntxt_x, int ntxt_y, int ntxt_s, const sf::Font& nft)
            :tm(),txt(std::string(),nft,ntxt_s),tick(),counter()
            {
                txt.setPosition(ntxt_x,ntxt_y);
            }

        /*FPS counter*/
        public:

            //Pass to the next frame
            void upd_frame();

            //Draw the fps counter
            void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                target.draw(txt,states);
            }

        /*Tick*/
        public:

            //Get the current tick in milliseconds
            tm_tick get_tick()
            {
                return (tm.getElapsedTime()-tick).asMicroseconds();
            }

            //Reset the tick
            void set_tick()
            {
                tick=tm.getElapsedTime();
            }

        /*Update*/
        public:

            //Update the tick and the frame count, returns the last tick
            tm_tick update_all()
            {
                unsigned long int t=get_tick();
                set_tick();
                upd_frame();
                return t;
            }
    };

    //Holds a keyboard or mouse event
    class Event_KM
    {
        /*Types*/
        public:

            typedef int ctrl_tp;//Type of a controller (keyboard or mouse)
            typedef int bind_tp;//Type of a binding (key or mouse button)

        /*Constants*/
        public:

            //Control valid values
            static constexpr ctrl_tp CTRL_KB=0, CTRL_M=1;//Keyboard and mouse control

        /*Attributes*/
        private:

            ctrl_tp control;//Type of the controller
            bind_tp binding;//Key or button

            bool state=false;//State of the binding

        /*Constructors, copy control*/
        public:

            //Default constructor
            Event_KM () = default;//Compiler generated

            //Complete constructor
            Event_KM (ctrl_tp icontrol, bind_tp ibinding)
            :control(icontrol),binding(ibinding)
            {}

            //Copy constructor
            Event_KM (const Event_KM & cpy)
            :Event_KM(cpy.control,cpy.binding)
            {}

            //Assignment operator
            Event_KM & operator= (const Event_KM & cpy)
            {
                control=cpy.control;
                binding=cpy.binding;
                return *this;
            }

        /*Operators*/
        public:

            //Equality operator
            bool operator == (const Event_KM & e) const
            {
                return ((control==e.control)&&(binding==e.binding));
            }

        /*Printing*/
        public:

            //Returns this binding on human readable format
            std::string print() const
            {
                std::stringstream ss;

                switch (control)
                {
                    case CTRL_KB:
                    {
                        //Key
                        if (binding>=sf::Keyboard::A && binding<=sf::Keyboard::W)
                        {
                            char arr[2]="A";
                            arr[0]+=(binding-sf::Keyboard::A);
                            ss<<arr;
                        }
                        //Number
                        else if (binding>=sf::Keyboard::Num0 && binding<=sf::Keyboard::Num9)
                        {
                            ss<<(binding-sf::Keyboard::Num0);
                        }
                        //NumPad number
                        else if (binding>=sf::Keyboard::Numpad0 && binding<=sf::Keyboard::Numpad9)
                        {
                            ss<<"NumPad "<<(binding-sf::Keyboard::Numpad0);
                        }
                        else
                        {
                            ss<<"Bad key";
                        }

                        break;
                    }

                    case CTRL_M:
                    {
                        if (binding>=0&&binding<sf::Mouse::ButtonCount)
                            ss<<"M"<<binding;
                        else
                            ss<<"Bad button";
                        break;
                    }

                    default:
                    {
                        ss<<"Bad control";
                        break;
                    }
                }

                return ss.str();
            }

        /*State*/
        public:

            //Get the state of a binding
            bool get_state() const
            {
                return state;
            }

            //Check if an event activates a binding
            bool activated (const sf::Event & e)
            {
                bool new_state=false;

                switch (e.type)
                {
                    case sf::Event::KeyPressed:
                        new_state=true;
                    case sf::Event::KeyReleased:
                        if (control==CTRL_KB && binding==e.key.code)
                        {
                            state=new_state;
                            return true;
                        }
                        return false;

                    case sf::Event::MouseButtonPressed:
                        new_state=true;
                    case sf::Event::MouseButtonReleased:

                        if (control==CTRL_M && binding==e.mouseButton.button)
                        {
                            state=new_state;
                            return true;
                        }
                        return false;

                    default:
                        return false;
                }
            }

        /*Configurable*/
        public:

            //Set this binding through an event
            bool set_from_event(const sf::Event & e)
            {
                if (e.type==sf::Event::KeyPressed)
                {
                    control=CTRL_KB;
                    binding=e.key.code;
                    return true;
                }
                else if (e.type==sf::Event::MouseButtonPressed)
                {
                    control=CTRL_M;
                    binding=e.mouseButton.button;
                    return true;
                }
                return false;
            }

            //Check if this binding is valid
            bool check_event() const
            {
                switch (control)
                {
                    case CTRL_KB:
                    {
                        if  (
                                (binding>=sf::Keyboard::A && binding<=sf::Keyboard::W)
                                ||
                                (binding>=sf::Keyboard::Num0 && binding<=sf::Keyboard::Num9)
                                ||
                                (binding>=sf::Keyboard::Numpad0 && binding<=sf::Keyboard::Numpad9)
                            )
                        return true;

                        break;
                    }

                    case CTRL_M:
                    {
                        if (binding>=0&&binding<sf::Mouse::ButtonCount)
                            return true;
                        break;
                    }

                    default:
                        break;
                }

                return false;
            }

        /*I/O*/
        public:

            //Write to disk
            void write_to(std::ostream & os) const
            {
                os.write(reinterpret_cast<const char *>(&control),sizeof(ctrl_tp));
                os.write(reinterpret_cast<const char *>(&binding),sizeof(bind_tp));
            }

            //Read from disk
            void read_from(std::istream & is)
            {
                is.read(reinterpret_cast<char *>(&control),sizeof(ctrl_tp));
                is.read(reinterpret_cast<char *>(&binding),sizeof(bind_tp));
            }

    };

    //Holds a gamepad button event
    class Event_GPB
    {
        /*Settings*/
        public:

            //Type of a button binding
            typedef int bind_tp;

            //Enums

            //Gamepad buttons
            enum Buttons : bind_tp
            {
                A=0,
                B,
                X,
                Y,
                LB,
                RB,
                SELECT,
                START,
                LS,
                RS,
                MAX_BUTTONS//End of the enum
            };

        /*Attributes*/
        private:

            bind_tp binding;//Button binding

            bool state=false;//Button state

        /*Constructors, copy control*/
        public:

            //Default constructor
            Event_GPB () = default;//Compiler generated

            //Complete constructor
            Event_GPB (bind_tp ibinding)
            :binding(ibinding)
            {}

            //Copy constructor
            Event_GPB(const Event_GPB & cpy)
            :Event_GPB(cpy.binding)
            {}

            //Assignment operator
            Event_GPB & operator= (const Event_GPB & cpy)
            {
                binding=cpy.binding;
                state=cpy.state;
                return *this;
            }

        /*Operators*/
        public:

            //Equality operator
            bool operator == (const Event_GPB & cpy) const
            {
                return binding==cpy.binding;
            }

        /*Printing*/
        public:

            //Returns this binding on human readable format
            std::string print() const
            {
                std::stringstream ss;
                switch (binding)
                {
                    case A:
                    {
                        ss<<"A";
                        break;
                    }
                    case B:
                    {
                        ss<<"B";
                        break;
                    }
                    case X:
                    {
                        ss<<"X";
                        break;
                    }
                    case Y:
                    {
                        ss<<"Y";
                        break;
                    }
                    case LB:
                    {
                        ss<<"LB";
                        break;
                    }
                    case RB:
                    {
                        ss<<"RB";
                        break;
                    }
                    case SELECT:
                    {
                        ss<<"SELECT";
                        break;
                    }
                    case START:
                    {
                        ss<<"START";
                        break;
                    }
                    case LS:
                    {
                        ss<<"LS";
                        break;
                    }
                    case RS:
                    {
                        ss<<"RS";
                        break;
                    }

                    default:
                        break;
                }

                return ss.str();
            }

        /*State*/
        public:

            //Get the state of a binding
            bool get_state() const
            {
                return state;
            }

            //Check if an event activates a binding
            bool activated (const sf::Event & e)
            {
                bool new_state=false;

                switch (e.type)
                {
                    case sf::Event::JoystickButtonPressed:
                        new_state=true;
                    case sf::Event::JoystickButtonReleased:
                        if (binding==static_cast<int>(e.joystickButton.button))
                        {
                            state=new_state;
                            return true;
                        }

                    default:
                        break;
                }

                return false;
            }

        /*Configurable*/
        public:

            //Set this binding through an event
            bool set_from_event(const sf::Event & e)
            {
                if (e.type==sf::Event::JoystickButtonPressed)
                {
                    binding=e.joystickButton.button;
                    return true;
                }
                return false;
            }

            //Check if this binding is valid
            bool check_event() const
            {
                if (binding>=0&&binding<MAX_BUTTONS)
                    return true;
                return false;
            }

        /*I/O*/
        public:

            //Write to disk
            void write_to(std::ostream & os) const
            {
                os.write(reinterpret_cast<const char *>(&binding),sizeof(bind_tp));
            }

            //Read from disk
            void read_from(std::istream & is)
            {
                is.read(reinterpret_cast<char *>(&binding),sizeof(bind_tp));
            }

    };


    //Holds a gamepad stick event
    class Event_GPS
    {
        /*Settings*/
        public:

            //Type of a stick binding
            typedef int bind_tp;

            //Enum of the binds
            enum Sticks : bind_tp
            {
                LSX=0,
                LSY,
                RSY=3,
                RSX,
                MAX_STICKS//End of enum
            };

            //Dead zone
            static constexpr unsigned int DEAD_ZONE_MIN=75u;//Min value to move the stick

        /*Attributes*/
        private:

            bind_tp stick;//X axe value of the picked stick

            arrow::Vct dir;//Direction the stick is pointing

        /*Constructors, copy control*/
        public:

            //Default constructor
            Event_GPS() = default;//Compiler generated

            //Complete constructors
            Event_GPS(bind_tp istick, const arrow::Vct & idir)
            :stick(istick),dir(idir)
            {}

            //Copy constructors
            Event_GPS(const Event_GPS & cpy)
            :Event_GPS(cpy.stick,cpy.dir)
            {}

            //Assignment operator
            Event_GPS & operator= (const Event_GPS & cpy)
            {
                stick=cpy.stick;
                dir=cpy.dir;
                return *this;
            }

        /*Operators*/
        public:

            //Equality operator
            bool operator == (const Event_GPS & e) const
            {
                return (stick==e.stick);
            }

        /*Printing*/
        public:

            //Returns this binding on human readable format
            std::string print() const
            {
                std::stringstream ss;

                switch (stick)
                {
                    case LSY:
                    case LSX:
                    {
                        ss<<"LS";
                        break;
                    }

                    case RSY:
                    case RSX:
                    {
                        ss<<"RS";
                        break;
                    }

                    default:
                    {
                        ss<<"Bad stick";
                        break;
                    }

                }

                return ss.str();
            }

        /*State*/
        public:

            //Get the state of a binding
            arrow::Vct get_state() const
            {
                if (dir.mod()<DEAD_ZONE_MIN)
                    return arrow::Vct(0,-100);
                return dir;
            }

            //Check if an event activates a binding
            bool activated (const sf::Event & e)
            {
                bool rv=false;
                arrow::Vct cpy_dir(dir);
                if (e.type==sf::Event::JoystickMoved)
                {
                    switch (stick)
                    {
                        case LSY:
                        case LSX:
                            if (static_cast<int>(e.joystickMove.axis)==static_cast<int>(LSX))
                            {
                                cpy_dir.x=e.joystickMove.position;
                                rv=true;
                            }
                            else if (static_cast<int>(e.joystickMove.axis)==static_cast<int>(LSY))
                            {
                                cpy_dir.y=e.joystickMove.position;
                                rv=true;
                            }
                            break;


                        case RSY:
                        case RSX:
                            if (static_cast<int>(e.joystickMove.axis)==static_cast<int>(RSX))
                            {
                                cpy_dir.x=e.joystickMove.position;
                                rv=true;
                            }
                            else if (static_cast<int>(e.joystickMove.axis)==static_cast<int>(RSY))
                            {
                                cpy_dir.y=e.joystickMove.position;
                                rv=true;
                            }
                            break;

                        default:
                            break;
                    }
                }
                if (cpy_dir.mod()>DEAD_ZONE_MIN)
                    dir=cpy_dir;
                return rv;
            }

        /*Configurable*/
        public:

            //Set this binding through an event
            bool set_from_event(const sf::Event & e)
            {
                if (e.type==sf::Event::JoystickMoved&&std::abs(e.joystickMove.position)>=DEAD_ZONE_MIN)
                {
                    switch (e.joystickMove.axis)
                    {
                        case LSX:
                        case LSY:

                            stick=LSX;
                            return true;


                        case RSX:
                        case RSY:

                            stick=RSX;
                            return true;

                        default:
                            break;
                    }
                }
                return false;
            }

            //Check if this binding is valid
            bool check_event() const
            {
                switch (stick)
                {
                    case LSX:
                    case LSY:
                    case RSX:
                    case RSY:

                        return true;
                }
                return false;
            }

        /*I/O*/
        public:

            //Write to disk
            void write_to(std::ostream & os) const
            {
                os.write(reinterpret_cast<const char *>(&stick),sizeof(bind_tp));
            }

            //Read from disk
            void read_from(std::istream & is)
            {
                is.read(reinterpret_cast<char *>(&stick),sizeof(bind_tp));
            }

    };

    //General interface for ship controller
    class Inter_Controller
    {
        /*Interface*/
        public:

            //Accelerate
            virtual bool ctrl_acc() const = 0;

            //Brake
            virtual bool ctrl_brake() const = 0;

            //Fire
            virtual bool ctrl_fire() const = 0;

            //Joke 1
            virtual bool ctrl_joke1() const = 0;

            //Joke 2
            virtual bool ctrl_joke2() const = 0;

            //Joke 3
            virtual bool ctrl_joke3() const = 0;

            //Point position

            virtual int point_x() const = 0;

            virtual int point_y() const = 0;
    };

    //Keyboard and mouse controllers
    class KM_Controller : public Inter_Controller
    {
        /*Default settings*/
        private:

            static const Event_KM DEF_ACC, DEF_BRAKE, DEF_FIRE;//Default key biindings
            static constexpr int DEF_POINT_X=0, DEF_POINT_Y=0;

        /*Attributes*/
        private:

            //Bindings
            Event_KM acc,brake,fire;

            Event_KM j1,j2,j3;

            //Point position
            int x, y;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            KM_Controller (const Event_KM & iacc, const Event_KM & ibrake, const Event_KM &ifire, int ix, int iy)
            :acc(iacc),brake(ibrake),fire(ifire),j1(Event_KM::CTRL_KB,sf::Keyboard::J),j2(Event_KM::CTRL_KB,sf::Keyboard::K),j3(Event_KM::CTRL_KB,sf::Keyboard::L),x(ix),y(iy)
            {}

            //Default constructor
            KM_Controller ()
            :KM_Controller(DEF_ACC,DEF_BRAKE,DEF_FIRE,DEF_POINT_X,DEF_POINT_Y)
            {}

            //Copy constructor
            KM_Controller(const KM_Controller & cpy)
            :KM_Controller(cpy.acc,cpy.brake,cpy.fire,cpy.x,cpy.y)
            {}

            //Assignment operator
            KM_Controller & operator= (const KM_Controller &cpy)
            {
                acc=cpy.acc;
                brake=cpy.brake;
                fire=cpy.fire;
                x=cpy.x;
                y=cpy.y;
                return *this;
            }

        /*Bindings*/
        public:

            //Set the bindings

            void set_acc (const Event_KM & iacc)
            {
                acc=iacc;
            }

            void set_brake (const Event_KM & ibrake)
            {
                brake=ibrake;
            }

            void set_fire (const Event_KM & ifire)
            {
                fire=ifire;
            }

            //Get the bindings

            Event_KM get_acc() const
            {
                return acc;
            }

            Event_KM get_brake() const
            {
                return brake;
            }

            Event_KM get_fire() const
            {
                return fire;
            }

        /*Events*/
        public:

            //Process a event
            bool process_event (const sf::Event &e)
            {
                return (acc.activated(e)||brake.activated(e)||fire.activated(e)||j1.activated(e)||j2.activated(e)||j3.activated(e));
            }

            //Set the mouse position
            void process_point (int ix, int iy)
            {
                x=ix;
                y=iy;
            }

        /*Interface*/
        public:

            //Accelerate
            virtual bool ctrl_acc() const
            {
                return acc.get_state();
            }

            //Brake
            virtual bool ctrl_brake() const
            {
                return brake.get_state();
            }

            //Fire
            virtual bool ctrl_fire() const
            {
                return fire.get_state();
            }

            virtual bool ctrl_joke1() const
            {
                return j1.get_state();
            }

            virtual bool ctrl_joke2() const
            {
                return j2.get_state();
            }

            virtual bool ctrl_joke3() const
            {
                return j3.get_state();
            }

            //Point position

            virtual int point_x() const
            {
                return x;
            }

            virtual int point_y() const
            {
                return y;
            }

        /*I/O*/
        public:

            //Write to disk
            void write_to(std::ostream & os) const
            {
                acc.write_to(os);
                fire.write_to(os);
                brake.write_to(os);
            }

            //Read from disk
            void read_from(std::istream & is)
            {
                acc.read_from(is);
                fire.read_from(is);
                brake.read_from(is);
            }
    };


    //Gamepad controller
    class GP_Controller : public Inter_Controller
    {
        /*Default settings*/
        private:

            static const Event_GPB DEF_ACC, DEF_BRAKE, DEF_FIRE;//Default button bindings
            static const Event_GPS DEF_STICK;//Default stick

        /*Attributes*/
        private:

            //Bindings
            Event_GPB acc,brake,fire;

            Event_GPB j1,j2,j3;

            //Stick
            Event_GPS stick;

            //Ship's position
            arrow::Vct center;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            GP_Controller (const Event_GPB & iacc, const Event_GPB & ibrake, const Event_GPB &ifire, Event_GPS istick)
            :acc(iacc),brake(ibrake),fire(ifire),j1(fdx::fighter::Event_GPB::LB),j2(fdx::fighter::Event_GPB::Y),j3(fdx::fighter::Event_GPB::RB),stick(istick)
            {}

            //Default constructor
            GP_Controller ()
            :GP_Controller(DEF_ACC,DEF_BRAKE,DEF_FIRE,DEF_STICK)
            {}

            //Copy constructor
            GP_Controller(const GP_Controller & cpy)
            :GP_Controller(cpy.acc,cpy.brake,cpy.fire,cpy.stick)
            {}

            //Assignment operator
            GP_Controller & operator= (const GP_Controller &cpy)
            {
                acc=cpy.acc;
                brake=cpy.brake;
                fire=cpy.fire;
                stick=cpy.stick;
                return *this;
            }

        /*Bindings*/
        public:

            //Set the bindings

            void set_acc (const Event_GPB & iacc)
            {
                acc=iacc;
            }

            void set_brake (const Event_GPB & ibrake)
            {
                brake=ibrake;
            }

            void set_fire (const Event_GPB & ifire)
            {
                fire=ifire;
            }

            void set_stick (const Event_GPS &istick)
            {
                stick=istick;
            }

            virtual bool ctrl_joke1() const
            {
                return j1.get_state();
            }

            virtual bool ctrl_joke2() const
            {
                return j2.get_state();
            }

            virtual bool ctrl_joke3() const
            {
                return j3.get_state();
            }

            //Get the bindings

            Event_GPB get_acc() const
            {
                return acc;
            }

            Event_GPB get_brake() const
            {
                return brake;
            }

            Event_GPB get_fire() const
            {
                return fire;
            }

            Event_GPS get_stick() const
            {
                return stick;
            }


        /*Events*/
        public:

            //Process a event
            bool process_event (const sf::Event &e)
            {
                return (acc.activated(e)||brake.activated(e)||fire.activated(e)||stick.activated(e)||j1.activated(e)||j2.activated(e)||j3.activated(e));
            }

            //Get the ship's position
            void process_center (const arrow::Vct & icenter)
            {
                center=icenter;
            }

        /*Interface*/
        public:

            //Accelerate
            virtual bool ctrl_acc() const
            {
                return acc.get_state();
            }

            //Brake
            virtual bool ctrl_brake() const
            {
                return brake.get_state();
            }

            //Fire
            virtual bool ctrl_fire() const
            {
                return fire.get_state();
            }

            //Point position

            virtual int point_x() const
            {
                return stick.get_state().x+center.x;
            }

            virtual int point_y() const
            {
                return stick.get_state().y+center.y;
            }

        /*I/O*/
        public:

            //Write to disk
            void write_to(std::ostream & os) const
            {
                acc.write_to(os);
                fire.write_to(os);
                brake.write_to(os);
                stick.write_to(os);
            }

            //Read from disk
            void read_from(std::istream & is)
            {
                acc.read_from(is);
                fire.read_from(is);
                brake.read_from(is);
                stick.read_from(is);
            }
    };

    //Control selected
    class Selected_Controller
    {
        /*Settings*/
        public:

            //Typedefs
            typedef int ctrl_tp;

            //Enum
            enum Controllers : ctrl_tp
            {
                CTRL_KM=0,
                CTRL_GP,
                MAX_CONTROLLERS//Num of controllers
            };

        private:

            static constexpr ctrl_tp DEF_SEL=CTRL_KM,DEF_SEL2=CTRL_GP;

        /*Attributes*/
        public:

            //Controllers

            KM_Controller km;
            GP_Controller gp;

            //Selected controller
            Inter_Controller *selected,*selected2;
            ctrl_tp sel_num,sel_num2;

        /*Constructors, copy control*/

        //Default constructor
        Selected_Controller ()
        :km(),gp(),selected(nullptr),sel_num(DEF_SEL),sel_num2(DEF_SEL2)
        {
            set_controller(sel_num);
        }

        /*Controllers*/
        public:

            void set_controller (ctrl_tp icontrol)
            {
                if (icontrol<0||icontrol>=MAX_CONTROLLERS)
                    icontrol=DEF_SEL;

                sel_num=icontrol;
                switch (icontrol)
                {
                    case CTRL_KM:
                    {
                        selected=&km;
                        break;
                    }

                    case CTRL_GP:
                    {
                        selected=&gp;
                        break;
                    }

                }
            }

            void set_controller2 (ctrl_tp icontrol)
            {
                if (icontrol<0||icontrol>=MAX_CONTROLLERS)
                    icontrol=DEF_SEL2;

                sel_num2=icontrol;
                switch (icontrol)
                {
                    case CTRL_KM:
                    {
                        selected2=&km;
                        break;
                    }

                    case CTRL_GP:
                    {
                        selected2=&gp;
                        break;
                    }
                }
            }

            Inter_Controller * get_controller() const
            {
                return selected;
            }

            Inter_Controller * get_controller2() const
            {
                return selected2;
            }

            ctrl_tp get_cont_num() const
            {
                return sel_num;
            }

            ctrl_tp get_cont_num2() const
            {
                return sel_num2;
            }

            void set_km(const KM_Controller & ikm)
            {
                km=ikm;
            }

            void set_gp(const GP_Controller & igp)
            {
                gp=igp;
            }

            const KM_Controller & get_km() const
            {
                return km;
            }

            const GP_Controller & get_gp() const
            {
                return gp;
            }

        /*I/O*/
        public:

            //Write to disk
            void write_to(std::ostream & os) const
            {
                os.write(reinterpret_cast<const char *>(&sel_num),sizeof(ctrl_tp));
                os.write(reinterpret_cast<const char *>(&sel_num2),sizeof(ctrl_tp));
                km.write_to(os);
                gp.write_to(os);
            }

            //Read from disk
            void read_from(std::istream & is)
            {
                is.read(reinterpret_cast<char *>(&sel_num),sizeof(ctrl_tp));
                is.read(reinterpret_cast<char *>(&sel_num2),sizeof(ctrl_tp));
                km.read_from(is);
                gp.read_from(is);

                set_controller(sel_num);
                set_controller2(sel_num2);
            }

    };

}}//End of namespace

#endif // _FDX_SYSTEM_H_
