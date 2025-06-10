#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Class to handle CSV-like data similar to pandas DataFrame
class CSVData {
public:
    vector<vector<string>> data;  // Store the CSV as a 2D vector (rows x columns)
    vector<string> headers;       // Store the column headers

    // Constructor that loads a CSV file
    CSVData(const string& filename) {
        ifstream file(filename);
        string line;

        // Check if the file is open
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            exit(1);
        }

        // Read headers (first line)
        if (getline(file, line)) {
            stringstream ss(line);
            string header;
            while (getline(ss, header, ',')) {
                headers.push_back(header);
            }
        }

        // Read data (subsequent lines)
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            vector<string> row;
            while (getline(ss, field, ',')) {
                row.push_back(field);
            }
            data.push_back(row);
        }

        file.close();
    }

    // Function to print the data
    void print() const {
        // Print headers
        for (const auto& header : headers) {
            cout << header << "\t";
        }
        cout << endl;

        // Print data
        for (const auto& row : data) {
            for (const auto& field : row) {
                cout << field << "\t";
            }
            cout << endl;
        }
    }

    // Function to get a specific column (returns a vector of strings)
    vector<string> getColumn(const string& column_name) const {
        vector<string> column_data;
        int column_index = -1;

        // Find the column index based on the header
        for (size_t i = 0; i < headers.size(); ++i) {
            if (headers[i] == column_name) {
                column_index = i;
                break;
            }
        }

        // If column is found, extract the data for that column
        if (column_index != -1) {
            for (const auto& row : data) {
                column_data.push_back(row[column_index]);
            }
        } else {
            cerr << "Column " << column_name << " not found!" << endl;
        }

        return column_data;
    }

    // Function to get a specific row (returns a vector of strings)
    vector<string> getRow(int row_index) const {
        if (row_index >= 0 && row_index < data.size()) {
            return data[row_index];
        } else {
            cerr << "Row index out of bounds!" << endl;
            return {};
        }
    }
};

int main() {
    // Load a CSV file
    CSVData csv("data.csv");

    // Print the CSV content
    csv.print();

    // Get a specific column (e.g., "Age")
    vector<string> age_column = csv.getColumn("Age");

    cout << "\nAge Column:" << endl;
    for (const string& value : age_column) {
        cout << value << endl;
    }

    // Get a specific row (e.g., row 1)
    vector<string> row = csv.getRow(1);

    cout << "\nRow 1:" << endl;
    for (const string& value : row) {
        cout << value << "\t";
    }
    cout << endl;

    return 0;
}

