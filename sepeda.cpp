#include <SFML/Graphics.hpp>
using namespace sf;

int main() {
    RenderWindow window(VideoMode(800, 600), "Moving Cycle");

    // Membuat siklus (cycle)
    CircleShape cycle(50.f, 3); // Menggunakan segitiga sebagai contoh siklus
    cycle.setFillColor(Color::Green);
    cycle.setPosition(400.f, 300.f);
    float speed = 5.f; // Kecepatan gerakan siklus

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Menggerakkan siklus menggunakan tombol keyboard
        if (Keyboard::isKeyPressed(Keyboard::Left))
            cycle.move(-speed, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Right))
            cycle.move(speed, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Up))
            cycle.move(0.f, -speed);
        if (Keyboard::isKeyPressed(Keyboard::Down))
            cycle.move(0.f, speed);

        window.clear();
        window.draw(cycle);
        window.display();
    }

    return 0;
}
