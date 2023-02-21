#include <iostream>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "registration.hpp"
#include "login.hpp"

using namespace ftxui;

int main() {
  auto registration_button = Button("Register", [] {
    // Call the registration function here
    registration();
  });

  auto login_button = Button("Login", [] {
    // Call the login function here
    login();
  });

  auto quit_button = Button("Quit", [] {
    // Exit the program
    exit(0);
  });

  auto container = Container::Vertical({
    // Add UI components here
    registration_button,
    login_button,
    quit_button,
  });

  auto renderer = Renderer(container, [&] {
    return container;
  });

  // Start the main loop
  for (;;) {
    // Render the UI
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(renderer->Render()));
    Render(screen, renderer->Render());

    // Handle user input
    auto input = screen->GetEvent();
    if (input == Event::Quit || input == Event::Escape) {
      break;
    }
    renderer->OnEvent(input);
  }

  return 0;
}
