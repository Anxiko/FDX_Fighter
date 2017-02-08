#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <array>
#include "ClassMenu.hpp"
#include <fstream>
#include "System.hpp"
#include "TutorialMain.hpp"
#include "main.hpp"

void launch_options(fdx::menu::Config &cfg, const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font);
void launch_options_colors(fdx::menu::Config &cfg, const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font);
void launch_options_language(fdx::menu::Config &cfg, const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font);
void launch_options_controls(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font);
void launch_options_configure_mouse(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font);
void launch_options_configure_xbox(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font);
int launch_options_second_control(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font);

int main()
{
    //Music

    sf::Music music;
    //Load the track
    if (!music.openFromFile("media/sounds/menu_music.ogg"))
        std::cout<<"Error loading music"<<std::endl;
    music.play();
    music.setLoop(true);

    /*Bindings's file*/
    std::ifstream fbindings("bindings.data", std::ios::binary);

    //Exists and it's open
    if (fbindings.is_open())
    {
        fdx::fighter::controllers.read_from(fbindings);
    }
    fbindings.close();

    /*Variables*/

    /*Button*/
    sf::Font font;
    sf::Sound over;
    sf::Sound press;
    sf::SoundBuffer buffer1, buffer2;
    int x,y,but = false;

     /*Load font*/
    if(!font.loadFromFile("media/fonts/ethnocentric rg.ttf"))
        std::cout << "Font not found" << std::endl;

    /*Load Sounds*/
    buffer1.loadFromFile("media/sounds/Pressed.wav");
    press.setBuffer(buffer1);
    buffer2.loadFromFile("media/sounds/Over.wav");
    over.setBuffer(buffer2);

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture menu_background;
    sf::Texture menu_options;
    sf::Texture menu_exit;

    /*Load images*/
    if (!menu_background.loadFromFile("media/images/background.png"))
        std::cout << "Image not found" << std::endl;

    if (!menu_options.loadFromFile("media/images/options_img.png"))
        std::cout << "Image not found" << std::endl;

    if (!menu_exit.loadFromFile("media/images/exit_img.png"))
        std::cout << "Image not found" << std::endl;

    menu_options.setSmooth(true);
    menu_exit.setSmooth(true);

    //Configuration

    fdx::menu::Config config;//Contains the default configuration, can be read from file if file exists

    std::ifstream if_config("config.data",std::ios::binary);

    //Exists and it's open
    if (if_config)
    {
        config.read_from(if_config);
    }
    //Doesn't exist, set config and write it
    else
    {
        launch_options_language(config, menu_background, over, press, font);
        launch_options_colors(config, menu_background, over, press, font);

        std::ofstream of_config("config.data",std::ios::binary|std::ios::trunc);
        config.write_to(of_config);
    }

    //Apply the config
    config.apply();

    /*Window*/
    sf::RenderWindow window(sf::VideoMode(600, 600), "SHIELD_ MENU");

    /*Create menu*/
    fdx::menu::Menu menu_principal(menu_background, "Shield_", 50, 160, 50, font, sf::Color::White);

    /*Draw principal menu's buttons*/
    menu_principal.add_button(new fdx::menu::ButtonText(160, 150, 275, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::Language::SINGLEPLAYER), font, 20, sf::Color::Black, sf::Color(191,191,191)));
    menu_principal.add_button(new fdx::menu::ButtonText(160, 250, 275, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::Language::MULTIPLAYER), font, 20, sf::Color::Black, sf::Color(191,191,191)));
    menu_principal.add_button(new fdx::menu::ButtonText(160, 350, 275, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::Language::STATISTICS), font, 20, sf::Color::Black, sf::Color(191,191,191)));
    menu_principal.add_button(new fdx::menu::ButtonImage(160, 450, 120, 70, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, menu_options));
    menu_principal.add_button(new fdx::menu::ButtonImage(315, 450, 120, 70, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, menu_exit));


    while (window.isOpen())
    {
        window.setSize(sf::Vector2u(600,600));
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }

                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /*Button selected*/

        switch (menu_principal.button_pressed(x, y, but))
        {
            //Singleplayer
            case 0:
            {
                music.stop();
                fdx::fighter::launch_single_player();
                music.play();
                break;
            }
            //Multiplayer
            case 1:
            {
                fdx::fighter::controllers.set_controller2(launch_options_second_control(menu_background, over, press, font));
                music.stop();
                fdx::fighter::launch_multi_player();
                music.play();
                break;
            }
            //Tutorial
            case 2:
            {
                music.stop();
                fdx::fighter::launch_tutorial();
                music.play();
                break;
            }
            //Options
            case 3:
            {
                int cp_lang = config.get_lang();
                launch_options(config, menu_background, over, press, font);
                //Language changed
                if (cp_lang != config.get_lang())
                    window.close();

                config.apply();
                std::ofstream of_config("config.data",std::ios::binary|std::ios::trunc);
                config.write_to(of_config);

                break;
            }
            //Exit
            case 4:
            {
                window.close();
                break;
            }
        }

        window.clear();
        window.draw(menu_principal);
        window.display();
    }

    return 0;
}


void launch_options(fdx::menu::Config &cfg, const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font)
{
    /*Window*/
    sf::RenderWindow options(sf::VideoMode(600, 500), fdx::menu::Language::getText(fdx::menu::Language::OPTIONS));

    /*Button*/
    int x,y,but = false;

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture go_back;

    /*Load image*/
    if (!go_back.loadFromFile("media/images/back_img.png"))
        std::cout << "Image not found" << std::endl;

    /*Create menu*/
    fdx::menu::Menu menu_options(menu_background, fdx::menu::Language::getText(fdx::menu::Language::OPTIONS), 25, 220, 30, font, sf::Color::White);


    /*Draw option menu's buttons*/
    menu_options.add_button(new fdx::menu::ButtonText(160, 75, 275, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), false, over, press, fdx::menu::Language::getText(fdx::menu::Language::PERSONALIZE), font, 20, sf::Color::Black,sf::Color(205,201,201,255)));
    menu_options.add_button(new fdx::menu::ButtonText(160, 155, 275, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::COLOR), font, 20, sf::Color::Black,sf::Color(205,201,201,255)));
    menu_options.add_button(new fdx::menu::ButtonText(160, 240, 275, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::LANGUAGE), font, 20, sf::Color::Black,sf::Color(205,201,201,255)));
    menu_options.add_button(new fdx::menu::ButtonText(160, 325, 275, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::CONTROLS), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_options.add_button(new fdx::menu::ButtonImage(160, 405, 140, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, go_back));

    while (options.isOpen())
    {
        options.setSize(sf::Vector2u(600,500));
        sf::Event event;
        while (options.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }

                case sf::Event::Closed:
                {
                    options.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                options.close();
        }

        /*Button selected*/

        switch (menu_options.button_pressed(x, y, but))
        {
            //Personalize
            case 0:
            {
                break;
            }
            //Color
            case 1:
            {
                launch_options_colors(cfg, menu_background, over, press, font);
                break;
            }
            //Language
            case 2:
            {
                int cp_lang = cfg.get_lang();

                launch_options_language(cfg, menu_background, over, press, font);
                //Language changed
                if(cp_lang != cfg.get_lang())
                    options.close();
                break;
            }
            //Controls
            case 3:
            {
                launch_options_controls(menu_background, over, press, font);
                break;
            }
            //Back
            case 4:
            {
                options.close();
                break;
            }
        }

        options.clear();
        options.draw(menu_options);
        options.display();
    }
}

void launch_options_colors(fdx::menu::Config &cfg, const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font)
{
    /*Window*/
    sf::RenderWindow colors(sf::VideoMode(600, 400), fdx::menu::Language::getText(fdx::menu::Language::OPTIONS));

    /*Button*/
    int x,y,but = false;

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture go_back;

    /*Load image*/
    if (!go_back.loadFromFile("media/images/back_img.png"))
        std::cout << "Image not found" << std::endl;

    /*Create menu*/
    fdx::menu::Menu options_colors(menu_background, fdx::menu::Language::getText(fdx::menu::Language::COLORS), 25, 220, 30, font, sf::Color::White);

    //Array of colors selected
    std::array<int, 3> color_selected ({cfg.get_ally(),cfg.get_neutral(),cfg.get_enemy()});
    //Array of names
    std::array<std::string, 3> names = {{"A", "N", "E"}};
    //Index of array
    int ptr = -1;
    //Number of buttons
    int nb = 11;

    /*Draw color menu's buttons*/
    for(int i = 0; i < nb; ++i)
    {
        if (i < 3)
            options_colors.add_button(new fdx::menu::ButtonText(100+(i*150), 100, 120, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, names[i], font, 50, sf::Color::Black, sf::Color(205,201,201,255)));
        else
            options_colors.add_button(new fdx::menu::ButtonImage(20+((i-3)*70), 200, 65, 50, 5, 5, fdx::menu::DictionaryColors::getSemiColor(i-3), sf::Color(150,60,0,255), false, over, press, sf::Texture()));
    }

    /*Confirm and back Button*/
    options_colors.add_button(new fdx::menu::ButtonText(330, 275, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), false, over, press, fdx::menu::Language::getText(fdx::menu::Language::CONFIRM), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    options_colors.add_button(new fdx::menu::ButtonImage(120, 275, 150, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, go_back));

    while (colors.isOpen())
    {
        colors.setSize(sf::Vector2u(600,400));
        sf::Event event;
        while (colors.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }

                case sf::Event::Closed:
                {
                    colors.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                colors.close();
        }

        //Copy of index
        int cp_ptr;

        //Button selected
        ptr = options_colors.button_pressed(x,y,but);

        //It's the default value, do nothing
        if (ptr == -1);

        //Button has been pressed
        else
        {
            //It's an ANE color
            if (ptr < 3)
            {
                //Get the copy for change color later
                cp_ptr = ptr;

                //Mark the button selected
                options_colors.change_color_rect(ptr, sf::Color::Transparent);

                //Unlock color buttons
                for (int i = 3; i < nb; ++i)
                {
                    if (color_selected[0] != i-3 && color_selected[1] != i-3 && color_selected[2] != i-3)
                    {
                        options_colors.unlock_buttons(i);
                        options_colors.change_color_rect(i, fdx::menu::DictionaryColors::getColor(i-3));
                    }
                }
            }

            //It's an button color or finish button
            else
            {
                //It's an button color
                if(ptr < 11)
                {
                    //Fill the mark button with the color selected
                    options_colors.change_color_rect(cp_ptr, fdx::menu::DictionaryColors::getColor(ptr-3));
                    color_selected[cp_ptr] = ptr - 3;

                    if (color_selected[0] != -1 && color_selected[1] != -1 && color_selected[2] != -1)
                    {
                        options_colors.unlock_buttons(11);
                    }

                    //Block the color buttons
                    for (int i = 3; i < nb; ++i)
                    {
                        options_colors.lock_buttons(i);
                        options_colors.change_color_rect(i, fdx::menu::DictionaryColors::getSemiColor(i-3));
                    }
                }

                //It's the finish button
                else
                {
                    //It's the back button
                    if(ptr == 12)
                    {
                        colors.close();
                    }

                    //It's the confirm button
                    else
                    {
                        cfg.set_ally(color_selected[0]);
                        cfg.set_neutral(color_selected[1]);
                        cfg.set_enemy(color_selected[2]);
                        colors.close();
                    }
                }
            }
        }


        colors.clear();
        colors.draw(options_colors);
        colors.display();
    }
}

void launch_options_language(fdx::menu::Config &cfg, const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font)
{
    /*Window*/
    sf::RenderWindow language(sf::VideoMode(600, 400), fdx::menu::Language::getText(fdx::menu::Language::OPTIONS));

    /*Button*/
    int x,y,but = false;

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture go_back;
    sf::Texture esp_flag;
    sf::Texture eng_flag;

    /*Load image*/
    if (!go_back.loadFromFile("media/images/back_img.png"))
        std::cout << "Image not found" << std::endl;

    if (!esp_flag.loadFromFile("media/images/esp_flag.png"))
        std::cout << "Image not found" << std::endl;

    if (!eng_flag.loadFromFile("media/images/eng_flag.png"))
        std::cout << "Image not found" << std::endl;


    /*Create menu*/
    fdx::menu::Menu menu_language(menu_background, fdx::menu::Language::getText(fdx::menu::Language::LANGUAGES), 25, 210, 30, font, sf::Color::White);


    /*Draw language menu's buttons*/
    menu_language.add_button(new fdx::menu::ButtonImage(100, 100, 125, 75, 10, 10, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, esp_flag));
    menu_language.add_button(new fdx::menu::ButtonImage(325, 100, 125, 75, 10, 10, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, eng_flag));
    menu_language.add_button(new fdx::menu::ButtonText(325, 240, 200, 50, 10, 10, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::CONFIRM), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_language.add_button(new fdx::menu::ButtonImage(100, 240, 140, 50, 10, 10, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, go_back));
    menu_language.add_button(new fdx::menu::ButtonText(120, 320, 350, 50, 10, 10, sf::Color::Transparent, sf::Color::Transparent, false, sf::Sound(), sf::Sound(), fdx::menu::Language::getText(fdx::menu::Language::RESTART), font, 10, sf::Color::Yellow, sf::Color::Transparent));

    //New chosen language
    fdx::menu::Language::en_type new_lang=cfg.get_lang();

    while (language.isOpen())
    {
        language.setSize(sf::Vector2u(600,400));
        sf::Event event;
        while (language.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }

                case sf::Event::Closed:
                {
                    language.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                language.close();
        }

        /*Button selected*/

        switch (menu_language.button_pressed(x, y, but))
        {
            //Spanish
            case 0:
            {
                new_lang = fdx::menu::Language::Langs::SPANISH;
                //Mark the esp flag, unmark the eng flag
                menu_language.change_color_image(0, sf::Color(255,255,255,128));
                menu_language.change_color_image(1, sf::Color(255,255,255,255));
                //It's the other option
                if (cfg.get_lang() != new_lang)
                {
                    //Show message
                    menu_language.unlock_buttons(4);
                }
                else
                {
                    //Hide message
                    menu_language.lock_buttons(4);
                }
                break;
            }
            //English
            case 1:
            {
                new_lang = fdx::menu::Language::Langs::ENGLISH;
                //Mark the esp flag, unmark the eng flag
                menu_language.change_color_image(1, sf::Color(255,255,255,128));
                menu_language.change_color_image(0, sf::Color(255,255,255,255));
                //It's the other option
                if (cfg.get_lang() != new_lang)
                {
                    //Show message
                    menu_language.unlock_buttons(4);
                }
                else
                {
                    //Hide message
                    menu_language.lock_buttons(4);

                }
                break;
            }
            //Confirm
            case 2:
            {
                cfg.set_lang(new_lang);
                language.close();
                break;
            }
            //Back
            case 3:
            {
                language.close();
                break;
            }
        }

        language.clear();
        language.draw(menu_language);
        language.display();
    }
}

void launch_options_controls(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font)
{
    /*Window*/
    sf::RenderWindow controls(sf::VideoMode(600, 420), fdx::menu::Language::getText(fdx::menu::Language::OPTIONS));

    /*Button*/
    int x,y,but = false;

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture go_back;
    sf::Texture xbox;
    sf::Texture mouse;

    /*Load image*/
    if (!go_back.loadFromFile("media/images/back_img.png"))
        std::cout << "Image not found" << std::endl;

    if (!xbox.loadFromFile("media/images/xbox.png"))
        std::cout << "Image not found" << std::endl;

    if (!mouse.loadFromFile("media/images/mouse.png"))
        std::cout << "Image not found" << std::endl;

    /*Create menu*/
    fdx::menu::Menu menu_controls(menu_background, fdx::menu::Language::getText(fdx::menu::Language::CONTROLS), 25, 200, 30, font, sf::Color::White);


    /*Draw option menu's buttons*/
    menu_controls.add_button(new fdx::menu::ButtonImage(100, 105, 100, 100, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, mouse));
    menu_controls.add_button(new fdx::menu::ButtonImage(415, 105, 100, 100, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, xbox));
    menu_controls.add_button(new fdx::menu::ButtonText(100, 240, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), false, over, press, fdx::menu::Language::getText(fdx::menu::Language::CONFIGURE), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_controls.add_button(new fdx::menu::ButtonText(320, 240, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), false, over, press, fdx::menu::Language::getText(fdx::menu::Language::CONFIRM), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_controls.add_button(new fdx::menu::ButtonImage(100, 320, 140, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, go_back));

    menu_controls.change_color_image(fdx::fighter::controllers.get_cont_num(), sf::Color(255,255,255,128));

    while (controls.isOpen())
    {
        sf::Event event;
        while (controls.pollEvent(event))
        {
            switch (event.type)
            {
                 case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }
                case sf::Event::Closed:
                {
                    controls.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                controls.close();
        }

        //Button selected
        int control;

        switch (menu_controls.button_pressed(x, y, but))
        {
            //Keyboard and mouse
            case 0:
            {
                //Keyboard and mouse
                control = 0;
                //Mark the button, unmark the others
                menu_controls.change_color_image(0, sf::Color(255,255,255,128));
                menu_controls.change_color_image(1, sf::Color(255,255,255,255));
                menu_controls.change_color_image(2, sf::Color(255,255,255,255));
                //Unlock confirm
                menu_controls.unlock_buttons(3);
                menu_controls.unlock_buttons(4);

                break;
            }
            //Xbox controlles
            case 1:
            {
                //Xbox controller
                control = 1;
                //Mark the button, unmark the others
                menu_controls.change_color_image(1, sf::Color(255,255,255,128));
                menu_controls.change_color_image(0, sf::Color(255,255,255,255));
                menu_controls.change_color_image(2, sf::Color(255,255,255,255));
                //Unlock confirm
                menu_controls.unlock_buttons(3);
                menu_controls.unlock_buttons(4);

                break;
            }
            case 2:
            {
                if (control == 0)
                    launch_options_configure_mouse(menu_background, over, press, font);
                else if (control == 1)
                    launch_options_configure_xbox(menu_background, over, press, font);
                break;
            }

            case 3:

                if (control>=0&&control<=2)
                    fdx::fighter::controllers.set_controller(control);
            case 4:

                //Save
                std::ofstream fbindings("bindings.data", std::ios::trunc);
                fdx::fighter::controllers.write_to(fbindings);
                fbindings.close();

                controls.close();

            break;

        }

        controls.clear();
        controls.draw(menu_controls);
        controls.display();
    }
}

void launch_options_configure_mouse(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font)
{
    /*Window*/
    sf::RenderWindow configure_mouse(sf::VideoMode(600, 420), fdx::menu::Language::getText(fdx::menu::Language::OPTIONS));

    /*Button*/
    int x,y,but = false;

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture go_back;

    /*Load image*/
    if (!go_back.loadFromFile("media/images/back_img.png"))
        std::cout << "Image not found" << std::endl;

    //Controller
    fdx::fighter::KM_Controller controller(fdx::fighter::controllers.get_km());
    int selected_bind=-1;

    /*Create menu with the tittle of the control*/
    fdx::menu::Menu menu_mouse(menu_background, fdx::menu::Language::getText(fdx::menu::Language::MOUSE), 25, 140, 30, font, sf::Color::White);


    /*Text with the spaceship's action*/
    sf::Text shoot(fdx::menu::Language::getText(fdx::menu::Language::SHOOT), font, 20);
    sf::Text speed(fdx::menu::Language::getText(fdx::menu::Language::ACCELERATE), font, 20);
    sf::Text brake(fdx::menu::Language::getText(fdx::menu::Language::BRAKE), font, 20);

    //Actions
    shoot.setPosition(70, 110);
    shoot.setColor(sf::Color::Yellow);
    speed.setPosition(70,190);
    speed.setColor(sf::Color::Yellow);
    brake.setPosition(70,275);
    brake.setColor(sf::Color::Yellow);

    /*Draw buttons*/
    menu_mouse.add_button(new fdx::menu::ButtonText(300, 95, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, controller.get_fire().print(), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_mouse.add_button(new fdx::menu::ButtonText(300, 175, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, controller.get_acc().print(), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_mouse.add_button(new fdx::menu::ButtonText(300, 260, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, controller.get_brake().print(), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_mouse.add_button(new fdx::menu::ButtonText(300, 340, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::CONFIRM), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_mouse.add_button(new fdx::menu::ButtonImage(70, 340, 140, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, go_back));

    while (configure_mouse.isOpen())
    {
        sf::Event event;
        while (configure_mouse.pollEvent(event))
        {
            if (selected_bind>=0&&selected_bind<=2)
            {
                fdx::fighter::Event_KM new_bind;
                if (new_bind.set_from_event(event)&&new_bind.check_event())
                {
                    switch (selected_bind)
                    {
                        case 0:
                        {
                            controller.set_fire(new_bind);
                            break;
                        }

                        case 1:
                        {
                            controller.set_acc(new_bind);
                            break;
                        }

                        case 2:
                        {
                            controller.set_brake(new_bind);
                            break;
                        }

                    }
                    menu_mouse.change_color_rect(selected_bind,sf::Color(255,127,42,255));
                    menu_mouse.change_text(selected_bind,new_bind.print());
                    selected_bind=-1;
                }
            }

            else switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }

                case sf::Event::Closed:
                {
                    configure_mouse.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                configure_mouse.close();
        }

        int pressed_button;
        switch (pressed_button=menu_mouse.button_pressed(x, y, but))
        {
            //Controls
            case 0:
            case 1:
            case 2:

                {
                    selected_bind=pressed_button;
                    menu_mouse.change_color_rect(pressed_button,sf::Color::Red);
                    break;
                }


            //Confirm
            case 3:
                //SAVE HERE
                fdx::fighter::controllers.set_km(controller);

            //Go back
            case 4:

              configure_mouse.close();
              break;
        }

        configure_mouse.clear();
        configure_mouse.draw(menu_mouse);
        configure_mouse.draw(shoot);
        configure_mouse.draw(speed);
        configure_mouse.draw(brake);
        configure_mouse.display();
    }
}

void launch_options_configure_xbox(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font)
{
    /*Window*/
    sf::RenderWindow configure_xbox(sf::VideoMode(600, 500), fdx::menu::Language::getText(fdx::menu::Language::OPTIONS));

    /*Button*/
    int x,y,but = false;

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture go_back;

    /*Load image*/
    if (!go_back.loadFromFile("media/images/back_img.png"))
        std::cout << "Image not found" << std::endl;

    /*Create menu with the tittle of the control*/
    fdx::menu::Menu menu_xbox(menu_background, fdx::menu::Language::getText(fdx::menu::Language::XBOX), 25, 60, 30, font, sf::Color::White);


    /*Text with the spaceship's action*/
    sf::Text shoot(fdx::menu::Language::getText(fdx::menu::Language::SHOOT), font, 20);
    sf::Text speed(fdx::menu::Language::getText(fdx::menu::Language::ACCELERATE), font, 20);
    sf::Text brake(fdx::menu::Language::getText(fdx::menu::Language::BRAKE), font, 20);
    sf::Text stick(fdx::menu::Language::getText(fdx::menu::Language::STICK), font, 20);

    //Actions
    shoot.setPosition(70, 110);
    shoot.setColor(sf::Color::Yellow);
    speed.setPosition(70,190);
    speed.setColor(sf::Color::Yellow);
    brake.setPosition(70,275);
    brake.setColor(sf::Color::Yellow);
    stick.setPosition(70,355);
    stick.setColor(sf::Color::Yellow);

    //Controller
    fdx::fighter::GP_Controller controller(fdx::fighter::controllers.get_gp());
    int selected_bind=-1;

    /*Draw buttons*/
    menu_xbox.add_button(new fdx::menu::ButtonText(300, 95, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, controller.get_fire().print(), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_xbox.add_button(new fdx::menu::ButtonText(300, 175, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, controller.get_acc().print(), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_xbox.add_button(new fdx::menu::ButtonText(300, 260, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, controller.get_brake().print(), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_xbox.add_button(new fdx::menu::ButtonText(300, 340, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, controller.get_stick().print(), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_xbox.add_button(new fdx::menu::ButtonText(300, 420, 200, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, fdx::menu::Language::getText(fdx::menu::Language::CONFIRM), font, 20, sf::Color::Black, sf::Color(205,201,201,255)));
    menu_xbox.add_button(new fdx::menu::ButtonImage(70, 420, 140, 50, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, go_back));

    while (configure_xbox.isOpen())
    {
        sf::Event event;
        while (configure_xbox.pollEvent(event))
        {
            if (selected_bind>=0&&selected_bind<=2)
            {
                fdx::fighter::Event_GPB new_bind;
                if (new_bind.set_from_event(event)&&new_bind.check_event())
                {
                    switch (selected_bind)
                    {
                        case 0:
                        {
                            controller.set_fire(new_bind);
                            break;
                        }

                        case 1:
                        {
                            controller.set_acc(new_bind);
                            break;
                        }

                        case 2:
                        {
                            controller.set_brake(new_bind);
                            break;
                        }
                    }

                        menu_xbox.change_color_rect(selected_bind,sf::Color(255,127,42,255));
                        menu_xbox.change_text(selected_bind,new_bind.print());
                        selected_bind=-1;
                }
            }
            else if (selected_bind==3)
            {
                fdx::fighter::Event_GPS new_bind;
                if (new_bind.set_from_event(event)&&new_bind.check_event())
                {
                    controller.set_stick(new_bind);
                    menu_xbox.change_color_rect(selected_bind,sf::Color(255,127,42,255));
                    menu_xbox.change_text(selected_bind,new_bind.print());
                    selected_bind=-1;
                }
            }
            else switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }

                case sf::Event::Closed:
                {
                    configure_xbox.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                configure_xbox.close();
        }

        int pressed_button;
        switch (pressed_button=menu_xbox.button_pressed(x, y, but))
        {
            //Controls
            case 0:
            case 1:
            case 2:
            case 3:

                {
                    selected_bind=pressed_button;
                    menu_xbox.change_color_rect(pressed_button,sf::Color::Red);
                    break;
                }

            //Confirm
            case 4:

                //SAVE HERE
                fdx::fighter::controllers.set_gp(controller);

            //Go back
            case 5:

                configure_xbox.close();

                break;
        }

        configure_xbox.clear();
        configure_xbox.draw(menu_xbox);
        configure_xbox.draw(shoot);
        configure_xbox.draw(speed);
        configure_xbox.draw(brake);
        configure_xbox.draw(stick);
        configure_xbox.display();
    }
}

int launch_options_second_control(const sf::Texture& menu_background, const sf::Sound& over, const sf::Sound& press, const sf::Font& font)
{
    /*Window*/
    sf::RenderWindow second_control(sf::VideoMode(600, 300), fdx::menu::Language::getText(fdx::menu::Language::OPTIONS));

    /*Button*/
    int x,y,but = false;

    /*Load position*/
    x=sf::Mouse::getPosition().x;
    y=sf::Mouse::getPosition().y;

    /*Images*/
    sf::Texture go_back;
    sf::Texture xbox;
    sf::Texture mouse;

    /*Load image*/
    if (!go_back.loadFromFile("media/images/back_img.png"))
        std::cout << "Image not found" << std::endl;

    if (!xbox.loadFromFile("media/images/xbox.png"))
        std::cout << "Image not found" << std::endl;

    if (!mouse.loadFromFile("media/images/mouse.png"))
        std::cout << "Image not found" << std::endl;

    /*Create menu*/
    fdx::menu::Menu menu_controls(menu_background, "Second control", 25, 175, 30, font, sf::Color::White);


    /*Draw option menu's buttons*/
    menu_controls.add_button(new fdx::menu::ButtonImage(100, 105, 100, 100, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, mouse));
    menu_controls.add_button(new fdx::menu::ButtonImage(415, 105, 100, 100, 5, 5, sf::Color(255,127,42,255), sf::Color(150,60,0,255), true, over, press, xbox));

    menu_controls.change_color_image(fdx::fighter::controllers.get_cont_num(), sf::Color(255,255,255,128));

    //Selected control
    int control=-1;

    while (second_control.isOpen())
    {
        sf::Event event;
        while (second_control.pollEvent(event))
        {
            switch (event.type)
            {
                 case sf::Event::MouseMoved:
                {
                    x=event.mouseMove.x;
                    y=event.mouseMove.y;
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=true;
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button==sf::Mouse::Button::Left)
                        but=false;
                    break;
                }
                case sf::Event::Closed:
                {
                    second_control.close();
                    break;
                }

                default:
                    break;
            }
            if (event.type == sf::Event::Closed)
                second_control.close();
        }

        switch (menu_controls.button_pressed(x, y, but))
        {
            //Keyboard and mouse
            case 0:
            {
                //Keyboard and mouse
                control=0;
                second_control.close();

                break;
            }
            //Xbox controlles
            case 1:
            {
                //Xbox controller
                control=1;
                second_control.close();

                break;
            }
        }

        second_control.clear();
        second_control.draw(menu_controls);
        second_control.display();
    }

    return control;
}
