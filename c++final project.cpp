#include <iostream>     // for input/output streams
#include <cstring>      // for C-style string handling (strncpy, strcmp)

using namespace std;

// Structure to represent individual items in a category
struct ItemData {
    char name[30];      // Name of the item
    int quantity;       // Quantity in stock
    float price;        // Price of the item
};

// Abstract base class representing a generic category
class Category {
protected:
    ItemData* items;    // Dynamic array to store items
    int itemCount;      // Number of items currently in the category

public:
    Category() : items(nullptr), itemCount(0) {}  // Constructor initializing pointers and count

    virtual ~Category() {
        delete[] items; // Free dynamically allocated memory for items
    }

    virtual void listItems() = 0; // Pure virtual function (must be overridden)

    // Function to add a new item to the category
    void addItem(const char* name, int qty, float price) {
        ItemData* newItems = new ItemData[itemCount + 1]; // Allocate new array with one more slot

        // Copy existing items into new array
        for (int i = 0; i < itemCount; ++i)
            newItems[i] = items[i];

        // Set name, quantity, and price for the new item
        strncpy(newItems[itemCount].name, name, 29);
        newItems[itemCount].name[29] = '\0'; // Ensure null-termination
        newItems[itemCount].quantity = qty;
        newItems[itemCount].price = price;

        delete[] items;      // Free old array memory
        items = newItems;    // Point to new array
        itemCount++;         // Increment item count
    }

    // Function to remove an item by name
    void removeItem(const char* name) {
        int idx = -1;

        // Find the item index to remove
        for (int i = 0; i < itemCount; ++i) {
            if (strcmp(items[i].name, name) == 0) {
                idx = i;
                break;
            }
        }

        if (idx == -1) return;  // Item not found

        // Create new array with one less item
        ItemData* newItems = new ItemData[itemCount - 1];
        for (int i = 0, j = 0; i < itemCount; ++i) {
            if (i != idx)
                newItems[j++] = items[i]; // Copy all except the one being removed
        }

        delete[] items;      // Free old array
        items = newItems;    // Update pointer
        itemCount--;         // Decrement item count
    }
};

// Derived class for Electronics category
class Electronics : public Category {
public:
    // Overrides the abstract function to list all electronics items
    void listItems() override {
        cout << "\nElectronics:\n";
        for (int i = 0; i < itemCount; ++i)
            cout << "  " << items[i].name << " (Qty: " << items[i].quantity << ", $" << items[i].price << ")\n";
    }
};

// Derived class for Furniture category
class Furniture : public Category {
public:
    // Overrides the abstract function to list all furniture items
    void listItems() override {
        cout << "\nFurniture:\n";
        for (int i = 0; i < itemCount; ++i)
            cout << "  " << items[i].name << " (Qty: " << items[i].quantity << ", $" << items[i].price << ")\n";
    }
};

// Inventory class manages dynamic array of Category pointers
class Inventory {
    Category** categories;  // Array of pointers to Category objects
    int count;              // Number of categories

public:
    Inventory() : categories(nullptr), count(0) {}  // Initialize members

    ~Inventory() {
        for (int i = 0; i < count; ++i)
            delete categories[i]; // Delete each category object
        delete[] categories;      // Delete category pointer array
    }

    // Adds a new category to the inventory
    void addCategory(Category* cat) {
        Category** newArr = new Category*[count + 1]; // Allocate space for one more category

        for (int i = 0; i < count; ++i)
            newArr[i] = categories[i]; // Copy existing categories

        newArr[count] = cat; // Add new category
        delete[] categories; // Free old array
        categories = newArr; // Point to new array
        count++;             // Increment category count
    }

    // Removes a category by name (hardcoded as "Electronics" or "Furniture")
    void removeCategory(const char* name) {
        int idx = -1;

        for (int i = 0; i < count; ++i) {
            Electronics* e = dynamic_cast<Electronics*>(categories[i]);
            Furniture* f = dynamic_cast<Furniture*>(categories[i]);

            // Check if the current category matches the name
            if ((e && strcmp(name, "Electronics") == 0) ||
                (f && strcmp(name, "Furniture") == 0)) {
                idx = i;
                break;
            }
        }

        if (idx == -1) return; // Not found

        delete categories[idx]; // Delete the matched category

        Category** newArr = new Category*[count - 1]; // Allocate smaller array

        // Copy all categories except the one being removed
        for (int i = 0, j = 0; i < count; ++i) {
            if (i != idx)
                newArr[j++] = categories[i];
        }

        delete[] categories;  // Free old array
        categories = newArr;  // Update pointer
        count--;              // Decrease count
    }

    // Lists all items in all categories
    void listAll() {
        for (int i = 0; i < count; ++i)
            categories[i]->listItems(); // Polymorphic call
    }
};

// Entry point of the program
int main() {
    Inventory inventory; // Create inventory object

    // Create Electronics category and add items
    Electronics* electronics = new Electronics();
    electronics->addItem("Laptop", 10, 999.99);
    electronics->addItem("Smartphone", 25, 499.99);

    // Create Furniture category and add items
    Furniture* furniture = new Furniture();
    furniture->addItem("Chair", 15, 79.99);
    furniture->addItem("Desk", 5, 199.99);

    // Add both categories to inventory
    inventory.addCategory(electronics);
    inventory.addCategory(furniture);

    // List all items before removals
    cout << "All Categories:\n";
    inventory.listAll();

    // Remove one item from Furniture category
    furniture->removeItem("Chair");

    // Remove entire Electronics category
    inventory.removeCategory("Electronics");

    // List remaining items/categories
    cout << "\nAfter removals:\n";
    inventory.listAll();

    return 0;
}

