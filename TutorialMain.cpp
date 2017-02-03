/*
 * TutorialMain.cpp
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
* FDX_Fighter - main.cpp
* Little spaceship game made with C++ and SFML
* Main game
*/

/*
    Versión 0.1 (dd/mm/yy, 21/3/2015 -> )
*/

/*
    Preprocessor
*/

/* Includes */

#include "TutorialMain.hpp"

namespace fdx{ namespace fighter
{

    /*
        Code
    */

    /* Functions */

    //Launches the single player mode
    void launch_tutorial ()
    {
        //SFML variables
        fdx::fighter::Camera cm(600,600,"Shield_",16,0);
        sf::Texture clock_txture;
        sf::Texture spt;
        sf::Texture bg_spt;
        sf::Texture hudtext;
        sf::Texture fire;
        sf::Texture tutorial_right_hud;
        sf::Sprite tutorial_right_hud_s;
        sf::Texture tutorial_left_hud;
        sf::Sprite tutorial_left_hud_s;
        sf::Event event;
        sf::SoundBuffer fr;

        sf::Music music;

        //Load the texure
        if (!music.openFromFile("media/sounds/battle_music.ogg"))
            std::cout<<"Error loading music"<<std::endl;
        music.play();
        music.setLoop(true);

        //Load the tutorial right hud
        if (!tutorial_right_hud.loadFromFile("media/images/tutorial1.png"))
            std::cout<<"Error loading clock"<<std::endl;

        if (!tutorial_left_hud.loadFromFile("media/images/tutorial2.png"))
            std::cout<<"Error loading clock"<<std::endl;

        //Load the texure
        if (!spt.loadFromFile("media/images/SFighter.png"))
            std::cout<<"Error loading texture"<<std::endl;
        spt.setSmooth(true);

        //Load the clock
        if (!clock_txture.loadFromFile("media/images/clock.png"))
            std::cout<<"Error loading clock"<<std::endl;

        if (!bg_spt.loadFromFile("media/images/fondo.jpg"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!hudtext.loadFromFile("media/images/HUD.png"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!fire.loadFromFile("media/images/Fire.png"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!fr.loadFromFile("media/sounds/fire.wav"))
            std::cout<<"Error loading texture"<<std::endl;

        //Laser model
        fdx::fighter::Laser_Model lm(5000000,5,10,1e-3,20,50,fdx::fighter::Team(true));
        fdx::fighter::Laser_Model lm2(5000000,5,10,1e-3,10,1,fdx::fighter::Team(false));

        //Spaceship
        fdx::fighter::Spaceship spshp   (fdx::arrow::Vct(200,200),spt,fdx::fighter::Spshp_config(),fdx::fighter::Team(true),300,2e4);

        //Cannon
        fdx::fighter::Cannon cn(lm,fdx::arrow::Vct(0.7,0),nullptr,nullptr,200000);
        fdx::fighter::Cannon cn2(lm2,fdx::arrow::Vct(0.7,0),nullptr,nullptr,200000);
        cn.put_sound(&fr);
        cn2.put_sound(&fr);
        spshp.put_cannon(cn);

        //Propeller
        spshp.put_propeller(fdx::fighter::Propeller(fire,fdx::arrow::Vct(-0.42,0)));

        //Game mode
        fdx::fighter::Tutorial tutorial
        (
            spt,fdx::fighter::Spshp_config(5e-10,3e-4,5e-10,1,0,1e-3,0),cn2
        );
        tutorial.put_player(&spshp);

        //Background
        fdx::fighter::Background bgd(bg_spt,0.05,fdx::arrow::Vct(200,200));
        fdx::fighter::Bg_Particles bgp(spshp.get_center(),300);

        //Font
        sf::Font ft;
        if (!ft.loadFromFile("media/fonts/arial.ttf"))
            std::cout<<"Error loading font"<<std::endl;

        //Clock
        fdx::fighter::Fps_manager ck(50,50,10,ft);

        //Text display
        fdx::fighter::Text_displayer td(ft,300,20);

        bool vsync=false;

        //HUD
        fdx::fighter::HUD_player hud(spshp,600,600,150,180,100,20,50,20,60,20,80,20);
        spshp.put_ind();

        fdx::fighter::HUD_mode hud2(tutorial,clock_txture,ft,0,600,200,250,200,20,20,50,20,120,10,20);

        //Controller
        fdx::fighter::Inter_Controller *ctrler=controllers.get_controller();
        spshp.set_controller(ctrler);

        //Tutorial
        //Image 1
        tutorial_right_hud_s.setTexture(tutorial_right_hud);
        tutorial_right_hud_s.setPosition(sf::Vector2f(120, 320));

        tutorial_left_hud_s.setTexture(tutorial_left_hud);
        tutorial_left_hud_s.setPosition(sf::Vector2f(50, 300));

        //Text for the button name
        std::string fire_but, acc_but, brake_but;

        //Show action to do
        sf::Text action_text;
        action_text.setFont(ft);
        action_text.setCharacterSize(40);
        action_text.setPosition(200.f, 25.f);
        action_text.setColor(sf::Color::Yellow);

        //Control of steps
        bool step_without_events = true;
        bool one_enemy = false;
        int n_step = 0;//Number of step

        while (cm.isOpen())
        {
            fdx::fighter::KM_Controller* ptr_km=dynamic_cast<KM_Controller *>(ctrler);
            fdx::fighter::GP_Controller* ptr_gp=dynamic_cast<GP_Controller *>(ctrler);
            fdx::fighter::Wii_Controller* ptr_wii=dynamic_cast<Wii_Controller *>(ctrler);

            while (cm.pollEvent(event))
            {
                bool processed=false;

                if (ptr_km)
                {
                    processed=ptr_km->process_event(event);
                    //Read button name
                    fire_but = ptr_km->get_fire().print();
                    acc_but = ptr_km->get_acc().print();
                    brake_but = ptr_km->get_brake().print();
                    //Next
                    if (ctrler->ctrl_acc() && (n_step == 0 || n_step == 5))
                    {
                        step_without_events = false;
                        if (n_step == 0)
                            n_step = 1;
                        else
                            n_step = 6;
                    }
                }
                else if (ptr_gp)
                {
                    processed=ptr_gp->process_event(event);
                    //Read button name
                    fire_but = ptr_gp->get_fire().print();
                    acc_but = ptr_gp->get_acc().print();
                    brake_but = ptr_gp->get_brake().print();
                    //Next
                    if (ctrler->ctrl_acc() && (n_step == 0 || n_step == 5))
                    {
                        step_without_events = false;
                        if (n_step == 0)
                            n_step = 1;
                        else
                            n_step = 6;
                    }
                }
                else if (ptr_wii)
                {
                    //Read button name
                    fire_but = "B";
                    acc_but = "A";
                    brake_but = "DOWN";
                    //Número de paso
                    switch (n_step)
                    {
                        case 0:
                        {
                            if (ctrler->ctrl_acc())
                            {
                                step_without_events = false;
                                n_step = 1;
                            }
                            break;
                        }
                        case 1:
                        {
                            if (ptr_wii->ctrl_fire())
                                n_step = 2;
                            break;
                        }
                        case 2:
                        {
                            if (ptr_wii->ctrl_acc())
                                n_step = 3;
                            break;
                        }
                        case 3:
                        {
                            if (ptr_wii->ctrl_brake())
                                n_step = 4;
                            break;
                        }
                        case 4:
                        {
                            step_without_events = true;
                            n_step = 5;
                            break;
                        }
                        case 5:
                        {
                            if (ctrler->ctrl_acc())
                            {
                                step_without_events = false;
                                n_step = 6;
                            }
                        }
                        case 6:
                        {
                            if (!one_enemy)
                            {
                                tutorial.add_enemy();
                                one_enemy = true;
                                n_step = 7;
                            }
                            break;
                        }
                        case 8:
                        {
                            if (ctrler->ctrl_acc())
                                cm.close();
                            break;
                        }
                    }
                }

                if (!processed&&!step_without_events) switch (event.type)
                {
                    case sf::Event::Closed:
                    {
                        cm.close();
                        break;
                    }

                    case sf::Event::KeyPressed:
                    {

                        switch (event.key.code)
                        {

                            case sf::Keyboard::Space:
                            {
                                vsync=!vsync;
                                cm.setVerticalSyncEnabled(vsync);

                                if (vsync)
                                {
                                    sf::Text new_text;
                                    new_text.setString(menu::Language::getText(menu::Language::VSYNC_ON));
                                    new_text.setColor(fdx::fighter::teams.get_enemy());
                                    td.add_text(new_text);
                                }
                                else
                                {
                                    sf::Text new_text;
                                    new_text.setString(menu::Language::getText(menu::Language::VSYNC_OFF));
                                    new_text.setColor(fdx::fighter::teams.get_friendly());
                                    td.add_text(new_text);
                                }
                                break;
                            }

                            default:

                                break;
                        }
                        break;
                    }

                    case sf::Event::KeyReleased:
                    {
                        switch (event.key.code)
                        {

                            default:
                                break;
                        }
                        break;
                    }

                    case sf::Event::MouseMoved:
                    {
                        cm.update_mouse(fdx::arrow::Vct(event.mouseMove.x,event.mouseMove.y));
                        break;
                    }

                    case sf::Event::MouseButtonPressed:
                    {
                        switch (event.mouseButton.button)
                        {
                            default:
                                break;
                        }
                        break;
                    }

                    case sf::Event::MouseButtonReleased:
                    {
                        switch (event.mouseButton.button)
                        {
                            default:
                                break;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }

            //Número de paso
            switch (n_step)
            {
                case 1:
                {
                    if (ctrler->ctrl_fire())
                        n_step = 2;
                    break;
                }
                case 2:
                {
                    if (ctrler->ctrl_acc())
                        n_step = 3;
                    break;
                }
                case 3:
                {
                    if (ctrler->ctrl_brake())
                        n_step = 4;
                    break;
                }
                case 4:
                {
                    step_without_events = true;
                    n_step = 5;
                    break;
                }
                case 6:
                {
                    if (!one_enemy)
                    {
                        tutorial.add_enemy();
                        one_enemy = true;
                        n_step = 7;
                    }
                    break;
                }
                case 8:
                {
                    if (ctrler->ctrl_acc())
                        cm.close();
                    break;
                }
            }

            if (ptr_km)
                ptr_km->process_point(cm.get_tra_ms().x,cm.get_tra_ms().y);
            else if (ptr_gp)
                ptr_gp->process_center(spshp.get_center());
            else if (ptr_wii)
            {
                ptr_wii->set_center(cm.get_center());
                ptr_wii->update();
            }

            //Update and move
            //spshp.point(cm.get_tra_ms());//Point the spaceship at the translated mouse
            unsigned long long int tick=ck.update_all();//Get the next tick
            spshp.mov(tick);//Move the spaceship
            hud.update(tick);//Update the HUD values

            while (tutorial.has_text())
                td.add_text(tutorial.get_text());

            td.update(tick);//Update the TD
            tutorial.update(tick);//Update the game mode

            cm.focus_target(spshp.get_center());//Focus the camera on the moved spaceship
            bgd.mov(cm.get_translation());//Move the background

            //Clear the camera
            cm.clear();

            //Draw on the default view
            cm.view_default();
            cm.draw(bgd);//Background
            cm.draw(ck);//Clock
            cm.draw(hud);//HUD
            if (3 < n_step)
                cm.draw(hud2);
            cm.draw(td);//Text displayer

            //Draw on the translated view
            cm.view_translated();
            cm.draw(spshp);//Spaceship
            cm.draw(tutorial);//Mode elements
            sf::CircleShape pointer(5);
            pointer.setFillColor(sf::Color::White);
            pointer.setOrigin(5,5);
            pointer.setPosition(ctrler->point_x(),ctrler->point_y());
            cm.draw(pointer);

            cm.view_default();
            switch (n_step)
            {
                case 0:
                {
                    action_text.setString("Press " + acc_but + "...");
                    cm.draw(action_text);
                    cm.draw(tutorial_right_hud_s);
                    break;
                }
                case 1:
                {
                    action_text.setString("Press " + fire_but + " to shot");
                    action_text.setPosition(150.f, 25.f);
                    cm.draw(action_text);
                    break;
                }
                case 2:
                {
                    action_text.setString("Press " + acc_but + " to accelerate");
                    cm.draw(action_text);
                    break;
                }
                case 3:
                {
                    action_text.setString("Press " + brake_but + " to brake");
                    cm.draw(action_text);
                    break;
                }
                case 5:
                {
                    action_text.setString("Press " + acc_but + "...");
                    action_text.setPosition(200.f, 25.f);
                    cm.draw(action_text);
                    cm.draw(tutorial_left_hud_s);
                    break;
                }
                case 7:
                {
                    action_text.setString("Defeat the enemy");
                    action_text.setPosition(175.f, 25.f);
                    cm.draw(action_text);
                    if (!tutorial.enemy_dead())
                        n_step = 8;
                    break;
                }
                case 8:
                {
                    action_text.setString("Good Game\nPress " + acc_but + " to exit.");
                    action_text.setPosition(175.f, 25.f);
                    cm.draw(action_text);
                    break;
                }
            }

            cm.display();//Draw to the screen

        }
        music.stop();
    }

}}
