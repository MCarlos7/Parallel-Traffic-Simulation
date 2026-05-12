#include "gui/simulation_window.hpp"
#include "gui/renderer.hpp"

#include <city/city.hpp>
#include <city/street.hpp>
#include <traffic/semaphore_controller.hpp>

namespace gui {

SimulationWindow::SimulationWindow()
    : city_(nullptr),
      vehicles_(nullptr),
      semaphores_(nullptr),
      cell_size_(40),
      speed_multiplier_(1.0f),
      is_paused_(false),
      window_closed_(false) {
    
    // Dark theme colors
    background_color_ = sf::Color(20, 20, 25);
    grid_color_ = sf::Color(40, 40, 45);
    street_color_ = sf::Color(80, 80, 85);
    intersection_color_ = sf::Color(60, 60, 65);
    
    // Neon-like traffic light colors
    green_light_ = sf::Color(0, 255, 100);
    yellow_light_ = sf::Color(255, 220, 0);
    red_light_ = sf::Color(255, 50, 50);
}

SimulationWindow::~SimulationWindow() {
    close();
}

bool SimulationWindow::initialize(int width, int height, const std::string& title) {
    window_.create(sf::VideoMode(width, height), title);
    window_.setFramerateLimit(60);
    return window_.isOpen();
}

void SimulationWindow::setCity(city::City* city) {
    city_ = city;
}

void SimulationWindow::setVehicles(std::vector<VehicleRenderInfo>* vehicles) {
    vehicles_ = vehicles;
}

void SimulationWindow::setSemaphoreController(traffic::SemaphoreController* semaphores) {
    semaphores_ = semaphores;
}

void SimulationWindow::setCellSize(int cell_size) {
    cell_size_ = cell_size;
}

void SimulationWindow::setSpeedMultiplier(float multiplier) {
    speed_multiplier_ = multiplier;
}

bool SimulationWindow::isOpen() const {
    return window_.isOpen() && !window_closed_;
}

bool SimulationWindow::isPaused() const {
    return is_paused_;
}

void SimulationWindow::togglePause() {
    is_paused_ = !is_paused_;
}

void SimulationWindow::setPaused(bool paused) {
    is_paused_ = paused;
}

void SimulationWindow::handleEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_closed_ = true;
            window_.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                togglePause();
            } else if (event.key.code == sf::Keyboard::Num1) {
                setSpeedMultiplier(1.0f);
            } else if (event.key.code == sf::Keyboard::Num2) {
                setSpeedMultiplier(2.0f);
            } else if (event.key.code == sf::Keyboard::Num3) {
                setSpeedMultiplier(3.0f);
            } else if (event.key.code == sf::Keyboard::Escape) {
                window_closed_ = true;
                window_.close();
            }
        } else if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window_.setView(sf::View(visibleArea));
        }
    }
}

void SimulationWindow::render() {
    window_.clear(background_color_);
    
    if (!city_) return;
    
    int grid_size = city_->getGridSize();
    
    // Draw grid background
    drawGrid();
    
    // Draw streets
    drawStreets();
    
    // Draw intersections with traffic lights
    drawIntersections();
    
    // Draw vehicles
    drawVehicles();
    
    window_.display();
}

void SimulationWindow::drawGrid() {
    int grid_size = city_->getGridSize();
    int total_size = grid_size * cell_size_;
    
    // Draw background
    sf::RectangleShape background(sf::Vector2f(total_size, total_size));
    background.setFillColor(sf::Color(25, 25, 30));
    window_.draw(background);
    
    // Draw grid lines
    sf::Color line_color(45, 45, 50);
    for (int i = 0; i <= grid_size; ++i) {
        // Vertical lines
        sf::RectangleShape v_line(sf::Vector2f(1, total_size));
        v_line.setPosition(i * cell_size_, 0);
        v_line.setFillColor(line_color);
        window_.draw(v_line);
        
        // Horizontal lines
        sf::RectangleShape h_line(sf::Vector2f(total_size, 1));
        h_line.setPosition(0, i * cell_size_);
        h_line.setFillColor(line_color);
        window_.draw(h_line);
    }
}

void SimulationWindow::drawStreets() {
    const auto& streets = city_->getStreets();
    for (const auto& street : streets) {
        auto start = street->getStartIntersection();
        auto end = street->getEndIntersection();
        
        if (start && end) {
            float start_x = start->getCoordinate().x * cell_size_ + cell_size_ / 2.0f;
            float start_y = start->getCoordinate().y * cell_size_ + cell_size_ / 2.0f;
            float end_x = end->getCoordinate().x * cell_size_ + cell_size_ / 2.0f;
            float end_y = end->getCoordinate().y * cell_size_ + cell_size_ / 2.0f;
            
            Renderer::drawStreet(window_, start_x, start_y, end_x, end_y, 
                              6.0f, sf::Color(90, 90, 95));
        }
    }
}

void SimulationWindow::drawIntersections() {
    int grid_size = city_->getGridSize();
    
    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            float pos_x = x * cell_size_;
            float pos_y = y * cell_size_;
            
            // Draw intersection base
            sf::RectangleShape intersection(sf::Vector2f(cell_size_, cell_size_));
            intersection.setPosition(pos_x, pos_y);
            intersection.setFillColor(intersection_color_);
            
            // Get traffic light color
            sf::Color light_color = getTrafficLightColor(x, y);
            
            // Draw intersection with traffic light indicator
            sf::CircleShape light_indicator(cell_size_ / 4.0f);
            light_indicator.setPosition(pos_x + cell_size_ / 2.0f - cell_size_ / 4.0f,
                                       pos_y + cell_size_ / 2.0f - cell_size_ / 4.0f);
            light_indicator.setFillColor(light_color);
            
            // Add glow effect for traffic lights
            if (light_color == green_light_ || 
                light_color == yellow_light_ || 
                light_color == red_light_) {
                sf::CircleShape glow(cell_size_ / 3.0f);
                glow.setPosition(pos_x + cell_size_ / 2.0f - cell_size_ / 3.0f,
                                pos_y + cell_size_ / 2.0f - cell_size_ / 3.0f);
                glow.setFillColor(sf::Color(light_color.r, light_color.g, light_color.b, 50));
                window_.draw(glow);
            }
            
            window_.draw(intersection);
            window_.draw(light_indicator);
        }
    }
}

void SimulationWindow::drawVehicles() {
    if (!vehicles_) return;
    
    for (const auto& vehicle : *vehicles_) {
        float pos_x = vehicle.x * cell_size_ + cell_size_ / 2.0f;
        float pos_y = vehicle.y * cell_size_ + cell_size_ / 2.0f;
        
        sf::Color color = Renderer::getVehicleColor(vehicle.id);
        
        sf::CircleShape shape(cell_size_ / 4.0f);
        shape.setPosition(pos_x - cell_size_ / 4.0f, pos_y - cell_size_ / 4.0f);
        shape.setFillColor(color);
        
        if (vehicle.is_waiting) {
            shape.setFillColor(sf::Color(color.r, color.g, color.b, 150));
        }
        
        window_.draw(shape);
    }
}

sf::Color SimulationWindow::getTrafficLightColor(int x, int y) {
    if (!semaphores_) {
        return intersection_color_;
    }
    
    city::Coordinate coord{x, y};
    auto* sem = semaphores_->getSemaphoreAt(coord);
    
    if (sem) {
        switch (sem->getState()) {
            case traffic::TrafficLightState::GREEN:
                return green_light_;
            case traffic::TrafficLightState::YELLOW:
                return yellow_light_;
            case traffic::TrafficLightState::RED:
                return red_light_;
        }
    }
    
    return intersection_color_;
}

void SimulationWindow::close() {
    if (window_.isOpen()) {
        window_.close();
    }
}

void SimulationWindow::run() {
    while (isOpen()) {
        handleEvents();
        
        if (!is_paused_) {
            // Add any per-frame update logic here if needed
        }
        
        render();
        
        // Small delay to control frame rate
        sf::sleep(sf::milliseconds(16));
    }
}

} // namespace gui
