#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Customer {
private:
    int customerId;
    string name;
    string phone;
    string email;

public:
    Customer(int id, string customerName, string customerPhone, string customerEmail)
        : customerId(id), name(std::move(customerName)), phone(std::move(customerPhone)), email(std::move(customerEmail)) {}

    int getCustomerId() const { return customerId; }
    const string &getName() const { return name; }
    const string &getPhone() const { return phone; }
    const string &getEmail() const { return email; }

    void setName(const string &customerName) { name = customerName; }
    void setPhone(const string &customerPhone) { phone = customerPhone; }
    void setEmail(const string &customerEmail) { email = customerEmail; }

    void display() const {
        cout << "Customer ID: " << customerId << '\n'
             << "Name       : " << name << '\n'
             << "Phone      : " << phone << '\n'
             << "Email      : " << email << '\n';
    }
};

class Room {
private:
    int roomNumber;
    string roomType;
    double basePrice;
    bool available;

public:
    Room(int number, string type, double price)
        : roomNumber(number), roomType(std::move(type)), basePrice(price), available(true) {}

    virtual ~Room() = default;

    int getRoomNumber() const { return roomNumber; }
    const string &getRoomType() const { return roomType; }
    double getBasePrice() const { return basePrice; }
    bool isAvailable() const { return available; }

    void setRoomNumber(int number) { roomNumber = number; }
    void setRoomType(const string &type) { roomType = type; }
    void setBasePrice(double price) { basePrice = price; }
    void setAvailable(bool status) { available = status; }

    virtual double calculatePrice(int nights) const = 0;

    virtual void display() const {
        cout << left << setw(12) << roomNumber
             << setw(15) << roomType
             << setw(15) << fixed << setprecision(2) << basePrice
             << (available ? "Available" : "Occupied") << '\n';
    }
};

class StandardRoom : public Room {
public:
    StandardRoom(int number, double price) : Room(number, "Standard", price) {}

    double calculatePrice(int nights) const override {
        return getBasePrice() * nights;
    }
};

class DeluxeRoom : public Room {
public:
    DeluxeRoom(int number, double price) : Room(number, "Deluxe", price) {}

    double calculatePrice(int nights) const override {
        return getBasePrice() * nights * 1.20;
    }
};

class SuiteRoom : public Room {
public:
    SuiteRoom(int number, double price) : Room(number, "Suite", price) {}

    double calculatePrice(int nights) const override {
        return getBasePrice() * nights * 1.50;
    }
};

class Payment {
public:
    virtual ~Payment() = default;
    virtual bool makePayment(double amount) = 0;
    virtual string paymentMethod() const = 0;
};

class CashPayment : public Payment {
public:
    bool makePayment(double amount) override {
        cout << "Cash payment of $" << fixed << setprecision(2) << amount << " received.\n";
        return true;
    }

    string paymentMethod() const override { return "Cash"; }
};

class CardPayment : public Payment {
public:
    bool makePayment(double amount) override {
        string cardNumber;
        cout << "Enter card number (last 4 digits are enough for demo): ";
        getline(cin, cardNumber);
        cout << "Card payment of $" << fixed << setprecision(2) << amount << " approved.\n";
        return true;
    }

    string paymentMethod() const override { return "Card"; }
};

class Booking {
private:
    int bookingId;
    int customerId;
    int roomNumber;
    string bookingDate;
    int nights;
    double billAmount;
    bool paid;
    string paymentType;

public:
    Booking(int id, int custId, int roomNo, string date, int stayNights, double amount)
        : bookingId(id), customerId(custId), roomNumber(roomNo), bookingDate(std::move(date)),
          nights(stayNights), billAmount(amount), paid(false), paymentType("Unpaid") {}

    int getBookingId() const { return bookingId; }
    int getCustomerId() const { return customerId; }
    int getRoomNumber() const { return roomNumber; }
    const string &getBookingDate() const { return bookingDate; }
    int getNights() const { return nights; }
    double getBillAmount() const { return billAmount; }
    bool isPaid() const { return paid; }
    const string &getPaymentType() const { return paymentType; }

    void markPaid(const string &method) {
        paid = true;
        paymentType = method;
    }

    void display() const {
        cout << left << setw(12) << bookingId
             << setw(13) << customerId
             << setw(12) << roomNumber
             << setw(15) << bookingDate
             << setw(8) << nights
             << "$" << setw(11) << fixed << setprecision(2) << billAmount
             << (paid ? "Paid (" + paymentType + ")" : "Unpaid") << '\n';
    }
};

class HotelManagementSystem {
private:
    vector<Customer> customers;
    vector<unique_ptr<Room>> rooms;
    vector<Booking> bookings;
    int nextCustomerId = 1001;
    int nextBookingId = 5001;

    Customer *findCustomer(int customerId) {
        auto it = find_if(customers.begin(), customers.end(), [customerId](const Customer &customer) {
            return customer.getCustomerId() == customerId;
        });
        return it == customers.end() ? nullptr : &(*it);
    }

    Room *findRoom(int roomNumber) {
        auto it = find_if(rooms.begin(), rooms.end(), [roomNumber](const unique_ptr<Room> &room) {
            return room->getRoomNumber() == roomNumber;
        });
        return it == rooms.end() ? nullptr : it->get();
    }

    Booking *findBooking(int bookingId) {
        auto it = find_if(bookings.begin(), bookings.end(), [bookingId](const Booking &booking) {
            return booking.getBookingId() == bookingId;
        });
        return it == bookings.end() ? nullptr : &(*it);
    }

    static int readInt(const string &prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number. Please try again.\n";
        }
    }

    static double readDouble(const string &prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount. Please try again.\n";
        }
    }

    static string readLine(const string &prompt) {
        string value;
        cout << prompt;
        getline(cin, value);
        return value;
    }

public:
    void addCustomer() {
        string name = readLine("Enter customer name : ");
        string phone = readLine("Enter phone number  : ");
        string email = readLine("Enter email address : ");

        customers.emplace_back(nextCustomerId++, name, phone, email);
        cout << "Customer added successfully. Assigned Customer ID: " << customers.back().getCustomerId() << "\n";
    }

    void viewCustomerDetails() {
        if (customers.empty()) {
            cout << "No customers registered.\n";
            return;
        }

        int customerId = readInt("Enter customer ID to search: ");
        Customer *customer = findCustomer(customerId);
        if (!customer) {
            cout << "Customer not found.\n";
            return;
        }
        customer->display();
    }

    void addRoom() {
        int roomNumber = readInt("Enter room number: ");
        if (findRoom(roomNumber)) {
            cout << "Room number already exists.\n";
            return;
        }

        cout << "Room Types: 1. Standard  2. Deluxe  3. Suite\n";
        int typeChoice = readInt("Select room type: ");
        double basePrice = readDouble("Enter base price per night: $");

        switch (typeChoice) {
        case 1:
            rooms.push_back(make_unique<StandardRoom>(roomNumber, basePrice));
            break;
        case 2:
            rooms.push_back(make_unique<DeluxeRoom>(roomNumber, basePrice));
            break;
        case 3:
            rooms.push_back(make_unique<SuiteRoom>(roomNumber, basePrice));
            break;
        default:
            cout << "Invalid room type. Room not added.\n";
            return;
        }

        cout << "Room added successfully.\n";
    }

    void displayRooms(bool onlyAvailable = false, bool onlyOccupied = false) const {
        cout << left << setw(12) << "Room No."
             << setw(15) << "Type"
             << setw(15) << "Base Price"
             << "Status\n";
        cout << string(55, '-') << '\n';

        bool found = false;
        for (const auto &room : rooms) {
            if ((onlyAvailable && !room->isAvailable()) || (onlyOccupied && room->isAvailable())) {
                continue;
            }
            room->display();
            found = true;
        }

        if (!found) {
            cout << "No rooms match the selected report.\n";
        }
    }

    void viewRoomsMenu() const {
        cout << "1. All Rooms\n2. Available Rooms\n3. Occupied Rooms\n";
        int choice = readInt("Choose room report: ");
        if (choice == 1) {
            displayRooms();
        } else if (choice == 2) {
            displayRooms(true, false);
        } else if (choice == 3) {
            displayRooms(false, true);
        } else {
            cout << "Invalid choice.\n";
        }
    }

    void bookRoom() {
        if (rooms.empty()) {
            cout << "No rooms available in the system. Add rooms first.\n";
            return;
        }

        int customerId = readInt("Enter customer ID: ");
        Customer *customer = findCustomer(customerId);
        if (!customer) {
            cout << "Customer not found. Add the customer before booking.\n";
            return;
        }

        cout << "Available rooms:\n";
        displayRooms(true, false);
        int roomNumber = readInt("Enter room number to book: ");
        Room *room = findRoom(roomNumber);
        if (!room) {
            cout << "Room not found.\n";
            return;
        }
        if (!room->isAvailable()) {
            cout << "Room is already occupied.\n";
            return;
        }

        string date = readLine("Enter booking date (YYYY-MM-DD): ");
        int nights = readInt("Enter number of nights: ");
        if (nights <= 0) {
            cout << "Nights must be greater than zero.\n";
            return;
        }

        double amount = room->calculatePrice(nights);
        bookings.emplace_back(nextBookingId++, customerId, roomNumber, date, nights, amount);
        room->setAvailable(false);

        cout << "Booking successful. Booking ID: " << bookings.back().getBookingId() << "\n";
        cout << "Estimated bill: $" << fixed << setprecision(2) << amount << "\n";
    }

    void cancelBooking() {
        int bookingId = readInt("Enter booking ID to cancel: ");
        auto it = find_if(bookings.begin(), bookings.end(), [bookingId](const Booking &booking) {
            return booking.getBookingId() == bookingId;
        });

        if (it == bookings.end()) {
            cout << "Booking not found.\n";
            return;
        }

        Room *room = findRoom(it->getRoomNumber());
        if (room) {
            room->setAvailable(true);
        }
        bookings.erase(it);
        cout << "Booking cancelled successfully.\n";
    }

    void generateBill() {
        int bookingId = readInt("Enter booking ID: ");
        Booking *booking = findBooking(bookingId);
        if (!booking) {
            cout << "Booking not found.\n";
            return;
        }

        Customer *customer = findCustomer(booking->getCustomerId());
        Room *room = findRoom(booking->getRoomNumber());

        cout << "\n========== HOTEL BILL ==========" << '\n';
        cout << "Booking ID : " << booking->getBookingId() << '\n';
        if (customer) {
            cout << "Customer   : " << customer->getName() << " (ID: " << customer->getCustomerId() << ")\n";
        }
        if (room) {
            cout << "Room       : " << room->getRoomNumber() << " - " << room->getRoomType() << '\n';
        }
        cout << "Date       : " << booking->getBookingDate() << '\n'
             << "Nights     : " << booking->getNights() << '\n'
             << "Amount Due : $" << fixed << setprecision(2) << booking->getBillAmount() << '\n'
             << "Status     : " << (booking->isPaid() ? "Paid" : "Unpaid") << '\n';

        if (!booking->isPaid()) {
            cout << "1. Cash Payment\n2. Card Payment\n3. Skip Payment\n";
            int choice = readInt("Choose payment option: ");
            unique_ptr<Payment> payment;
            if (choice == 1) {
                payment = make_unique<CashPayment>();
            } else if (choice == 2) {
                payment = make_unique<CardPayment>();
            } else {
                cout << "Payment skipped. Status remains unpaid.\n";
                cout << "================================\n\n";
                return;
            }

            if (payment->makePayment(booking->getBillAmount())) {
                booking->markPaid(payment->paymentMethod());
                cout << "Payment status updated to Paid.\n";
            }
        }
        cout << "================================\n\n";
    }

    void viewAllBookings() const {
        if (bookings.empty()) {
            cout << "No bookings found.\n";
            return;
        }

        cout << left << setw(12) << "Booking ID"
             << setw(13) << "Customer ID"
             << setw(12) << "Room No."
             << setw(15) << "Date"
             << setw(8) << "Nights"
             << setw(12) << "Amount"
             << "Payment\n";
        cout << string(85, '-') << '\n';

        for (const auto &booking : bookings) {
            booking.display();
        }
    }

    void viewCustomerBookingHistory() const {
        int customerId = readInt("Enter customer ID: ");
        bool found = false;
        cout << "Booking history for Customer ID " << customerId << ":\n";
        for (const auto &booking : bookings) {
            if (booking.getCustomerId() == customerId) {
                booking.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No bookings found for this customer.\n";
        }
    }

    void displayReports() {
        cout << "1. View All Bookings\n"
             << "2. View Available Rooms\n"
             << "3. View Occupied Rooms\n"
             << "4. View Customer Booking History\n"
             << "5. Search Customer by ID\n";
        int choice = readInt("Choose report: ");

        switch (choice) {
        case 1:
            viewAllBookings();
            break;
        case 2:
            displayRooms(true, false);
            break;
        case 3:
            displayRooms(false, true);
            break;
        case 4:
            viewCustomerBookingHistory();
            break;
        case 5:
            viewCustomerDetails();
            break;
        default:
            cout << "Invalid report option.\n";
        }
    }

    void seedSampleRooms() {
        rooms.push_back(make_unique<StandardRoom>(101, 100.00));
        rooms.push_back(make_unique<DeluxeRoom>(201, 150.00));
        rooms.push_back(make_unique<SuiteRoom>(301, 250.00));
    }

    void run() {
        seedSampleRooms();
        while (true) {
            cout << "\n===== Hotel Booking Management System =====\n"
                 << "1. Add Customer\n"
                 << "2. Add Room\n"
                 << "3. View Rooms\n"
                 << "4. Book Room\n"
                 << "5. Cancel Booking\n"
                 << "6. Generate Bill\n"
                 << "7. View All Bookings / Reports\n"
                 << "8. Exit\n";
            int choice = readInt("Enter your choice: ");

            switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                addRoom();
                break;
            case 3:
                viewRoomsMenu();
                break;
            case 4:
                bookRoom();
                break;
            case 5:
                cancelBooking();
                break;
            case 6:
                generateBill();
                break;
            case 7:
                displayReports();
                break;
            case 8:
                cout << "Thank you for using the Hotel Booking Management System.\n";
                return;
            default:
                cout << "Invalid choice. Please select a valid option.\n";
            }
        }
    }
};

int main() {
    HotelManagementSystem system;
    system.run();
    return 0;
}
