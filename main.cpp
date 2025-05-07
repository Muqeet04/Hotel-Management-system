#include <iostream>
#include <fstream>
#include <typeinfo>
using namespace std;

class Room {
protected:
    int roomnumber;
    double price;
    bool isbooked;

public:
    Room(int n, double p) {
        roomnumber = n;
        price = p;
        isbooked = false;
    }
    void loadFromFile(ifstream& in) {
        in >> roomnumber >> price >> isbooked;
    }

    virtual void displaydetails() =0;

    int getroomnumber() {
        return roomnumber;
    }

    double getprice() {
        return price;
    }

    bool getisbooked() {
        return isbooked;
    }

    void bookroom() {
        isbooked = true;
    }

    void cancelbooking() {
        isbooked = false;
    }
    void saveToFile(ofstream& out) {
        out << roomnumber << " " << price << " " << isbooked << endl;
    }

    virtual ~Room() {
    }
};

class StandardRoom : public Room {
public:
    StandardRoom(int rn): Room(rn, 1000) {
    }

    void displaydetails() override {
        cout << "Standard Room #" << roomnumber << " - PKR " << price << endl;
    }
};

class DeluxeRoom : public Room {
public:
    DeluxeRoom(int rn): Room(rn, 10000) {
    }

    void displaydetails() override {
        cout << "Deluxe Room #" << roomnumber << " - PKR " << price << endl;
    }
};

class SuitedRoom : public Room {
public:
    SuitedRoom(int rn): Room(rn, 100000) {
    }

    void displaydetails() override {
        cout << "Suite Room #" << roomnumber << " - PKR " << price << endl;
    }
};

class Booking {
private:
    string customerName;
    int roomNumber;
    int numberOfDays;
    double totalCost;

public:
    Booking(string cname, int rnum, int days, double cost)
        : customerName(cname), roomNumber(rnum), numberOfDays(days), totalCost(cost) {
    }

    void showBooking() {
        cout << "Customer: " << customerName
                << ", Room #: " << roomNumber
                << ", Days: " << numberOfDays
                << ", Total Cost: PKR " << totalCost << endl;
    }

    int getRoomNumber() const {
        return roomNumber;
    }
};

const int maxr = 100;
const int maxservices = 100;

class ServiceRequest {
private:
    int roomNumbers[maxservices];
    string services[maxservices];
    int totalreq;

public:
    ServiceRequest() {
        totalreq = 0;
    }

    void addservice(int roomn, string s) {
        if (totalreq < maxservices) {
            roomNumbers[totalreq] = roomn;
            services[totalreq] = s;
            totalreq++;
            cout << s << " requested for Room # " << roomn << endl;
        } else {
            cout << "Service queue full\n";
        }
    }

    void viewservices(int roomn) {
        cout << "Services requested for Room # " << roomn << ":\n";
        bool found = false;
        for (int i = 0; i < totalreq; i++) {
            if (roomNumbers[i] == roomn) {
                cout << "- " << services[i] << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No services requested for this room\n";
        }
    }

    double calculateServiceCost(int roomn) {
        double cost = 0.0;
        for (int i = 0; i < totalreq; i++) {
            if (roomNumbers[i] == roomn) {
                if (services[i] == "Food") {
                    cost += 500;
                } else if (services[i] == "Laundry") {
                    cost += 200;
                } else if (services[i] == "Cleaning") {
                    cost += 300;
                }
            }
        }
        return cost;
    }

    void saveToFile() {
        ofstream out("services.txt");
        for(int i = 0; i < totalreq; i++) {
            out << roomNumbers[i] << " " << services[i] << endl;
        }
        out.close();
    }

    void loadFromFile() {
        ifstream in("services.txt");
        totalreq = 0;
        while(in >> roomNumbers[totalreq]) {
            in.ignore();
            getline(in, services[totalreq]);
            totalreq++;
        }
        in.close();
    }
};

class Membership {
public:
    bool isMember;

    Membership() {
        isMember = false;
    }

    void activateMembership() {
        isMember = true;
        cout << "Membership activated! You will receive a 10% discount on bookings.\n";
    }

    bool checkMembership() {
        return isMember;
    }
};

class Payment {
public:
    static void makePayment(Room *room, Membership m, ServiceRequest &sr, int roomNumber, int numberOfDays) {
        double roomCost = room->getprice() * numberOfDays;
        sr.viewservices(roomNumber);
        double serviceCost = sr.calculateServiceCost(roomNumber);

        double totalCost = roomCost + serviceCost;
        if (m.checkMembership()) {
            totalCost *= 0.90;
        }

        cout << "Total Room Cost: PKR " << roomCost << endl;
        cout << "Total Service Cost: PKR " << serviceCost << endl;
        cout << "Total Amount to Pay: PKR " << totalCost << endl;
    }
};

class Hotel {
private:
    Room *room[maxr];
    int totalr;

    Booking *bookings[100];
    int totalBookings;

public:
    Hotel() {
        totalr = 0;
        totalBookings = 0;
    }

    void addroom(Room *r) {
        if (totalr < maxr) {
            room[totalr++] = r;
            cout << "Room added successfully\n";
        } else {
            cout << "Cannot add more rooms to hotel\n";
        }
    }

    void viewavailability() {
        cout << "Available Rooms:\n";
        for (int i = 0; i < totalr; i++) {
            if (!room[i]->getisbooked()) {
                room[i]->displaydetails();
            }
        }
    }

    Room *findroom(int i) {
        for (int j = 0; j < totalr; j++) {
            if (room[j]->getroomnumber() == i) {
                return room[j];
            }
        }
        return nullptr;
    }

    void bookRoomWithDetails(int rnum, string cname, int days) {
        Room *temp = findroom(rnum);
        if (temp && !temp->getisbooked()) {
            double cost = temp->getprice() * days;
            bookings[totalBookings++] = new Booking(cname, rnum, days, cost);
            temp->bookroom();
            cout << "Room booked successfully for " << cname << " for " << days << " days.\n";
        } else {
            cout << "Room not available or already booked.\n";
        }
    }

    void cancelroom(int r) {
        Room *temp = findroom(r);
        if (temp != nullptr) {
            if (temp->getisbooked()) {
                temp->cancelbooking();
                cout << "Room # " << r << " booking cancelled\n";
            } else {
                cout << "Room # " << r << " is not booked\n";
            }
        } else {
            cout << "Room not found\n";
        }
    }

    void viewBookings() {
        if (totalBookings == 0) {
            cout << "No bookings found.\n";
            return;
        }
        cout << "\nAll Bookings:\n";
        for (int i = 0; i < totalBookings; i++) {
            bookings[i]->showBooking();
        }
    }
    void saveRooms() {
        ofstream out("rooms.txt");
        for(int i=0; i<totalr; i++) {
            out << typeid(*room[i]).name() << " ";
            room[i]->saveToFile(out);
        }
        out.close();
    }

    void loadRooms() {
        ifstream in("rooms.txt");
        totalr = 0;
        string type;
        while (in >> type) {
            int rn;
            double price;
            bool booked;
            in >> rn >> price >> booked;

            Room* r = nullptr;
            if (type.find("StandardRoom") != string::npos)
                r = new StandardRoom(rn);
            else if (type.find("DeluxeRoom") != string::npos)
                r = new DeluxeRoom(rn);
            else if (type.find("SuitedRoom") != string::npos)
                r = new SuitedRoom(rn);

            if (r) {
                if (booked) r->bookroom();
                room[totalr++] = r;
            }
        }
        in.close();
    }

    ~Hotel() {
        for (int i = 0; i < totalr; i++) {
            delete room[i];
        }
        for (int i = 0; i < totalBookings; i++) {
            delete bookings[i];
        }
    }
};

class Review {
private:
    int roomNumbers[maxr];
    string comments[maxr];
    int ratings[maxr];
    int total;

public:
    Review() {
        total = 0;
    }

    void addReview(int roomn, int rating, const string &comment) {
        if (total < maxr) {
            roomNumbers[total] = roomn;
            ratings[total] = rating;
            comments[total] = comment;
            total++;
            cout << "Review added for Room #" << roomn << endl;
        } else {
            cout << "Review storage full.\n";
        }
    }

    void viewReviews(int roomn) {
        bool found = false;
        cout << "Reviews for Room #" << roomn << ":\n";
        for (int i = 0; i < total; i++) {
            if (roomNumbers[i] == roomn) {
                cout << "- Rating: " << ratings[i] << " | Comment: " << comments[i] << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No reviews for this room.\n";
        }
    }

    void saveToFile() {
        ofstream out("reviews.txt");
        for (int i = 0; i < total; i++) {
            out << roomNumbers[i] << " " << ratings[i] << " " << comments[i] << endl;
        }
        out.close();
    }

    void loadFromFile() {
        ifstream in("reviews.txt");
        total = 0;
        while (in >> roomNumbers[total] >> ratings[total]) {
            in.ignore();
            getline(in, comments[total]);
            total++;
        }
        in.close();
    }
};

int main() {
    Hotel hotel;
    ServiceRequest servicereq;
    Membership membership;
    Review review;

    hotel.loadRooms();
    servicereq.loadFromFile();
    review.loadFromFile();

    hotel.addroom(new StandardRoom(100));
    hotel.addroom(new DeluxeRoom(200));
    hotel.addroom(new SuitedRoom(300));
    hotel.addroom(new StandardRoom(101));

    int choice;

    do {
        cout << "\n---------Main Menu---------\n";
        cout << "1. Customer Login\n";
        cout << "2. Admin Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            do {
                cout << "\n--------Customer Menu--------\n";
                cout << "1. View Available Rooms\n";
                cout << "2. Book Room\n";
                cout << "3. Cancel Booking\n";
                cout << "4. Request Room Services\n";
                cout << "5. Activate membership\n";
                cout << "6. Make payment\n";
                cout << "7. Leave a review\n";
                cout << "8. View Room Reviews\n";
                cout << "9. Logout\n";
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        hotel.viewavailability();
                        break;
                    case 2: {
                        int rnum, days;
                        string cname;
                        cout << "Enter your name: ";
                        cin.ignore();
                        getline(cin, cname);
                        cout << "Enter room number: ";
                        cin >> rnum;
                        cout << "Enter number of days: ";
                        cin >> days;
                        hotel.bookRoomWithDetails(rnum, cname, days);
                        break;
                    }
                    case 3: {
                        int r;
                        cout << "Enter room number to cancel booking: ";
                        cin >> r;
                        hotel.cancelroom(r);
                        break;
                    }
                    case 4: {
                        int r, s;
                        cout << "Enter room number: ";
                        cin >> r;
                        cout << "Select service:\n1. Food\n2. Laundry\n3. Cleaning\n";
                        cin >> s;
                        if (s == 1) servicereq.addservice(r, "Food");
                        else if (s == 2) servicereq.addservice(r, "Laundry");
                        else if (s == 3) servicereq.addservice(r, "Cleaning");
                        else cout << "Invalid choice\n";
                        break;
                    }
                    case 5:
                        membership.activateMembership();
                        break;
                    case 6: {
                        int r, days;
                        cout << "Enter room number to make payment: ";
                        cin >> r;
                        cout << "Enter number of days booked: ";
                        cin >> days;
                        Room *temp = hotel.findroom(r);
                        if (temp != nullptr && temp->getisbooked()) {
                            Payment::makePayment(temp, membership, servicereq, r, days);
                        } else {
                            cout << "Room not found or not booked\n";
                        }
                        break;
                    }
                    case 7: {
                        int r, rating;
                        cout << "Enter room number to review: ";
                        cin >> r;
                        cout << "Enter rating (1-5): ";
                        cin >> rating;
                        cin.ignore();
                        string rev;
                        cout << "Enter your review: ";
                        getline(cin, rev);
                        review.addReview(r, rating, rev);
                        break;
                    }
                    case 8: {
                        int r;
                        cout<<"Enter room number to view reviews: ";
                        cin >> r;
                        review.viewReviews(r);
                        break;
                    }
                    case 9: {
                        cout<<"Logging out ...............";
                        break;
                    }
                    default:
                        cout << "Invalid choice\n";
                }
            } while (choice != 9);
        } else if (choice == 2) {
            string pass;
            cout << "Enter password: ";
            cin >> pass;
            if (pass == "admin") {
                int achoice;
                do {
                    cout << "\n---------Admin Menu---------\n";
                    cout << "1. Add New Room\n";
                    cout << "2. View Available Rooms\n";
                    cout << "3. View All Bookings\n";
                    cout << "4. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> achoice;

                    switch (achoice) {
                        case 1: {
                            int num, type;
                            cout << "Enter new room number: ";
                            cin >> num;
                            cout << "Room Type:\n1. Standard\n2. Deluxe\n3. Suite\n";
                            cin >> type;
                            if (type == 1) hotel.addroom(new StandardRoom(num));
                            else if (type == 2) hotel.addroom(new DeluxeRoom(num));
                            else if (type == 3) hotel.addroom(new SuitedRoom(num));
                            else cout << "Invalid Room Type\n";
                            break;
                        }
                        case 2:
                            hotel.viewavailability();
                            break;
                        case 3:
                            hotel.viewBookings();
                            break;
                        case 4:
                            cout << "Admin logged out.\n";
                            break;
                        default:
                            cout << "Invalid choice\n";
                    }
                } while (achoice != 4);
            } else {
                cout << "Wrong password\n";
            }
        }
    } while (choice != 3);

    hotel.saveRooms();
    servicereq.saveToFile();
    review.saveToFile();
    cout << "Program terminated.\n";

    return 0;
}
