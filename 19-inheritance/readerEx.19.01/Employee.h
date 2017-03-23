//
// Employee.h
//
// This file defines the interface for an Employee class hierarchy
// discussed in the course reader.  It provides a simple illustration
// of among classes.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 19, Exercise 1
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// This file extends the interface provided in Figure 19-2.
// --------------------------------------------------------------------------
//
// Extended by Glenn Streiff on 3/23/17.
// Copyright © 2017 Glenn Streiff. All rights reserved. (derivative work)
//

#ifndef Employee_h
#define Employee_h

class Employee {
public:
    std::string getName() const;
    virtual double getPay() const = 0;  // abstract method -must- be overridden
                                        // in the subclass.
protected:
    Employee();
    Employee(std::string name);
    std::string name;
};

class HourlyEmployee: public Employee {
public:
    HourlyEmployee();
    HourlyEmployee(std::string name,
                   double hourlyRate = 0,
                   double hoursWorked = 0);
    virtual double getPay() const;
    void setHourlyRate(double rate);
    void setHoursWorked(double hours);
    
private:
    double hourlyRate;
    double hoursWorked;
};

class CommissionedEmployee: public Employee {
public:
    CommissionedEmployee();
    CommissionedEmployee(std::string name,
                         double baseSalary = 0,
                         double commissionRate = 0,
                         double salesVolume = 0);
    virtual double getPay() const;
    void setBaseSalary(double dollars);
    void setCommissionRate(double rate);
    void setSalesVolume(double dollars);
    
private:
    double baseSalary;
    double commissionRate;
    double salesVolume;
};

class SalariedEmployee: public Employee {
public:
    SalariedEmployee();
    SalariedEmployee(std::string name,
                     double salary = 0);
    virtual double getPay() const;
    void setSalary(double salary);
    
private:
    double salary;
};
#endif // Employee_h
