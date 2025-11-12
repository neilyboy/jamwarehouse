# Contributing to Jam Wysteria

First off, thank you for considering contributing to Jam Wysteria! It's people like you that make this project such a great tool for the maker community.

## Code of Conduct

This project and everyone participating in it is governed by our Code of Conduct. By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check the existing issues list as you might find out that you don't need to create one. When you are creating a bug report, please include as many details as possible:

* **Use a clear and descriptive title** for the issue to identify the problem.
* **Describe the exact steps which reproduce the problem** in as much detail as possible.
* **Provide specific examples to demonstrate the steps**.
* **Describe the behavior you observed after following the steps** and point out what exactly is the problem with that behavior.
* **Explain which behavior you expected to see instead and why.**
* **Include screenshots or video recordings** if possible.
* **Include your hardware setup details**: ESP32 board version, display model, amplifier model, etc.
* **Include firmware version** and any modifications you've made.

#### Bug Report Template

```markdown
**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

**Expected behavior**
A clear and concise description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Hardware (please complete the following information):**
 - ESP32 Board: [e.g., ESP32-2432S028R]
 - Display: [e.g., ILI9341 2.8" TFT]
 - Amplifier: [e.g., MAX98357A]
 - Speaker: [e.g., 4Ω 5W]

**Firmware Version:**
 - Version: [e.g., 1.0.0]
 - Custom modifications: [if any]

**Additional context**
Add any other context about the problem here.
```

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, please include:

* **Use a clear and descriptive title** for the issue to identify the suggestion.
* **Provide a step-by-step description of the suggested enhancement** in as much detail as possible.
* **Provide specific examples to demonstrate the steps**.
* **Describe the current behavior** and **explain which behavior you expected to see instead** and why.
* **Explain why this enhancement would be useful** to most Jam Wysteria users.

### Pull Requests

* Fill in the required template
* Do not include issue numbers in the PR title
* Follow the C++ style guide used in the project
* Include comments in your code where necessary
* Update documentation (README, comments) as needed
* Test your changes thoroughly on hardware before submitting
* Include screenshots or videos demonstrating the changes if applicable

## Development Setup

### Prerequisites

1. **Hardware:**
   - ESP32-2432S028R development board
   - MAX98357A I2S amplifier
   - Speaker (4-8Ω)
   - MicroSD card (Class 10, formatted as FAT32)

2. **Software:**
   - Arduino IDE 2.0+ or PlatformIO
   - ESP32 board support package
   - Required libraries (see README.md)

### Setting Up Development Environment

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/jam-wysteria.git
   cd jam-wysteria
   ```

2. **Install dependencies:**
   - Follow the installation instructions in README.md
   - Configure TFT_eSPI library using provided `User_Setup.h`

3. **Create a feature branch:**
   ```bash
   git checkout -b feature/your-feature-name
   ```

4. **Make your changes and test thoroughly**

5. **Commit your changes:**
   ```bash
   git commit -am 'Add some feature'
   ```

6. **Push to the branch:**
   ```bash
   git push origin feature/your-feature-name
   ```

7. **Submit a pull request**

## Style Guide

### C++ Code Style

* **Indentation:** 2 spaces (no tabs)
* **Braces:** Opening brace on same line (K&R style)
* **Naming conventions:**
  * Classes: PascalCase (e.g., `DisplayManager`)
  * Functions: camelCase (e.g., `drawButton`)
  * Variables: camelCase (e.g., `currentVolume`)
  * Constants: UPPER_SNAKE_CASE (e.g., `SCREEN_WIDTH`)
  * Private members: camelCase (e.g., `savedSSID`)

* **Comments:**
  * Use `//` for single-line comments
  * Use `/* */` for multi-line comments
  * Document all public functions with descriptive comments
  * Include file headers with description and purpose

#### Example:

```cpp
/**
 * Display Manager for Jam Wysteria
 * 
 * Handles all display operations using TFT_eSPI library
 */

class DisplayManager {
public:
  /**
   * Initialize the display
   * Sets up SPI, configures pins, and initializes backlight
   */
  void init();
  
  /**
   * Draw a button on screen
   * @param x X coordinate
   * @param y Y coordinate
   * @param w Width
   * @param h Height
   * @param label Button label text
   * @param bgColor Background color (RGB565)
   * @param textColor Text color (RGB565)
   * @param pressed Whether button appears pressed
   */
  void drawButton(int x, int y, int w, int h, const String& label, 
                  uint16_t bgColor, uint16_t textColor, bool pressed = false);

private:
  TFT_eSPI tft;
  uint8_t currentBrightness;
};
```

### Web Interface Code Style

* **HTML/CSS:** Follow modern web standards
* **JavaScript:** Use ES6+ features
* **Indentation:** 2 spaces
* **Semicolons:** Always use semicolons in JavaScript

### Documentation

* Update README.md if you change functionality
* Add inline code comments for complex logic
* Document all new features in the wiki (if applicable)
* Include wiring diagrams for hardware changes
* Add usage examples for new features

## Testing

Before submitting a pull request, please ensure:

1. **Hardware testing:**
   - Code compiles without errors
   - Functionality works as expected on actual hardware
   - No regressions in existing features
   - Touch screen responds correctly
   - Audio plays without issues
   - Web interface is accessible and functional

2. **Code quality:**
   - No compiler warnings
   - Code follows style guide
   - All debug statements are removed or properly wrapped in `#ifdef DEBUG_MODE`
   - Memory usage is reasonable (check free heap)

3. **Documentation:**
   - Code is well-commented
   - README is updated if needed
   - Commit messages are clear and descriptive

## Feature Requests

We love to hear ideas for new features! Before submitting a feature request:

1. **Check if it already exists** in the issues
2. **Consider if it fits the project scope** (portable internet radio)
3. **Think about implementation complexity** vs benefit
4. **Provide use cases and examples**

### Good Feature Requests Include:

* Clear description of the feature
* Use cases and benefits
* Potential implementation approach (if you have ideas)
* Mockups or diagrams (if applicable)
* Consideration for hardware limitations

## Community

* **GitHub Discussions:** Ask questions, share projects, get help
* **GitHub Issues:** Bug reports and feature requests
* **Pull Requests:** Code contributions

## Recognition

Contributors will be recognized in:
* README.md contributors section
* Release notes for significant contributions
* Project documentation

## License

By contributing to Jam Wysteria, you agree that your contributions will be licensed under the MIT License.

## Questions?

Don't hesitate to ask questions! Create an issue with the `question` label or start a discussion.

Thank you for contributing! 🎵
