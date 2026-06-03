# Hotel Booking Management System

A console-based C++ application for hotel staff to manage customers, rooms, bookings, billing, payments, and reports while demonstrating Object-Oriented Programming concepts.

## OOP Concepts Demonstrated

- **Encapsulation:** Class data members are private and accessed through public getters/setters.
- **Inheritance:** `StandardRoom`, `DeluxeRoom`, and `SuiteRoom` inherit from the abstract `Room` base class.
- **Polymorphism:** Each room category overrides `calculatePrice()` to calculate charges differently.
- **Abstraction:** `Payment` is an abstract base class implemented by `CashPayment` and `CardPayment`.

## Features

- Add and search customers.
- Add Standard, Deluxe, and Suite rooms.
- View all, available, and occupied rooms.
- Book rooms with automatically generated booking IDs.
- Cancel bookings and release rooms.
- Generate bills and mark payments as cash or card.
- View all bookings and customer booking history reports.

## Build and Run

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o hotel_management
./hotel_management
```

## Main Menu

1. Add Customer
2. Add Room
3. View Rooms
4. Book Room
5. Cancel Booking
6. Generate Bill
7. View All Bookings / Reports
8. Exit
