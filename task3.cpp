// SnakeGame.cpp - CodTech Internship Task 3 (SFML Snake Game)
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace sf;

const int WIDTH = 800;
const int HEIGHT = 600;
const int SIZE = 20;

int score = 0;
int speed = 150;
Vector2i foodPos;

struct SnakeSegment {
    Vector2i position;
};

class Snake {
public:
    Snake() {
        body.push_back({ Vector2i(10, 10) });
        dir = Vector2i(1, 0);
    }

    void move() {
        for (int i = body.size() - 1; i > 0; --i)
            body[i].position = body[i - 1].position;
        body[0].position += dir;
    }

    void grow() {
        body.push_back({ body.back().position });
    }

    bool checkSelfCollision() {
        for (size_t i = 1; i < body.size(); ++i)
            if (body[i].position == body[0].position)
                return true;
        return false;
    }

    void setDirection(Vector2i newDir) {
        if (dir + newDir != Vector2i(0, 0))
            dir = newDir;
    }

    Vector2i getHeadPosition() {
        return body[0].position;
    }

    void draw(RenderWindow& window) {
        RectangleShape segment(Vector2f(SIZE - 1, SIZE - 1));
        segment.setFillColor(Color::Green);
        for (auto& s : body) {
            segment.setPosition(s.position.x * SIZE, s.position.y * SIZE);
            window.draw(segment);
        }
    }

private:
    std::vector<SnakeSegment> body;
    Vector2i dir;
};

void spawnFood() {
    foodPos.x = rand() % (WIDTH / SIZE);
    foodPos.y = rand() % (HEIGHT / SIZE);
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Snake Game - CodTech");
    window.setFramerateLimit(60);

    // Load font
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Font not found
    }

    // Load sound
    SoundBuffer eatBuffer;
    if (!eatBuffer.loadFromFile("eat.wav")) {
        return -1; // Sound file not found
    }
    Sound eatSound;
    eatSound.setBuffer(eatBuffer);

    Snake snake;
    spawnFood();

    Clock clock;
    Time timer = Time::Zero;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::W) snake.setDirection({ 0, -1 });
                if (e.key.code == Keyboard::S) snake.setDirection({ 0, 1 });
                if (e.key.code == Keyboard::A) snake.setDirection({ -1, 0 });
                if (e.key.code == Keyboard::D) snake.setDirection({ 1, 0 });
            }
        }

        timer += clock.restart();
        if (timer.asMilliseconds() > speed) {
            snake.move();

            if (snake.getHeadPosition() == foodPos) {
                eatSound.play();
                snake.grow();
                spawnFood();
                score += 10;
                if (speed > 50) speed -= 5;
            }

            Vector2i head = snake.getHeadPosition();
            if (head.x < 0 || head.y < 0 || head.x >= WIDTH / SIZE || head.y >= HEIGHT / SIZE || snake.checkSelfCollision()) {
                window.close(); // Game Over
            }

            timer = Time::Zero;
        }

        // Draw everything
        window.clear(Color::Black);

        // Draw food
        RectangleShape food(Vector2f(SIZE - 1, SIZE - 1));
        food.setFillColor(Color::Red);
        food.setPosition(foodPos.x * SIZE, foodPos.y * SIZE);
        window.draw(food);

        // Draw snake
        snake.draw(window);

        // Draw score
        Text scoreText("Score: " + std::to_string(score), font, 20);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
