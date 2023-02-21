#include "login.hpp"
#include <iostream>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

using namespace ftxui;

class LoginForm : public ComponentBase {
 public:
  // Constructor.
  LoginForm() {
    Add(&email_input_);
    Add(&password_input_);
    Add(&submit_button_);
  }

  // Component overrides.
  Element Render() override {
    return vbox({
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
    std::string email = email_input_->str();
    std::string password = password_input_->password();

    // TODO: Validate the login data and authenticate the user

    // Reset the form after submission
    email_input_->set_str("");
    password_input_->set_password("");
  }

 private:
  Input email_input_ = Input(&email_);
  PasswordInput password_input_ = PasswordInput(&password_);
  Button submit_button_ = Button("Submit", [&] { Submit(); });

  std::wstring email_;
  std::wstring password_;
};

void login() {
  auto screen = ScreenInteractive::TerminalOutput();
  LoginForm form;
  screen.Loop(&form);
}

ftxui::Component login_form() {
  auto form = std::make_shared<LoginForm>();
  return CapturedMouse(std::make_shared<ScreenInteractive>(
      [form] { return form; }));
}
