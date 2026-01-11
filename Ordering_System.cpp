#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

struct Order {
    int orderId{};
    string itemName;
    int quantity{};
    long long orderTime{}; // simple timestamp counter (increasing)

    void print() const {
        cout << left
             << setw(10) << orderId
             << setw(20) << itemName
             << setw(10) << quantity
             << setw(10) << orderTime
             << "\n";
    }
};

class OrderQueue {
private:
    vector<Order> q; // front is index 0
public:
    bool isEmpty() const {
        return q.empty();
    }

    void enqueue(const Order& order) {
        q.push_back(order);
    }

    bool dequeue(Order &served) {
        if (isEmpty()) return false;
        served = q.front();
        q.erase(q.begin());
        return true;
    }

    const vector<Order>& getAll() const {
        return q;
    }

    // Search order by ID in the pending queue
    bool searchById(int id, Order &found) const {
        for (const auto &o : q) {
            if (o.orderId == id) {
                found = o;
                return true;
            }
        }
        return false;
    }

    // Sort pending orders by orderId or orderTime (FCFS time)
    void sortOrders(int mode) {
        // mode 1 = by ID ascending
        // mode 2 = by time ascending
        if (mode == 1) {
            sort(q.begin(), q.end(), [](const Order &a, const Order &b) {
                return a.orderId < b.orderId;
            });
        } else if (mode == 2) {
            sort(q.begin(), q.end(), [](const Order &a, const Order &b) {
                return a.orderTime < b.orderTime;
            });
        }
    }
};

// ---------- Utility Functions ----------
static void clearInputLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int readInt(const string &prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    while (true) {
        cout << prompt;
        int x;
        if (cin >> x) {
            clearInputLine();
            if (x < minVal || x > maxVal) {
                cout << "Invalid range. Please enter between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            return x;
        }
        cout << "Invalid input. Please enter a number.\n";
        clearInputLine();
    }
}

static string readNonEmptyLine(const string &prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "Input cannot be empty. Try again.\n";
    }
}

static void printHeader() {
    cout << left
         << setw(10) << "OrderID"
         << setw(20) << "Item"
         << setw(10) << "Qty"
         << setw(10) << "Time"
         << "\n";
    cout << string(50, '-') << "\n";
}

static bool isOrderIdUnique(const OrderQueue &oq, int id) {
    Order tmp;
    return !oq.searchById(id, tmp);
}

// ---------- Main Program ----------
int main() {
    OrderQueue orders;
    long long timeCounter = 1; // simple increasing counter to represent order time

    while (true) {
        cout << "\n=== Cafeteria Ordering System ===\n";
        cout << "1. Place Order\n";
        cout << "2. Serve Next Order\n";
        cout << "3. View Pending Orders\n";
        cout << "4. Search Order (by Order ID)\n";
        cout << "5. Sort Orders\n";
        cout << "6. Exit System\n";

        int choice = readInt("Choose an option (1-6): ", 1, 6);

        if (choice == 1) {
            // Place Order (Validate Order Details)
            cout << "\n--- Place Order ---\n";
            int id = readInt("Enter Order ID (positive integer): ", 1, 1000000000);

            // Ensure unique ID (common real-life constraint)
            while (!isOrderIdUnique(orders, id)) {
                cout << "Order ID already exists. Please enter a different ID.\n";
                id = readInt("Enter Order ID (positive integer): ", 1, 1000000000);
            }

            string item = readNonEmptyLine("Enter Item Name: ");
            int qty = readInt("Enter Quantity (1-100): ", 1, 100);

            Order o;
            o.orderId = id;
            o.itemName = item;
            o.quantity = qty;
            o.orderTime = timeCounter++;

            orders.enqueue(o);

            cout << "✅ Order placed successfully.\n";
        }
        else if (choice == 2) {
            // Serve Next Order (Check Pending Orders)
            cout << "\n--- Serve Next Order ---\n";
            if (orders.isEmpty()) {
                cout << "⚠️ No pending orders to serve.\n";
                continue;
            }

            Order served;
            orders.dequeue(served);

            cout << "✅ Served Order:\n";
            printHeader();
            served.print();
        }
        else if (choice == 3) {
            // View Pending Orders (Check Pending Orders)
            cout << "\n--- Pending Orders ---\n";
            if (orders.isEmpty()) {
                cout << "⚠️ No pending orders.\n";
                continue;
            }

            printHeader();
            for (const auto &o : orders.getAll()) {
                o.print();
            }
        }
        else if (choice == 4) {
            // Search Order (Check Pending Orders)
            cout << "\n--- Search Order ---\n";
            if (orders.isEmpty()) {
                cout << "⚠️ No pending orders to search.\n";
                continue;
            }

            int id = readInt("Enter Order ID to search: ", 1, 1000000000);
            Order found;

            if (orders.searchById(id, found)) {
                cout << "✅ Order found:\n";
                printHeader();
                found.print();
            } else {
                cout << "❌ Order ID not found.\n";
            }
        }
        else if (choice == 5) {
            // Sort Orders (Check Pending Orders)
            cout << "\n--- Sort Orders ---\n";
            if (orders.isEmpty()) {
                cout << "⚠️ No pending orders to sort.\n";
                continue;
            }

            cout << "1. Sort by Order ID (Ascending)\n";
            cout << "2. Sort by Order Time (FCFS)\n";
            int mode = readInt("Choose sorting method (1-2): ", 1, 2);

            orders.sortOrders(mode);

            cout << "✅ Orders sorted successfully.\n";
            cout << "Sorted result:\n";
            printHeader();
            for (const auto &o : orders.getAll()) {
                o.print();
            }
        }
        else if (choice == 6) {
            cout << "Exiting system... Goodbye!\n";
            break;
        }
    }

    return 0;
}
