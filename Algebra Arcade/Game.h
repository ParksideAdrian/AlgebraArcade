#include "Graph2.h"

using namespace sf;

class Game
{

public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

private:
    RenderWindow mWindow;
    RectangleShape Bkgd, ftnBkgd, containerRect, turnRect, scoreRect, algebroidRect,
                   leftBoundRect_x, rightBoundRect_x, upBoundRect_y, downBoundRect_y,
                   titleRect, outlineRect;
    Axis ax, ay;
    Font font;
    Text text, ftext, rightBound_x, leftBound_x, upBound_y, downBound_y, algebraArcade, turns, score;
    Texture algebroids, graphGobbler;
    Sprite algs[11], rAlgebroid01, rGraphGobbler;
    std::vector <Function*> fns;
    std::string str;
    Clock clock;
    Time time;
    float t{0};

public:
    int randPosition = rand() % 440 + 60;
};

Game::Game():
    mWindow(VideoMode(wWidth, wHeight), "ALGEBRA ARCADE"),
    ftnBkgd(Vector2f(660,28)), Bkgd(Vector2f(wWidth, wHeight)),
    outlineRect(Vector2f(600, 440)), rightBoundRect_x(Vector2f(20, 120)), leftBoundRect_x(Vector2f(20, 120)),
    upBoundRect_y(Vector2f(120, 20)), downBoundRect_y(Vector2f(120, 20)),
    titleRect(Vector2f(190, 20)), containerRect(Vector2f(190, 20)),
    scoreRect(Vector2f(230, 30)), algebroidRect(Vector2f(230, 30)),
    turnRect(Vector2f(40, 30)), ax(0, Vector2f(margin, wHeight/2), wWidth-2*margin, 10),
    ay(1, Vector2f(wWidth/2, wHeight-margin), wHeight-2*margin, 10)
{

    if(!algebroids.loadFromFile("algebroid01.png"))
        std::cout << "error" << std::endl;
    rAlgebroid01.setTexture(algebroids);
    rAlgebroid01.setPosition(32,532);

    if(!graphGobbler.loadFromFile("graphGobbler.png"))
        std::cout << "error" << std::endl;
    rGraphGobbler.setTexture(graphGobbler);
    rGraphGobbler.setPosition(randPosition, randPosition);
    rGraphGobbler.setScale(2.5,2.5);

    for(int i = 0; i < 10; i++ )
    {
        algs[i].setTexture(algebroids);
        algs[i].setOrigin(12, 13);
        algs[i].setPosition(0, randPosition);
        randPosition = rand() % 440 + 60;

    }

    ///
    Bkgd.setFillColor(Color(0, 0, 255));


    outlineRect.setFillColor(Color(0, 0, 0, 255));
    outlineRect.setOutlineThickness(2);
    outlineRect.setOutlineColor(Color::White);
    outlineRect.setPosition(60,60);

    ftnBkgd.setFillColor(Color(86,255,20));
    ftnBkgd.setPosition(Vector2f(30,530));


   /// outlineRect.setOrigin(2,3);


    algebroidRect.setFillColor(Color(255,255,255));
    algebroidRect.setPosition(Vector2f(480,5));
    algebroidRect.setOutlineThickness(2);
    algebroidRect.setOutlineColor(sf::Color::Black);

    scoreRect.setFillColor(Color(255,255,255));
    scoreRect.setPosition(Vector2f(480,5));
    scoreRect.setOutlineThickness(2);
    scoreRect.setOutlineColor(sf::Color::Black);

    containerRect.setFillColor(Color(255,255,255));
    containerRect.setPosition(Vector2f(295,5));
    containerRect.setOutlineThickness(2);
    containerRect.setOutlineColor(sf::Color::Black);

    titleRect.setFillColor(Color(255,255,255));
    titleRect.setPosition(Vector2f(485,5));
    titleRect.setOutlineThickness(2);
    titleRect.setOutlineColor(sf::Color::Black);

    downBoundRect_y.setFillColor(Color(0,0,0));
    downBoundRect_y.setPosition(Vector2f(305,503));
    downBoundRect_y.setOutlineThickness(2);
    downBoundRect_y.setOutlineColor(Color::White);

    upBoundRect_y.setFillColor(Color(0,0,0));
    upBoundRect_y.setPosition(Vector2f(305,38));
    upBoundRect_y.setOutlineThickness(2);
    upBoundRect_y.setOutlineColor(Color::White);

    leftBoundRect_x.setFillColor(Color(0,0,0));
    leftBoundRect_x.setPosition(Vector2f(38,225));
    leftBoundRect_x.setOutlineThickness(2);
    leftBoundRect_x.setOutlineColor(Color::White);

    rightBoundRect_x.setFillColor(Color(0,0,0));
    rightBoundRect_x.setPosition(Vector2f(662,225));
    rightBoundRect_x.setOutlineThickness(2);
    rightBoundRect_x.setOutlineColor(Color::White);



    turnRect.setFillColor(Color::White);
    turnRect.setPosition(360, 360);



    if(!font.loadFromFile("arial.ttf"))
        std::cout << "font not loaded" << std::endl;
    text = Text("Y = ", font, 24);
    ftext = Text("", font, 24);
    rightBound_x = Text("5", font, 16);
    leftBound_x = Text("-5", font, 16);
    upBound_y = Text("5", font, 16);
    downBound_y = Text("-5", font, 16);
    algebraArcade = Text("ALGEBRA ARCADE", font, 18);
    turns = Text("00", font, 20);
    score = Text("000000", font, 20);

    /// set the color
    text.setColor(Color(100,100,100)); ///57,165,0));
    ftext.setColor(Color(100,100,100));
    algebraArcade.setColor(Color(0,0,0));

    /// set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    /// set the text position
    text.setPosition(64,530);
    ftext.setPosition(112,528);
    rightBound_x.setPosition(685, 270);
    rightBound_x.rotate(90);
    leftBound_x.setPosition(35, 295);
    leftBound_x.rotate(270);
    upBound_y.setPosition(360, 38);
    downBound_y.setPosition(356, 500);
    algebraArcade.setPosition(500, 5);

    clock.restart();
    time = clock.getElapsedTime();
}

void Game::run()
{
    /// let's you hold keys
    mWindow.setKeyRepeatEnabled(true);

    while (mWindow.isOpen())
    {
        render(); /// Draws a graph
        processEvents(); /// get its input
        update(); // updates the graph


    }
}

void Game::processEvents()
{
    Event event;

    while(mWindow.pollEvent(event))
    {
        if(event.type == Event::Closed) mWindow.close();
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode < 128)
                std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {

                std::cout << "the escape key was pressed" << std::endl;
                std::cout << "control:" << event.key.control << std::endl;
                std::cout << "alt:" << event.key.alt << std::endl;
                std::cout << "shift:" << event.key.shift << std::endl;
                std::cout << "system:" << event.key.system << std::endl;
            }
        }
        ///if (Event.Type == sf::Event::Closed) App.Close();

        if (event.type == Event::TextEntered)
        {
            if (event.text.unicode == 's')
                str.append("sin(");
            if (event.text.unicode == 'c')
                str.append("cos(");
            if (event.text.unicode == 'i')
                str.append("int(");
            if (event.text.unicode == 't')
                str.append("tan(");
            if (event.text.unicode == 'a')
                str.append("arctan(");
            if (event.text.unicode == 'q')
                str.append("sqr(");
            if (event.text.unicode == 'b')
                str.append("abs(");
            if (event.text.unicode == '\b')
                str.erase(str.size() -1, 1);
            if (event.text.unicode < '~' && event.text.unicode > ' '
                && event.text.unicode != 's' && event.text.unicode != 'c' && event.text.unicode != 'i'
                && event.text.unicode != 't' && event.text.unicode != 'a' && event.text.unicode != 'q'
                && event.text.unicode != 'b')
                str += static_cast<char>(event.text.unicode);
            if (event.text.unicode == 13)
                fns.push_back(new Function(float(margin), float(wWidth-margin), Vector2f(margin,wHeight/2), 400, 50, 50, str+";"));
            ///std::cout << str.substr(3,str.length());
        }
        ftext.setString(str);
    }
}

void Game::update()
{
    for(int i = 0; i < 10; ++i)
    {
        if(algs[i].getPosition().x < margin*i+60)
            algs[i].move(.5, 0);
    }

}

void Game::render()
{
    /// draws these to the window sequentially
    mWindow.clear();

    mWindow.draw(Bkgd);
    mWindow.draw(ftnBkgd);
    mWindow.draw(outlineRect);
    if(fns.size()>0) mWindow.draw(*(fns[0]));
    mWindow.draw(text);

    mWindow.draw(rGraphGobbler);
    mWindow.draw(ftext);
    mWindow.draw(containerRect);
    mWindow.draw(turnRect);
    mWindow.draw(scoreRect);
    mWindow.draw(algebroidRect);
    mWindow.draw(leftBoundRect_x);
    mWindow.draw(rightBoundRect_x);
    mWindow.draw(upBoundRect_y);
    mWindow.draw(downBoundRect_y);
    mWindow.draw(titleRect);
    mWindow.draw(leftBound_x);
    mWindow.draw(rightBound_x);
    mWindow.draw(upBound_y);
    mWindow.draw(downBound_y);
    mWindow.draw(algebraArcade);
    mWindow.draw(score);
    mWindow.draw(turns);
    mWindow.draw(ax);
    mWindow.draw(ay);
    for(int i = 0; i < 10; i++) mWindow.draw(algs[i]);

    /// makes the algebroid blink next to function
    t += 5000*time.asSeconds();
    if(sin(t)>0)
    mWindow.draw(rAlgebroid01);
    mWindow.display();
}
