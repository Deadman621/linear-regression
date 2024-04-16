bool areAllFieldsCorrect(const string& temp) {
    char prev = 0; // No initial comma, start with a non-comma character
    for (int i = 0; i < temp.size(); i++) {
        char c = temp[i];

        if (c == ',' && prev == ',') {
            return false; // Two consecutive commas are not allowed
        }
        if (!isValid(c) && c != ',') {
            return false; // Invalid character check, allowing commas
        }
        prev = c;
    }
    return true; // If loop completes without returning false, the string is correct
}

bool isValid(char c) {
    return (isdigit(c) || c == '.'); // Simplified check for digit or period
}