/*
 * ClassMenu.hpp
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
    FDX_Fighter - ClassMenu.hpp
    Little spaceship game made with C++ and SFML
    Menu and windows - header file
*/

/*
    Version 0.1 (dd/mm/yy, 24/11/2015 ->)
*/

/*
    Preprocessor
*/

/*Header guard*/
#ifndef _FDX_CLASSMENU_H_
#define _FDX_CLASSMENU_H_


/* Includes */

//SFML
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

//STD
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

//Teams
#include "Teams.hpp"

/* Defines */

/*Constants*/

/*Macros*/

/*
    Definitions
*/

/*Classes*/

/*BUTTONS*/

/*Button*/

namespace fdx { namespace menu
{

    /*
        Class definitions
    */

    //Button class
    class Button;

    //Button with an image on it
    class ButtonImage;

    //Button with text on it
    class ButtonText;

    //Dictionary of colours
    class DictionaryColors;

    //Window menu
    class Menu;

    //Stores the program's configuration
    class Config;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    /* Classes */

    //Button class
    class Button : public sf::Drawable
    {
        /*Attributes*/
        private:

            //Button's shape
            sf::RectangleShape front_rect;
            //Button's shadow
            sf::RectangleShape back_rect;
            //Button pressed
            bool pressed;
            //Button available
            bool available;
            //Play once
            bool played;
            //Sound: mouse over button
            sf::Sound over_sound;
            //Sound: button pressed
            sf::Sound pressed_sound;

        /*Constructors, copy control*/
        protected:

            //Complete constructor
            Button  (
                        int ix, int iy, int iwidth, int iheight,
                        int idif_x, int idif_y,
                        const sf::Color& ifront_color, const sf::Color& iback_color,
                        bool iavailable,
                        const sf::Sound& iover, const sf::Sound& ipress
                    )
            :front_rect(sf::Vector2f(iwidth, iheight)),back_rect(sf::Vector2f(iwidth+idif_x*2, iheight+idif_y*2))
            ,pressed(false),available(iavailable),over_sound(iover),pressed_sound(ipress)
            {
                //Front rect
                front_rect.setPosition(ix, iy);
                front_rect.setFillColor(ifront_color);

                //Back rect
                back_rect.setPosition(ix - idif_x, iy - idif_y);
                back_rect.setFillColor(iback_color);
            }

        public:

            //Virtual destructor
            virtual ~Button(){}

        /*Button methods*/
        public:

            //Test if the button is pressed by the given mouse
            bool isPressed(int mos_x, int mos_y, bool but);

            //Return the last pressed status of the button
            bool lastPressed()
            {
                return pressed;
            }

            //Check if the button is available
            bool isAvailable()
            {
                return available;
            }

            //Lock/unlock the button

            virtual void unlock()
            {
                available=true;
            }

            virtual void lock()
            {
                available=false;
            }

            //Change the front color of the button
            void change_color_button(const sf::Color& color)
            {
                front_rect.setFillColor(color);
            }

        /*Draw*/
        public:

            //Draw the button
            virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const
            {
                target.draw(back_rect,states);
                if (!pressed) target.draw(front_rect,states);
            }
    };

    //Button with text on it
    class ButtonText : public Button
    {

        /*Attributes*/
        private:

            //Button's text
            sf::Text button_text;

            //Text's colors

            //Locked text color
            sf::Color lock_color;
            //Unlocked text color
            sf::Color unlock_color;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            ButtonText  (
                             int ix, int iy, int iwidth, int iheight,
                             int idif_x, int idif_y,
                             const sf::Color& ifront_color,const sf::Color& iback_color,
                             bool iavailable,
                             const sf::Sound& iover, const sf::Sound& ipress,
                             std::string itext, const sf::Font &ifont, int ifont_size,
                             const sf::Color& iunlock_color,const sf::Color& ilock_color
                         )
            :Button(ix, iy, iwidth, iheight, idif_x, idif_y, ifront_color, iback_color, iavailable, iover, ipress), button_text(itext, ifont, ifont_size),lock_color(ilock_color),unlock_color(iunlock_color)
            {
                //Set the text's color
                change_color_text();

                //Center the text
                sf::FloatRect textRect = button_text.getLocalBounds();
                button_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

                //Position
                button_text.setPosition(ix+iwidth/2,iy+iheight/2);
            }

        /*Locking/unlocking*/
        private:

            //Update the text's color
            void change_color_text()
            {
                if (isAvailable())
                {
                    button_text.setFillColor(unlock_color);
                    button_text.setOutlineColor(unlock_color);
                }
                else
                {
                    button_text.setFillColor(lock_color);
                    button_text.setOutlineColor(lock_color);
                }
            }

        public:
            //Lock/unlock the button

            virtual void unlock()
            {
                Button::unlock();
                change_color_text();
            }

            virtual void lock()
            {
                Button::lock();
                change_color_text();
            }

        /*Set text*/
        public:

            void set_text(const std::string & str)
            {
                button_text.setString(str);

                //Center the text
                sf::FloatRect textRect = button_text.getLocalBounds();
                button_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
            }

        /*Draw*/

            //Draw
            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };

    /*ButtonImage*/

    class ButtonImage : public Button
    {
        private:
            //Image
            sf::Sprite image;

        public:

            //Constructor
            ButtonImage(int ix, int iy, float iwidth, float iheight, int idif_x, int idif_y, const sf::Color& ifront_color,const sf::Color& iback_color,  bool iavailable, const sf::Sound& iover, const sf::Sound& ipress, const sf::Texture& ibackground)
            :Button(ix, iy, iwidth, iheight, idif_x, idif_y, ifront_color, iback_color, iavailable, iover, ipress), image()
            {
                //Create the sprite
                image.setTexture(ibackground);

                //Center the image
                sf::FloatRect imgRect = image.getLocalBounds();
                image.setOrigin(imgRect.left + imgRect.width/2.0f, imgRect.top  + imgRect.height/2.0f);

                //Position
                image.setPosition(ix+iwidth/2,iy+iheight/2);
            }

            void change_color_image(const sf::Color& color)
            {
                image.setColor(color);
            }

            virtual void unlock()
            {
                Button::unlock();
            }

            virtual void lock()
            {
                Button::lock();
            }

            //Draw function
            void draw (sf::RenderTarget &target, sf::RenderStates states) const
            {
                Button::draw(target,states);
                target.draw(image,states);
            }
    };

    /*LANGUAGE*/

    class Language
    {

        /*Settings*/
        public:

            //Type of enums
            typedef unsigned char en_type;

            //Enums

            //Enum of words
            enum Texts : en_type
            {
                SINGLEPLAYER=0,
                MULTIPLAYER,
                STATISTICS,
                OPTIONS,
                PERSONALIZE,
                COLOR,
                LANGUAGE,
                COLORS,
                LANGUAGES,
                CONFIRM,
                RESTART,
                VSYNC_ON,
                VSYNC_OFF,
                YOU_DIED,
                ENEMY_DIED,
                CONTROLS,
                CONFIGURE,
                MOUSE,
                XBOX,
                WII,
                SHOOT,
                ACCELERATE,
                BRAKE,
                STICK,
                WON,
                LOSE,
                MAX_TEXT//Limit
            };

            //Enum of languages
            enum Langs : en_type
            {
                ENGLISH=0,
                SPANISH,
                MAX_LANG//Limit
            };

        public:

            //Default config

            static constexpr en_type DEF_LANG=ENGLISH;

        /*Attributes*/

        //Languages
        private:

            //Language matrix
            static const std::string language_text[MAX_LANG][MAX_TEXT];
            //Language selected
            static en_type lang;

        /*Constructors, copy control*/
        private:

            //Default constructor
            Language() = delete;//Static class, not to be used

        /*Language methods*/
        public:

            static void setLang(en_type nl)
            {
                lang=nl;
            }

            static en_type getLang()
            {
                return lang;
            }

            static std::string getText(en_type ntext)
            {
                if (lang<MAX_LANG&&ntext<MAX_TEXT)
                    return language_text[lang][ntext];
                return std::string();
            }
    };

    class DictionaryColors
    {

        /*Settings*/
        public:

            //Type of enums
            typedef unsigned char en_type;

        /*Attributes*/
        private:

            //Array of colors
            static const sf::Color colors[];

        public:

        /*Constructors, copy control*/

            //Default constructor
            DictionaryColors() = delete;//Static class, not to be used

            //Get a color from the dictionary
            static const sf::Color& getColor(unsigned int pos)
            {
                return colors[pos];
            }

            //Get a color from the dictionary with a modified alpha (half transparent)
            static sf::Color getSemiColor(unsigned int pos)
            {
                sf::Color rv(getColor(pos));
                rv.a=127;
                return rv;
            }
    };

    /*MENU*/

    class Menu : public sf::Drawable
    {
        /*Atributtes*/
        private:

            //Menu's background
            sf::Sprite background;
            //Menu's tittle
            sf::Text title;
            //Menu's bottons
            std::vector<Button*> menu_buttons;

        /*Constructors, copy control*/
        public:

            //Complete constructor
            Menu(const sf::Texture& iimage,std::string imenu_title, int ititle_size, int itittle_x, int itittle_y, const sf::Font &ifont, const sf::Color &ititle_color)
            :background(iimage),title(imenu_title, ifont, ititle_size),menu_buttons()
            {
                //Title
                title.setPosition(sf::Vector2f(itittle_x, itittle_y));
                title.setFillColor(ititle_color);
                title.setOutlineColor(ititle_color);
            }

            //Destructor
            ~Menu()
            {
                for (Button *b : menu_buttons)
                    delete b;
            }

        /*Button methods*/
        public:

            //Add a button
            void add_button(Button* ptr_b)
            {
                if (ptr_b)
                    menu_buttons.push_back(ptr_b);
            }

            //Returns the position on the vector of the first pressed button
            int button_pressed(int x, int y, bool but);

            //Unlocks a button
            void unlock_buttons(unsigned int but)
            {
                menu_buttons.at(but)->unlock();
            }

            //Locks  button
            void lock_buttons(int but)
            {
                menu_buttons.at(but)->lock();
            }

            //Changes the color of the front rect
            void change_color_rect(int but, const sf::Color& color)
            {
                menu_buttons.at(but)->change_color_button(color);
            }

            //Change the color of an image
            void change_color_image(int but, const sf::Color& color)
            {
                ButtonImage* bi;
                if ((bi=dynamic_cast<ButtonImage*>(menu_buttons.at(but))))
                    bi->change_color_image(color);
            }

            //Change the text of a button
            void change_text(int but, const std::string &str)
            {
                ButtonText *bt;
                if ((bt=dynamic_cast<ButtonText*>(menu_buttons.at(but))))
                    bt->set_text(str);
            }

        /*Draw*/
        public:

            //Draw the menu
            void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    };


    //Stores the program's configuration
    class Config
    {
        /* Config */

        /*Language*/
        private:

            Language::en_type DEF_LANG=Language::DEF_LANG;//Default language

        /*Colors*/
        private:

            static constexpr int DEF_A_COLOR=3;//Default ally color
            static constexpr int DEF_N_COLOR=6;//Default neutral color
            static constexpr int DEF_E_COLOR=2;//Default enemy color


        /* Attributes */

        /*Language*/
        private:

            Language::en_type lang;//Selected language

        /*Colors*/
        private:

            int a_color;//Code for the ally color
            int n_color;//Code for the neutral color
            int e_color;//Code for the enemy color

        /* Constructors, copy control */

        /*Constructors*/
        public:

            //Complete constructor
            Config(Language::en_type ilang, int ia_color, int in_color, int ie_color)
            :lang(ilang),a_color(ia_color),n_color(in_color),e_color(ie_color)
            {}

            //Default constructor
            Config()
            :Config(DEF_LANG,DEF_A_COLOR,DEF_N_COLOR,DEF_E_COLOR)
            {}

        /* Methods */

        /*Get/set*/
        public:

            //Get

            //Get the language
            Language::en_type get_lang() const
            {
                return lang;
            }

            //Get the ally color
            int get_ally() const
            {
                return a_color;
            }

            //Get the neutral color
            int get_neutral() const
            {
                return n_color;
            }

            //Get the enemy color
            int get_enemy() const
            {
                return e_color;
            }

            //Set

            //Set the language
            void set_lang(Language::en_type ilang)
            {
                lang=ilang;
            }

            //Set the ally color code
            void set_ally(int ially)
            {
                a_color=ially;
            }

            //Set the neutral color code
            void set_neutral(int ineutral)
            {
                n_color=ineutral;
            }

            //Set the enemy color code
            void set_enemy(int ienemy)
            {
                e_color=ienemy;
            }

        /*I/O*/
        public:

            //Write to file
            void write_to(std::ostream& os) const;

            //Read from file
            void read_from(std::istream& is);

        /*Update*/
        public:

            //Apply current config
            void apply() const;
    };
}}//End of namespace

#endif // _FDX_CLASSMENU_H_
