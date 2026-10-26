// CS 300: ABCU Course Advising Program
// Stores courses in a binary search tree by course number.
// An in-order traversal prints courses in alphanumeric order.

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

struct Course {
    string number;
    string title;
    vector<string> prerequisites;
};

string trim(const string& value) {
    const auto start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

string uppercase(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char ch) { return static_cast<char>(toupper(ch)); });
    return value;
}

class CourseTree {
private:
    struct Node {
        Course course;
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        explicit Node(Course c) : course(std::move(c)) {}
    };

    unique_ptr<Node> root;
    size_t count = 0;

    static void printInOrder(const Node* node) {
        if (!node) return;
        printInOrder(node->left.get());
        cout << node->course.number << ", " << node->course.title << '\n';
        printInOrder(node->right.get());
    }

    static void collectCourses(const Node* node, vector<const Course*>& courses) {
        if (!node) return;
        collectCourses(node->left.get(), courses);
        courses.push_back(&node->course);
        collectCourses(node->right.get(), courses);
    }

public:
    bool insert(Course course) {
        unique_ptr<Node>* link = &root;
        while (*link) {
            if (course.number == (*link)->course.number) return false;
            link = (course.number < (*link)->course.number)
                       ? &(*link)->left : &(*link)->right;
        }
        *link = make_unique<Node>(std::move(course));
        ++count;
        return true;
    }

    const Course* find(const string& number) const {
        const Node* node = root.get();
        while (node) {
            if (number == node->course.number) return &node->course;
            node = (number < node->course.number) ? node->left.get() : node->right.get();
        }
        return nullptr;
    }

    void printAll() const { printInOrder(root.get()); }

    vector<const Course*> courses() const {
        vector<const Course*> result;
        result.reserve(count);
        collectCourses(root.get(), result);
        return result;
    }

    size_t size() const { return count; }
    void swap(CourseTree& other) noexcept {
        root.swap(other.root);
        std::swap(count, other.count);
    }
};

// Parse comma-separated course data with at least a number and title.
// Empty trailing fields are permitted in the supplied ABCU data file.
bool parseCourse(const string& line, Course& course) {
    vector<string> parts;
    istringstream stream(line);
    string part;
    while (getline(stream, part, ',')) parts.push_back(trim(part));
    while (parts.size() > 2 && parts.back().empty()) parts.pop_back();

    if (parts.size() < 2 || parts[0].empty() || parts[1].empty()) return false;
    course.number = uppercase(parts[0]);
    course.title = parts[1];
    course.prerequisites.clear();
    for (size_t i = 2; i < parts.size(); ++i) {
        if (parts[i].empty()) return false;
        course.prerequisites.push_back(uppercase(parts[i]));
    }
    return true;
}

bool loadCourses(const string& filename, CourseTree& current) {
    ifstream file(filename);
    if (!file) {
        cerr << "Could not open file: " << filename << '\n';
        return false;
    }

    CourseTree candidate;
    string line;
    size_t lineNumber = 0;
    bool valid = true;

    while (getline(file, line)) {
        ++lineNumber;
        Course course;
        if (!parseCourse(line, course)) {
            cerr << "Invalid course record on line " << lineNumber << '\n';
            valid = false;
            continue;
        }
        string number = course.number;
        if (!candidate.insert(std::move(course))) {
            cerr << "Duplicate course number on line " << lineNumber
                 << ": " << number << '\n';
            valid = false;
        }
    }

    if (candidate.size() == 0) {
        cerr << "The file contains no valid courses.\n";
        valid = false;
    }

    // Validate after reading all records so courses can appear in any order.
    for (const Course* course : candidate.courses()) {
        for (const string& prerequisite : course->prerequisites) {
            if (prerequisite == course->number || !candidate.find(prerequisite)) {
                cerr << "Invalid prerequisite " << prerequisite << " for "
                     << course->number << '\n';
                valid = false;
            }
        }
    }

    if (!valid) {
        cerr << "The file was rejected. Previously loaded courses were kept.\n";
        return false;
    }
    current.swap(candidate);
    cout << "Successfully loaded " << current.size() << " courses.\n";
    return true;
}

void printCourse(const CourseTree& courses, const string& courseNumber) {
    const Course* course = courses.find(uppercase(trim(courseNumber)));
    if (!course) {
        cout << "Course not found.\n";
        return;
    }
    cout << course->number << ", " << course->title << '\n';
    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None\n";
        return;
    }
    cout << "Prerequisites:\n";
    for (const string& prerequisite : course->prerequisites) {
        const Course* prereqCourse = courses.find(prerequisite);
        cout << "  " << prerequisite;
        if (prereqCourse) cout << ", " << prereqCourse->title;
        cout << '\n';
    }
}

int main() {
    CourseTree courses;
    bool loaded = false;
    string input;
    cout << "Welcome to the ABCU course advising program.\n";

    while (true) {
        cout << "\n1. Load Data Structure\n"
             << "2. Print Course List\n"
             << "3. Print Course\n"
             << "9. Exit\n"
             << "What would you like to do? ";
        if (!getline(cin, input)) break;
        input = trim(input);

        if (input == "9") {
            cout << "Thank you for using the course advising program!\n";
            break;
        }
        if (input == "1") {
            cout << "Enter the course data file name: ";
            string filename;
            if (!getline(cin, filename)) break;
            if (loadCourses(trim(filename), courses)) loaded = true;
        } else if (input == "2" || input == "3") {
            if (!loaded) {
                cout << "Please load course data first using option 1.\n";
                continue;
            }
            if (input == "2") {
                cout << "\nCourses in alphanumeric order:\n";
                courses.printAll();
            } else {
                cout << "Enter the course number: ";
                string number;
                if (!getline(cin, number)) break;
                printCourse(courses, number);
            }
        } else {
            cout << "Invalid option. Please enter 1, 2, 3, or 9.\n";
        }
    }
    return 0;
}
