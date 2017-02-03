/*
 * Graphics.cpp
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
* FDX_Fighter - Graphics.cpp
* Little spaceship game made with C++ and SFML
* SFML layer - code file
*/

/*
    Version 0.1 (dd/mm/yy, 20/3/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

//Headers file
#include "Graphics.hpp"

/* Defines */

/*Constants*/

/*Macros*/


//Namespace
namespace fdx { namespace fighter
{
    /*
        Code
    */

    /* Initializations */

    //Default particle drawing configuration
    sf::CircleShape Particle::circle(0,Particle::DEF_QUAL);//Circle to be drawn

    //RNG of Bg_Particles
    std::uniform_int_distribution<unsigned> Bg_Particles::RNG_UI(0,255);//Generates random integers
    std::uniform_real_distribution<double> Bg_Particles::RNG_D(0,1);//Generates random doubles
    std::default_random_engine Bg_Particles::RE;//Engine for the random numbers

    //Default energy displayer configuration
    const arrow::Vct Energy_displayer::DEF_POS(100,100);//Position
    const arrow::Vct Energy_displayer::DEF_SZ(100,20);//Size
    const sf::Color Energy_displayer::DEF_COL=sf::Color::Blue;//Default color

    /* Functions */

    /*Camera*/

    //Focus on target

    //Keep a target withing the focus
    void Camera::focus_target(const arrow::Vct& target)
    {
        //Check if the target is out of focus
        arrow::Vct despl(target-center);//Distance between focus and target

        //The distance must be below the limit
        if (despl.sq_mod()>dist*dist)//If the target is out of focus
        {
            //Move the camera enough to put the spaceship in the limit of the focus
            despl.limmod(despl.mod()-dist);//Apply this translation to the center
            set_center(center+despl);
        }
    }

    //Center a target on screen
    void Camera::center_target(const arrow::Vct& target)
    {
        set_center(target);
    }

    //Set the given center as center of the view
    void Camera::set_center(const arrow::Vct& ncenter)
    {
        //Set the center of the camera
        center=ncenter;

        //Update the view
        vw.setCenter(center.x,center.y);
    }

    /*Particle*/

    //Draw

    //Draw the particle
    void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        //Put the circle in its position
        circle.setPosition(r.x,r.y);

        //Draw the circle
        target.draw(circle,states);
    }

    //Set the drawing configuration
    void Particle::set_size(unsigned int nsz)
    {
        circle.setOrigin(nsz/2,nsz/2);//Set the origin as the center
        circle.setRadius(nsz);
    }

    //Set the color
    //Copy from another color
    void Particle::set_color(const sf::Color& ncolor)
    {
        circle.setFillColor(ncolor);
    }
    //Set the color by its components
    void Particle::set_color(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)
    {
        set_color(sf::Color(nr,ng,nb,na));
    }

    /*Bg_Particles*/

    //Particle management

    //Checks if the given particle is in a valid position (true if the position is valid, false otherwise)
    bool Bg_Particles::check_particle (const Particle& p) const
    {
        return (p.get_pos()-center).sq_mod()<=sq_radius;
    }

    //Set position of the given particle to a valid random state
    void Bg_Particles::set_particle (Particle& p) const
    {
        p.set_pos(center+arrow::Vct::mk_ang_mod(RNG_D(RE)*2*PI,RNG_D(RE)*get_radius()));
    }

    //Set position of the given particle to its opposite position
    void Bg_Particles::reset_particle (Particle& p) const
    {
        arrow::Vct dist(p.get_pos()-center);//Current distance to the center
        dist.limmod(-get_radius());//Limit the distance and reverse it
        p.set_pos(center+dist);//Set the new position
    }

    //Check the state of all particles within the array, and change the position of those in an invalid state
    void Bg_Particles::mng_particles()
    {
        for (Particle& particle : particles)
        {
            if(!check_particle(particle))
                reset_particle(particle);
        }
    }

    //Draw

    //Draw the particles
    void Bg_Particles::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        draw_config();
        for (const auto& p : particles)
        {
            target.draw(p,states);
        }
    }

    //Set the drawing options of the particles
    void Bg_Particles::draw_config()
    {
        Particle::set_size(RNG_UI(RE)%MAX_RAD_PART);
        Particle::set_color(RNG_UI(RE),RNG_UI(RE),RNG_UI(RE),RNG_UI(RE));
    }

    /*Energy displayer*/

    //Draw

    //Draw the energy bar
    void Energy_displayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        //Get the new size
        arrow::Vct nsz(s);//Copy of the size because it's going to be modified
        //Modify the size on the specified axe
        if (axe==HORIZONTAL_AXE)
            nsz.x*=perc;
        else
            nsz.y*=perc;

        //Draw the rectangle
        sf::RectangleShape dr(sf::Vector2f(nsz.x,nsz.y));//Create the rectangle with its size
        dr.setPosition(r.x,r.y);//Set the position of the rectangle
        dr.setFillColor(col);//Set the color of the rectangle
        target.draw(dr,states);//Draw the rectangle
    }

    /*HUD for the player's spaceship*/

    //Copy control, constructors

    //Complete constructor
    HUD_player::HUD_player (const Spaceship& nspshp, int rdcornerx, int rdcornery, int tamx1, int tamx2, int tamy, int xpos1, int xpos2, int ypos1, int ypos2, int sziconx, int szbarx, int szy)
    :spshp(nspshp),background(),cross(),speed()
    ,ener_bar
        (spshp.get_energy()
        ,arrow::Vct(rdcornerx-tamx1+xpos2,rdcornery-tamy+ypos1)
        ,arrow::Vct(szbarx,szy)
        ,fdx::fighter::Energy_displayer::HORIZONTAL_AXE)
    ,speed_bar
        (spshp.get_v_perc()
        ,arrow::Vct(rdcornerx-tamx1+xpos2,rdcornery-tamy+ypos2)
        ,arrow::Vct(szbarx,szy)
        ,fdx::fighter::Energy_displayer::HORIZONTAL_AXE)
    ,prev_ener(1.0f),delta_ener(0.0f)
    {
        put_background_points(tamx1,tamx2,tamy);
        put_cross_points(sziconx/3,szy/3);
        put_speed_points(sziconx/2,szy/4);

        background.setPosition(rdcornerx,rdcornery);
        sf::Color background_color(teams.get_friendly());
        background_color.a=31;
        background.setFillColor(background_color);
        put_background_points(tamx1,tamx2,tamy);
        cross.setPosition(rdcornerx-tamx1+xpos1,rdcornery-tamy+ypos1);
        speed[0].setPosition(rdcornerx-tamx1+xpos1,rdcornery-tamy+ypos2);
        speed[1].setPosition(rdcornerx-tamx1+xpos1,rdcornery-tamy+ypos2+szy/2);
    }

    //Icons, bars, background

    //Put the points on the cross
    void HUD_player::put_cross_points(int tamx, int tamy)
    {
        cross.setPointCount(12);
        cross.setPoint(0,sf::Vector2f(1*tamx,0*tamy));
        cross.setPoint(1,sf::Vector2f(2*tamx,0*tamy));
        cross.setPoint(2,sf::Vector2f(2*tamx,1*tamy));
        cross.setPoint(3,sf::Vector2f(3*tamx,1*tamy));
        cross.setPoint(4,sf::Vector2f(3*tamx,2*tamy));
        cross.setPoint(5,sf::Vector2f(2*tamx,2*tamy));
        cross.setPoint(6,sf::Vector2f(2*tamx,3*tamy));
        cross.setPoint(7,sf::Vector2f(1*tamx,3*tamy));
        cross.setPoint(8,sf::Vector2f(1*tamx,2*tamy));
        cross.setPoint(9,sf::Vector2f(0*tamx,2*tamy));
        cross.setPoint(10,sf::Vector2f(0*tamx,1*tamy));
        cross.setPoint(11,sf::Vector2f(1*tamx,1*tamy));
    }

    //Put the points on the arrow
    void HUD_player::put_speed_points(int tamx, int tamy)
    {
        speed[0].setPointCount(4);
        speed[0].setPoint(0,sf::Vector2f(0*tamx,2*tamy));
        speed[0].setPoint(1,sf::Vector2f(1*tamx,0*tamy));
        speed[0].setPoint(2,sf::Vector2f(2*tamx,2*tamy));
        speed[0].setPoint(3,sf::Vector2f(1*tamx,1*tamy));

        speed[1].setPointCount(4);
        speed[1].setPoint(0,sf::Vector2f(0*tamx,4*tamy));
        speed[1].setPoint(1,sf::Vector2f(1*tamx,2*tamy));
        speed[1].setPoint(2,sf::Vector2f(2*tamx,4*tamy));
        speed[1].setPoint(3,sf::Vector2f(1*tamx,3*tamy));
    }

    //Put the points on the backgrond
    void HUD_player::put_background_points(int tamx1,int tamx2,int tamy)
    {
        background.setPointCount(4);

        background.setPoint(0,sf::Vector2f(0,0));
        background.setPoint(1,sf::Vector2f(-tamx2,0));
        background.setPoint(2,sf::Vector2f(-tamx1,-tamy));
        background.setPoint(3,sf::Vector2f(0,-tamy));
    }

    //Update

    //Update the HUD bars and icons automatically
    void HUD_player::update(unsigned int tick)
    {
        //Update the health bar

        //Get the previous value
        float current_ener=ener_bar.get_perc();

        //Get the new value
        ener_bar.set_perc();//Auto update

        //Set the colour
        if (ener_bar.get_perc()<PERC_BAR_FULL)//Half or low
        {
            if (ener_bar.get_perc()<PERC_BAR_HALF)//Low
                ener_bar.col=teams.get_enemy();
            else//Half
                ener_bar.col=teams.get_neutral();
        }
        else//Full
            ener_bar.col=teams.get_friendly();

        //Update the cross

        float new_delta_ener=current_ener-prev_ener;
        prev_ener=current_ener;

        if (new_delta_ener<0&&new_delta_ener<delta_ener)
            delta_ener=new_delta_ener;

        if (!delta_ener)
        {
            delta_ener=COEF_POS_INIT*(1-ener_bar.get_perc());
        }
        else
        {
            delta_ener*=std::pow(DEG_ENER,static_cast<float>(tick)/TICK_UPD);
        }
        if (std::abs(delta_ener)<PERC_RESET)delta_ener=0;

        //Set the cross color
        sf::Color ener_color_mix;//Color to add to the mix (enemy or friendly)
        sf::Color ener_color_neu=teams.get_neutral();//Neutral color
        if (delta_ener>=0)//More energy
            ener_color_mix=teams.get_friendly();
        else//Less energy
            ener_color_mix=teams.get_enemy();

        float ener_coef_col=std::abs((delta_ener))/PERC_MAX;//Coeficient to add colours
        //Mix the color and set it
        cross.setFillColor(sf::Color  (
                                    ener_color_mix.r*ener_coef_col+ener_color_neu.r*(1-ener_coef_col),
                                    ener_color_mix.g*ener_coef_col+ener_color_neu.g*(1-ener_coef_col),
                                    ener_color_mix.b*ener_coef_col+ener_color_neu.b*(1-ener_coef_col),
                                    255
                                ));

        //Update the speed bar

        //Get the previous value
        speed_bar.get_perc();

        //Get the new value
        speed_bar.set_perc(spshp.get_v_perc());//Manual update

        //Set the colour
        if (speed_bar.get_perc()<PERC_BAR_FULL)//Half or low
        {
            if (speed_bar.get_perc()<PERC_BAR_HALF)//Low
                speed_bar.col=teams.get_enemy();
            else//Half
                speed_bar.col=teams.get_neutral();
        }
        else//Full
            speed_bar.col=teams.get_friendly();

        //Update the arrows

        if  ((spshp.get_a_perc()==1&&spshp.get_v_perc()==1)//Full speed
            ||//Or
            (spshp.get_a_perc()==0&&spshp.get_v_perc()==0))//Completly stopped
        {
            speed[0].setFillColor(teams.get_neutral());
            speed[1].setFillColor(teams.get_neutral());
        }
        else
        {
            sf::Color speed_color_mix;//Color to mix with
            sf::Color speed_color_neu=teams.get_neutral();//Get the neutral color
            float speed_coef_col;//Coefficient to mix the colors

            if (spshp.get_a_perc())//Accelerating
            {
                speed_color_mix=teams.get_friendly();
                speed_coef_col=spshp.get_a_perc();
            }
            else
            {
                speed_color_mix=teams.get_enemy();
                speed_coef_col=spshp.get_v_perc();
            }

            //Mix the color and set it
            sf::Color  speed_color_final(
                                    speed_color_mix.r*speed_coef_col+speed_color_neu.r*(1-speed_coef_col),
                                    speed_color_mix.g*speed_coef_col+speed_color_neu.g*(1-speed_coef_col),
                                    speed_color_mix.b*speed_coef_col+speed_color_neu.b*(1-speed_coef_col),
                                    255
                                );
            speed[0].setFillColor(speed_color_final);
            speed[1].setFillColor(speed_color_final);
        }
    }


    //Draw

    //Draw the HUD
    void HUD_player::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(background,states);
        target.draw(ener_bar,states);
        target.draw(speed_bar,states);
        target.draw(cross,states);
        target.draw(speed[0],states);
        target.draw(speed[1],states);
    }

    /*Text_displayer*/

    //Interface
    void Text_displayer::add_text(const sf::Text& text)
    {
        while(texts.size()>=DEF_LINES)
            delete_text();

        t=0;

        //Create the text
        sf::Text new_text;
        new_text.setString(text.getString());
        new_text.setColor(text.getColor());
        new_text.setFont(font);

        //Center the text
        sf::FloatRect textRect = new_text.getLocalBounds();
        new_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

        //Position to the text
        new_text.setPosition(x,y);

        texts.push_front(new_text);

        upd_y();
    }
}}//End of namespace
