/*
 * main.cpp
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

#include "main.hpp"

namespace fdx{ namespace fighter
{

    /*
        Code
    */

    /* Functions */

    //Launches the single player mode
    void launch_single_player ()
    {
        //SFML variables
        fdx::fighter::Camera cm(600,600,"Shield_",16,0);
        sf::Texture clock_txture;
        sf::Texture spt;
        sf::Texture bg_spt;
        sf::Texture hudtext;
        sf::Texture fire;
        sf::Event event;
        sf::Music music;

        //Load the texure
        if (!spt.loadFromFile("media/images/SFighter.png"))
            std::cout<<"Error loading texture"<<std::endl;
        spt.setSmooth(true);

         //Load the texure
        if (!music.openFromFile("media/sounds/battle_music.ogg"))
            std::cout<<"Error loading music"<<std::endl;
        music.play();
        music.setLoop(true);

        //Load the clock
        if (!clock_txture.loadFromFile("media/images/clock.png"))
            std::cout<<"Error loading clock"<<std::endl;

        if (!bg_spt.loadFromFile("media/images/fondo.jpg"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!hudtext.loadFromFile("media/images/HUD.png"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!fire.loadFromFile("media/images/Fire.png"))
            std::cout<<"Error loading texture"<<std::endl;

        sf::SoundBuffer fr;
        if (!fr.loadFromFile("media/sounds/fire.wav"))
            std::cout<<"Error loading texture"<<std::endl;

        //Laser model
        fdx::fighter::Laser_Model lm(5000000,5,10,1e-3,20,50,fdx::fighter::Team(true));
        fdx::fighter::Laser_Model lm2(5000000,5,10,1e-3,10,1,fdx::fighter::Team(false));

        //Spaceship
        fdx::fighter::Spaceship spshp   (fdx::arrow::Vct(200,200),spt,fdx::fighter::Spshp_config(),fdx::fighter::Team(true),300,2e4);

        //Cannon
        fdx::fighter::Cannon cn(lm,fdx::arrow::Vct(0.7,0),nullptr,nullptr,200000);
        cn.put_sound(&fr);
        fdx::fighter::Cannon cn2(lm2,fdx::arrow::Vct(0.7,0),nullptr,nullptr,200000);
        cn2.put_sound(&fr);
        spshp.put_cannon(cn);

        //Propeller
        spshp.put_propeller(fdx::fighter::Propeller(fire,fdx::arrow::Vct(-0.42,0)));

        //Game mode
        fdx::fighter::Offline_Survival mode
        (
            spt,fdx::fighter::Spshp_config(5e-10,3e-4,5e-10,1,0,1e-3,0),cn2
        );
        mode.put_player(&spshp);

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

        fdx::fighter::HUD_mode hud2(mode,clock_txture,ft,0,600,200,250,200,20,20,50,20,120,10,20);

        //Controller
        fdx::fighter::Inter_Controller *ctrler=controllers.get_controller();
        spshp.set_controller(ctrler);

        while (cm.isOpen())
        {
            fdx::fighter::KM_Controller* ptr_km=dynamic_cast<KM_Controller *>(ctrler);
            fdx::fighter::GP_Controller* ptr_gp=dynamic_cast<GP_Controller *>(ctrler);

            while (cm.pollEvent(event))
            {
                bool processed=false;

                if (ptr_km)
                {
                    processed=ptr_km->process_event(event);
                }
                else if (ptr_gp)
                {
                    processed=ptr_gp->process_event(event);
                }

                if (!processed) switch (event.type)
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
                                    new_text.setFillColor(fdx::fighter::teams.get_enemy());
                                    new_text.setOutlineColor(fdx::fighter::teams.get_enemy());
                                    td.add_text(new_text);
                                }
                                else
                                {
                                    sf::Text new_text;
                                    new_text.setString(menu::Language::getText(menu::Language::VSYNC_OFF));
                                    new_text.setFillColor(fdx::fighter::teams.get_friendly());
                                    new_text.setOutlineColor(fdx::fighter::teams.get_friendly());
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


            if (ptr_km)
                ptr_km->process_point(cm.get_tra_ms().x,cm.get_tra_ms().y);
            else if (ptr_gp)
                ptr_gp->process_center(spshp.get_center());

            //Update and move
            //spshp.point(cm.get_tra_ms());//Point the spaceship at the translated mouse
            unsigned long long int tick=ck.update_all();//Get the next tick
            spshp.mov(tick);//Move the spaceship
            hud.update(tick);//Update the HUD values

            while (mode.has_text())
                td.add_text(mode.get_text());

            td.update(tick);//Update the TD
            mode.update(tick);//Update the game mode

            cm.focus_target(spshp.get_center());//Focus the camera on the moved spaceship
            bgd.mov(cm.get_translation());//Move the background

            //Clear the camera
            cm.clear();

            //Draw on the default view
            cm.view_default();
            cm.draw(bgd);//Background
            cm.draw(ck);//Clock
            cm.draw(hud);//HUD
            cm.draw(hud2);
            cm.draw(td);//Text displayer

            //Draw on the translated view
            cm.view_translated();
            cm.draw(spshp);//Spaceship
            cm.draw(mode);//Mode elements
            sf::CircleShape pointer(5);
            pointer.setFillColor(sf::Color::White);
            pointer.setOrigin(5,5);
            pointer.setPosition(ctrler->point_x(),ctrler->point_y());
            cm.draw(pointer);

            cm.display();//Draw to the screen
        }
        music.stop();
    }

    //Launches the single player mode
    void launch_multi_player()
    {
        //SFML variables
        fdx::fighter::Camera cm(1200,600,"Shield_",16,0);
        sf::Texture clock_txture;
        sf::Texture spt;
        sf::Texture bg_spt;
        sf::Texture hudtext;
        sf::Texture fire;
        sf::Event event;
        sf::SoundBuffer died;
        sf::SoundBuffer fr;
        sf::Texture died_img;
        sf::Sound fr_snd;

        sf::Texture jtexture1;
        sf::Texture jtexture2;
        sf::Texture jtexture3;
        sf::SoundBuffer jbuffer1, jbuffer2, jbuffer3;
        sf::Music music;

        //Load the texure
        if (!music.openFromFile("media/sounds/battle_music.ogg"))
            std::cout<<"Error loading music"<<std::endl;
        music.play();
        music.setLoop(true);

        //Load the texure
        if (!spt.loadFromFile("media/images/SFighter.png"))
            std::cout<<"Error loading texture"<<std::endl;
        spt.setSmooth(true);

        if (!died_img.loadFromFile("media/images/died_img.png"))
            std::cout<<"Error loading texture"<<std::endl;
        spt.setSmooth(true);

        if (!died.loadFromFile("media/sounds/died.wav"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!fr.loadFromFile("media/sounds/fire.wav"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!jtexture1.loadFromFile("media/images/joke1.png"))
            std::cout<<"Error loading texture"<<std::endl;
        jtexture1.setSmooth(true);

        if (!jtexture2.loadFromFile("media/images/joke2.png"))
            std::cout<<"Error loading texture"<<std::endl;
        jtexture2.setSmooth(true);

        if (!jtexture3.loadFromFile("media/images/joke3.png"))
            std::cout<<"Error loading texture"<<std::endl;
        jtexture3.setSmooth(true);

        if (!jbuffer1.loadFromFile("media/sounds/joke1.wav"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!jbuffer2.loadFromFile("media/sounds/joke2.wav"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!jbuffer3.loadFromFile("media/sounds/joke3.wav"))
            std::cout<<"Error loading texture"<<std::endl;

        //Load the clock
        if (!clock_txture.loadFromFile("media/images/clock.png"))
            std::cout<<"Error loading clock"<<std::endl;

        if (!bg_spt.loadFromFile("media/images/fondo.jpg"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!hudtext.loadFromFile("media/images/HUD.png"))
            std::cout<<"Error loading texture"<<std::endl;

        if (!fire.loadFromFile("media/images/Fire.png"))
            std::cout<<"Error loading texture"<<std::endl;

        //Laser model
        fdx::fighter::Laser_Model lm(5000000,5,10,1e-3,20,50,fdx::fighter::Team(true));
        fdx::fighter::Laser_Model lm2(5000000,5,10,1e-3,20,50,fdx::fighter::Team(false));

        //Spaceship
        fdx::fighter::Spaceship spshp       (fdx::arrow::Vct(0,0),spt,fdx::fighter::Spshp_config(),fdx::fighter::Team(true),300,2e4);
        fdx::fighter::Spaceship spshp2      (fdx::arrow::Vct(600,600),spt,fdx::fighter::Spshp_config(),fdx::fighter::Team(false),300,2e4);

        //Cannon
        fdx::fighter::Cannon cn(lm,fdx::arrow::Vct(0.7,0),nullptr,nullptr,200000);
        fdx::fighter::Cannon cn2(lm2,fdx::arrow::Vct(0.7,0),nullptr,nullptr,200000);

        cn.put_sound(&fr);
        cn2.put_sound(&fr);

        spshp.put_cannon(cn);
        spshp2.put_cannon(cn2);

        //Propeller
        spshp.put_propeller(fdx::fighter::Propeller(fire,fdx::arrow::Vct(-0.42,0)));
        spshp2.put_propeller(fdx::fighter::Propeller(fire,fdx::arrow::Vct(-0.42,0)));

        //Game mode
        fdx::fighter::Multi_DM mode;

        mode.sel_player(fdx::fighter::Multi_DM::PLAYER_1);
        mode.put_player(&spshp);

        mode.sel_player(fdx::fighter::Multi_DM::PLAYER_2);
        mode.put_player(&spshp2);

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
        fdx::fighter::Text_displayer td2(ft,900,20);

        bool vsync=false;

        //HUD
        fdx::fighter::HUD_player hud(spshp,600,600,150,180,100,20,50,20,60,20,80,20);
        spshp.put_ind();

        fdx::fighter::HUD_mode hud2(mode,clock_txture,ft,0,600,200,250,200,20,20,50,20,120,10,20);

        fdx::fighter::HUD_player hud_1(spshp2,1200,600,150,180,100,20,50,20,60,20,80,20);
        spshp2.put_ind();

        fdx::fighter::HUD_mode hud_2(mode,clock_txture,ft,600,600,200,250,200,20,20,50,20,120,10,20);

        //Controller

        fdx::fighter::Inter_Controller *ctrler=controllers.get_controller();
        fdx::fighter::Inter_Controller *ctrler2=controllers.get_controller2();
        spshp.set_controller(ctrler);
        spshp2.set_controller(ctrler2);

        //Naming
        fdx::fighter::Spaceship::set_font(&ft);
        spshp.set_name("Player 1");
        spshp2.set_name("Player 2");

		//Joke manager
		fdx::fighter::JokeManager joke;

        fdx::fighter::Joke jk1(ft, "Hahah", jtexture1, jbuffer1, fdx::fighter::teams.get_neutral());
        fdx::fighter::Joke jk2(ft, "Good Job", jtexture2, jbuffer2, fdx::fighter::teams.get_friendly());
        fdx::fighter::Joke jk3(ft, "Nooo", jtexture3, jbuffer3, fdx::fighter::teams.get_enemy());

        mode.put_sound(&died);
        mode.put_image(&died_img);
        fr_snd.setBuffer(fr);

        while (cm.isOpen())
        {
            fdx::fighter::KM_Controller* ptr_km=dynamic_cast<KM_Controller *>(ctrler);
            fdx::fighter::GP_Controller* ptr_gp=dynamic_cast<GP_Controller *>(ctrler);

            fdx::fighter::KM_Controller* ptr_km2=dynamic_cast<KM_Controller *>(ctrler2);
            fdx::fighter::GP_Controller* ptr_gp2=dynamic_cast<GP_Controller *>(ctrler2);

            while (cm.pollEvent(event))
            {
                bool processed=false;

                if (ptr_km)
                {
                    processed=ptr_km->process_event(event);
                }
                else if (ptr_gp)
                {
                    processed=ptr_gp->process_event(event);
                }

                if (!processed)
                {
                    if (ptr_km2)
                    {
                        processed=ptr_km2->process_event(event);
                    }
                    else if (ptr_gp2)
                    {
                        processed=ptr_gp2->process_event(event);
                    }
                }

                if (!processed) switch (event.type)
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
                                    new_text.setFillColor(fdx::fighter::teams.get_enemy());
                                    new_text.setOutlineColor(fdx::fighter::teams.get_enemy());
                                    td.add_text(new_text);
                                    td2.add_text(new_text);
                                }
                                else
                                {
                                    sf::Text new_text;
                                    new_text.setString(menu::Language::getText(menu::Language::VSYNC_OFF));
                                    new_text.setFillColor(fdx::fighter::teams.get_friendly());
                                    new_text.setOutlineColor(fdx::fighter::teams.get_friendly());
                                    td.add_text(new_text);
                                    td2.add_text(new_text);
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

            if (ctrler->ctrl_joke1())
            {
                jk1.set_pos(spshp.get_tip().x, spshp.get_tip().y);
                joke.add_joke(jk1);
            }
            if (ctrler->ctrl_joke2())
            {
                jk2.set_pos(spshp.get_tip().x, spshp.get_tip().y);
                joke.add_joke(jk2);
            }
            if (ctrler->ctrl_joke3())
            {
                jk3.set_pos(spshp.get_tip().x, spshp.get_tip().y);
                joke.add_joke(jk3);
            }

            if (ctrler2->ctrl_joke1())
            {
                jk1.set_pos(spshp2.get_tip().x, spshp2.get_tip().y);
                joke.add_joke(jk1);
            }
            if (ctrler2->ctrl_joke2())
            {
                jk2.set_pos(spshp2.get_tip().x, spshp2.get_tip().y);
                joke.add_joke(jk2);
            }
            if (ctrler2->ctrl_joke3())
            {
                jk3.set_pos(spshp2.get_tip().x, spshp2.get_tip().y);
                joke.add_joke(jk3);
            }



            cm.focus_target(spshp.get_center());
            cm.view_translated();
            cm.set_player_multi(fdx::fighter::Camera::PLAYER_1);
            if (ptr_km)
                ptr_km->process_point(cm.get_tra_ms().x,cm.get_tra_ms().y);
            else if (ptr_gp)
                ptr_gp->process_center(spshp.get_center());



            cm.focus_target(spshp2.get_center());
            cm.view_translated();
            cm.set_player_multi(fdx::fighter::Camera::PLAYER_2);
            if (ptr_km2)
                ptr_km2->process_point(cm.get_tra_ms().x,cm.get_tra_ms().y);
            else if (ptr_gp2)
                ptr_gp2->process_center(spshp2.get_center());

            //Update and move
            //spshp.point(cm.get_tra_ms());//Point the spaceship at the translated mouse
            unsigned long long int tick=ck.update_all();//Get the next tick
            if (!mode.end_game())
            {
                spshp.mov(tick);//Move the spaceship
                spshp2.mov(tick);
                hud.update(tick);//Update the HUD values
                hud_1.update(tick);
            }

            mode.update(tick);//Update the game mode
            td.update(tick);//Update the TD
            td2.update(tick);
            joke.update(tick);
            mode.sel_player(fdx::fighter::Multi_DM::PLAYER_1);
            while (mode.has_text())
                td.add_text(mode.get_text());

            mode.sel_player(fdx::fighter::Multi_DM::PLAYER_2);
            while (mode.has_text())
                td2.add_text(mode.get_text());

            //Clear the camera
            cm.clear();

            //PLAYER 1

            cm.focus_target(spshp.get_center());//Focus the camera on the moved spaceship
            bgd.mov(cm.get_translation());//Move the background

            //Draw on the default view
            cm.view_default();
            cm.set_player_multi(fdx::fighter::Camera::PLAYER_1);
            cm.draw(bgd);//Background

            cm.view_default();
            cm.set_player_single();
            mode.sel_player(fdx::fighter::Multi_DM::PLAYER_1);
            cm.draw(hud);//HUD
            cm.draw(hud2);
            cm.draw(ck);//Clock

            //Draw on the translated view
            cm.view_translated();
            cm.set_player_multi(fdx::fighter::Camera::PLAYER_1);
            cm.draw(spshp);//Spaceship
            cm.draw(spshp2);
            cm.draw(mode);//Mode elements
            cm.draw(joke);
            sf::CircleShape pointer(5);
            pointer.setFillColor(sf::Color::White);
            pointer.setOrigin(5,5);
            pointer.setPosition(ctrler->point_x(),ctrler->point_y());
            cm.draw(pointer);

            //PLAYER_2

            cm.focus_target(spshp2.get_center());//Focus the camera on the moved spaceship
            bgd.mov(cm.get_translation());//Move the background

            //Draw on the default view
            cm.view_default();
            cm.set_player_multi(fdx::fighter::Camera::PLAYER_2);
            cm.draw(bgd);//Background

            cm.view_default();
            cm.set_player_single();
            mode.sel_player(fdx::fighter::Multi_DM::PLAYER_2);
            cm.draw(hud_1);//HUD
            cm.draw(hud_2);

            //Draw on the translated view
            cm.view_translated();
            cm.set_player_multi(fdx::fighter::Camera::PLAYER_2);
            cm.draw(spshp2);//Spaceship
            cm.draw(spshp);
            cm.draw(mode);//Mode elements
            cm.draw(joke);
            sf::CircleShape pointer2(5);
            pointer2.setFillColor(sf::Color::White);
            pointer2.setOrigin(5,5);
            pointer2.setPosition(ctrler2->point_x(),ctrler2->point_y());
            cm.draw(pointer2);

            //Text

            cm.view_default();
            cm.set_player_single();
            cm.draw(td);
            cm.draw(td2);

            cm.display();//Draw to the screen
        }
        music.stop();
    }
}}
