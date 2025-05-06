#include <iostream>
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
    virtual void displaydetails()=0;

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
    virtual ~Room() {}
};

class StandardRoom:public Room {
    public:
       StandardRoom(int rn): Room(rn,1000){}
       void displaydetails() override {
           cout<<"Standard room number: "<<roomnumber<<" PKR"<<price<<endl;
       }
};
class DeluxorRoom:public Room {
    public:
        DeluxorRoom(int rn): Room(rn,10000){}
        void displaydetails() override {
            cout<<"Deluxe room number: "<<roomnumber<<" PKR"<<price<<endl;
        }
};
class SuitedRoom:public Room {
    public:
       SuitedRoom(int rn): Room(rn,100000){}
       void displaydetails() override {
           cout<<"Suit room number: "<<roomnumber<<" PKR"<<price<<endl;
       }
};

const int maxr=100;
class Hotel {
    private:
       Room *room[maxr];
       int totalr;
    public:
        Hotel() {
            totalr=0;
        }

        void addroom(Room *r) {
            if(totalr<maxr) {
                room[totalr++] = r;
                cout<<"Room added successfully"<<endl;
            }
            else {
                cout<<"Can't add more room to hotel"<<endl;
            }
        }
        void viewavailability() {
            cout<<"Available rooms:"<<endl;
            for(int i=0;i<totalr;i++) {
                if(!room[i]->getisbooked()) {
                    room[i]->displaydetails();
                }
            }
        }
        Room *findroom(int i) {
            for(int j=0;j<totalr;j++) {
                if (room[i]->getroomnumber()==i) {
                    return room[i];
                }
            }
            return nullptr;
        }

        void Bookroom(int r) {
            Room *temp = findroom(r);
            if (temp!=nullptr) {
                if(temp->getisbooked()) {
                    temp->bookroom();
                    cout<<"Room # "<<r<<" Booked successfully"<<endl;
                }
                else {
                    cout<<"Room # "<<r<<" already booked"<<endl;
                }
            }
            else {
                cout<<"Room not found";
            }
        }
};

class Booking{};

class ServiceRequest{};

class Payment{};

class Review{};

class Membership{};



class User{};

class Admin{};

int main() {
    Hotel hotel;

    hotel.addroom(new StandardRoom(100));
    hotel.addroom(new DeluxorRoom(200));
    hotel.addroom(new SuitedRoom(300));
    hotel.addroom(new StandardRoom(101));

    int choice;

    do {
        cout<<"---------Main Menu-------\n";
        cout<<"1. Customer Login\n";
        cout<<"2. Admin Login\n";
        cout<<"3. Exit\n";
        cout<<"Enter your choice: ";
        cin>>choice;

        if(choice == 1) {
            do {
                cout<<"------------------Customer Menu------------------\n";
                cout<<"1. View Available Rooms\n";
                cout<<"2. Book Room\n";
                cout<<"3. Exit\n";
                cout<<"Enter your choice: ";
                cin>>choice;
                switch(choice) {
                    case 1:
                        hotel.viewavailability();
                    break;
                    case 2:
                        int rnum;
                    cout<<"Enter the number of rooms you want to book: ";
                    cin>>rnum;
                    hotel.Bookroom(rnum);
                    break;
                    case 3:
                        cout<<"Thank you for Visiting!";
                    break;
                    default:
                        cout<<"Invalid Choice";
                }
            }while(choice!=3);
        }
        else if(choice == 2) {
            string pass;
            cout<<"Enter the password: ";
            cin>>pass;
            if(pass=="admin") {
                int achoice;
                do {
                    cout<<"------Admin Menu-------\n";
                    cout<<"1. Add New Room\n";
                    cout<<"2. View Available Rooms\n";
                    cout<<"3. Logout\n";
                    cout<<"Enter your choice: ";
                    cin>>achoice;
                    switch(achoice) {
                        case 1:
                            
                    }

                }
            }
        }
    }
    return 0;
}