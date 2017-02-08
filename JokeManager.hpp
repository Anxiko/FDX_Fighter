#ifndef _FDX_JOKEMANAGER_H_
#define _FDX_JOKEMANAGER_H_

#include "Joke.hpp"

namespace fdx { namespace fighter
{
class JokeManager;

class JokeManager : public sf::Drawable
{
    private:

        unsigned int t;//Time to delete a text
        int tr=0;
        static constexpr unsigned int DEF_TIME=1000000u;//Time to delete 1 lines
        static constexpr unsigned int DEF_REF=100000;
        std::deque<fdx::fighter::Joke> jokes;//Texts being displayed

    public:

        JokeManager ()
        {

        }

        virtual ~JokeManager() {}

    private:

            void delete_joke()
            {
                if (!jokes.empty())
                    jokes.pop_back();
            }

    public:

            void add_joke(fdx::fighter::Joke& jk)
            {
                if (tr==0)
                {
                    jokes.push_front(jk);
                    jk.play_sound_joke();
                    tr=DEF_REF;
                }
            }

            void update(unsigned int tick)
            {
                t+=tick;
                if(tr)
                {
                    if (static_cast<int>(tick)>tr)
                        tr=0;
                    else
                        tr-=tick;
                }
                while (t>=DEF_TIME)
                {
                    delete_joke();
                    t-=DEF_TIME;
                }
            }

            void draw(sf::RenderTarget& target, sf::RenderStates states) const//Draw the joke
            {
                for (const fdx::fighter::Joke& jk : jokes)
                {
                    target.draw(jk, states);
                }
            }
};
}}

#endif // _FDX_JOKEMANAGER_H_
