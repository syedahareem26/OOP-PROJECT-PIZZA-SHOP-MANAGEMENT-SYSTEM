//syeda hareem basit
//230201092
//haram asif
//230201094
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

const int MAX_TOPPINGS = 5;
const int MAX_PIZZAS = 10;

// Represents a pizza with size, toppings, price, and cost
class Pizza {
public:
    string size;
    vector<string> toppings;
    double price;
    double cost;

    Pizza(string size = "", double price = 0.0, double cost = 0.0) 
        : size(size), price(price), cost(cost), toppings(MAX_TOPPINGS, "") {}

    void setToppings(vector<string>& newToppings) {
        for (int i = 0; i < newToppings.size() && i < MAX_TOPPINGS; ++i) {
            toppings[i] = newToppings[i];
        }
    }

    double calculateProfit() const {
        return price - cost;
    }

    double calculateProfitPercentage() const {
        return ((price - cost) / cost) * 100;
    }
};

// Represents an order with a list of pizzas and total cost
class Order {
public:
    vector<Pizza> pizzas;
    double totalCost;
    double totalProfit;
    string customerName;
    string customerContact;
    string customerID;

    Order() : totalCost(0), totalProfit(0) {}

    void addPizza(const Pizza& pizza) {
        if (pizzas.size() < MAX_PIZZAS) {
            pizzas.push_back(pizza);
            totalCost += pizza.price;
            totalProfit += pizza.calculateProfit();
        } else {
            cout << "Cannot add more pizzas to this order.\n";
        }
    }

    void saveOrderToFile(const char* filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << "Order Details:\n";
            file << "Customer ID: " << customerID << "\n";
            file << "Customer Name: " << customerName << "\n";
            file << "Customer Contact: " << customerContact << "\n";
            for (int i = 0; i < pizzas.size(); ++i) {
                file << "Pizza " << (i + 1) << ": " << pizzas[i].size << " size, $" << pizzas[i].price 
                     << ", Cost: $" << pizzas[i].cost << ", Toppings: ";
                for (int j = 0; j < MAX_TOPPINGS && !pizzas[i].toppings[j].empty(); ++j) {
                    file << pizzas[i].toppings[j] << " ";
                }
                double profitPercentage = pizzas[i].calculateProfitPercentage();
                file << ", Profit: $" << pizzas[i].calculateProfit() 
                     << " (" << profitPercentage << "%)\n";
            }
            double totalProfitPercentage = (totalProfit / totalCost) * 100;
            file << "Total Cost: $" << totalCost << "\n";
            file << "Total Profit: $" << totalProfit << " (" << totalProfitPercentage << "%)\n\n";
            file.close();
        } else {
            cout << "Unable to open file " << filename << " for writing.\n";
        }
    }
};

class Menu {
public:
    vector<Pizza> pizzas;

    void addPizzaToMenu(Pizza& pizza) {
        if (pizzas.size() < MAX_PIZZAS) {
            pizzas.push_back(pizza);
        } else {
            cout << "Cannot add more pizzas to the menu.\n";
        }
    }

    void displayMenu() const {
        cout << "Pizza Menu:\n";
        for (int i = 0; i < pizzas.size(); ++i) {
            cout << (i + 1) << ". " << pizzas[i].size << " size, $" << pizzas[i].price 
                 << ", Cost: $" << pizzas[i].cost << ", Toppings: ";
            for (int j = 0; j < MAX_TOPPINGS && !pizzas[i].toppings[j].empty(); ++j) {
                cout << pizzas[i].toppings[j] << " ";
            }
            cout << "\n";
        }
    }
};

// Function to calculate total sales and profit from the orders file
void calculateTotalSalesAndProfit(const char* filename, map<string, double>& customerSpending, map<string, int>& customerOrders) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file " << filename << " for reading.\n";
        return;
    }

    string line;
    double totalSales = 0;
    double totalProfit = 0;
    string currentCustomerID;
    while (getline(file, line)) {
        if (line.find("Customer ID:") != string::npos) {
            currentCustomerID = line.substr(line.find(":") + 2);
            customerOrders[currentCustomerID]++;
        } else if (line.find("Total Cost:") != string::npos) {
            double cost;
            sscanf(line.c_str(), "Total Cost: $%lf", &cost);
            totalSales += cost;
            if (!currentCustomerID.empty()) {
                customerSpending[currentCustomerID] += cost;
            }
        } else if (line.find("Total Profit:") != string::npos) {
            double profit;
            sscanf(line.c_str(), "Total Profit: $%lf", &profit);
            totalProfit += profit;
        }
    }
    file.close();

    cout << "Total Sales till now: $" << totalSales << "\n";
    cout << "Total Profit till now: $" << totalProfit << "\n";
}

// Function to read customer spending from file
void loadCustomerSpending(const char* filename, map<string, double>& customerSpending, map<string, int>& customerOrders) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file " << filename << " for reading.\n";
        return;
    }

    string line;
    string currentCustomerID;
    while (getline(file, line)) {
        if (line.find("Customer ID:") != string::npos) {
            currentCustomerID = line.substr(line.find(":") + 2);
            customerOrders[currentCustomerID]++;
        } else if (line.find("Total Cost:") != string::npos) {
            double cost;
            sscanf(line.c_str(), "Total Cost: $%lf", &cost);
            if (!currentCustomerID.empty()) {
                customerSpending[currentCustomerID] += cost;
            }
        }
    }
    file.close();
}

// Function to find the customer with the most orders
void findCustomerWithMostOrders(const map<string, int>& customerOrders, string& customerID, int& maxOrders) {
    for (const auto& entry : customerOrders) {
        if (entry.second > maxOrders) {
            maxOrders = entry.second;
            customerID = entry.first;
        }
    }
}

// Function to find the customer who has spent the most money
void findCustomerWithMostSpending(const map<string, double>& customerSpending, string& customerID, double& maxSpending) {
    for (const auto& entry : customerSpending) {
        if (entry.second > maxSpending) {
            maxSpending = entry.second;
            customerID = entry.first;
        }
    }
}

// Main function demonstrating the pizza shop management system
int main() {
    Menu menu;
    Order order;
    map<string, double> customerSpending;
    map<string, int> customerOrders;

    // Load customer spending and orders from file
    loadCustomerSpending("orders.txt", customerSpending, customerOrders);

    // Find and display the customer with the most orders and the one who spent the most
    string mostOrdersCustomerID = "N/A";
    int maxOrders = 0;
    findCustomerWithMostOrders(customerOrders, mostOrdersCustomerID, maxOrders);

    string mostSpendingCustomerID = "N/A";
    double maxSpending = 0.0;
    findCustomerWithMostSpending(customerSpending, mostSpendingCustomerID, maxSpending);

    
    cout << "Welcome to the Pizza Shop :)" << endl;

    // Add pizzas to the menu with price and cost
    vector<string> margheritaToppings = {"Cheese", "Tomato"};
    Pizza margheritaPizza("Medium", 12.5, 7.0);
    margheritaPizza.setToppings(margheritaToppings);
    menu.addPizzaToMenu(margheritaPizza);

    vector<string> pepperoniToppings = {"Cheese", "Pepperoni"};
    Pizza pepperoniPizza("Large", 15.0, 8.0);
    pepperoniPizza.setToppings(pepperoniToppings);
    menu.addPizzaToMenu(pepperoniPizza);

    vector<string> veggieToppings = {"Cheese", "Tomato", "Bell Peppers"};
    Pizza veggiePizza("Small", 10.0, 5.5);
    veggiePizza.setToppings(veggieToppings);
    menu.addPizzaToMenu(veggiePizza);

    vector<string> hawaiianToppings = {"Cheese", "Ham", "Pineapple"};
    Pizza hawaiianPizza("Large", 16.0, 8.5);
    hawaiianPizza.setToppings(hawaiianToppings);
    menu.addPizzaToMenu(hawaiianPizza);

    vector<string> bbqChickenToppings = {"Cheese", "Chicken", "BBQ Sauce"};
    Pizza bbqChickenPizza("Medium", 14.0, 7.5);
    bbqChickenPizza.setToppings(bbqChickenToppings);
    menu.addPizzaToMenu(bbqChickenPizza);

    vector<string> fourCheeseToppings = {"Mozzarella", "Cheddar", "Parmesan", "Gorgonzola"};
    Pizza fourCheesePizza("Small", 13.0, 6.0);
    fourCheesePizza.setToppings(fourCheeseToppings);
    menu.addPizzaToMenu(fourCheesePizza);

    // Display the menu
    menu.displayMenu();
    cout << "Press enter to start your order." << endl;

    // Clear the input buffer before taking customer details
    cin.ignore(); // Just ignore one character to clear the newline

    // Take customer ID
    cout << "Enter customer ID: ";
    getline(cin, order.customerID);
    
    if (customerSpending.find(order.customerID) == customerSpending.end()) {
        // New customer
        order.customerID = "0000";
        cout << "Could not find the ID." << endl;
        cout << "New customer assigned ID: 0000\n";
    } else {
        cout << "Welcome back, customer ID: " << order.customerID << "\n";
    }

    // Take an order from the menu
    int choice;
    while (true) {
        cout << "Enter the number of the pizza you want to order (0 to finish): ";
        cin >> choice;
        if (choice == 0) break;
        if (choice > 0 && choice <= menu.pizzas.size()) {
            order.addPizza(menu.pizzas[choice - 1]);
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    // Apply discount if applicable
    if (order.customerID != "0000" && customerSpending[order.customerID] > 100.0) {
        order.totalCost *= 0.9;
        cout << "A 10% discount has been applied to your order.\n";
    }

    // Save order details and profit to files
    order.saveOrderToFile("orders.txt");

    // Display the total bill and profit
    cout << "Total Bill: $" << order.totalCost << "\n";

    while (true) {
        string option;
        cout << "\nFor checking total sales and profit now type yes, else type no: ";
        cin >> option;
        if (option == "no") break;
        if (option == "yes") {
            // Calculate and display total sales and profit
            calculateTotalSalesAndProfit("orders.txt", customerSpending, customerOrders);
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    // Display the ad-like messages again at the end
    cout << "---------------------------------------------\n";
    cout << "AD: Our top customer with the most orders is:\n";
    cout << "Customer ID: " << mostOrdersCustomerID << " with " << maxOrders << " orders!\n";
    cout << "---------------------------------------------\n";
    cout << "AD: Our top customer who spent the most is:\n";
    cout << "Customer ID: " << mostSpendingCustomerID << " with $" << maxSpending << " spent!\n";
    cout << "---------------------------------------------\n";

    return 0;
}