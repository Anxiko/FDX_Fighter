/*
 * ClassMenu.cpp
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
    FDX_Fighter - ClassMenu.cpp
    Little spaceship game made with C++ and SFML
    Menu and windows - code file
*/

/*
    Version 0.1 (dd/mm/yy, 24/11/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

//Header file
#include "ClassMenu.hpp"

namespace fdx{ namespace menu
{
    /*
        Code
    */

    /* Initializations */

    //Language

    //Selected language
    Language::en_type Language::lang=Language::DEF_LANG;

    //Language dictionary
    const std::string Language::language_text[MAX_LANG][MAX_TEXT] =
    {
        {"Singleplayer", "Multiplayer", "Tutorial", "OPTIONS", "Personalize", "Color", "Language", "COLORS", "LANGUAGE", "Confirm", "Confirm and start the program to see the changes","VSYNC on","VSYNC off","You died!","Enemy destroyed!", "CONTROLS", "Configure", "CONFIGURE MOUSE", "CONFIGURE XBOX", "CONFIGURE WIIMOTE", "SHOOT", "ACCELERATE", "BRAKE", "STICK","You win!","You lose"},
        {"Un jugador", "Multijugador", "Tutorial", "OPCIONES", "Personalizar", "Color", "Idioma", "COLORES", "IDIOMA", "Confirmar", "Confirme y inicie el programa para ver los cambios","VSYNC activado","VSYNC desactivado","¡Has muerto!","¡Enemigo destruido!", "CONTROLES", "Configurar", "CONFIGURAR RATÓN", "CONFIGURAR MANDO XBOX", "CONFIGURAR WIIMOTE", "DISPARO", "ACELERAR", "FRENAR", "STICK","¡Has ganado","¡Has perdido!"}
    };

    //Dictionary of colors
    const sf::Color DictionaryColors::colors [] =
    {sf::Color(0,0,255), sf::Color::Yellow, sf::Color::Red, sf::Color::Green, sf::Color(128,0,128), sf::Color::Magenta, sf::Color::White, sf::Color(179,179,179)};


    /* Functions */

    /* Methods */

    /*Button*/

    //Button methods

    //Test if the button is pressed by the given mouse
    bool Button::isPressed(int mos_x, int mos_y, bool but)
    {
        //Button is disable
        if (!isAvailable())
            return false;

        //Rect over RectShape
        const sf::RectangleShape& current_rect=pressed?back_rect:front_rect;
        sf::FloatRect rect_bounds(current_rect.getGlobalBounds());

        //Button is pressed
        if(pressed)
        {
            //Mouse is pressed
            if (but)
            {
                //Mouse not over button
                if(!rect_bounds.contains(sf::Vector2f(mos_x,mos_y)))
                    pressed = false;
                //Mouse over button: keep it pressed, do not change anything
            }
            //Mouse not pressed
            else
            {
                pressed=false;
                //Mouse was released over the button
                if (rect_bounds.contains(sf::Vector2f(mos_x,mos_y)))
                {
                    pressed_sound.play();
                    return true;//Button pressed
                }
            }
        }

        //Button is not pressed
        else
        {
            //Mouse is pressed and over the button
            if (but&&rect_bounds.contains(sf::Vector2f(mos_x,mos_y)))
                pressed = true;
            else
            {
                if (!played&&rect_bounds.contains(sf::Vector2f(mos_x, mos_y)))
                {
                    over_sound.play();
                    played = true;
                }
                if (!rect_bounds.contains(sf::Vector2f(mos_x,mos_y)))
                    played = false;
            }
        }

        //Button not pressed
        return false;
    }

    /*ButtonText*/

    //Draw function
    void ButtonText::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        Button::draw(target,states);
        target.draw(button_text, states);
    }

    /*Menu*/

    //Returns the button's pressed ID
    int Menu::button_pressed(int x, int y, bool but)
    {
        for(unsigned int nbutton = 0; nbutton < menu_buttons.size(); ++nbutton)
        {
            if (menu_buttons[nbutton]->isPressed(x, y, but))
                return nbutton;
        }
        return -1;
    }

    //Draw function
    void Menu::draw (sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(background, states);
        target.draw(title,states);

        for(const Button* x : menu_buttons)
        {
            target.draw(*x, states);
        }
    }

    /*
        Config
    */

    /* Methods */

    /*I/O*/

    //Write to file
    void Config::write_to(std::ostream& os) const
    {
        //Fixed width integer for writing
        int32_t temp;

        //Write the language
        temp=lang;
        os.write(reinterpret_cast<char *>(&temp),sizeof(int32_t));

        //Write the colors
        temp=a_color;
        os.write(reinterpret_cast<char *>(&temp),sizeof(int32_t));

        temp=n_color;
        os.write(reinterpret_cast<char *>(&temp),sizeof(int32_t));

        temp=e_color;
        os.write(reinterpret_cast<char *>(&temp),sizeof(int32_t));
    }

    //Read from file
    void Config::read_from(std::istream& is)
    {
        //Fixed width integer for reading
        int32_t temp;

        //Read the language
        is.read(reinterpret_cast<char *>(&temp),sizeof(int32_t));
        lang=temp;

        //Read the colors
        is.read(reinterpret_cast<char *>(&temp),sizeof(int32_t));
        a_color=temp;

        is.read(reinterpret_cast<char *>(&temp),sizeof(int32_t));
        n_color=temp;

        is.read(reinterpret_cast<char *>(&temp),sizeof(int32_t));
        e_color=temp;
    }

    /*Update*/

    //Apply current config
    void Config::apply() const
    {
        fdx::menu::Language::setLang(lang);
        fdx::fighter::teams.set_friendly(fdx::menu::DictionaryColors::getColor(a_color));
        fdx::fighter::teams.set_neutral(fdx::menu::DictionaryColors::getColor(n_color));
        fdx::fighter::teams.set_enemy(fdx::menu::DictionaryColors::getColor(e_color));
    }

}}//End of namespace
