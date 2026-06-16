#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <conio.h> // For TUI interactive input
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// --- Helper Functions for TUI & Input ---

string readString(const string& prompt) {
    cout << prompt;
    string res;
    getline(cin >> ws, res);
    return res;
}

double readDouble(const string& prompt) {
    cout << prompt;
    double res;
    cin >> res;
    if(cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return 0.0; }
    return res;
}

int readInt(const string& prompt) {
    cout << prompt;
    int res;
    cin >> res;
    if(cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); return 0; }
    return res;
}

int runMenu(const string& title, const vector<string>& options) {
    int selected = 0;
    while(true) {
        system("cls"); // Clear screen for Windows
        cout << "==========================================\n";
        cout << "           " << title << "\n";
        cout << "==========================================\n\n";
        
        for(size_t i = 0; i < options.size(); ++i) {
            if((int)i == selected) {
                cout << "  -> [ \x1b[7m" << options[i] << "\x1b[0m ]\n"; // Reverse video highlight
            } else {
                cout << "     " << options[i] << "\n";
            }
        }
        cout << "\n------------------------------------------\n";
        cout << "Use Up/Down arrows to navigate, Enter to select.\n";
        
        int c = _getch();
        if(c == 224 || c == 0) { // Arrow key prefix
            c = _getch();
            if(c == 72) { // Up
                selected = (selected - 1 + options.size()) % options.size();
            } else if(c == 80) { // Down
                selected = (selected + 1) % options.size();
            }
        } else if(c == 13) { // Enter
            return selected;
        }
    }
}

// --- Data Structures ---

struct Interaction {
    int views = 0;
    int purchases = 0;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Interaction, views, purchases)
};

class Product {
public:
    string id;
    string name;
    string category;
    double price;
    int stock;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Product, id, name, category, price, stock)
};

class User {
public:
    string id;
    string username;
    string password;
    string role; // "customer" or "admin"
    
    // For customers
    map<string, Interaction> interactions; // productId -> Interaction

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, id, username, password, role, interactions)
};

struct OrderItem {
    string productId;
    int quantity;
    double price;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderItem, productId, quantity, price)
};

class Order {
public:
    string id;
    string customerId;
    vector<OrderItem> items;
    double total;
    string status;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Order, id, customerId, items, total, status)
};

// --- System Manager ---

class SystemManager {
private:
    vector<User> users;
    vector<Product> products;
    vector<Order> orders;
    
    const string usersFile = "data/users.json";
    const string productsFile = "data/products.json";
    const string ordersFile = "data/orders.json";
    
    User* loggedInUser = nullptr;
    map<string, int> shoppingCart; // productId -> quantity

public:
    SystemManager() {
        loadData();
    }
    
    ~SystemManager() {
        saveData();
    }
    
    void loadData() {
        try {
            ifstream uf(usersFile);
            if(uf.is_open() && uf.peek() != ifstream::traits_type::eof()) {
                json j; uf >> j; users = j.get<vector<User>>();
            }
            ifstream pf(productsFile);
            if(pf.is_open() && pf.peek() != ifstream::traits_type::eof()) {
                json j; pf >> j; products = j.get<vector<Product>>();
            }
            ifstream of(ordersFile);
            if(of.is_open() && of.peek() != ifstream::traits_type::eof()) {
                json j; of >> j; orders = j.get<vector<Order>>();
            }
        } catch(const exception& e) {
            cerr << "Error loading data: " << e.what() << endl;
        }
    }
    
    void saveData() {
        ofstream uf(usersFile);
        uf << json(users).dump(4);
        
        ofstream pf(productsFile);
        pf << json(products).dump(4);
        
        ofstream of(ordersFile);
        of << json(orders).dump(4);
    }

    Product* getProduct(const string& id) {
        for(auto& p : products) {
            if(p.id == id) return &p;
        }
        return nullptr;
    }

    // Auth
    bool login(const string& username, const string& password) {
        for(auto& u : users) {
            if(u.username == username && u.password == password) {
                loggedInUser = &u;
                shoppingCart.clear();
                return true;
            }
        }
        return false;
    }
    
    void logout() {
        loggedInUser = nullptr;
        shoppingCart.clear();
        saveData();
    }
    
    bool registerUser(const string& username, const string& password, const string& role) {
        for(auto& u : users) {
            if(u.username == username) return false; // Exists
        }
        User nu;
        nu.id = "U" + to_string(users.size() + 1);
        nu.username = username;
        nu.password = password;
        nu.role = role;
        users.push_back(nu);
        saveData();
        return true;
    }

    // Customer actions
    void browseProducts() {
        cout << "--- Products ---\n";
        for(auto& p : products) {
            cout << p.id << " | " << p.name << " | $" << p.price << " | Stock: " << p.stock << "\n";
        }
    }
    
    void viewProduct(const string& id) {
        Product* p = getProduct(id);
        if(!p) { cout << "Not found!\n"; return; }
        cout << "Viewing: " << p->name << " (" << p->category << ") - $" << p->price << "\n";
        if(loggedInUser && loggedInUser->role == "customer") {
            loggedInUser->interactions[id].views++;
            saveData();
        }
    }
    
    void addToCart(const string& id, int qty) {
        Product* p = getProduct(id);
        if(!p) { cout << "Not found!\n"; return; }
        if(p->stock < qty) { cout << "Insufficient stock!\n"; return; }
        shoppingCart[id] += qty;
        cout << "Added to cart.\n";
    }
    
    void checkout() {
        if(shoppingCart.empty()) { cout << "Cart is empty.\n"; return; }
        if(!loggedInUser) return;
        
        Order o;
        o.id = "O" + to_string(orders.size() + 1);
        o.customerId = loggedInUser->id;
        o.total = 0;
        o.status = "Completed";
        
        for(auto& pair : shoppingCart) {
            Product* p = getProduct(pair.first);
            if(p && p->stock >= pair.second) {
                p->stock -= pair.second;
                o.items.push_back({p->id, pair.second, p->price});
                o.total += pair.second * p->price;
                loggedInUser->interactions[p->id].purchases += pair.second;
            }
        }
        orders.push_back(o);
        shoppingCart.clear();
        saveData();
        cout << "Order placed! Total: $" << o.total << "\n";
    }

    void showRecommendations() {
        if(!loggedInUser || loggedInUser->role != "customer") return;
        
        // score = view_count * 0.2 + purchase_count * 0.8 + category_bonus
        vector<pair<string, double>> scores;
        for(auto& p : products) {
            double score = 0.0;
            if(loggedInUser->interactions.count(p.id)) {
                score += loggedInUser->interactions[p.id].views * 0.2;
                score += loggedInUser->interactions[p.id].purchases * 0.8;
            }
            // Add a simple category bonus (e.g. +1.0 if they ever bought from this category)
            for(auto& interact : loggedInUser->interactions) {
                Product* ip = getProduct(interact.first);
                if(ip && ip->category == p.category && interact.second.purchases > 0) {
                    score += 1.0;
                    break;
                }
            }
            scores.push_back({p.name, score});
        }
        sort(scores.begin(), scores.end(), [](auto& a, auto& b){ return a.second > b.second; });
        
        cout << "--- Recommended for you ---\n";
        for(int i=0; i<min(5, (int)scores.size()); i++) {
            if(scores[i].second > 0)
                cout << scores[i].first << " (Score: " << scores[i].second << ")\n";
        }
    }

    // Admin actions
    void addProduct(const string& name, const string& cat, double price, int stock) {
        Product p;
        p.id = "P" + to_string(products.size() + 1);
        p.name = name;
        p.category = cat;
        p.price = price;
        p.stock = stock;
        products.push_back(p);
        saveData();
        cout << "Product added.\n";
    }
    
    void editProduct(const string& id, double price, int stock) {
        Product* p = getProduct(id);
        if(p) {
            p->price = price;
            p->stock = stock;
            saveData();
            cout << "Product updated.\n";
        } else cout << "Not found.\n";
    }

    void deleteProduct(const string& id) {
        auto it = remove_if(products.begin(), products.end(), [&](Product& p){ return p.id == id; });
        if(it != products.end()) {
            products.erase(it, products.end());
            saveData();
            cout << "Product deleted.\n";
        } else cout << "Not found.\n";
    }
    
    void showStats() {
        cout << "Total Users: " << users.size() << "\n";
        cout << "Total Products: " << products.size() << "\n";
        cout << "Total Orders: " << orders.size() << "\n";
        
        map<string, int> sold;
        map<string, int> views;
        int activeUsers = 0;
        
        for(auto& u : users) {
            if(!u.interactions.empty()) activeUsers++;
            for(auto& i : u.interactions) {
                views[i.first] += i.second.views;
            }
        }
        
        for(auto& o : orders) {
            for(auto& i : o.items) sold[i.productId] += i.quantity;
        }
        
        cout << "Active Users: " << activeUsers << "\n";
        
        string bestP = ""; int maxS = 0;
        for(auto& s : sold) { if(s.second > maxS) { maxS = s.second; bestP = s.first; } }
        if(maxS > 0) {
            Product* p = getProduct(bestP);
            if(p) cout << "Best selling: " << p->name << " (" << maxS << " sold)\n";
        }
        
        string mostV = ""; int maxV = 0;
        for(auto& v : views) { if(v.second > maxV) { maxV = v.second; mostV = v.first; } }
        if(maxV > 0) {
            Product* p = getProduct(mostV);
            if(p) cout << "Most viewed: " << p->name << " (" << maxV << " views)\n";
        }
    }
    
    User* getCurrentUser() { return loggedInUser; }
};

int main() {
    SystemManager sys;
    
    // Seed some data if empty
    if(sys.getProduct("P1") == nullptr) {
        sys.addProduct("Laptop", "Electronics", 999.99, 10);
        sys.addProduct("Mouse", "Electronics", 49.99, 50);
        sys.addProduct("Desk", "Furniture", 199.99, 5);
        sys.registerUser("admin", "admin", "admin");
        sys.registerUser("john", "123", "customer");
    }

    while(true) {
        User* u = sys.getCurrentUser();
        if(!u) {
            vector<string> opts = {"Login", "Register", "Exit"};
            int ch = runMenu("Main Menu", opts);
            system("cls");
            
            if(ch == 0) {
                cout << "--- Login ---\n";
                string user = readString("Username: ");
                string pass = readString("Password: ");
                if(!sys.login(user, pass)) { cout << "Login Failed!\n"; system("pause"); }
            } else if(ch == 1) {
                cout << "--- Register ---\n";
                string user = readString("Username: ");
                string pass = readString("Password: ");
                if(sys.registerUser(user, pass, "customer")) { cout << "Registered successfully!\n"; system("pause"); }
                else { cout << "Username already taken!\n"; system("pause"); }
            } else if(ch == 2) {
                break;
            }
        } else if(u->role == "admin") {
            vector<string> opts = {"Add Product", "Edit Product", "Delete Product", "View Stats", "Logout"};
            int ch = runMenu("Admin Menu (" + u->username + ")", opts);
            system("cls");
            
            if(ch == 0) {
                cout << "--- Add Product ---\n";
                string name = readString("Name: ");
                string cat = readString("Category: ");
                double price = readDouble("Price: ");
                int stock = readInt("Stock: ");
                sys.addProduct(name, cat, price, stock);
                system("pause");
            } else if(ch == 1) {
                cout << "--- Edit Product ---\n";
                string id = readString("Product ID: ");
                double price = readDouble("New Price: ");
                int stock = readInt("New Stock: ");
                sys.editProduct(id, price, stock);
                system("pause");
            } else if(ch == 2) {
                cout << "--- Delete Product ---\n";
                string id = readString("Product ID: ");
                sys.deleteProduct(id);
                system("pause");
            } else if(ch == 3) {
                cout << "--- System Stats ---\n";
                sys.showStats();
                system("pause");
            } else if(ch == 4) {
                sys.logout();
            }
        } else {
            vector<string> opts = {"Browse Products", "View Product", "Add to Cart", "Checkout", "View Recommendations", "Logout"};
            int ch = runMenu("Customer Menu (" + u->username + ")", opts);
            system("cls");
            
            if(ch == 0) {
                sys.browseProducts();
                system("pause");
            } else if(ch == 1) {
                cout << "--- View Product ---\n";
                string id = readString("Product ID: ");
                sys.viewProduct(id);
                system("pause");
            } else if(ch == 2) {
                cout << "--- Add To Cart ---\n";
                string id = readString("Product ID: ");
                int qty = readInt("Qty: ");
                sys.addToCart(id, qty);
                system("pause");
            } else if(ch == 3) {
                cout << "--- Checkout ---\n";
                sys.checkout();
                system("pause");
            } else if(ch == 4) {
                sys.showRecommendations();
                system("pause");
            } else if(ch == 5) {
                sys.logout();
            }
        }
    }
    return 0;
}
