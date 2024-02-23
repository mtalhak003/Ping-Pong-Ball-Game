#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Paddle {
public:
    RectangleShape shape;
    Vector2f velocity;

    Paddle(float x, float y, Color paddleColor) {
        shape.setSize(Vector2f(20, 100));
        shape.setPosition(x, y);
        shape.setFillColor(paddleColor);
        velocity = Vector2f(0, 0);
    }

    void move(float windowHeight) {
        // Limit paddle movement within the window
        if (shape.getPosition().y + velocity.y >= 0 && shape.getPosition().y + shape.getSize().y + velocity.y <= windowHeight) {
            shape.move(velocity);
        }
    }
};

class Ball {
public:
    CircleShape shape;
    Vector2f velocity;

    Ball(float x, float y) {
        shape.setRadius(10);
        shape.setPosition(x, y);
        shape.setFillColor(Color::Yellow);
        velocity = Vector2f(-0.090, -0.090);
    }

    void move() {
        shape.move(velocity);
    }
};

bool playAgain() {
    string input;
    cout << "Do you want to play again? (yes/no): ";
    cin >> input;
    return (input == "yes");
}

int main() {
    while (true) {
        RenderWindow window(VideoMode(800, 600), "Pong Clone");

        Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("images/bricks.jpeg")) {
            cerr << "Failed to load background image" << endl;
            return -1;
        }

        Sprite backgroundSprite(backgroundTexture);
        backgroundSprite.setScale(Vector2f(800.0f / backgroundSprite.getTexture()->getSize().x, 600.0f / backgroundSprite.getTexture()->getSize().y));

        Paddle leftPaddle(50, 250, Color::Red);
        Paddle rightPaddle(730, 250, Color::Blue);
        Ball ball(400, 300);

        bool gameIsOver = false;

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            leftPaddle.velocity.y = Keyboard::isKeyPressed(Keyboard::W) ? -0.2 : Keyboard::isKeyPressed(Keyboard::S) ? 0.2 : 0;
            rightPaddle.velocity.y = Keyboard::isKeyPressed(Keyboard::Up) ? -0.2 : Keyboard::isKeyPressed(Keyboard::Down) ? 0.2 : 0;

            if (!gameIsOver) {
                ball.move();

                if (ball.shape.getGlobalBounds().intersects(leftPaddle.shape.getGlobalBounds()) || ball.shape.getGlobalBounds().intersects(rightPaddle.shape.getGlobalBounds())) {
                    ball.velocity.x = -ball.velocity.x;
                }

                if (ball.shape.getPosition().y <= 0 || ball.shape.getPosition().y >= window.getSize().y - ball.shape.getRadius() * 2) {
                    ball.velocity.y = -ball.velocity.y;
                }

                if (ball.shape.getPosition().x <= 0 || ball.shape.getPosition().x >= window.getSize().x - ball.shape.getRadius() * 2) {
                    cout << "End Game" << endl;
                    gameIsOver = true;
                }
            }

            leftPaddle.move(window.getSize().y);
            rightPaddle.move(window.getSize().y);

            window.clear();
            window.draw(backgroundSprite);
            window.draw(leftPaddle.shape);
            window.draw(rightPaddle.shape);
            window.draw(ball.shape);
            window.display();

            if (gameIsOver && Keyboard::isKeyPressed(Keyboard::Enter)) {
                break;
            }
        }

        if (!playAgain()) {
            break;
        }
    }

    return 0;
}
