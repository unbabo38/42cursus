#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include <iostream>

void test_intern_creates_presidential_pardon() {
    Intern some_intern;
    AForm *form = NULL;

    form = some_intern.makeForm("presidential pardon", "TargetA");
    if (form != NULL) {
        std::cout << "SUCCESS: Form created -> " << form->getName() << std::endl;
        delete form;
    } else {
        std::cout << "FAILURE: Form was NULL" << std::endl;
    }
}

void test_intern_creates_robotomy_request() {
    Intern some_intern;
    AForm *form = NULL;

    form = some_intern.makeForm("robotomy request", "Bender");
    if (form != NULL) {
        std::cout << "SUCCESS: Form created -> " << form->getName() << std::endl;
        delete form;
    } else {
        std::cout << "FAILURE: Form was NULL" << std::endl;
    }
}

void test_intern_creates_shrubbery_creation() {
    Intern some_intern;
    AForm *form = NULL;

    form = some_intern.makeForm("shrubbery creation", "Garden");
    if (form != NULL) {
        std::cout << "SUCCESS: Form created -> " << form->getName() << std::endl;
        delete form;
    } else {
        std::cout << "FAILURE: Form was NULL" << std::endl;
    }
}

void test_intern_creates_unknown_form_failure() {
    Intern some_intern;
    AForm *form = NULL;

    form = some_intern.makeForm("invalid request name", "Nobody");
    if (form == NULL) {
        std::cout << "SUCCESS: Correctly returned NULL for invalid form name." << std::endl;
    } else {
        std::cout << "FAILURE: Created an unknown form unexpectedly." << std::endl;
        delete form;
    }
}

int main() {
    test_intern_creates_presidential_pardon();
    test_intern_creates_robotomy_request();
    test_intern_creates_shrubbery_creation();
    test_intern_creates_unknown_form_failure();

    return 0;
}
