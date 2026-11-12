#define UNIT_TEST
#include "main.cpp"
#include <iostream>
using namespace std;

int main(){
    int passed = 0;
    int failed = 0;

    auto check = [&](bool result, string test_name){
        if(result){
            cout << "PASS: " << test_name << endl;
            passed++;
        }
        else{
            cout << "FAIL: " << test_name << endl;
            failed++;
        }
    };

// Test 1: Homepage
    BrowserHistory history("google.com");
    check(history.current_page() == "google.com (page 1 out of 1)",
          "Homepage initialization");

// Test 2: Visit
    history.visit("youtube.com");
    history.visit("facebook.com");
    check(history.current_page() == "facebook.com (page 3 out of 3)",
          "Visit pages");

// Test 3: Back
    check(history.back(1) == "youtube.com", "Back navigation");

// Test 4: Forward
    check(history.forward(1) == "facebook.com", "Forward navigation");

// Test 5: Back 100 steps
    check(history.back(100) == "google.com", "Back boundary");

// Test 6: Forward 100 steps
    check(history.forward(100) == "facebook.com", "Forward boundary");

// Test 7: Branching
    history.back(1);
    history.visit("reddit.com");
    check(!history.delete_url("facebook.com"), "Branching history");

// Test 8: Delete URL
    check(history.delete_url("youtube.com"), "Delete URL");

// Test 9: Restore URL
    check(history.restore(), "Restore URL");
    check(history.back(1) == "youtube.com", "Restored position");

// Test 10: Maximum capacity
    BrowserHistory capacity_test("google.com");

    for(int i = 1; i <= 51; i++){
        capacity_test.visit("page" + to_string(i) + ".com");
    }

    check(capacity_test.back(100) == "page2.com",
          "Maximum capacity of 50 pages");

// Test 11: Clear history
    BrowserHistory clear_test("google.com");
    clear_test.delete_url("google.com");
    clear_test.delete_history();

    check(clear_test.current_page() == "", "Clear history");
    check(!clear_test.restore(), "Clear restoration stack");
    
// Test 12: Export and Import
    BrowserHistory file_test("google.com");

    file_test.visit("youtube.com");
    file_test.visit("facebook.com");

// Test exporting
    check(file_test.export_history("test_history.txt"),
        "Export history");

// Clear history
    file_test.delete_history();

// Test importing
    check(file_test.import_history("test_history.txt"),
        "Import history");

// Check first imported page
    check(file_test.back(100) == "google.com",
        "Imported first page");

// Check last imported page
    check(file_test.forward(100) == "facebook.com",
        "Imported last page");

// Test importing a nonexistent file
    check(!file_test.import_history("nonexistent_test_file_12345.txt"),
        "Import nonexistent file");

// Test 13: Deleting first and last pages
    BrowserHistory edge_test("google.com");
    edge_test.visit("youtube.com");
    edge_test.visit("facebook.com");

    check(edge_test.delete_url("google.com"),
        "Delete first page");

    check(edge_test.back(100) == "youtube.com",
        "Head updated after deletion");

    edge_test.forward(100);

    check(edge_test.delete_url("facebook.com"),
        "Delete current last page");

    check(edge_test.current_page() == "youtube.com (page 1 out of 1)",
        "Current pointer after deletion");

// Test restoring deleted pages
    check(edge_test.restore(),
        "Restore last page");

    check(edge_test.forward(100) == "facebook.com",
        "Restored last page position");

    check(edge_test.restore(),
        "Restore first page");

    check(edge_test.back(100) == "google.com",
        "Restored first page position");

// Test deleting nonexistent URL
    check(!edge_test.delete_url("unknown.com"),
        "Delete nonexistent URL");

// Test deleting the only page
    BrowserHistory single_test("google.com");

    check(single_test.delete_url("google.com"),
        "Delete only page");

    check(single_test.restore() &&
        single_test.current_page() == "google.com (page 1 out of 1)",
        "Restore into empty history");
    
    cout << "\nTests passed: " << passed << endl;
    cout << "Tests failed: " << failed << endl;

    return failed == 0 ? 0 : 1;
}