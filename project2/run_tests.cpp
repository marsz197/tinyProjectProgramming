#define TEST_MODE
#include "ecommerce.cpp"
#include <cassert>

int main() {
    // Reset Data
    remove("data/users.json");
    remove("data/products.json");
    remove("data/orders.json");

    SystemManager sys;

    // Seed init data
    if(sys.getProduct("P1") == nullptr) {
        sys.addProduct("Laptop", "Electronics", 999.99, 10);
        sys.addProduct("Mouse", "Electronics", 49.99, 50);
        sys.addProduct("Desk", "Furniture", 199.99, 5);
        sys.registerUser("admin", "admin", "admin");
        sys.registerUser("john", "123", "customer");
    }

    cout << "\n--- STEP 1: Admin adds/removes products ---\n";
    assert(sys.login("admin", "admin"));
    sys.addProduct("Dell Latt 7850", "Electronics", 299.99, 1);
    sys.addProduct("Gemini User License", "Freebies", 0.0, 2);
    
    // Find Gemini ID to remove
    string geminiId = "";
    for(int i=1; i<=10; i++) {
        Product* p = sys.getProduct("P" + to_string(i));
        if(p && p->name == "Gemini User License") { geminiId = p->id; break; }
    }
    assert(geminiId != "");
    sys.deleteProduct(geminiId);
    assert(sys.getProduct(geminiId) == nullptr);
    sys.logout();

    cout << "\n--- STEP 2: Register Minh, Browse, Cart, Checkout ---\n";
    assert(sys.registerUser("Minh", "minh123", "customer"));
    assert(sys.login("Minh", "minh123"));
    sys.browseProducts();
    sys.addToCart("P2", 1); // Mouse
    sys.addToCart("P1", 3); // Laptop
    sys.removeFromCart("P1", 1); // Remove 1 laptop, should have 2 left
    
    // Check cart contents directly through User pointer
    User* u = sys.getCurrentUser();
    assert(u->shoppingCart["P1"] == 2);
    assert(u->shoppingCart["P2"] == 1);
    
    sys.checkout();
    assert(u->shoppingCart.empty()); // cart empty after checkout
    sys.viewOrderHistory();
    sys.logout();

    cout << "\n--- STEP 3: John Recommendation, Carts, Logout ---\n";
    assert(sys.login("john", "123"));
    sys.showRecommendations(); // works
    sys.addToCart("P1", 11); // Will fail, 8 left (started 10, Minh bought 2)
    u = sys.getCurrentUser();
    assert(u->shoppingCart["P1"] == 0); // Cart shouldn't have 11 laptops
    
    sys.addToCart("P1", 6);
    sys.addToCart("P3", 3); // Desk
    sys.removeFromCart("P3", 3); // Remove all desks
    assert(u->shoppingCart["P3"] == 0);
    sys.logout();

    cout << "\n--- STEP 4: John Relog, Cart Persists, Checkout ---\n";
    assert(sys.login("john", "123"));
    u = sys.getCurrentUser();
    assert(u->shoppingCart["P1"] == 6); // Persistent cart verification
    sys.checkout();
    sys.logout();

    cout << "\n--- STEP 5: Admin Stats, History, Updates ---\n";
    assert(sys.login("admin", "admin"));
    sys.showStats();
    sys.viewAllOrderHistory();
    sys.editProduct("P1", 999.99, 15); // Update qty to 15
    assert(sys.getProduct("P1")->stock == 15);
    sys.viewActiveUsers();
    sys.changeUserPassword("Minh", "MINHMINH");
    sys.logout();

    cout << "\n--- STEP 6: Minh Relog (new pass), Security tests ---\n";
    assert(!sys.login("Minh", "minh123")); // Old pass should fail
    assert(sys.login("Minh", "MINHMINH")); // New pass works
    
    cout << "\n--- ALL TESTS PASSED SUCCESSFULLY! ---\n";
    return 0;
}
