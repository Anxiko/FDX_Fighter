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
#ifndef _FDX_JOKE_H_
#define _FDX_JOKE_H_


/* Includes */
//Graphics
#include "SFML/Graphics.hpp"

//Audio
#include "SFML/Audio.hpp"

//Double-ended queue
#include <deque>

namespace fdx { namespace fighter
{
    /*
        Class definitions
    */

    //Jokes
    class Joke;

    /*
        Function prototypes
    */

    /*
        Data types
     */

    /* Typedefs */

    /* Classes */

    class Joke : public sf::Drawable
    {

        /*Attributes*/
        private:
        //Joke text
        sf::Text jk_text;
        //Joke sound
        sf::Sound jk_sound;
        //Joke image
        sf::Sprite jk_img;

        /*Constructors, copy control*/
        public:

            //Default constructor
            Joke (const sf::Font& jfont, std::string jtext, const sf::Texture& jimg, const sf::SoundBuffer& jsound, const sf::Color& color) :
            jk_text(jtext, jfont, 40), jk_sound(), jk_img()
            {
                jk_text.setColor(color);
                jk_img.setOrigin(40.f, 40.f);
                jk_sound.setBuffer(jsound);
                jk_img.setTexture(jimg);
            }

            //Destructor
            virtual ~Joke() {}

        /*Implementation*/
        void set_pos(float x, float y)
        {
            jk_text.setPosition(x-50, y-100);
            jk_img.setPosition(x, y);
        }

        void play_sound_joke()
        {
            jk_sound.play();
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const//Draw the joke
        {
            target.draw(jk_text, states);
            target.draw(jk_img, states);
        }
    };

}}//End of namespace

#endif // _FDX_JOKE_H_
