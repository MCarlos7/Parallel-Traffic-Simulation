#pragma once

#include <SFML/Graphics.hpp>
#include "simulation_window.hpp"

namespace traffic_simulation {
    struct SimulationConfig;
}

namespace gui {

/**
 * @brief Control panel for user input and simulation configuration
 */
class ControlPanel {
public:
    /**
     * @brief Construct a new ControlPanel
     */
    ControlPanel();

    /**
     * @brief Render the control panel overlay
     * @param window Reference to the SFML render window
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Handle mouse click events
     * @param x Mouse X coordinate
     * @param y Mouse Y coordinate
     * @return true if a control was clicked
     */
    bool handleClick(int x, int y);

    /**
     * @brief Get the current simulation configuration from panel values
     * @return SimulationConfig with current slider values
     */
    traffic_simulation::SimulationConfig getConfig() const;

    /**
     * @brief Check if pause button is currently pressed
     * @return true if paused via button
     */
    bool isPaused() const;

    /**
     * @brief Check if reset was requested
     * @return true if reset button was clicked
     */
    bool isResetRequested() const;

    /**
     * @brief Clear the reset request flag
     */
    void clearResetRequest();

    /**
     * @brief Get the current speed multiplier
     * @return Speed multiplier value
     */
    float getSpeedMultiplier() const;

private:
    // Sliders
    sf::RectangleShape vehicle_count_track_;
    sf::RectangleShape vehicle_count_slider_;
    int vehicle_count_value_;

    sf::RectangleShape green_duration_track_;
    sf::RectangleShape green_duration_slider_;
    int green_duration_value_;

    sf::RectangleShape yellow_duration_track_;
    sf::RectangleShape yellow_duration_slider_;
    int yellow_duration_value_;

    sf::RectangleShape red_duration_track_;
    sf::RectangleShape red_duration_slider_;
    int red_duration_value_;

    sf::RectangleShape speed_track_;
    sf::RectangleShape speed_slider_;
    float speed_multiplier_value_;

    // Buttons
    sf::RectangleShape pause_button_;
    sf::RectangleShape reset_button_;
    bool is_paused_;
    bool reset_requested_;

    // Labels (stored as text objects for rendering)
    sf::Text title_text_;
    sf::Text vehicle_label_;
    sf::Text green_label_;
    sf::Text yellow_label_;
    sf::Text red_label_;
    sf::Text speed_label_;
    sf::Text pause_text_;
    sf::Text reset_text_;
    sf::Text speed_value_text_;

    // Fonts
    sf::Font font_;

    // Panel dimensions
    int panel_x_;
    int panel_y_;
    int panel_width_;
    int panel_height_;

    /**
     * @brief Initialize fonts and text objects
     */
    void initializeText();

    /**
     * @brief Draw a slider with label
     */
    void drawSlider(sf::RenderWindow& window, const sf::RectangleShape& track,
                    const sf::RectangleShape& slider, const sf::Text& label,
                    const std::string& value_str, int x, int y, int width);

    /**
     * @brief Draw a button
     */
    void drawButton(sf::RenderWindow& window, const sf::RectangleShape& button,
                    const sf::Text& text, int x, int y, int width, int height);

    /**
     * @brief Update a slider position based on mouse
     */
    bool updateSlider(int mouse_x, int mouse_y, const sf::RectangleShape& track,
                      int& value, int min_val, int max_val);
};

} // namespace gui
