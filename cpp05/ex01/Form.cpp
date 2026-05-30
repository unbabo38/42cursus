#include "Form.hpp"
#include "Bureaucrat.hpp"

// Form::Form(){};

Form::Form(const std::string &name, int gradeToSign, int gradeToExecute)
    : _name(name), _sign(false), _gradeToSign(gradeToSign),
      _gradeToExecute(gradeToExecute) {
  this->_checkGrades();
}
Form::Form(const Form &other)
    : _name(other._name), _sign(other._sign), _gradeToSign(other._gradeToSign),
      _gradeToExecute(other._gradeToExecute) {}

Form &Form::operator=(const Form &other) {
  if (this != &other) {
    this->_sign = other._sign;
  }
  return *this;
}
Form::~Form() {}
void Form::_checkGrades() const {
  if (this->_gradeToSign < 1 || this->_gradeToExecute < 1) {
    throw Form::GradeTooHighException();
  }
  if (this->_gradeToSign > 150 || this->_gradeToExecute > 150) {
    throw Form::GradeTooLowException();
  }
}

const std::string Form::getName() const { return this->_name; }
bool Form::getSign() const { return this->_sign; }
int Form::getGradeToSign() const { return this->_gradeToSign; }
int Form::getGradeToExecute() const { return this->_gradeToExecute; }
bool Form::beSigned(Bureaucrat &obj) {
  if (obj.getGrade() <= getGradeToSign()) {
    this->_sign = true;
    return true;
  } else {
    this->_sign = false;
    return false;
  }
}
