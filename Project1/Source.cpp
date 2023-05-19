#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <conio.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <list>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std;

// EXIT, RETURN de thoat hoac quay lai
const int EXIT = 27;
const int RETURN = 13;

// Lay vi tri cua dau nhay
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

// Ham cho
void waiting(unsigned int mseconds);

// Ham tao mau cho background va chu
void setColor(int background_color, int text_color);

// Ham hien thi dau * khi nhap PIN/Password
string password(unsigned maxLength);

// Ham di chuyen toi vi tri (x, y)
void gotoXY(int x, int y);

// Ham de thuc hien chuc nang quay lai hoac thoat chuong trinh
bool pressESCorENTER(int x, int y);

// Ham hien thi menu chinh cua chuong trinh
int showMainMenu();

// Ham hien thi menu cua admin
int showMenuAdmin();

// Ham tao dinh dang ban dau cho chuong trinh
void format();

class Admin {
protected:
	string user, password;
public:
	// Khoi tao va huy
	Admin(string _user = "", string _password = "") {
		user = _user;
		password = _password;
	}
	Admin(const Admin& admin) {
		user = admin.user;
		password = admin.password;
	}
	~Admin() {}

	// Qua tai nhap, xuat
	friend istream& operator>> (istream& is, Admin& admin) {
		is >> admin.user >> admin.password;
		return is;
	}
	friend ostream& operator<< (ostream& os, Admin admin) {
		os << "User:\t" << admin.user << endl;
		os << "Pass:\t" << admin.password << endl;
		return os;
	}
	// Ham tai danh sach tai khoan admin de dang nhap vao quan ly
	friend void loadAdmin(list<Admin>& admin) {
		ifstream in("Admin.txt");
		if (in.is_open()) {
			Admin temp;
			while (in >> temp) {
				admin.push_back(temp);
			}
			in.close();
		}
		else {
			system("Color 04");
			cout << setw(70) << right << "HE THONG DANG BAO TRI, VUI LONG QUAY LAI SAU!";
		}
	}
	// Ham hien thi danh sach tai khoan admin
	friend void showAdmin(list<Admin> admin) {
		for (list<Admin>::iterator i = admin.begin(); i != admin.end(); i++)
			cout << *i << endl;
	}

	// Ham kiem tra tai khoan admin
	bool checkAdmin(string _user, string _pass) {
		return user == _user && password == _pass;
	}
};

class Ticket {
protected:
	string ticketCode, flightCode, id, name;
	int number, flag;
public:
	// Khoi tao va huy
	Ticket(string _ticketCode = "", string _flightCode = "", int _number = 0, string _id = "", string _name = "", int _flag = 0) {
		ticketCode = _ticketCode;
		flightCode = _flightCode;
		id = _id;
		name = _name;
		number = _number;
		flag = _flag;
	}
	Ticket(const Ticket& ticket) {
		ticketCode = ticket.ticketCode;
		flightCode = ticket.flightCode;
		id = ticket.id;
		name = ticket.name;
		number = ticket.number;
		flag = ticket.flag;
	}
	~Ticket() {}

	// Get
	string getTicketCode() { return ticketCode; }
	string getFlightcode() { return flightCode; }
	string getID() { return id; }
	string getName() { return name; }
	int getNumber() { return number; }
	int getFlag() {
		return flag;
	}
	// Set
	void setFlag(int x) {
		flag = x;
	}

	// Qua tai nhap, xuat
	friend istream& operator>> (istream& is, Ticket& ticket) {
		is >> ticket.ticketCode >> ticket.flightCode;
		is >> ticket.id;
		is.ignore();
		getline(is, ticket.name);
		is >> ticket.number;
		string file = ticket.ticketCode + ".txt";
		ofstream os(file);
		os << ticket.ticketCode << "     " << ticket.flightCode << "     " << ticket.id << "     ";
		os << ticket.name << "     " << ticket.number;
		os.close();
		return is;
	}
	friend ostream& operator<< (ostream& os, Ticket ticket) {
		os << ticket.ticketCode << "\t" << ticket.flightCode << endl;
		os << ticket.id << endl;
		os << ticket.name << endl;
		os << ticket.number << endl;
		string file = ticket.ticketCode + ".txt";
		ofstream out(file);
		out << ticket.ticketCode << "     " << ticket.flightCode << "     " << ticket.id << "     ";
		out << ticket.name << "     " << ticket.number;
		out.close();
		return os;
	}
};

class Customer {
protected:
	string id, name, ticketCode, flightCode;
	int number, flag;
public:
	// Khoi tao va huy
	Customer(string _ticketCode = "", string _flightCode = "", int _number = 0, string _id = "", string _name = "", int _flag = 0) {
		id = _id;
		name = _name;
		ticketCode = _ticketCode;
		flightCode = _flightCode;
		number = _number;
		flag = _flag;
	}
	Customer(const Customer& cus) {
		id = cus.id;
		name = cus.name;
		ticketCode = cus.ticketCode;
		flightCode = cus.flightCode;
		number = cus.number;
		flag = cus.flag;
	}
	~Customer() {}

	// Get
	string getTicketCode() {
		return ticketCode;
	}
	string getFlightCode() {
		return flightCode;
	}
	string getID() {
		return id;
	}
	string getName() {
		return name;
	}
	int getNumber() {
		return number;
	}
	int getFlag() {
		return flag;
	}
	// Set
	void setFlag(int x) {
		flag = x;
	}

	// Qua tai nhap, xuat
	friend istream& operator>> (istream& is, Customer& cus) {
		is >> cus.ticketCode >> cus.flightCode;
		is >> cus.number >> cus.id;
		is.ignore();
		getline(is, cus.name);
		
		return is;
	}
	bool operator== (Customer cus) {
		return ticketCode == cus.ticketCode && flightCode == cus.flightCode
			&& number == cus.number && id == cus.id
			&& name == cus.name;
	}

	// Ham tai len danh sach cac khach hang tu file txt
	friend void loadCustomer(list<Customer>& cus) {
		ifstream in("KhachHang.txt");
		cus.clear();
		if (in.is_open()) {
			Customer temp;
			while (in >> temp) {
				cus.push_back(temp);
			}
			in.close();
		}
		else {
			system("Color 04");
			cout << setw(70) << right << "HE THONG DANG BAO TRI, VUI LONG QUAY LAI SAU!";
		}
	}
	// Ham tai len file txt danh sach khach hang tu list
	friend void updateCustomer(list<Customer> cus) {
		ofstream out("KhachHang.txt");
		for (list<Customer>::iterator i = cus.begin(); i != cus.end(); i++) {
			out << i->ticketCode << "\t" << i->flightCode << endl;
			out << i->number << "\t" << i->id << endl;
			out << i->name << endl << endl;
		}
		out.close();
	}

	// Ham hien thi khach hang theo form
	friend void showCustomer(Customer cus, int x) {
			setColor(0, 15); 
			gotoXY(1, x); cout << cus.ticketCode;
			gotoXY(15, x); cout << cus.flightCode;
			gotoXY(30, x); cout <<cus.id;
			gotoXY(54, x); cout << cus.name;
			gotoXY(89, x); cout << cus.number;
			x++;
	}

	// Ham hien thi danh sach cac khach hang theo form
	friend void showAllCustomer(list<Customer> cus, int x) {
		for (list<Customer>::iterator i = cus.begin(); i != cus.end(); i++) {
			showCustomer(*i, x);
			gotoXY(100, x); cout << "    NO    ";
			gotoXY(114, x); cout << "   YES   ";
			x++;
		}
	}
};

class Plane {
protected:
	string aircraftNumber;
	int numberOfSeats;
public:
	// Khoi tao va huy
	Plane(string _aircraftNumber = "", int _numberOfSeats = 0) {
		aircraftNumber = _aircraftNumber;
		numberOfSeats = _numberOfSeats;
	}
	Plane(const Plane& plane) {
		aircraftNumber = plane.aircraftNumber;
		numberOfSeats = plane.numberOfSeats;
	}
	~Plane() {}

	// Qua tai nhap, xuat
	friend istream& operator>> (istream& is, Plane& plane) {
		is >> plane.aircraftNumber >> plane.numberOfSeats;
		return is;
	}
	friend ostream& operator<< (ostream& os, Plane plane) {
		os << "So hieu may bay:\t" << plane.aircraftNumber << endl;
		os << "So cho:\t" << plane.numberOfSeats << endl;
		return os;
	}
	
	// Ham tai len danh sach cac may bay
	friend void loadPlane(list<Plane>& plane) {
		ifstream in("MayBay.txt");
		if (in.is_open()) {
			Plane temp;
			while (in >> temp) {
				plane.push_back(temp);
			}
			in.close();
		}
		else {
			system("Color 04");
			cout << setw(70) << right << "HE THONG DANG BAO TRI, VUI LONG QUAY LAI SAU!";
		}
	}
	// Ham hien thi danh sach cac may bay
	friend void showPlane(list<Plane> plane) {
		for (list<Plane>::iterator i = plane.begin(); i != plane.end(); i++)
			cout << *i << endl;
	}
};

class Flight {
protected:
	string flightCode, planeSign, departureDay, airportLocation;
	int status;
	list<Ticket> tickets;
	list<int> seats;
public:
	// Khoi tao va huy
	Flight() {
		flightCode = planeSign = departureDay = airportLocation = "";
		status = -1;
		tickets.clear();
		seats.clear();
	}
	Flight(string _flightCode, string _planeSign, string _departureDay, string _airportLocation, int _status) {
		flightCode = _flightCode;
		planeSign = _planeSign;
		departureDay = _departureDay;
		airportLocation = _airportLocation;
		status = _status;
	}
	Flight(const Flight& flight) {
		flightCode = flight.flightCode;
		planeSign = flight.planeSign;
		departureDay = flight.departureDay;
		airportLocation = flight.airportLocation;
		status = flight.status;
		tickets = flight.tickets;
		seats = flight.seats;
	}
	~Flight() {}

	// Get
	int getTicketsNumber() {
		return tickets.size() + 1;
	}
	string getFlightCode() {
		return flightCode;
	}
	string getDepartureDay() {
		return departureDay;
	}
	string getAircarftNumber() {
		return planeSign;
	}
	int getStatus() {
		return status;
	}

	// Qua tai 
	friend istream& operator>> (istream& is, Flight& flight) {
		flight.tickets.clear();
		flight.seats.clear();
		is >> flight.flightCode >> flight.planeSign;
		is >> flight.departureDay;
		is.ignore();
		getline(is, flight.airportLocation);
		is >> flight.status;
		int n;
		is >> n;
		Ticket temp;
		for (int i = 0; i < n; i++) {
			is >> temp;
			flight.tickets.push_back(temp);
		}
		is >> n;
		int tmp;
		for (int i = 0; i < n; i++) {
			is >> tmp;
			flight.seats.push_back(tmp);
		}
		return is;
	}

	// Ham xuat thong tin chuyen bay
	void outputFlight(int x) {
		gotoXY(2, x); cout << flightCode;
		gotoXY(16, x); cout << departureDay;
		gotoXY(31, x); cout << airportLocation;
		gotoXY(64, x); 
		if (status == 0)
			setColor(0, 12);
		else if (status == 1)
			setColor(0, 10);
		else if (status == 2)
			setColor(0, 8);
		else
			setColor(0, 9);
		cout << status;
		int y = x;
		setColor(0, 15);

		for (list<Ticket>::iterator i = tickets.begin(); i != tickets.end(); i++) {
			gotoXY(71, y++);
			cout << i->getTicketCode() << "; ";
			cout << i->getFlightcode() << "; ";
			cout << i->getID() << "; ";
			cout << i->getName() << "; ";
			cout << i->getNumber();
		}
	}
	// Ham tai len danh sach cac chuyen bay
	friend void loadFlight(list<Flight>& flight) {
		ifstream in("ChuyenBay.txt");
		if (in.is_open()) {
			Flight temp;
			while (in >> temp)
				flight.push_back(temp);
			in.close();
		}
		else {
			system("Color 04");
			cout << setw(70) << right << "HE THONG DANG BAO TRI, VUI LONG QUAY LAI SAU!";
		}
	}
	// Ham tai len file txt danh sach chuyen bay tu list
	friend void updateFlight(list<Flight> flight) {
		ofstream out("ChuyenBay.txt");
		for (list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) {
			out << i->flightCode << "\t" << i->planeSign << endl;
			out << i->departureDay << endl;
			out << i->airportLocation << endl;
			out << i->status << endl;
			out << i->tickets.size() << endl;
			for (list<Ticket>::iterator it = i->tickets.begin(); it != i->tickets.end(); it++) 
				out << *it;
			out << i->seats.size() << endl;
			for (list<int>::iterator it = i->seats.begin(); it != i->seats.end(); it++)
				out << *it << "   ";
			out << endl << endl;
		}
		out.close();
	}
	// Ham hien thi danh sach cac chuyen bay
	friend void showFlight(list<Flight> flight) {
		system("cls");
		setColor(0, 14);
		cout << endl;
		cout << setw(85) << right << "--------------------------------------" << endl;
		cout << setw(49) << right << "||";
		setColor(0, 10);
		cout << setw(24) << right << "    L I S T  O F  F L I G H T S   ";
		setColor(0, 14);
		cout << "||" << endl;
		cout << setw(85) << right << "--------------------------------------" << endl;

		// Tao bang thong tin cho danh sach cac chuyen bay
		int x = 5; // x la bien vi tri de tien cho viec neu co cap nhat vi tri
		gotoXY(2, x); cout << "FLIGHTCODE";
		gotoXY(15, x); cout << "DEPARTURE DAY";
		gotoXY(37, x); cout << "AIRPORT LOCATION";
		gotoXY(62, x); cout << "STATUS";
		gotoXY(88, x); cout << "LIST OF TICKETS";
		gotoXY(13, x); cout << "|";
		gotoXY(29, x); cout << "|";
		gotoXY(60, x); cout << "|";
		gotoXY(69, x); cout << "|";
		gotoXY(1, x + 1); cout << "------------" << char(193) <<  "---------------" << char(193) << "------------------------------" << char(193) << "--------" << char(193) << "---------------------------------------------------------";
		
		int pos = x + 2;
		for (list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) {
			setColor(0, 15);
			i->outputFlight(pos);
			if (i->tickets.size() == 0)
				pos += 2;
			else
				pos += i->tickets.size() + 1;
		}

		cout << "\n\n\n\n\n\n\n\n\n\n";
	}
	// Ham hien thi danh sach ghe trong
	int showSeats(int x) {
		setColor(0, 15); gotoXY(74, x);
		if (seats.size() == 0) {
			gotoXY(74, x); cout << "No seat";
			return -1;
		}
		int n = 0;
		string number;
		for (unsigned int i = 0; i < seats.size(); i++) {
			list<int>::iterator it = seats.begin();
			advance(it, i);
			if (n > 4) {
				n = 0;
				x++;
			}
			if (n == 0) {
				gotoXY(70, x); cout << *it;
				n++;
				continue;
			}
			if (n == 1) {
				gotoXY(75, x); cout << *it;
				n++;
				continue;
			}
			if (n == 2) {
				gotoXY(80, x); cout << *it;
				n++;
				continue;
			}
			if (n == 3) {
				gotoXY(85, x); cout << *it;
				n++;
				continue;
			}
			if (n == 4) {
				gotoXY(90, x); cout << *it;
				n++;
				continue;
			}
		}
		tryagain3:
		setColor(0, 14); gotoXY(39, x + 2); cout << "Enter seat";
		gotoXY(61, x + 2); cout << ":";
		setColor(0, 15); gotoXY(74, x + 2); getline(cin, number);
		if (number == "" || number == " ") {
			setColor(0, 12); gotoXY(48, 25); cout << "DO NOT LEAVE BLANKS OR ENTER SPACES!";
			goto tryagain3;
		}
		n = 0;
		for (unsigned int i = 0; i < seats.size(); i++) {
			list<int>::iterator it = seats.begin();
			advance(it, i);
			if (stoi(number) == *it) {
				n = i;
				return *it;
			}
		}
		setColor(0, 12); gotoXY(51, 23); cout << "INVALID SEATS, TRY AGAIN!";
		gotoXY(74, x + 2); cout << "                   ";
		goto tryagain3;
	}
	// Ham hien thi danh sach chuyen bay chua hoan tat
	friend void showUnfinishFlight(list<Flight> flight, int x, int z) {
		for (list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) {
			if (i->status != 0 && i->status != 3) {
				for (list<Ticket>::iterator it = i->tickets.begin(); it != i->tickets.end(); it++) {
					setColor(0, 15);
					gotoXY(1, x); cout << it->getTicketCode();
					gotoXY(15, x); cout << it->getFlightcode();
					gotoXY(31, x); cout << i->departureDay;
					gotoXY(46, x); cout << it->getName();
					if (i->status == 0)
						setColor(0, 12);
					else if (i->status == 1)
						setColor(0, 10);
					else if (i->status == 2)
						setColor(0, 8);
					else
						setColor(0, 9);
					gotoXY(83, x); cout << i->status;
					setColor(0, 15);
					gotoXY(98, x); cout << "    NO    ";
					gotoXY(108, x); cout <<"    YES    ";
					x++;
				}
			}
		}
		x = 7;
		for (list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) {
			if (i->status != 0 && i->status != 3) {
				for (list<Ticket>::iterator it = i->tickets.begin(); it != i->tickets.end(); it++) {
					int choose = 0, y = 0;
					while (true) {
						if (choose == 0) {
							setColor(3, 0); gotoXY(98, x); cout << "    NO    ";
						}
						else {
							setColor(0, 15); gotoXY(98, x); cout << "    NO    ";
						}
						if (choose == 1) {
							setColor(3, 0); gotoXY(108, x); cout << "    YES    ";
						}
						else {
							setColor(0, 15); gotoXY(108, x); cout << "    YES    ";
						}
						setColor(0, 0);
						setColor(0, 8);
						gotoXY(41, z); cout << "Using arrow keys & press [ENTER] to select\n";
						system("pause>nut");
						if (GetAsyncKeyState(VK_RIGHT) && y != 1) {
							y++;
							choose++;
							continue;
						}
						if (GetAsyncKeyState(VK_LEFT) && y != 0) {
							y--;
							choose--;
							continue;
						}
						if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
							break;
						}
					}
					if (choose == 1) {
						i->seats.push_back(it->getNumber());
						it->setFlag(1);
					}
					x++;
				}
			}
		}
		for(list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) {
			list<Ticket> l;
			for (list<Ticket>::iterator it = i->tickets.begin(); it != i->tickets.end(); it++) {
				if (it->getFlag() == 0) {
					l.push_back(*it);
				}
			}
			i->seats.sort();
			i->tickets.clear();
			i->tickets = l;
		}
		updateFlight(flight);
	}

	// Ham them ve vao danh sach ve cua chuyen bay
	Flight addTicket(Ticket ticket) {
		this->tickets.push_back(ticket);
		this->seats.remove(ticket.getNumber());
		return *this;
	}
	// Ham thong ke 
	void statistical(int &x) {
		x = 7;
		// Hien thi danh sach khach hang - ve cua mot chuyen bay
		setColor(0, 14); gotoXY(37, x); cout << "List of passenger";
		gotoXY(64, x); cout << ":";
		int n = 1;
		setColor(0, 15);
		if (tickets.size() == 0) {
			gotoXY(70, x); cout << "Empty!";
			x++;
		}
		else {
			for (list<Ticket>::iterator i = tickets.begin(); i != tickets.end(); i++) {
				gotoXY(68, x); cout << n++ << ". ";
				gotoXY(70, x); cout << "Ticketcode"; gotoXY(85, x); cout << ":"; gotoXY(90, x); cout << i->getTicketCode(); x++;
				gotoXY(70, x); cout << "Flightcode"; gotoXY(85, x); cout << ":"; gotoXY(90, x); cout << i->getFlightcode(); x++;
				gotoXY(70, x); cout << "Aircarftnumber"; gotoXY(85, x); cout << ":"; gotoXY(90, x); cout << planeSign; x++;
				gotoXY(70, x); cout << "Passenger"; gotoXY(85, x); cout << ":"; gotoXY(90, x); cout << i->getName(); x++;
				gotoXY(70, x); cout << "ID"; gotoXY(85, x); cout << ":"; gotoXY(90, x); cout << i->getID(); x++;
				gotoXY(70, x); cout << "Destination"; gotoXY(85, x); cout << ":"; gotoXY(90, x); cout << airportLocation; x++;
				gotoXY(70, x); cout << "Seat"; gotoXY(85, x); cout << ":"; gotoXY(90, x); cout << i->getNumber(); x++;
				x++;
			}

			// Hien thi danh sach ghe trong cua mot chuyen bay
			n = 0;
			setColor(0, 14); gotoXY(37, x); cout << "Seats of this flight code ";
			gotoXY(64, x); cout << ":";
			setColor(0, 15);
			for (unsigned int i = 0; i < seats.size(); i++) {
				list<int>::iterator it = seats.begin();
				advance(it, i);
				if (n > 4) {
					n = 0;
					x++;
				}
				if (n == 0) {
					gotoXY(70, x); cout << *it;
					n++;
					continue;
				}
				if (n == 1) {
					gotoXY(75, x); cout << *it;
					n++;
					continue;
				}
				if (n == 2) {
					gotoXY(80, x); cout << *it;
					n++;
					continue;
				}
				if (n == 3) {
					gotoXY(85, x); cout << *it;
					n++;
					continue;
				}
				if (n == 4) {
					gotoXY(90, x); cout << *it;
					n++;
					continue;
				}
			}
		}
		x++;
	}
};

// Ham lay tong so luong ve cua tat ca chuyen bay
int getNumberOfTickets(list<Flight> flight);

// Ham dat ve chuyen bay
void bookTickets(list<Flight> flight);

// Ham hien thi man hinh dang nhap admin
int loginAdmin(list<Admin> admin);

// Ham hien thi va xu ly dat ve
void ticketBookingProcess(list<Flight>& flight, list<Customer>& cus);

// Ham hien thi va xu ly tra ve TICKET REFUND PROCESS
void ticketRefundProcess(list<Flight>& flight);

int main()	 {
	format();
	// Tao danh sach va tai len cac danh sach may bay, chuyen bay, khach hàng, admin
	list<Plane> plane;
	loadPlane(plane);

	list<Flight> flight;
	loadFlight(flight);

	list<Customer> customer;
	loadCustomer(customer);

	list<Admin> admin;
	loadAdmin(admin);
	
	
	// Main menu
tryagain: // Tao moc de quay lai
	system("cls");
	switch (showMainMenu())
	{
	// Hien thi danh sach cac chuyen bay
	case 1: {
		showFlight(flight);
		int x = getNumberOfTickets(flight) + 6;
		if (x < 19)
			x = 25;
		else
			x += 6;
		setColor(0, 14);
		gotoXY(1, x - 6); cout << "------------------------------------------------------------------------------------------------------------------------------";
		gotoXY(2, x - 4); 
		setColor(0, 12); cout << "0";
		setColor(0, 15); cout << ": Flight canceled";
		gotoXY(2, x - 3);
		setColor(0, 10); cout << "1";
		setColor(0, 15); cout << ": The flight still has tickets\t";
		gotoXY(2, x - 2);
		setColor(0, 8); cout << "2";
		setColor(0, 15); cout << ": Sold out";
		gotoXY(2, x - 1);
		setColor(0, 9); cout << "3";
		setColor(0, 15); cout << ": Completed";
		if (pressESCorENTER(32, x))
			goto tryagain;
		break;
	}

	// Dat ve may bay
	case 2: {
		bookTickets(flight);
		loadCustomer(customer);
		if (pressESCorENTER(32, 25))
			goto tryagain;
		break;
	}

	// Quan ly ve may bay
	case 3: {
		int check = loginAdmin(admin);
		if (check == 0) {
			setColor(0, 12); gotoXY(44, 25); cout << "                                                              ";
			setColor(0, 0);
			return 0;
		}
		tryagain1:
		switch (showMenuAdmin()) {
		// Xu ly dat ve
		case 1: { 
			int x = customer.size() + 6;
			if (x < 19)
				x = 25;
			else
				x += 6;
			ticketBookingProcess(flight, customer);
			if (pressESCorENTER(32, x))
				goto tryagain1;
			break;
		}

		// Xu ly tra ve
		case 2: {
			int x = getNumberOfTickets(flight) + 6;
			if (x < 19)
				x = 25;
			else
				x += 6;
			ticketRefundProcess(flight);
			flight.clear();
			loadFlight(flight);
			if (pressESCorENTER(32, x))
				goto tryagain1;
			break;
		}

		// Thong ke - STATISTICAL
		case 3: {
			system("cls");
			setColor(0, 14);
			cout << endl;
			cout << setw(85) << right << "--------------------------------------" << endl;
			cout << setw(49) << right << "||";
			setColor(0, 10);
			cout << setw(24) << right << "       S T A T I S T I C A L      ";
			setColor(0, 14);
			cout << "||" << endl;
			cout << setw(85) << right << "--------------------------------------" << endl;

			gotoXY(37, 5); cout << "Enter the flightcode";
			gotoXY(64, 5); cout << ":";

			string flightCode;
			int x;
			setColor(0, 15);  gotoXY(70, 5); getline(cin, flightCode);
			for (list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) {
				if (i->getFlightCode() == flightCode)
					i->statistical(x);
			}

			// Thong ke so luong chuyen bay cua mot may bay
			int count = 0;
			x++;
			string aircarftnumber;
			setColor(0, 14); gotoXY(37, x); cout << "Enter the aircarftnumber";
			gotoXY(64, x); cout << ":";

			setColor(0, 15);  gotoXY(70, x); getline(cin, aircarftnumber);
			for (list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) {
				if (i->getAircarftNumber() == aircarftnumber)
					count++;
			}

			x += 2;
			setColor(0, 14); gotoXY(37, x); cout << "The number of flights";
			x++;
			gotoXY(37, x); cout << "made by an aircraft";
			gotoXY(64, x); cout << ":";
			setColor(0, 15);  gotoXY(70, x); cout << count;
			if (pressESCorENTER(32, x + 3))
				goto tryagain1;
			break;
		}

		// Tro ve main menu
		case 4:
			goto tryagain;
			break;

		// Thoat
		default:
			setColor(0, 11);
			gotoXY(51, 22); cout << "CAM ON VI DA SU DUNG DICH VU!";
			setColor(0, 8);
			gotoXY(36, 24); cout << "               Press [ENTER] to exit               ";
			setColor(0, 0);
			return 0;
		}
		break;
	}

	// Thoat chuong trinh
	case 4: {
		setColor(0, 11);
		gotoXY(47, 22); cout << "CAM ON VI DA SU DUNG DICH VU!";
		setColor(0, 8);
		gotoXY(36, 24); cout << "               Press [ENTER] to exit               ";
		setColor(0, 0);
		return 0;
	}

	default: {
		break;
	}
	}	
	return 0;
}

void ticketBookingProcess(list<Flight>& flight, list<Customer>& cus) {
	system("cls");
	setColor(0, 14);
	gotoXY(37, 1); cout << "--------------------------------------------------" << endl;
	cout << setw(39) << right << "||";
	setColor(0, 10);
	cout << "  T I C K E T  B O O K I N G  P R O C E S S   ";
	setColor(0, 14);
	cout << "||" << endl;
	gotoXY(37, 3); cout << "--------------------------------------------------" << endl;

	int x = 5; // x la bien vi tri de tien cho viec neu co cap nhat vi tri
	gotoXY(2, x); cout << "TICKETCODE";
	gotoXY(16, x); cout << "FLIGHTCODE";
	gotoXY(39, x); cout << "ID";
	gotoXY(63, x); cout << "PASSENGER NAME";
	gotoXY(90, x); cout << "SEAT";
	gotoXY(105, x); cout << "ACCEPT TICKET";
	gotoXY(13, x); cout << "|";
	gotoXY(28, x); cout << "|";
	gotoXY(52, x); cout << "|";
	gotoXY(87, x); cout << "|";
	gotoXY(96, x); cout << "|";
	gotoXY(1, x + 1); cout << "------------" << char(193) << "--------------" << char(193) << "-----------------------" << char(193) << "----------------------------------" << char(193) << "--------" << char(193) << "------------------------------";
	
	x = cus.size() + 6;
	if (x < 19)
		x = 25;
	else
		x += 6;
	int z = x;
	setColor(0, 14);
	gotoXY(1, x - 6); cout << "------------------------------------------------------------------------------------------------------------------------------";
	setColor(0, 8);
	gotoXY(32, x);
	x = 7;
	if (cus.size() != 0) {
		showAllCustomer(cus, x);
		for (list<Customer>::iterator i = cus.begin(); i != cus.end(); i++) {
			int choose = 0, y = 0;
			while (true) {
				if (choose == 0) {
					setColor(3, 0); gotoXY(100, x); cout << "    NO    ";
				}
				else {
					setColor(0, 15); gotoXY(100, x); cout << "    NO    ";
				}
				if (choose == 1) {
					setColor(3, 0); gotoXY(114, x); cout << "   YES   ";
				}
				else {
					setColor(0, 15); gotoXY(114, x); cout << "   YES   ";
				}
				setColor(0, 0);
				setColor(0, 8);
				gotoXY(41, z); cout << "Using arrow keys & press [ENTER] to select\n";
				system("pause>nut");
				if (GetAsyncKeyState(VK_RIGHT) && y != 1) {
					y++;
					choose++;
					continue;
				}
				if (GetAsyncKeyState(VK_LEFT) && y != 0) {
					y--;
					choose--;
					continue;
				}
				if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
					break;
				}
			}

			if (choose == 1) {
				for (list<Flight>::iterator it = flight.begin(); it != flight.end(); it++) {
					if (it->getFlightCode() == i->getFlightCode()) {
						*it = it->addTicket(Ticket(i->getTicketCode(), i->getFlightCode(), i->getNumber(), i->getID(), i->getName()));
						i->setFlag(1);
						break;
					}
				}
			}

			x++;
		}
		list<Customer> l;
		for (list<Customer>::iterator i = cus.begin(); i != cus.end(); i++) {
			if (i->getFlag() == 0)
				l.push_back(*i);
		}
		cus.clear();
		cus = l;
		updateCustomer(cus);
		updateFlight(flight);
	}
	
	setColor(0, 0);
}

void ticketRefundProcess(list<Flight>& flight) {
	system("cls");
	setColor(0, 14);
	gotoXY(40, 1); cout << "--------------------------------------------------" << endl;
	cout << setw(42) << right << "||";
	setColor(0, 10);
	cout << "   T I C K E T  R E F U N D  P R O C E S S    ";
	setColor(0, 14);
	cout << "||" << endl;
	gotoXY(40, 3); cout << "--------------------------------------------------" << endl;

	int x = 5; // x la bien vi tri de tien cho viec neu co cap nhat vi tri
	gotoXY(2, x); cout << "TICKETCODE";
	gotoXY(16, x); cout << "FLIGHTCODE";
	gotoXY(30, x); cout << "DEPARTURE DAY";
	gotoXY(55, x); cout << "PASSENGER NAME";
	gotoXY(81, x); cout << "STATUS";
	gotoXY(102, x); cout << "REFUND TICKET";
	gotoXY(13, x); cout << "|";
	gotoXY(28, x); cout << "|";
	gotoXY(44, x); cout << "|";
	gotoXY(79, x); cout << "|";
	gotoXY(88, x); cout << "|";
	gotoXY(1, x + 1); cout << "------------" << char(193) << "--------------" << char(193) << "---------------" << char(193) << "----------------------------------" << char(193) << "--------" << char(193) << "--------------------------------------";

	x = getNumberOfTickets(flight) + 6;
	if (x < 19)
		x = 25;
	else
		x += 6;
	int z = x;
	setColor(0, 14);
	gotoXY(1, x - 6); cout << "------------------------------------------------------------------------------------------------------------------------------";
	gotoXY(2, x - 4);
	setColor(0, 12); cout << "0";
	setColor(0, 15); cout << ": Flight canceled";
	gotoXY(2, x - 3);
	setColor(0, 10); cout << "1";
	setColor(0, 15); cout << ": The flight still has tickets\t";
	gotoXY(2, x - 2);
	setColor(0, 8); cout << "2";
	setColor(0, 15); cout << ": Sold out";
	gotoXY(2, x - 1);
	setColor(0, 9); cout << "3";
	setColor(0, 15); cout << ": Completed";

	if (getNumberOfTickets(flight) != 0) {
		x = 7;
		showUnfinishFlight(flight, x, z);
	}
}

void waiting(unsigned int mseconds) {
	clock_t goal = mseconds + clock();
	while (goal > clock());
}

void setColor(int background_color, int text_color) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	int color_code = background_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

int getNumberOfTickets(list<Flight> flight) {
	int x = 0;
	for (list<Flight>::iterator i = flight.begin(); i != flight.end(); i++) 
		x += i->getTicketsNumber() + 1;
	return x;
}

int loginAdmin(list<Admin> admin) {
	int count = 1;
	tryagain0:
	system("cls");
	setColor(0, 14);
	
	gotoXY(43, 1); cout << "--------------------------------------" << endl;
	cout << setw(45) << right << "||";

	setColor(0, 10);
	cout << setw(26) << right << "       L O G I N  A D M I N       ";
	setColor(0, 14);
	cout << "||" << endl;
	cout << setw(81) << right << "--------------------------------------" << endl << endl;
	setColor(0, 15);
	cout << setw(88) << right << "****************************************************";
	setColor(0, 14);
	gotoXY(43, 7); cout << "--------------------------------------";
	gotoXY(43, 8); cout << "|";
	gotoXY(43, 9); cout << "|";
	gotoXY(80, 8); cout << "|";
	gotoXY(80, 9); cout << "|";

	int x = 15;
	setColor(0, 2); 
	gotoXY(47, 8); cout << "User";
	gotoXY(47, 9); cout << "Password";
	setColor(0, 14);
	gotoXY(58, 8); cout << ":";
	gotoXY(58, 9); cout << ":";
	setColor(0, 14); gotoXY(43, 10); cout << "--------------------------------------";

	setColor(0, 8); gotoXY(47, 24); cout << "Press [ESC] & [ENTER] to exit";

	tryagain1:
	string user, pass;
	setColor(0, 15);
	gotoXY(65, 8); 
	if (GetAsyncKeyState(VK_ESCAPE)) {
		setColor(0, 3); gotoXY(50, 23); cout << "Exit the program........";
		setColor(0, 0);
		return 0;
	}
	getline(cin, user);
	if (user == "" || user == " ") {
		setColor(0, 12); gotoXY(44, 25); cout << "Do not leave blanks or enter spaces!";
		goto tryagain1;
	}
	gotoXY(44, 25); cout << "                                                        ";

	tryagain2:
	gotoXY(65, 9);
	if (GetAsyncKeyState(VK_ESCAPE)) {
		setColor(0, 3); gotoXY(50, 23); cout << "Exit the program........";
		setColor(0, 0);
		return 0;
	}
	pass = password(6);
	if (pass == "" || pass == " ") {
		setColor(0, 12); gotoXY(44, 25); cout << "Do not leave blanks or enter spaces!";
		goto tryagain2;
	}
	gotoXY(44, 25); cout << "                                                        ";
	for (list<Admin>::iterator i = admin.begin(); i != admin.end(); i++) {
		if (i->checkAdmin(user, pass))
			return 1;
	}

	if (count == 3) {
		setColor(0, 12); gotoXY(40, 22); cout << "Enter wrong 3 times, please come back after!";
		setColor(0, 12); gotoXY(41, 23); cout << "                                         ";
		setColor(0, 12); gotoXY(41, 23); cout << "                                         ";
		setColor(0, 3); gotoXY(50, 24); cout << "EXIT THE PROGRAM........";
		setColor(0, 0);
		system("pause");
		exit(0);
	}
	setColor(0, 14); gotoXY(43, 10); cout << "--------------------------------------";
	//setColor(0, 14); gotoXY(43, 10); cout << "______________________________________";
	setColor(0, 12); gotoXY(41, 23); cout << "Invalid user or password, please try again!";
	setColor(0, 0);
	waiting(2500);
	count++;
	goto tryagain0;
}

void bookTickets(list<Flight> flight) {
	system("cls");// xóa màn hình
	setColor(0, 14);
	cout << endl;
	cout << setw(85) << right << "--------------------------------------" << endl;
	cout << setw(49) << right << "||";
	setColor(0, 10);
	cout << setw(24) << right << "      B O O K  T I C K E T S      ";
	setColor(0, 14);
	cout << "||" << endl;
	cout << setw(85) << right << "--------------------------------------" << endl;

	int x = 5, number;
	string ticketCode, name, id, flightCode;

tryagain:
	setColor(0, 14); gotoXY(39, x); cout << "Enter your name";
	gotoXY(61, x); cout << ":";
	setColor(0, 15); gotoXY(74, x); getline(cin, name);
	setColor(0, 12); gotoXY(48, 25); cout << "                                                  ";
	if (name == "" || name == " ") {
		setColor(0, 12); gotoXY(48, 25); cout << "DO NOT LEAVE BLANKS OR ENTER SPACES!";
		goto tryagain;
	}

tryagain1:
	setColor(0, 14);  gotoXY(39, x + 2); cout << "Enter your ID";
	gotoXY(61, x + 2); cout << ":";
	setColor(0, 15); ;  gotoXY(74, x + 2); getline(cin, id);
	setColor(0, 12); gotoXY(48, 25); cout << "                                                  ";
	if (id == "" || id == " ") {
		setColor(0, 12); gotoXY(48, 25); cout << "DO NOT LEAVE BLANKS OR ENTER SPACES!";
		goto tryagain1;
	}

	setColor(0, 14);  gotoXY(39, x + 4); cout << "Choose your gender";
	gotoXY(61, x + 4); cout << ":";
	int gender_item = 0, y = 0;
	while (true) {
		if (gender_item == 0) {
			setColor(3, 0); gotoXY(65, x + 4); cout << "    Male    ";
		}
		else {
			setColor(0, 15); gotoXY(65, x + 4); cout << "    Male    ";
		}
		if (gender_item == 1) {
			setColor(3, 0);  gotoXY(78, x + 4); cout << "   Female   ";
		}
		else {
			setColor(0, 15);  gotoXY(78, x + 4); cout << "   Female   ";
		}
		if (gender_item == 2) {
			setColor(3, 0); gotoXY(91, x + 4); cout << "   Other   " << endl;
		}
		else {
			setColor(0, 15);  gotoXY(91, x + 4); cout << "   Other   " << endl;
		}
		setColor(0, 0);
		system("pause>nut");
		if (GetAsyncKeyState(VK_RIGHT) && y != 2) {
			y++;
			gender_item++;
			continue;
		}
		if (GetAsyncKeyState(VK_LEFT) && y != 0) {
			y--;
			gender_item--;
			continue;
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			break;
		}
	}

	setColor(0, 14); gotoXY(39, x + 6); cout << "Flights are available";
	gotoXY(61, x + 6); cout << ":";
	setColor(0, 15);
	if (flight.size() == 0) {
		x += 6;
		gotoXY(70, x); cout << "No flight";
		setColor(0, 12); gotoXY(39, 24); cout << "THERE ARE CURRENTLY NO FLIGHTS, PLEASE COME BACK LATER";
	}
	else {
		int n = 0;
		x += 6;
		for (unsigned int i = 0; i < flight.size(); i++) {
			list<Flight>::iterator it = flight.begin(); 
			advance(it,i);
			if (it->getStatus() != 3) {
				if (n > 2) {
					n = 0; x++;
				}
				if (n == 0) {
					gotoXY(70, x); cout << it->getFlightCode(); n++;
					continue;
				}
				if (n == 1) {
					gotoXY(80, x); cout << it->getFlightCode(); n++;
					continue;
				}
				if (n == 2) {
					gotoXY(90, x); cout << it->getFlightCode(); n++;
					continue;
				}
			}
		}
		x += 2;
		tryagain2:
			setColor(0, 14); gotoXY(39, x); cout << "Enter flightcode";
			gotoXY(61, x); cout << ":";
			setColor(0, 15); gotoXY(74, x); getline(cin, flightCode);
			if (flightCode == "" || flightCode == " ") {
				setColor(0, 12); gotoXY(48, 25); cout << "DO NOT LEAVE BLANKS OR ENTER SPACES!";
				goto tryagain2;
			}
			n = 0;
			bool flag = 0;
			for (unsigned int i = 0; i < flight.size(); i++) {
				list<Flight>::iterator it = flight.begin();
				advance(it, i);
				if (flightCode == it->getFlightCode()) {
					n = i;
					flag = 1;
				}
			}
			if (flag == 0) {
				setColor(0, 12); gotoXY(51, 23); cout << "INVALID FLIGHTCODE, TRY AGAIN!";
				gotoXY(74, x); cout << "                   ";
				goto tryagain2;
			}
			setColor(0, 12); gotoXY(51, 23); cout << "                                    ";

			list<Flight>::iterator it = flight.begin();
			advance(it, n);
			setColor(0, 14); gotoXY(39, x + 2); cout << "Seats are available";
			gotoXY(61, x + 2); cout << ":";
			number = it->showSeats(x + 2);
			if (number == -1) {
				setColor(0, 12); gotoXY(39, 24); cout << "THERE ARE CURRENTLY NO SEATS, PLEASE COME BACK LATER";
			}
			else {
				system("cls");
				setColor(0, 14);
				cout << endl;
				cout << setw(85) << right << "--------------------------------------" << endl;
				cout << setw(49) << right << "||";
				setColor(0, 10);
				cout << setw(29) << right << "      Y O U R  T I C K E T S      ";
				setColor(0, 14);
				cout << "||" << endl;
				cout << setw(85) << right << "--------------------------------------" << endl;

				x = 6;
				setColor(0, 14); gotoXY(44, x); cout << "Ticketcode";
				gotoXY(61, x); cout << ":";
				ticketCode = flightCode + "-" + to_string(number);
				setColor(0, 15); gotoXY(74, x); cout << ticketCode;
				x++;
				setColor(0, 14); gotoXY(44, x); cout << "Flightcode";
				gotoXY(61, x); cout << ":";
				setColor(0, 15); gotoXY(74, x); cout << flightCode;
				x++;
				setColor(0, 14); gotoXY(44, x); cout << "Name";
				gotoXY(61, x); cout << ":";
				setColor(0, 15); gotoXY(74, x); cout << name;
				x++;
				setColor(0, 14); gotoXY(44, x); cout << "ID";
				gotoXY(61, x); cout << ":";
				setColor(0, 15); gotoXY(74, x); cout << id;
				x++;
				setColor(0, 14); gotoXY(44, x); cout << "Seat";
				gotoXY(61, x); cout << ":";
				setColor(0, 15); gotoXY(74, x); cout << number;

				n = 0;
				y = 0;
				while (true) {
					if (n == 0) {
						setColor(3, 0); gotoXY(50, x + 4); cout << "    CANCEL    ";
					}
					else {
						setColor(0, 15); gotoXY(50, x + 4); cout << "    CANCEL    ";
					}
					if (n == 1) {
						setColor(3, 0);  gotoXY(69, x + 4); cout << "   ACCEPT   ";
					}
					else {
						setColor(0, 15);  gotoXY(69, x + 4); cout << "   ACCEPT   ";
					}
					setColor(0, 0);
					system("pause>nut");
					if (GetAsyncKeyState(VK_RIGHT) && y != 1) {
						y++;
						n++;
						continue;
					}
					if (GetAsyncKeyState(VK_LEFT) && y != 0) {
						y--;
						n--;
						continue;
					}
					if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
						break;
					}
				}
				if (n == 0) 
					return;
				else {

					gotoXY(0, 17);
					setColor(0, 14); cout << setw(85) << right << "--------------------------------------" << endl;
					setColor(0, 14); gotoXY(51, 18); cout << "YOUR TICKET IS BEING PROCESSED";
					ofstream out("KhachHang.txt", ios::out | ios::app);
					out << ticketCode << '\t' << flightCode << endl;
					out << number << '\t' << id << endl;
					out << name << endl << endl;
					out.close();
					return;

				}
			}
	}

}

void gotoXY(int x, int y) {
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

int showMenuAdmin() {
	// Hien thi logo
	system("cls");
	setColor(0, 14);
	cout << setw(85) << right << "______________________________________";
	gotoXY(47, 2); cout << "|*";
	gotoXY(83, 2); cout << "*|";
	gotoXY(47, 3); cout << "|*";
	gotoXY(83, 3); cout << "*|";
	gotoXY(47, 4); cout << "|*";
	gotoXY(83, 4); cout << "*|";
	gotoXY(47, 5); cout << "|*";
	gotoXY(83, 5); cout << "*|";
	gotoXY(47, 6); cout << "|*";
	gotoXY(83, 6); cout << "*|";

	setColor(0, 15);
	gotoXY(0, 1); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 2); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 3); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 4); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 5); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 6); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(84, 1); cout << "  \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 2); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 3); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 4); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 5); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 6); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";

	setColor(0, 15);
	gotoXY(55, 2); cout << "\xB2\xB2\xB2\xB2\xB2\xB2\xB2  \xB2\xB2\xB2\xB2     \xB2\xB2\xB2\xB2";
	gotoXY(55, 3); cout << "\xB2\xB2       \xB2\xB2 \xB2\xB2   \xB2\xB2 \xB2\xB2";
	gotoXY(55, 4); cout << "\xB2\xB2\xB2\xB2\xB2\xB2   \xB2\xB2  \xB2\xB2 \xB2\xB2  \xB2\xB2";
	gotoXY(55, 5); cout << "\xB2\xB2       \xB2\xB2   \xB2\xB2\xB2   \xB2\xB2";
	gotoXY(55, 6); cout << "\xB2\xB2       \xB2\xB2         \xB2\xB2" << endl;
	setColor(0, 14);
	cout << setw(85) << right << "_____WELCOME TO FLIGHT MANAGEMENT_____" << endl << endl << endl;
	cout << setw(85) << right << "--------------------------------------" << endl;
	cout << setw(49) << right << "||";

	// Hien thi menu chinh
	setColor(0, 10);
	cout << setw(26) << right << "        A D M I N  M E N U        ";
	setColor(0, 14);
	cout << "||" << endl;
	cout << setw(85) << right << "--------------------------------------" << endl;
	cout << setw(85) << right << "______________________________________";
	gotoXY(47, 15); cout << "|";
	gotoXY(47, 16); cout << "|";
	gotoXY(47, 17); cout << "|";
	gotoXY(47, 18); cout << "|";
	gotoXY(47, 19); cout << "|";
	gotoXY(84, 15); cout << "|";
	gotoXY(84, 16); cout << "|";
	gotoXY(84, 17); cout << "|";
	gotoXY(84, 18); cout << "|";
	gotoXY(84, 19); cout << "|" << endl;
	cout << setw(85) << right << "______________________________________";

	int menu_item = 0, x = 1;

	// Vong long den khi thoat hoac nhan vao cac chuc nang
	while (true) {
		// Xu ly dat ve
		if (menu_item == 0) {
			gotoXY(51, 15);
			setColor(3, 0);
			cout << " 1. TICKET BOOKING PROCESS   ";
		}
		else {
			gotoXY(51, 15);
			setColor(0, 14);
			cout << " 1. TICKET BOOKING PROCESS   ";
		}
		// Xu ly tra ve
		if (menu_item == 1) {
			gotoXY(51, 16);
			setColor(3, 0);
			cout << " 2. TICKET REFUND PROCESS    ";
		}
		else {
			gotoXY(51, 16);
			setColor(0, 14);
			cout << " 2. TICKET REFUND PROCESS    ";
		}
		// Thong ke
		if (menu_item == 2) {
			gotoXY(51, 17);
			setColor(3, 0);
			cout << " 3. STATISTICAL              ";
		}
		else {
			gotoXY(51, 17);
			setColor(0, 14);
			cout << " 3. STATISTICAL              ";
		}
		// Tro ve main menu
		if (menu_item == 3) {
			gotoXY(51, 18);
			setColor(3, 0);
			cout << " 4. MAIN MENU                ";
		}
		else {
			gotoXY(51, 18);
			setColor(0, 14);
			cout << " 4. MAIN MENU                ";
		}
		// Thoat
		if (menu_item == 4) {
			gotoXY(51, 19);
			setColor(3, 0);
			cout << " 5. EXIT                     ";
		}
		else {
			gotoXY(51, 19);
			setColor(0, 14);
			cout << " 5. EXIT                     ";
		}
		setColor(0, 8);
		gotoXY(45, 24); cout << "Using arrow keys & press [ENTER] to select\n";
		system("pause>nut");

		// Phim mui ten xuong
		if (GetAsyncKeyState(VK_DOWN) && x != 5) {
			x++;
			menu_item++;
			continue;
		}
		//Phim mui ten len
		if (GetAsyncKeyState(VK_UP) && x != 1) {
			x--;
			menu_item--;
			continue;
		}
		// Phim ENTER
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			return menu_item + 1;
	}
	return 0;
}

int showMainMenu() {
	// Hien thi logo
	system("cls");
	setColor(0, 14);
	cout << setw(85) << right << "______________________________________";
	gotoXY(47, 2); cout << "|*";
	gotoXY(83, 2); cout << "*|";
	gotoXY(47, 3); cout << "|*";
	gotoXY(83, 3); cout << "*|";
	gotoXY(47, 4); cout << "|*";
	gotoXY(83, 4); cout << "*|";
	gotoXY(47, 5); cout << "|*";
	gotoXY(83, 5); cout << "*|";
	gotoXY(47, 6); cout << "|*";
	gotoXY(83, 6); cout << "*|";

	setColor(0, 15);
	gotoXY(0, 1); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 2); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 3); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 4); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 5); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(0, 6); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(84, 1); cout << "  \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 2); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 3); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 4); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 5); cout << " \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";
	gotoXY(85, 6); cout << "\xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2 \xB2";

	setColor(0, 15);
	gotoXY(55, 2); cout << "\xB2\xB2\xB2\xB2\xB2\xB2\xB2  \xB2\xB2\xB2\xB2     \xB2\xB2\xB2\xB2";
	gotoXY(55, 3); cout << "\xB2\xB2       \xB2\xB2 \xB2\xB2   \xB2\xB2 \xB2\xB2";
	gotoXY(55, 4); cout << "\xB2\xB2\xB2\xB2\xB2\xB2   \xB2\xB2  \xB2\xB2 \xB2\xB2  \xB2\xB2";
	gotoXY(55, 5); cout << "\xB2\xB2       \xB2\xB2   \xB2\xB2\xB2   \xB2\xB2";
	gotoXY(55, 6); cout << "\xB2\xB2       \xB2\xB2         \xB2\xB2" << endl;
	setColor(0, 14);
	cout << setw(85) << right << "_____WELCOME TO FLIGHT MANAGEMENT_____" << endl << endl << endl;
	cout << setw(85) << right << "--------------------------------------" << endl;
	cout << setw(49) << right << "||";

	// Hien thi menu chinh
	setColor(0, 10);
	cout << setw(31) << right << "         M A I N  M E N U         ";
	setColor(0, 14);
	cout << "||" << endl;
	cout << setw(85) << right << "--------------------------------------" << endl;
	cout << setw(85) << right << "______________________________________";
	gotoXY(47, 15); cout << "|";
	gotoXY(47, 16); cout << "|";
	gotoXY(47, 17); cout << "|";
	gotoXY(47, 18); cout << "|";
	gotoXY(84, 15); cout << "|";
	gotoXY(84, 16); cout << "|";
	gotoXY(84, 17); cout << "|";
	gotoXY(84, 18); cout << "|" << endl;
	cout << setw(85) << right << "______________________________________";

	int menu_item = 0, x = 15;

	// Vong long den khi thoat hoac nhan vao cac chuc nang
	while (true) {
		if (menu_item == 0) {
			gotoXY(51, 15);
			setColor(3, 0);
			cout << " 1. LIST OF FLIGHTS          ";
		}
		else {
			gotoXY(51, 15);
			setColor(0, 14);
			cout << " 1. LIST OF FLIGHTS          ";
		}
		if (menu_item == 1) {
			gotoXY(51, 16);
			setColor(3, 0);
			cout << " 2. BOOK TICKETS             ";
		}
		else {
			gotoXY(51, 16);
			setColor(0, 14);
			cout << " 2. BOOK TICKETS             ";
		}
		if (menu_item == 2) {
			gotoXY(51, 17);
			setColor(3, 0);
			cout << " 3. TICKET MANAGEMENT        ";
		}
		else {
			gotoXY(51, 17);
			setColor(0, 14);
			cout << " 3. TICKET MANAGEMENT        ";
		}
		if (menu_item == 3) {
			gotoXY(51, 18);
			setColor(3, 0);
			cout << " 4. EXIT                     ";
		}
		else {
			gotoXY(51, 18);
			setColor(0, 14);
			cout << " 4. EXIT                     ";
		}
		setColor(0, 8);
		gotoXY(45, 24); cout << "Using arrow keys & press [ENTER] to select\n";
		system("pause>nut");

		// Phim mui ten xuong
		if (GetAsyncKeyState(VK_DOWN) && x != 18) {
			x++;
			menu_item++;
			continue;
		}
		//Phim mui ten len
		if (GetAsyncKeyState(VK_UP) && x != 15) {
			x--;
			menu_item--;
			continue;
		}
		// Phim ENTER
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			return menu_item + 1;
	}
}

bool pressESCorENTER(int x, int y) {
	setColor(0, 8);
	gotoXY(x + 5, y);
	cout << "Press [ESC] & [ENTER] to exit or [ENTER] to return menu\n";
	char c;
	while (true) {
		c = getch();
		if (c == EXIT) {
			setColor(0, 11);
			gotoXY(x + 15, y - 2); cout << "CAM ON VI DA SU DUNG DICH VU!" << endl << endl;
			setColor(0, 0);
			exit(0);
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			return 1;
	}
}

void format() {
	// Dat ten cho console
	SetConsoleTitle(L"Flight Management");

	// Dat kich co cho console
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1250, 610, TRUE);

	// Xoa thanh truot cua console
	ShowScrollBar(GetConsoleWindow(), SB_VERT, 0);
}

string password(unsigned maxLength) {
	string pw;
	char x;
	setColor(0, 15);
	for (char c; (c = getch()); )
	{
		if (c == '\n' || c == '\r') {
			break;
		}
		else if (c == '\b') {
			cout << "\b \b";
			if (!pw.empty()) pw.erase(pw.size() - 1);
		}
		else if (c == -32 || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == ' ') {
			x = _getch();
		}
		else if (isprint(c) && pw.size() < maxLength) {
			cout << "*";
			pw += c;
		}
	}
	return pw;
}