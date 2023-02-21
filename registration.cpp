#include "registration.hpp"
#include <iostream>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

using namespace ftxui;

class RegistrationForm : public ComponentBase {
 public:
  // Constructor.
  RegistrationForm() {
    Add(&name_input_);
    Add(&email_input_);
    Add(&password_input_);
    Add(&submit_button_);
  }

  // Component overrides.
  Element Render() override {
    return vbox({
        hbox({
            text("Name: "),
            name_input_->Render(),
        }),
        hbox({
            text("Email: "),
            email_input_->Render(),
        }),
        hbox({
            text("Password: "),
            password_input_->Render(),
        }),
        hbox({
            filler(),
            submit_button_->Render(),
            filler(),
        }),
    });
  }

  bool OnEvent(Event event) override {
    if (event == Event::Return) {
      Submit();
      return true;
    }
    return ComponentBase::OnEvent(event);
  }

  void Submit() {
    std::string name = name_input_->str();
    std::string email = email_input_->str();
    std::string password = password_input_->password();

    // TODO: Validate and process the registration data

    // Reset the form after submission
    name_input_->set_str("");
    email_input_->set_str("");
    password_input_->set_password("");
  }

 private:
  Input name_input_ = Input(&name_);
  Input email_input_ = Input(&email_);
  PasswordInput password_input_ = PasswordInput(&password_);
  Button submit_button_ = Button("Submit", [&] { Submit(); });

  std::wstring name_;
  std::wstring email_;
  std::wstring password_;
};

void registration() {
  auto screen = ScreenInteractive::TerminalOutput();
  RegistrationForm form;
  screen.Loop(&form);
}

ftxui::Component registration_form() {
  auto form = std::make_shared<RegistrationForm>();
  return CapturedMouse(std::make_shared<ScreenInteractive>(
      [form] { return form; }));
}
