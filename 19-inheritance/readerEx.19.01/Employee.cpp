//
// Employee.cpp
//
// This file implements the Employee class hierarchy.  Notice the use
// of an initializer list to invoke the superclass constructor when name
// is passed into the subclass constructor.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
// --------------------------------------------------------------------------
//
// Extemded by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved.
//

#include <string>
#include "Employee.h"

// Class: Employee

Employee::Employee() {
    // Empty
}

Employee::Employee(std::string name) {
    this->name = name;
}

std::string Employee::getName() const {
    return this->name;
}

// Class: HourlyEmployee

HourlyEmployee::HourlyEmployee() {
    hourlyRate = 0;
    hoursWorked = 0;
}

HourlyEmployee::HourlyEmployee(std::string name,
                               double hourlyRate,
                               double hoursWorked): Employee(name) {
    this->hourlyRate = hourlyRate;
    this->hoursWorked = hoursWorked;
}

double HourlyEmployee::getPay() const {
    return hourlyRate * hoursWorked;
}

void HourlyEmployee::setHourlyRate(double rate) {
    this->hourlyRate = rate;
}

void HourlyEmployee::setHoursWorked(double hours) {
    this->hoursWorked = hours;
}

// Class: CommissionedEmployee

CommissionedEmployee::CommissionedEmployee() {
    baseSalary = 0;
    commissionRate = 0;
    salesVolume = 0;
}

CommissionedEmployee::CommissionedEmployee(std::string name,
                                           double baseSalary,
                                           double commissionRate,
                                           double salesVolume): Employee(name) {
    this->baseSalary = baseSalary;
    this->commissionRate = commissionRate;
    this->salesVolume = salesVolume;
}

double CommissionedEmployee::getPay() const {
    return baseSalary + (commissionRate * salesVolume);
}


void CommissionedEmployee::setBaseSalary(double dollars) {
    this->baseSalary = dollars;
}

void CommissionedEmployee::setCommissionRate(double rate) {
    this->commissionRate = rate;
}

void CommissionedEmployee::setSalesVolume(double dollars) {
    this->salesVolume = dollars;
}

// Class: SalariedEmployee

SalariedEmployee::SalariedEmployee() {
    salary = 0;
}

SalariedEmployee::SalariedEmployee(std::string name,
                                   double salary): Employee(name) {
    this->salary = salary;
}

double SalariedEmployee::getPay() const {
    return salary;
}

void SalariedEmployee::setSalary(double salary) {
    this->salary = salary;
}
