/*
 * Hostel Management System
 * Author: M. Mansoor Ur Rehman 
 *
 * Concepts demonstrated:
 *   - Classes and Objects
 *   - Inheritance (BoysHostel / GirlsHostel extend Hostel)
 *   - Polymorphism (virtual functions: displayFacilities, displayRules)
 *   - Templates (Payment<T>)
 *   - File I/O (saving resident records to .txt)
 *   - Vectors (dynamic resident and room lists)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

// ============================================================
// CONSTANTS
// ============================================================

const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "1234";
const int    MAX_ROOMS      = 200;

const double REGISTRATION_FEE       = 5000.0;
const double ROOM_PRICE_1_PERSON    = 18000.0;
const double ROOM_PRICE_2_PERSON    = 16000.0;
const double ROOM_PRICE_3_PERSON    = 14500.0;
const double ROOM_PRICE_4_PERSON    = 12500.0;

// ============================================================
// UTILITY — Input helpers to prevent crashes on bad input
// ============================================================

// Reads a valid integer from the user; keeps asking on bad input
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
        } else {
            cin.ignore();
            return value;
        }
    }
}

// Reads a valid positive double from the user
double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid amount.\n";
        } else {
            cin.ignore();
            return value;
        }
    }
}

// ============================================================
// CLASS: Resident
// Stores all personal and academic details of a hostel resident
// ============================================================

class Resident {
private:
    string name;
    string fatherName;
    string regNumber;
    string cnic;
    string phone;
    int    age;
    string department;
    int    semester;
    string address;
    string roomNumber;

public:
    Resident(const string& name, const string& fatherName, const string& regNumber,
             const string& cnic, const string& phone, int age,
             const string& department, int semester, const string& address,
             const string& room = "")
        : name(name), fatherName(fatherName), regNumber(regNumber),
          cnic(cnic), phone(phone), age(age), department(department),
          semester(semester), address(address), roomNumber(room) {}

    // Getters
    string getName()       const { return name; }
    string getFatherName() const { return fatherName; }
    string getRegNumber()  const { return regNumber; }
    string getCNIC()       const { return cnic; }
    string getPhone()      const { return phone; }
    string getDepartment() const { return department; }
    string getAddress()    const { return address; }
    string getRoomNumber() const { return roomNumber; }
    int    getAge()        const { return age; }
    int    getSemester()   const { return semester; }

    void setRoomNumber(const string& room) { roomNumber = room; }

    // Prints a short summary (used in admin view)
    void displayInfo() const {
        cout << "Name:                " << name       << "\n"
             << "Registration Number: " << regNumber  << "\n"
             << "Department:          " << department << "\n"
             << "Semester:            " << semester   << "\n";
    }

    // Prints full details (used when saving to file)
    void displayFullInfo() const {
        cout << "Name:                " << name        << "\n"
             << "Father's Name:       " << fatherName  << "\n"
             << "Permanent Address:   " << address     << "\n"
             << "Registration Number: " << regNumber   << "\n"
             << "CNIC:                " << cnic        << "\n"
             << "Phone Number:        " << phone       << "\n"
             << "Age:                 " << age         << "\n"
             << "Department:          " << department  << "\n"
             << "Semester:            " << semester    << "\n"
             << "Room Number:         " << roomNumber  << "\n";
    }

    // Allows admin to update editable fields only
    void editDetails() {
        cout << "\n--- Edit Resident Data ---\n";

        cout << "Current Name: " << name
             << "\nEnter new Name (or press Enter to keep): ";
        string input;
        getline(cin, input);
        if (!input.empty()) name = input;

        cout << "Current Father's Name: " << fatherName
             << "\nEnter new Father's Name (or press Enter to keep): ";
        getline(cin, input);
        if (!input.empty()) fatherName = input;

        cout << "Current Address: " << address
             << "\nEnter new Address (or press Enter to keep): ";
        getline(cin, input);
        if (!input.empty()) address = input;

        cout << "Current Phone: " << phone
             << "\nEnter new Phone (or press Enter to keep): ";
        getline(cin, input);
        if (!input.empty()) phone = input;

        cout << "Resident data updated successfully!\n";
    }
};

// ============================================================
// CLASS: Room
// Represents a single physical room in the hostel
// ============================================================

class Room {
private:
    string roomNumber;
    bool   isOccupied;

public:
    Room(const string& number) : roomNumber(number), isOccupied(false) {}

    void   markOccupied()      { isOccupied = true; }
    string getRoomNumber() const { return roomNumber; }
    bool   getIsOccupied() const { return isOccupied; }
};

// ============================================================
// CLASS: Hostel (Abstract Base Class)
// Defines the interface all hostels must implement
// ============================================================

class Hostel {
protected:
    string         hostelName;
    string         dataFile;       // output file name for this hostel
    vector<Resident> residents;
    vector<Room>     rooms;

public:
    Hostel(const string& name, const string& file, const string& roomPrefix)
        : hostelName(name), dataFile(file) {
        // Create MAX_ROOMS rooms with the given prefix (B1–B200 or G1–G200)
        for (int i = 1; i <= MAX_ROOMS; ++i) {
            rooms.emplace_back(roomPrefix + to_string(i));
        }
    }

    // Pure virtual — each hostel type provides its own list
    virtual void displayFacilities() const = 0;
    virtual void displayRules()      const = 0;

    // Returns pointer to first available room, or nullptr if full
    Room* findAvailableRoom() {
        for (auto& room : rooms) {
            if (!room.getIsOccupied()) return &room;
        }
        return nullptr;
    }

    void addResident(const Resident& resident) {
        residents.push_back(resident);
    }

    // Shows count of available rooms
    void showAvailableRooms() const {
        int count = 0;
        for (const auto& room : rooms)
            if (!room.getIsOccupied()) count++;
        cout << "Available Rooms: " << count << " / " << MAX_ROOMS << "\n";
    }

    // Shows count of occupied rooms
    void showBookedRooms() const {
        int count = 0;
        for (const auto& room : rooms)
            if (room.getIsOccupied()) count++;
        cout << "Booked Rooms: " << count << " / " << MAX_ROOMS << "\n";
    }

    // Displays brief info for all residents
    void showResidentsData() const {
        if (residents.empty()) {
            cout << "No residents registered yet.\n";
            return;
        }
        cout << "\n--- " << hostelName << " Residents ---\n";
        for (const auto& resident : residents) {
            resident.displayInfo();
            cout << "Room Number: " << resident.getRoomNumber() << "\n";
            cout << string(35, '-') << "\n";
        }
    }

    // Finds resident by reg number and opens edit menu
    void editResidentData() {
        cout << "Enter Registration Number of resident to edit: ";
        string regNumber;
        getline(cin, regNumber);

        for (auto& resident : residents) {
            if (resident.getRegNumber() == regNumber) {
                resident.editDetails();
                saveToFile();
                return;
            }
        }
        cout << "Resident with Registration Number '" << regNumber << "' not found.\n";
    }

    // Appends all residents to the data file (overwrites for full refresh)
    void saveToFile() const {
        ofstream outFile("data/" + dataFile);
        if (!outFile.is_open()) {
            cout << "Error: Could not open file '" << dataFile << "' for saving.\n";
            return;
        }

        outFile << hostelName << " — Resident Records\n";
        outFile << string(40, '=') << "\n";

        for (const auto& resident : residents) {
            outFile << "Name:                " << resident.getName()        << "\n"
                    << "Father's Name:       " << resident.getFatherName()  << "\n"
                    << "Permanent Address:   " << resident.getAddress()     << "\n"
                    << "Registration Number: " << resident.getRegNumber()   << "\n"
                    << "CNIC:                " << resident.getCNIC()        << "\n"
                    << "Phone Number:        " << resident.getPhone()       << "\n"
                    << "Age:                 " << resident.getAge()         << "\n"
                    << "Department:          " << resident.getDepartment()  << "\n"
                    << "Semester:            " << resident.getSemester()    << "\n"
                    << "Room Number:         " << resident.getRoomNumber()  << "\n"
                    << string(40, '-') << "\n";
        }

        outFile.close();
        cout << "Records saved to data/" << dataFile << "\n";
    }

    virtual ~Hostel() = default;
};

// ============================================================
// CLASS: BoysHostel
// Inherits from Hostel; provides boys-specific facilities/rules
// ============================================================

class BoysHostel : public Hostel {
public:
    BoysHostel(const string& name)
        : Hostel(name, "boys_hostel_records.txt", "B") {}

    void displayFacilities() const override {
        cout << "\n--- Facilities: " << hostelName << " ---\n"
             << "  - Mosque\n"
             << "  - Library\n"
             << "  - UPS / Generator\n"
             << "  - 24/7 Fast Internet\n"
             << "  - Laundry\n"
             << "  - Hot and Cold Water\n"
             << "  - Daily Room Cleaning\n"
             << "  - Gym\n"
             << "  - Gaming Room\n"
             << "  - First Aid\n"
             << "  - Three Meals a Day\n"
             << string(40, '-') << "\n";
    }

    void displayRules() const override {
        cout << "\n--- Rules & Restrictions: " << hostelName << " ---\n"
             << "  - No smoking on premises.\n"
             << "  - No visitors after 8:00 PM.\n"
             << "  - Maintain cleanliness in all common areas.\n"
             << "  - Respect hostel staff and fellow residents.\n"
             << string(40, '-') << "\n";
    }
};

// ============================================================
// CLASS: GirlsHostel
// Inherits from Hostel; provides girls-specific facilities/rules
// ============================================================

class GirlsHostel : public Hostel {
public:
    GirlsHostel(const string& name)
        : Hostel(name, "girls_hostel_records.txt", "G") {}

    void displayFacilities() const override {
        cout << "\n--- Facilities: " << hostelName << " ---\n"
             << "  - Mosque\n"
             << "  - Library\n"
             << "  - Nanny / Childcare\n"
             << "  - UPS / Generator\n"
             << "  - 24/7 Fast Internet\n"
             << "  - Laundry\n"
             << "  - Hot and Cold Water\n"
             << "  - Daily Room Cleaning\n"
             << "  - Gym\n"
             << "  - Gaming Room\n"
             << "  - First Aid\n"
             << "  - Three Meals a Day\n"
             << string(40, '-') << "\n";
    }

    void displayRules() const override {
        cout << "\n--- Rules & Restrictions: " << hostelName << " ---\n"
             << "  - No smoking on premises.\n"
             << "  - No visitors after 8:00 PM.\n"
             << "  - Maintain cleanliness in all common areas.\n"
             << "  - Respect hostel staff and fellow residents.\n"
             << string(40, '-') << "\n";
    }
};

// ============================================================
// CLASS TEMPLATE: Payment<T>
// Handles registration fee and room fee collection
// T allows int or double for fee amounts
// ============================================================

template <typename T>
class Payment {
public:
    const T registrationFee = static_cast<T>(REGISTRATION_FEE);

    // Returns room price based on selected room type (1–4 occupants)
    T getRoomPrice(int roomType) const {
        switch (roomType) {
            case 1: return static_cast<T>(ROOM_PRICE_1_PERSON);
            case 2: return static_cast<T>(ROOM_PRICE_2_PERSON);
            case 3: return static_cast<T>(ROOM_PRICE_3_PERSON);
            case 4: return static_cast<T>(ROOM_PRICE_4_PERSON);
            default: return static_cast<T>(0);
        }
    }

    // Collects registration fee payment; returns true if paid in full
    bool processRegistrationPayment() {
        cout << fixed << setprecision(0);
        cout << "Registration Fee: PKR " << registrationFee << "\n";
        T amountPaid = static_cast<T>(readDouble("Enter amount paid: PKR "));

        if (amountPaid >= registrationFee) {
            cout << "Registration payment successful!\n";
            return true;
        }
        cout << "Insufficient amount. Registration payment failed.\n";
        return false;
    }

    // Collects room fee payment; returns true if paid in full
    bool processRoomPayment(T roomPrice) {
        cout << "Room Price: PKR " << roomPrice << "\n";
        T amountPaid = static_cast<T>(readDouble("Enter amount paid: PKR "));

        if (amountPaid >= roomPrice) {
            cout << "Room payment successful!\n";
            return true;
        }
        cout << "Insufficient amount. Room payment failed.\n";
        return false;
    }
};

// ============================================================
// FUNCTION: printReceipt
// Displays a formatted payment receipt after registration
// ============================================================

void printReceipt(const Resident& resident, const string& roomNumber,
                  double regFee, double roomFee) {
    cout << "\n" << string(40, '=') << "\n";
    cout << "           PAYMENT RECEIPT\n";
    cout << string(40, '=') << "\n";
    cout << "Room Number:         " << roomNumber               << "\n";
    cout << "Name:                " << resident.getName()       << "\n";
    cout << "Registration Number: " << resident.getRegNumber()  << "\n";
    cout << "Department:          " << resident.getDepartment() << "\n";
    cout << "Semester:            " << resident.getSemester()   << "\n";
    cout << string(40, '-') << "\n";
    cout << fixed << setprecision(0);
    cout << "Registration Fee:    PKR " << regFee            << "\n";
    cout << "Room Fee:            PKR " << roomFee           << "\n";
    cout << "Total Paid:          PKR " << (regFee + roomFee) << "\n";
    cout << string(40, '=') << "\n";
    cout << "Registration complete! Welcome to CUST Residencia.\n\n";
}

// ============================================================
// FUNCTION: collectResidentDetails
// Collects all form fields for a new resident from user input
// Extracted to avoid duplicating the same 15 lines twice in main
// ============================================================

Resident collectResidentDetails() {
    string name, fatherName, regNumber, cnic, phone, department, address;
    int age, semester;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Father's Name: ";
    getline(cin, fatherName);

    cout << "Enter Registration Number: ";
    getline(cin, regNumber);

    cout << "Enter CNIC (e.g. 12345-1234567-1): ";
    getline(cin, cnic);

    cout << "Enter Phone Number: ";
    getline(cin, phone);

    age      = readInt("Enter Age: ");
    
    cout << "Enter Department: ";
    getline(cin, department);

    semester = readInt("Enter Semester (1-8): ");

    cout << "Enter Permanent Address: ";
    getline(cin, address);

    return Resident(name, fatherName, regNumber, cnic, phone,
                    age, department, semester, address);
}

// ============================================================
// FUNCTION: registerResident
// Shared registration flow for both Boys and Girls hostels
// ============================================================

void registerResident(Hostel& hostel) {
    Resident resident = collectResidentDetails();

    Room* room = hostel.findAvailableRoom();
    if (room == nullptr) {
        cout << "Sorry, no rooms are currently available.\n";
        return;
    }

    Payment<double> payment;

    if (!payment.processRegistrationPayment()) return;

    cout << "\n--- Select Room Type ---\n"
         << "1. Single Room  — PKR 18,000/month\n"
         << "2. Double Room  — PKR 16,000/month\n"
         << "3. Triple Room  — PKR 14,500/month\n"
         << "4. Quad Room    — PKR 12,500/month\n";

    int roomType = readInt("Enter room type (1-4): ");
    double roomPrice = payment.getRoomPrice(roomType);

    if (roomPrice == 0.0) {
        cout << "Invalid room type selected.\n";
        return;
    }

    if (!payment.processRoomPayment(roomPrice)) return;

    // All payments successful — assign room and save
    resident.setRoomNumber(room->getRoomNumber());
    room->markOccupied();
    hostel.addResident(resident);
    hostel.saveToFile();

    printReceipt(resident, room->getRoomNumber(),
                 payment.registrationFee, roomPrice);
}

// ============================================================
// FUNCTION: adminLogin
// Validates admin credentials; returns true on success
// ============================================================

bool adminLogin() {
    cout << "\n--- Admin Login ---\n";
    string username, password;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    cin.ignore();

    if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
        cout << "Login successful!\n";
        return true;
    }
    cout << "Invalid credentials. Access denied.\n";
    return false;
}

// ============================================================
// FUNCTION: hostelAdminMenu
// Admin sub-menu for a given hostel (boys or girls)
// ============================================================

void hostelAdminMenu(Hostel& hostel, const string& label) {
    char choice;
    do {
        cout << "\n--- " << label << " Admin Menu ---\n"
             << "1. Available Rooms\n"
             << "2. Booked Rooms\n"
             << "3. View All Residents\n"
             << "4. Edit Resident Data\n"
             << "5. Back\n"
             << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case '1': hostel.showAvailableRooms();  break;
            case '2': hostel.showBookedRooms();     break;
            case '3': hostel.showResidentsData();   break;
            case '4': hostel.editResidentData();    break;
            case '5': break;
            default:  cout << "Invalid choice. Try again.\n";
        }
    } while (choice != '5');
}

// ============================================================
// FUNCTION: adminMenu
// Top-level admin menu — choose boys or girls hostel
// ============================================================

void adminMenu(BoysHostel& boysHostel, GirlsHostel& girlsHostel) {
    char choice;
    do {
        cout << "\n--- Admin Panel ---\n"
             << "1. Boys Hostel\n"
             << "2. Girls Hostel\n"
             << "3. Exit Admin Panel\n"
             << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case '1': hostelAdminMenu(boysHostel,  "Boys Hostel");  break;
            case '2': hostelAdminMenu(girlsHostel, "Girls Hostel"); break;
            case '3': cout << "Exiting admin panel.\n"; break;
            default:  cout << "Invalid choice. Try again.\n";
        }
    } while (choice != '3');
}

// ============================================================
// FUNCTION: main
// Entry point — displays main menu and routes to actions
// ============================================================

int main() {
    // Create data directory if it doesn't exist (platform-safe attempt)
    system("mkdir -p data 2>/dev/null || mkdir data 2>nul");

    BoysHostel  boysHostel("Boys Hostel");
    GirlsHostel girlsHostel("Girls Hostel");

    char choice;
    do {
        cout << "\n" << string(30, '=') << "\n";
        cout << "   Hostel Management System\n";
        cout << string(30, '=') << "\n";
        cout << "1. Register — Boys Hostel\n";
        cout << "2. Register — Girls Hostel\n";
        cout << "3. View Hostel Facilities\n";
        cout << "4. Rules & Restrictions\n";
        cout << "5. Admin Login\n";
        cout << "q. Quit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case '1':
                registerResident(boysHostel);
                break;

            case '2':
                registerResident(girlsHostel);
                break;

            case '3': {
                cout << "\n1. Boys Hostel\n2. Girls Hostel\nChoice: ";
                char sub; cin >> sub; cin.ignore();
                if      (sub == '1') boysHostel.displayFacilities();
                else if (sub == '2') girlsHostel.displayFacilities();
                else cout << "Invalid choice.\n";
                break;
            }

            case '4': {
                cout << "\n1. Boys Hostel\n2. Girls Hostel\nChoice: ";
                char sub; cin >> sub; cin.ignore();
                if      (sub == '1') boysHostel.displayRules();
                else if (sub == '2') girlsHostel.displayRules();
                else cout << "Invalid choice.\n";
                break;
            }

            case '5':
                if (adminLogin())
                    adminMenu(boysHostel, girlsHostel);
                break;

            case 'q':
                cout << "Thank you for using CUST Residencia. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 'q');

    return 0;
}
