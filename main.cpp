/* ======================================
   Hotel Booking System with Loyalty Program
   The Hotel Booking System with Loyalty Program is a C++ application designed to manage hotel room bookings 
   efficiently and reward loyal customers. It features room management, customer tracking, and a 
   loyalty program,utilizing various standard libraries for robust functionality. 
   Custom exception handling ensures graceful error management and a smooth user experience
   ====================================== */

#include <iostream>      // For standard input and output
#include <string>        // For using string class
#include <vector>        // For using vector container
#include <memory>        // For using smart pointers
#include <unordered_map> // For using unordered_map container
#include <algorithm>     // For using algorithms like sort, find, etc.
#include <iomanip>       // For setting output format
#include <regex>         // For using regular expressions
#include <limits>        // For numeric limits
#include <deque>         // For using deque container
#include <fstream>       // For file input and output
#include <sstream>       // For string stream operations
#include <chrono>        // For date and time operations
#include <ctime>         // For C-style date and time operations
#include <map>           // For using map container

using namespace std;

// Custom Exceptions
/**
 * @class BookingException
 * @brief Exception class for handling booking related errors.
 */
class BookingException : public exception {
    string message;
public:
/**
     * @brief Constructor for BookingException.
     * @param msg Error message.
     */
    BookingException(const string& msg) : message(msg) {}
    /**
     * @brief Override what() method to return error message.
     * @return Error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class PaymentException : public exception {
    string message;
public:
    PaymentException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Room base class
/**
 * @class Room
 * @brief Abstract base class for different types of rooms.
 */
class Room {
protected:
    int roomNumber;
    double price;
    bool isBooked;

public:
    /**
     * @brief Constructor for Room.
     * @param number Room number.
     * @param price Price of the room.
     */
    Room(int number, double price) : roomNumber(number), price(price), isBooked(false) {}
    /**
     * @brief Virtual destructor for Room.
     */
    virtual ~Room() = default;

    /**
     * @brief Get the booking status of the room.
     * @return True if the room is booked, false otherwise.
     */

    bool getIsBooked() const { return isBooked; }

    /**
     * @brief Set the booking status of the room.
     * @param status Booking status to set.
     */

    void setBooked(bool status) { isBooked = status; }

    /**
     * @brief Get the room number.
     * @return Room number.
     */

    int getRoomNumber() const { return roomNumber; }

    /**
     * @brief Get the price of the room.
     * @return Price of the room.
     */ 

    double getPrice() const { return price; }
   
    /**
     * @brief Pure virtual function to display room information.
     */
    virtual void displayInfo() const = 0;
    /**
     * @brief Pure virtual function to get the room type.
     * @return Room type as a string.
     */
    virtual string getRoomType() const = 0;
};
// StandardRoom class
/**
 * @class StandardRoom
 * @brief Derived class for standard rooms.
 */
class StandardRoom : public Room {
public:
    StandardRoom(int number) : Room(number, 3000.0) {} // Price in INR

    void displayInfo() const override {
        cout << "Room " << roomNumber << " - Standard (₹" << price << ")\n";
    }

    string getRoomType() const override {
        return "Standard";
    }
};

class DeluxeRoom : public Room {
public:
    DeluxeRoom(int number) : Room(number, 5000.0) {} // Price in INR

    void displayInfo() const override {
        cout << "Room " << roomNumber << " - Deluxe (₹" << price << ")\n";
    }

    string getRoomType() const override {
        return "Deluxe";
    }
};

class SuiteRoom : public Room {
public:
    SuiteRoom(int number) : Room(number, 8000.0) {} // Price in INR

    void displayInfo() const override {
        cout << "Room " << roomNumber << " - Suite (₹" << price << ")\n";
    }

    string getRoomType() const override {
        return "Suite";
    }
};

// Customer class with additional getter methods
class Customer {
public:
    enum class LoyaltyTier { REGULAR, SILVER, GOLD, PLATINUM };

private:
    string name;
    string email;
    string phone;
    string adhaar;
    string password;
    int loyaltyPoints;
    LoyaltyTier tier;

public:
    Customer(const string& name, const string& email, const string& phone, const string& adhaar, const string& pwd)
        : name(name), email(email), phone(phone), adhaar(adhaar), password(pwd), loyaltyPoints(0), tier(LoyaltyTier::REGULAR) {}

    // Getter methods
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getAdhaar() const { return adhaar; }
    string getPassword() const { return password; }
    int getLoyaltyPoints() const { return loyaltyPoints; }

    void addLoyaltyPoints(int points) {
        if (points > 0) {
            loyaltyPoints += points;
            updateTier();
        }
        else if (points < 0) {
            loyaltyPoints += points;
            if (loyaltyPoints < 0)
                loyaltyPoints = 0;
            updateTier();
        }
    }

    void display() const {
        cout << "Name: " << name << endl
             << "Email: " << email << endl
             << "Phone: " << phone << endl
             << "Adhaar: " << adhaar << endl
             << "Loyalty Points: " << loyaltyPoints << endl
             << "Tier: " << tierToString(tier) << endl;
    }

    string tierToString(LoyaltyTier t) const { // Helper function to convert LoyaltyTier to string
        switch (t) {
            case LoyaltyTier::REGULAR: return "Regular";
            case LoyaltyTier::SILVER: return "Silver";
            case LoyaltyTier::GOLD: return "Gold";
            case LoyaltyTier::PLATINUM: return "Platinum";
            default: return "Unknown";
        }
    }

private: // Helper function to update tier based on loyalty points
    void updateTier() {
        if (loyaltyPoints >= 1000) tier = LoyaltyTier::PLATINUM;
        else if (loyaltyPoints >= 500) tier = LoyaltyTier::GOLD;
        else if (loyaltyPoints >= 200) tier = LoyaltyTier::SILVER;
        else tier = LoyaltyTier::REGULAR;
    }
};

// Validator Class
class Validator {
public:
    template<typename T>
    static T getInput(const string& prompt) {
        T input;
        while (true) {
            cout << prompt;
            cin >> input;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please try again.\n";
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return input;
            }
        }
    }

    static bool isValidEmail(const string& email) {
        const regex pattern("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
        return regex_match(email, pattern);
    }

    static bool isValidPhone(const string& phone) {
        return phone.length() == 10 && all_of(phone.begin(), phone.end(), ::isdigit);
    }

    static bool isValidAdhaar(const string& adhaar) {
        return adhaar.length() == 12 && all_of(adhaar.begin(), adhaar.end(), ::isdigit);
    }
};

// Hotel class with Reporting Features
class Hotel {
private:
    unordered_map<string, shared_ptr<Customer>> customers; // username -> Customer
    unordered_map<int, shared_ptr<Room>> rooms; // room number -> Room
    unordered_map<int, string> bookings; // roomNumber -> username

    const string CUSTOMERS_FILE = "customers.txt";
    const string ROOMS_FILE = "rooms.txt";
    const string BOOKINGS_FILE = "bookings.txt";

    void saveCustomersToFile() {
        ofstream file(CUSTOMERS_FILE);
        if (!file) {
            cout << "Error opening " << CUSTOMERS_FILE << " for writing.\n";
            return;
        }
        for (const auto& [username, customer] : customers) {
            // Format: username,name,email,phone,adhaar,password,loyaltyPoints
            file << username << ","
                 << customer->getName() << ","
                 << customer->getEmail() << ","
                 << customer->getPhone() << ","
                 << customer->getAdhaar() << ","
                 << customer->getPassword() << ","
                 << customer->getLoyaltyPoints() << "\n";
        }
        file.close();
    }

    void loadCustomersFromFile() { // Load customers from customers.txt
        ifstream file(CUSTOMERS_FILE);
        if (!file.is_open()) {
            // No customers file exists yet
            return;
        }
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string username, name, email, phone, adhaar, password, pointsStr;
            if (getline(ss, username, ',') &&
                getline(ss, name, ',') &&
                getline(ss, email, ',') &&
                getline(ss, phone, ',') &&
                getline(ss, adhaar, ',') &&
                getline(ss, password, ',') &&
                getline(ss, pointsStr)) {

                int points = stoi(pointsStr);
                auto customer = make_shared<Customer>(name, email, phone, adhaar, password);
                customer->addLoyaltyPoints(points);
                customers[username] = customer;
            }
        }
        file.close();
    }

    void saveRoomsToFile() { // Save rooms to rooms.txt
        ofstream file(ROOMS_FILE);
        if (!file) {
            cout << "Error opening " << ROOMS_FILE << " for writing.\n";
            return;
        }
        for (const auto& [number, room] : rooms) {
            file << number << "," 
                 << (room->getIsBooked() ? "1" : "0") << ","
                 << room->getRoomType() << "\n";
        }
        file.close();
    }

    void loadRoomsFromFile() { // Load rooms from rooms.txt
        ifstream file(ROOMS_FILE);
        if (!file.is_open()) {
            // No rooms file exists yet
            return;
        }
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string numStr, bookedStr, roomType;
            if (getline(ss, numStr, ',') && 
                getline(ss, bookedStr, ',') && 
                getline(ss, roomType)) {

                int number;
                try {
                    number = stoi(numStr);
                }
                catch (const invalid_argument& e) {
                    cout << "Invalid room number format in rooms file.\n";
                    continue;
                }

                bool booked = (bookedStr == "1");
                shared_ptr<Room> room;

                if (roomType == "Standard") {
                    room = make_shared<StandardRoom>(number);
                }
                else if (roomType == "Deluxe") {
                    room = make_shared<DeluxeRoom>(number);
                }
                else if (roomType == "Suite") {
                    room = make_shared<SuiteRoom>(number);
                }
                else {
                    cout << "Unknown room type '" << roomType << "' for room number " << number << ".\n";
                    continue;
                }

                room->setBooked(booked);
                rooms[number] = room;
            }
        }
        file.close();
    }

    // Load bookings from bookings.txt
    void loadBookingsFromFile() {
        ifstream file(BOOKINGS_FILE);
        if (!file.is_open()) {
            // No bookings file exists yet
            return;
        }
        string line;

        while (getline(file, line)) {
            if (line.empty()) {
                cout << "Skipping empty line in bookings file.\n";
                continue;
            }

            stringstream ss(line);
            string roomStr, username;
            
            if (getline(ss, roomStr, ',') && getline(ss, username)) {
                cout << "Processing booking: Room " << roomStr << ", User " << username << "\n";
                try {
                    int roomNumber = stoi(roomStr);
                    
                    // Validate room existence
                    auto it = rooms.find(roomNumber);
                    if (it != rooms.end()) {
                        bookings[roomNumber] = username;
                        it->second->setBooked(true);
                    } else {
                        cout << "Warning: Room number " << roomNumber 
                             << " in bookings file does not exist.\n";
                    }
                }
                catch (const invalid_argument& e) {
                    cout << "Error: Invalid room number '" << roomStr 
                         << "'. " << e.what() << "\n";
                    continue; // Skip invalid entries
                }
                catch (const out_of_range& e) {
                    cout << "Error: Room number '" << roomStr 
                         << "' is out of range. " << e.what() << "\n";
                    continue; // Skip out-of-range entries
                }
            }
            else {
                cout << "Error: Malformed line '" << line 
                     << "'. Skipping.\n";
            }
        }
        file.close();
    }

    // Save bookings to bookings.txt
    void saveBookingsToFile() {
        ofstream file(BOOKINGS_FILE);
        if (!file) {
            cout << "Error opening " << BOOKINGS_FILE << " for writing.\n";
            return;
        }
        for (const auto& [roomNumber, username] : bookings) {
            // Format: roomNumber,username
            file << roomNumber << "," << username << "\n";
        }
        file.close();
    }

    // Reporting Methods
    void generateOccupancyReport() const {
        int totalRooms = rooms.size();
        int bookedRooms = bookings.size();
        double occupancyRate = (static_cast<double>(bookedRooms) / totalRooms) * 100.0;

        cout << "\n=== Occupancy Report ===\n";
        cout << "Total Rooms: " << totalRooms << endl;
        cout << "Booked Rooms: " << bookedRooms << endl;
        cout << "Available Rooms: " << (totalRooms - bookedRooms) << endl;
        cout << fixed << setprecision(2) << "Occupancy Rate: " << occupancyRate << "%" << endl;
    }

    void generatePopularRoomTypesReport() const {
        unordered_map<string, int> roomTypeCount;

        for (const auto& [roomNumber, username] : bookings) {
            auto it = rooms.find(roomNumber);
            if (it != rooms.end()) {
                string roomType = it->second->getRoomType();
                roomTypeCount[roomType]++;
            }
        }

        // Sort room types by popularity
        vector<pair<string, int>> sortedRoomTypes(roomTypeCount.begin(), roomTypeCount.end());
        sort(sortedRoomTypes.begin(), sortedRoomTypes.end(),
            [](const pair<string, int>& a, const pair<string, int>& b) -> bool {
                return a.second > b.second;
            });

        cout << "\n=== Popular Room Types Report ===\n";
        if (sortedRoomTypes.empty()) {
            cout << "No bookings yet.\n";
            return;
        }

        cout << left << setw(20) << "Room Type" << setw(10) << "Bookings" << endl;
        cout << "------------------------------\n";
        for (const auto& [type, count] : sortedRoomTypes) {
            cout << left << setw(20) << type << setw(10) << count << endl;
        }
    }

public:
    Hotel() {
        loadCustomersFromFile();
        loadRoomsFromFile();
        loadBookingsFromFile();
        if (rooms.empty()) {
            initializeRooms();
            saveRoomsToFile();
        }
    }

    ~Hotel() {
        saveCustomersToFile();
        saveRoomsToFile();
        saveBookingsToFile();
    }

    void initializeRooms() {
        // Initialize rooms for 5 floors, each with 3 types
        // Room numbers: 101, 102, 103; 201, 202, 203; ..., 501, 502, 503
        for (int floor = 1; floor <= 5; ++floor) {
            int base = floor * 100;
            rooms[base + 1] = make_shared<StandardRoom>(base + 1);
            rooms[base + 2] = make_shared<DeluxeRoom>(base + 2);
            rooms[base + 3] = make_shared<SuiteRoom>(base + 3);
        }
    }

    void registerCustomer() {
        string username, password, name, email, phone, adhaar;

        cout << "=== User Registration ===\n";
        cout << "Enter username: ";
        getline(cin, username);

        if (customers.find(username) != customers.end()) {
            cout << "Username already exists. Please choose a different username.\n";
            return;
        }

        cout << "Enter password: ";
        getline(cin, password);

        cout << "Enter your full name: ";
        getline(cin, name);

        while (true) {
            cout << "Enter your email: ";
            getline(cin, email);
            if (Validator::isValidEmail(email)) {
                break;
            }
            else {
                cout << "Invalid email format. Please try again.\n";
            }
        }

        while (true) {
            cout << "Enter your phone number (10 digits): ";
            getline(cin, phone);
            if (Validator::isValidPhone(phone)) {
                break;
            }
            else {
                cout << "Invalid phone number. Please enter exactly 10 digits.\n";
            }
        }

        while (true) {
            cout << "Enter your Adhaar number (12 digits): ";
            getline(cin, adhaar);
            if (Validator::isValidAdhaar(adhaar)) {
                break;
            }
            else {
                cout << "Invalid Adhaar number. Please enter exactly 12 digits.\n";
            }
        }

        auto customer = make_shared<Customer>(name, email, phone, adhaar, password);
        customers[username] = customer;
        saveCustomersToFile();
        cout << "Registration successful!\n";
    }

    shared_ptr<Customer> login() {
        string username, password;
        cout << "=== User Login ===\n";
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);

        auto it = customers.find(username);
        if (it != customers.end()) {
            if (it->second->getPassword() == password) {
                cout << "Login successful! Welcome, " << it->second->getName() << "!\n";
                return it->second;
            }
            else {
                cout << "Incorrect password. Please try again.\n";
                return nullptr;
            }
        }
        else {
            cout << "Username not found. Please register first.\n";
            return nullptr;
        }
    }

    void userMenu(shared_ptr<Customer> customer) {
        if (!customer) return;

        int userChoice;
        do {
            cout << "\n=== User Menu ===\n";
            cout << "1. View Available Rooms\n";
            cout << "2. Book Room\n";
            cout << "3. Cancel Booking\n";
            cout << "4. View Loyalty Points\n";
            cout << "5. Logout\n";
            cout << "Enter your choice: ";
            cin >> userChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

            switch (userChoice) {
                case 1:
                    viewAvailableRooms();
                    break;
                case 2:
                    bookRoom(customer);
                    break;
                case 3:
                    cancelBooking(customer);
                    break;
                case 4:
                    cout << "Loyalty Points: " << customer->getLoyaltyPoints() << endl;
                    break;
                case 5:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (userChoice != 5);
    }

   // Admin Menu
    void adminMenu() {
        int adminChoice;
        do {
            cout << "\n=== Admin Menu ===\n";
            cout << "1. View Customer Details\n";
            cout << "2. View Customer Bookings\n";
            cout << "3. Generate Occupancy Report\n";
            cout << "4. Generate Popular Room Types Report\n";
            cout << "5. Checkout Room\n";
            cout << "6. Logout\n";
            cout << "Enter your choice: ";
            cin >> adminChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

            switch (adminChoice) {
                case 1:
                    viewCustomerDetails();
                    break;
                case 2:
                    viewCustomerBookings();
                    break;
                case 3:
                    generateOccupancyReport();
                    break;
                case 4:
                    generatePopularRoomTypesReport();
                    break;
                case 5:
                    checkoutRoom();
                    break;
                case 6:
                    cout << "Logging out from admin account...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (adminChoice != 6);
    }
   // Room Management
    void checkoutRoom() {
        int roomNumber;
        cout << "Enter room number to checkout (e.g., 101): ";
        cin >> roomNumber;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        auto it = rooms.find(roomNumber);
        if (it != rooms.end()) {
            if (it->second->getIsBooked()) {
                // Remove from bookings map
                auto bookingIt = bookings.find(roomNumber);
                if (bookingIt != bookings.end()) {
                    string username = bookingIt->second;
                    bookings.erase(bookingIt);
                    cout << "Room " << roomNumber << " has been checked out by user '" << username << "' and is now available.\n";
                } else {
                    cout << "Room " << roomNumber << " was booked but no booking record found.\n";
                }
                it->second->setBooked(false);
                saveRoomsToFile();
                saveBookingsToFile();
            }
            else {
                cout << "Room " << roomNumber << " is already available.\n";
            }
        }
        else {
            cout << "Invalid room number.\n";
        }
    }

    void viewAvailableRooms() const {
        // Organize rooms by floor
        map<int, vector<shared_ptr<Room>>> floors; // Use map to maintain order
        for (const auto& [number, room] : rooms) {
            if (!room->getIsBooked()) {
                int floor = number / 100;
                floors[floor].push_back(room);
            }
        }

        // Display rooms floor by floor in order
        for (const auto& [floor, roomsOnFloor] : floors) {
            cout << "\n--- Floor " << floor << " ---\n";
            cout << left << setw(10) << "Room No" << setw(15) << "Type" << endl;
            cout << "------------------------------\n";
            for (const auto& room : roomsOnFloor) {
                cout << left << setw(10) << room->getRoomNumber()
                     << setw(15) << room->getRoomType()
                     << endl;
            }
        }
    }

    int getValidatedRoomNumber() const {
        string input;
        int roomNumber;

        while (true) {
            cout << "Enter room number: ";
            getline(cin, input);

            // Trim whitespace
            input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());

            try {
                roomNumber = stoi(input);
                if (rooms.find(roomNumber) != rooms.end()) {
                    return roomNumber;
                }
                else {
                    cout << "Room number " << roomNumber << " does not exist. Please try again.\n";
                }
            }
            catch (const invalid_argument& e) {
                cout << "Invalid input. Please enter a valid numeric room number.\n";
            }
            catch (const out_of_range& e) {
                cout << "Room number is out of range. Please enter a valid room number.\n";
            }
        }
    }
    // Booking and Cancellation
    void bookRoom(shared_ptr<Customer> customer) {
        // Show available rooms before booking
        viewAvailableRooms();

        int roomNumber = getValidatedRoomNumber();

        auto it = rooms.find(roomNumber);
        if (it != rooms.end()) {
            if (!it->second->getIsBooked()) {
                it->second->setBooked(true);
                bookings[roomNumber] = getUsernameByCustomer(customer); // Associate room with username
                int pointsEarned = static_cast<int>(it->second->getPrice() * 0.1);
                customer->addLoyaltyPoints(pointsEarned);
                saveCustomersToFile();
                saveRoomsToFile();
                saveBookingsToFile();
                cout << "Room " << roomNumber << " booked successfully! You earned " << pointsEarned << " loyalty points.\n";
            }
            else {
                cout << "Room " << roomNumber << " is already booked by user '" << bookings[roomNumber] << "'.\n";
            }
        }
        else {
            // This should not happen due to validation
            cout << "Invalid room number.\n";
        }
    }

    void cancelBooking(shared_ptr<Customer> customer) {
        // Display rooms booked by this customer
        vector<int> userRooms;
        for (const auto& [roomNumber, username] : bookings) {
            if (username == getUsernameByCustomer(customer)) {
                userRooms.push_back(roomNumber);
            }
        }

        if (userRooms.empty()) {
            cout << "You have no bookings to cancel.\n";
            return;
        }

        cout << "\nYour Booked Rooms:\n";
        cout << left << setw(10) << "Room No" << setw(15) << "Type" << endl;
        cout << "------------------------------\n";
        for (const auto& roomNumber : userRooms) {
            cout << left << setw(10) << roomNumber
                 << setw(15) << rooms[roomNumber]->getRoomType()
                 << endl;
        }

        int roomNumber;
        cout << "\nEnter room number to cancel booking: ";
        cin >> roomNumber;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        auto it = bookings.find(roomNumber);
        if (it != bookings.end() && it->second == getUsernameByCustomer(customer)) {
            bookings.erase(it);
            rooms[roomNumber]->setBooked(false);
            int pointsDeducted = static_cast<int>(rooms[roomNumber]->getPrice() * 0.1);
            customer->addLoyaltyPoints(-pointsDeducted);
            saveCustomersToFile();
            saveRoomsToFile();
            saveBookingsToFile();
            cout << "Booking for room " << roomNumber << " has been canceled. You lost " << pointsDeducted << " loyalty points.\n";
        }
        else {
            cout << "You do not have a booking for room " << roomNumber << ".\n";
        }
    }

    // Helper function to get username by customer pointer
    string getUsernameByCustomer(shared_ptr<Customer> customer) const {
        for (const auto& [username, cust] : customers) {
            if (cust == customer) {
                return username;
            }
        }
        return "";
    }

    void viewCustomerDetails() const {
        cout << left << setw(15) << "Username" << setw(15) << "Name" << setw(25) << "Email" << setw(15) << "Phone" << setw(15) << "Adhaar" << setw(15) << "Points" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------\n";
        for (const auto& [username, customer] : customers) {
            cout << left << setw(15) << username
                 << setw(15) << customer->getName()
                 << setw(25) << customer->getEmail()
                 << setw(15) << customer->getPhone()
                 << setw(15) << customer->getAdhaar()
                 << setw(15) << customer->getLoyaltyPoints()
                 << "\n";
        }
    }

    void viewCustomerBookings() const {
        if (bookings.empty()) {
            cout << "No current bookings.\n";
            return;
        }

        cout << left << setw(10) << "Room No" << setw(20) << "Username" << setw(15) << "Type" << endl;
        cout << "-----------------------------------------------\n";
        for (const auto& [roomNumber, username] : bookings) {
            auto roomIt = rooms.find(roomNumber);
            if (roomIt != rooms.end()) {
                cout << left << setw(10) << roomNumber
                     << setw(20) << username
                     << setw(15) << roomIt->second->getRoomType()
                     << endl;
            }
            else {
                cout << left << setw(10) << roomNumber
                     << setw(20) << "Unknown"
                     << setw(15) << "Unknown"
                     << endl;
            }
        }
    }
};

// Main function
int main() {
    Hotel hotel;

    int initialChoice;
    do {
        cout << "\n===== Welcome to the Hotel Booking System =====\n";
        cout << "1. Admin Login\n";
        cout << "2. User Login\n";
        cout << "3. Register as User\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> initialChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (initialChoice) {
            case 1: {
                // Admin login functionality
                string adminUsername, adminPassword;
                cout << "==== Admin Login ====\n";
                cout << "Enter admin username: ";
                getline(cin, adminUsername);
                cout << "Enter admin password: ";
                getline(cin, adminPassword);

                // Simple admin authentication (can be enhanced)
                if (adminUsername == "admin" && adminPassword == "adminpass") {
                    cout << "Admin login successful!\n";
                    hotel.adminMenu();
                }
                else {
                    cout << "Incorrect admin credentials. Access denied.\n";
                }
                break;
            }
            case 2: {
                auto customer = hotel.login();
                if (customer) {
                    hotel.userMenu(customer);
                }
                break;
            }
            case 3:
                hotel.registerCustomer();
                break;
            case 4:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (initialChoice != 4);

    return 0;
}
