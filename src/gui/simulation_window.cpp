#include "gui/simulation_window.hpp"
#include "gui/renderer.hpp"

#include <city/city.hpp>
#include <city/street.hpp>
#include <traffic/semaphore_controller.hpp>
#include "common/simulation_controller.hpp"

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
    
    // Fondo de la ciudad (simulando edificios/pasto oscuro)
    sf::RectangleShape background(sf::Vector2f(total_size, total_size));
    background.setFillColor(sf::Color(35, 40, 35)); 
    window_.draw(background);
}

void SimulationWindow::drawStreets() {
    int grid_size = city_->getGridSize();
    float road_width = cell_size_ * 0.5f; // Las calles ocupan la mitad de la celda
    
    // Dibujamos el asfalto forzando la cuadrícula (ya que el generador no instanció StreetSegments)
    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            float start_x = x * cell_size_ + cell_size_ / 2.0f;
            float start_y = y * cell_size_ + cell_size_ / 2.0f;
            
            // Calle hacia la derecha (Este)
            if (x < grid_size - 1) {
                float end_x = (x + 1) * cell_size_ + cell_size_ / 2.0f;
                Renderer::drawStreet(window_, start_x, start_y, end_x, start_y, road_width, sf::Color(60, 60, 65));
            }
            // Calle hacia abajo (Sur)
            if (y < grid_size - 1) {
                float end_y = (y + 1) * cell_size_ + cell_size_ / 2.0f;
                Renderer::drawStreet(window_, start_x, start_y, start_x, end_y, road_width, sf::Color(60, 60, 65));
            }
        }
    }
}

void SimulationWindow::drawIntersections() {
    int grid_size = city_->getGridSize();
    float road_width = cell_size_ * 0.5f; 
    float offset = (cell_size_ - road_width) / 2.0f;
    
    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            float pos_x = x * cell_size_;
            float pos_y = y * cell_size_;
            
            // 1. Asfalto cuadrado en el centro de la intersección
            sf::RectangleShape intersection(sf::Vector2f(road_width, road_width));
            intersection.setPosition(pos_x + offset, pos_y + offset);
            intersection.setFillColor(sf::Color(60, 60, 65));
            window_.draw(intersection);
            
            // 2. Semáforos pequeños en la esquina de la acera
            sf::Color light_color = getTrafficLightColor(x, y);
            if (light_color != intersection_color_) {
                sf::CircleShape light(cell_size_ * 0.10f);
                light.setPosition(pos_x + offset - (cell_size_ * 0.05f), pos_y + offset - (cell_size_ * 0.05f));
                light.setFillColor(light_color);
                
                // Efecto de luz LED (Brillo sutil)
                sf::CircleShape glow(cell_size_ * 0.18f);
                glow.setPosition(pos_x + offset - (cell_size_ * 0.13f), pos_y + offset - (cell_size_ * 0.13f));
                glow.setFillColor(sf::Color(light_color.r, light_color.g, light_color.b, 70));
                
                window_.draw(glow);
                window_.draw(light);
            }
        }
    }
}

void SimulationWindow::drawVehicles() {
    if (!vehicles_) return;
    
    // PASO A: Dibujar primero la RUTA PLANEADA para que quede debajo de los autos
    for (const auto& vehicle : *vehicles_) {
        if (vehicle.remaining_path.size() > 1) {
            // LineStrip conecta una lista de puntos
            sf::VertexArray routeLines(sf::LineStrip, vehicle.remaining_path.size() + 1);
            
            float start_x = vehicle.x * cell_size_ + cell_size_ / 2.0f;
            float start_y = vehicle.y * cell_size_ + cell_size_ / 2.0f;
            routeLines[0].position = sf::Vector2f(start_x, start_y);
            
            // Usamos el mismo color del auto, pero muy transparente
            sf::Color routeColor = Renderer::getVehicleColor(vehicle.id);
            routeColor.a = 90; 
            routeLines[0].color = routeColor;
            
            for (size_t i = 0; i < vehicle.remaining_path.size(); ++i) {
                float px = vehicle.remaining_path[i].x * cell_size_ + cell_size_ / 2.0f;
                float py = vehicle.remaining_path[i].y * cell_size_ + cell_size_ / 2.0f;
                routeLines[i+1].position = sf::Vector2f(px, py);
                routeLines[i+1].color = routeColor;
            }
            window_.draw(routeLines);
        }
    }
    
    // PASO B: Dibujar los autos
    for (const auto& vehicle : *vehicles_) {
        float pos_x = vehicle.x * cell_size_ + cell_size_ / 2.0f;
        float pos_y = vehicle.y * cell_size_ + cell_size_ / 2.0f;
        Renderer::drawVehicle(window_, pos_x, pos_y, cell_size_ / 3.0f, vehicle.id, vehicle.is_waiting, vehicle.angle);
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

void SimulationWindow::run(traffic_simulation::SimulationController& controller) {
    std::vector<gui::VehicleRenderInfo> render_info;
    
    while (isOpen()) {
        handleEvents();
        
        if (!is_paused_) {
            render_info = controller.getVehicleManager().getRenderInfo();
            setVehicles(&render_info);
        }
        
        render(); 
        sf::sleep(sf::milliseconds(16));
    }
}

} // namespace gui
