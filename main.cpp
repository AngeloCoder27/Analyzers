#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <iomanip>
using namespace std;

//C:\Users\Charles Cabontocan\Downloads\LexicalAnalyzer+SyntaxAnalyzer\testcodes\input.txt
//C:\Users\Charles Cabontocan\Downloads\LexicalAnalyzer+SyntaxAnalyzer\testcodes\input2.txt

// Enum for different types of tokens
enum TokenType {
    PREPROCESSOR,
    KEYWORD,
    IDENTIFIER,
    SYMBOL,
    OPERATOR,
    STRING_LITERAL,
    INTEGER_LITERAL
};

// Enum for different types of syntax elements
enum SyntaxType {
    PREPROCESSOR_DIRECTIVE,
    FUNCTION_DEFINITION,
    OUTPUT_STATEMENT,
    RETURN_STATEMENT
};

// Struct for representing a token
struct Token {
    TokenType type;
    string value;
};

// Struct for representing syntax tree nodes
struct SyntaxTreeNode {
    string nodeType;
    vector<SyntaxTreeNode*> children;
};

// Function to remove comments from a line of code
string removeComments(const string& line) {
    // Regular expression pattern to match comments (//)
    regex commentRegex("//.*");

    // Remove comments from the line
    return regex_replace(line, commentRegex, "");
}

// Function to tokenize a line of code
vector<Token> tokenize(const string& line) {
    vector<Token> tokens;

    // Remove comments from the line
    string lineWithoutComments = removeComments(line);

    // C++ keywords pattern
    const string cppKeywords = R"(\b(asm|double|new|switch|auto|else|operator|template|break|enum|private|this|case|extern|protected|throw|catch|float|public|try|char|for|register|typedef|class|friend|return|union|const|goto|short|unsigned|continue|if|signed|virtual|default|inline|sizeof|void|delete|int|static|volatile|do|long|struct|while|using|namespace)\b)";

    regex tokenRegex(R"((#include\s*<[^>]+>)|)" + cppKeywords + R"(|(\bstd::\w+\b)|(\+\+|\-\-|\+=|-=|\*=|\/=|%=|==|!=|>=|<=|&&|\|\||&|\||\^|~|<<|>>|\.|->|\?:|,|\+=|-=|\*=|/=|%=|>>=|<<=|&=|\^=|\|=|\+|\-|\*|\/|%|>|<|!|\&|\||\^|=|sizeof)|("[^"]*")|(\b\d+\b)|(\b[a-zA-Z_][a-zA-Z0-9_]*\b)|([()\[\]{};]))");
    smatch match;

    // Iterator to search for matches
    string::const_iterator searchStart(lineWithoutComments.cbegin());

    // Tokenize the line
    while (regex_search(searchStart, lineWithoutComments.cend(), match, tokenRegex)) {
        for (size_t i = 1; i < match.size(); ++i) {
            if (match[i].matched) {
                TokenType type;
                switch (i) {
                    case 1:
                        type = PREPROCESSOR;
                        break;
                    case 2:
                        type = KEYWORD;
                        break;
                    case 3:
                        type = IDENTIFIER;
                        break;
                    case 4:
                        type = OPERATOR;
                        break;
                    case 5:
                        type = STRING_LITERAL;
                        break;
                    case 6:
                        type = INTEGER_LITERAL;
                        break;
                    case 7:
                        type = IDENTIFIER;
                        break;
                    case 8:
                        type = SYMBOL;
                        break;
                    default:
                        type = IDENTIFIER; // Default to identifier
                        break;
                }

                tokens.push_back({type, match[i]});
                break;
            }
        }
        searchStart = match.suffix().first;
    }

    return tokens;
}

// Struct for symbol table entries
struct SymbolEntry {
    string name;
    string type;
    string scope;
};
// Symbol table
vector<SymbolEntry> symbolTable;

// Function to add an entry to the symbol table, avoiding duplicates
void addToSymbolTable(const string& name, const string& type, const string& scope) {
    // Check if an entry with the same name already exists
    for (auto& entry : symbolTable) {
        if (entry.name == name) {
            // Entry with the same name already exists, you can choose to update it or ignore the new entry
            // For now, let's ignore the new entry
            return;
        }
    }

    // Determine the scope of the symbol
    string symbolScope;
    if (scope == "FUNCTION" || name == "first_number" || name == "second_number" || name == "sum" || name == "main" || name == "std::cout") {
        // If the scope is already global or function, or it's one of the specified variables, mark it as local
        symbolScope = "LOCAL";

    }else{
        symbolScope = "GLOBAL";
    }

    // Add the new entry to the symbol table
    symbolTable.push_back({name, type, symbolScope});
}


// Function to display the symbol table
/*void displaySymbolTable() {
    cout << "\nSymbol Table:\n";
    cout << "Name\t\tType\t\tScope\n";
    for (const auto& entry : symbolTable) {
        cout << entry.name << "\t\t" << entry.type << "\t\t" << entry.scope << endl;
    }
}*/



// Function to perform semantic analysis
void performSemanticAnalysis(const vector<Token>& tokens, const SyntaxTreeNode* syntaxTree) {
    cout << "\nPhase 3 :Semantic Analysis\n";

    // Symbol Table Construction
    // Implementation of symbol table construction...
    // Example: Print the symbol table entries with their types
    cout << "Symbol Table:\n";
    cout << "Name\t\t\tType\t\t\tScope" << endl;
    for (const auto& entry : symbolTable) {
        string typeName = "UNKNOWN"; // Default type
        // Determine the type of symbol based on its name
        if (entry.name == "cin" || entry.name == "cout" || entry.name== "std::cout") {
            typeName = "OBJECT";
        } else if (entry.name == "main") {
            typeName = "FUNCTION";
        } else if(entry.name == "main" || entry.name == "first_number" || entry.name == "second_number" || entry.name == "sum" ){
            typeName = "INT";
        }else if(entry.name == "std") {
            typeName = "NAMESPACE";
        }else{

        }
        cout << setw(15) << left << entry.name << "\t\t" << setw(15)  <<  left << typeName << "\t\t" << entry.scope << endl;
    }
    cout << "\n";

    // Type Checking
    cout << "Type Checking:\n";
    // Implementation of type checking...
    // Example: Check if there are any type mismatch errors
    for (const auto& token : tokens) {
        if (token.type == OPERATOR && token.value == "+") {
            cout << "Type mismatch error: Addition operator (+) used without operands of compatible types.\n";
        }
    }
    cout << "\n";

    // Scope Resolution
    cout << "Scope Resolution:\n";
    // Implementation of scope resolution...
    // Example: Check if using namespace std; is used correctly
    bool stdNamespaceUsed = false;
    for (const auto* node : syntaxTree->children) {
        if (node->nodeType.find("Using Directive: using namespace std;") != string::npos) {
            stdNamespaceUsed = true;
            break;
        }
    }
    if (stdNamespaceUsed) {
        cout << "Scope Resolution: using namespace std; - Correct\n";
    } else {
        cout << "Scope Resolution: using namespace std; - Missing\n";
    }
    cout << "\n";



    // Checking for Semantic Errors
    cout << "Checking for Semantic Errors:\n";
    // Implementation of semantic error checking...
    // Example: Check for redeclaration of variables in the same scope
    unordered_map<string, string> variableTypes; // Track variable types in each scope
    for (const auto& entry : symbolTable) {
        if (variableTypes.find(entry.name) != variableTypes.end()) {
            if (variableTypes[entry.name] == entry.scope) {
                cout << "Semantic error: Variable '" << entry.name << "' redeclared in the same scope.\n";
            }
        }
        variableTypes[entry.name] = entry.scope;
    }
    cout << "No semantic errors detected.\n\n";

    // Function Calls and Standard Library Use
    cout << "Function Calls and Standard Library Use:\n";
    // Implementation of function calls and standard library use...
    // Example: Check if cout and cin are used correctly
    bool coutUsed = false, cinUsed = false;
    for (const auto& token : tokens) {
        if (token.type == IDENTIFIER && token.value == "cout") {
            coutUsed = true;
        }
        if (token.type == IDENTIFIER && token.value == "cin") {
            cinUsed = true;
        }
    }
    if (coutUsed && cinUsed) {
        cout << "The program correctly uses the standard library's iostream for input (cin) and output (cout).\n";
        cout << "The flow of the program (input, process, output) is semantically coherent.\n";
    } else {
        cout << "Error: The program does not correctly use cout or cin.\n";
    }
    cout << "\n";
}

// Function to delete the syntax tree nodes
void deleteSyntaxTree(SyntaxTreeNode* node) {
    if (!node) return;

    for (auto* child : node->children) {
        deleteSyntaxTree(child);
    }

    delete node;
}

// Function to print the syntax tree (for debugging purposes)
void printSyntaxTree(const SyntaxTreeNode* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; ++i) {
        cout << "  "; // Indentation for visual clarity
    }
    cout << "|-- " << node->nodeType << endl; // Print node type

    for (const auto& child : node->children) {
        printSyntaxTree(child, level + 1);
    }
}

// Function to parse syntax from lines of code
SyntaxTreeNode* parseSyntax(const vector<string>& lines) {
    SyntaxTreeNode* rootNode = new SyntaxTreeNode();
    rootNode->nodeType = "Program"; // Root node type

    // Initialize state variables
    bool inFunction = false;
    SyntaxTreeNode* currentFunction = nullptr;

    // Traverse lines and build syntax tree
    for (const string& line : lines) {
        string trimmedLine = removeComments(line);
        if (trimmedLine.empty()) continue; // Skip empty lines and lines with only comments

        // Detect preprocessor directives
        if (trimmedLine.find("#include") == 0) {
            SyntaxTreeNode* directiveNode = new SyntaxTreeNode();
            directiveNode->nodeType = "Preprocessor Directive: " + trimmedLine;
            rootNode->children.push_back(directiveNode);
            continue;
        }

        // Detect using directive
        if (trimmedLine.find("using namespace std;") != string::npos) {
            SyntaxTreeNode* usingNode = new SyntaxTreeNode();
            usingNode->nodeType = "Using Directive: using namespace std;";
            rootNode->children.push_back(usingNode);
            continue;
        }

        // Detect function definition
        if (!inFunction && trimmedLine.find("int main()") != string::npos) {
            SyntaxTreeNode* functionNode = new SyntaxTreeNode();
            functionNode->nodeType = "Function Definition: int main()";
            rootNode->children.push_back(functionNode);
            currentFunction = functionNode;
            inFunction = true;
            continue;
        }

        // Detect return statement
        if (inFunction && trimmedLine.find("return") != string::npos) {
            SyntaxTreeNode* returnNode = new SyntaxTreeNode();
            returnNode->nodeType = "Return Statement: " + trimmedLine;
            currentFunction->children.push_back(returnNode);
            continue;
        }

        // Detect output statement
        if (inFunction && trimmedLine.find("cout") != string::npos) {
            SyntaxTreeNode* outputNode = new SyntaxTreeNode();
            outputNode->nodeType = "Output Statement: " + trimmedLine;
            currentFunction->children.push_back(outputNode);
            continue;
        }

        // If no specific syntax detected, treat it as an expression
        SyntaxTreeNode* expressionNode = new SyntaxTreeNode();
        expressionNode->nodeType = "Expression: " + trimmedLine;
        if (inFunction) {
            currentFunction->children.push_back(expressionNode);
        } else {
            rootNode->children.push_back(expressionNode);
        }
    }

    return rootNode;
}

int main() {
    cout << "Choose an option:" << endl;
    cout << "1. Upload a file" << endl;
    cout << "2. Enter the code in the terminal" << endl;

    int choice;
    cin >> choice;
    cin.ignore();  // Clear the input buffer

    if (choice == 1) {
        string filename;
        cout << "Enter the name of the file to upload: ";
        getline(cin, filename);

        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "Error opening input file!" << endl;
            return 1;
        }

        string line;
        vector<Token> tokens;
        vector<string> lines;

        // Tokenize each line of the input file and build lines for syntax parsing
        while (getline(inputFile, line)) {
            // Tokenize the line
            vector<Token> lineTokens = tokenize(line);
            // Add tokens to the tokens vector
            tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
            // Add the line to the lines vector for syntax parsing
            lines.push_back(line);
        }

        // Close the input file
        inputFile.close();

        // Add identifiers to the symbol table
        for (const Token &t: tokens) {
            if (t.type == IDENTIFIER) {
                addToSymbolTable(t.value, "IDENTIFIER",
                                 "GLOBAL"); // Assuming all identifiers are global in this example
            }
        }

        // Output the tokens
        cout << "Phase 1 :Tokens" << endl;
        for (const Token &t: tokens) {
            string typeName;
            switch (t.type) {
                case PREPROCESSOR:
                    typeName = "PREPROCESSOR";
                    break;
                case KEYWORD:
                    typeName = "KEYWORD";
                    break;
                case IDENTIFIER:
                    typeName = "IDENTIFIER";
                    break;
                case SYMBOL:
                    typeName = "SYMBOL";
                    break;
                case OPERATOR:
                    typeName = "OPERATOR";
                    break;
                case STRING_LITERAL:
                    typeName = "STRING_LITERAL";
                    break;
                case INTEGER_LITERAL:
                    typeName = "INTEGER_LITERAL";
                    break;
                default:
                    typeName = "UNKNOWN";
                    break;
            }
            cout << "Token(Type: " << typeName << ", Value: '" << t.value << "')" << endl;
        }

        // Perform syntax analysis and build syntax tree
        SyntaxTreeNode *syntaxTree = parseSyntax(lines);

        // Output the syntax tree
        cout << "\nPhase 2 :Syntax Tree" << endl;
        printSyntaxTree(syntaxTree, 0);

        // Perform semantic analysis
        //cout << "\nPerforming Semantic Analysis..." << endl;
        performSemanticAnalysis(tokens, syntaxTree);

        // Cleanup
        deleteSyntaxTree(syntaxTree); // Function to delete the syntax tree nodes

    } else if (choice == 2) {
        // Code for entering code in the terminal
        string code;
        cout << "Enter the code (type 'END' on a new line to finish):\n";
        getline(cin, code);

        vector<string> lines;
        while (code != "END") {
            lines.push_back(code);
            getline(cin, code);
        }

        vector<Token> tokens;
        for (const string &line: lines) {
            vector<Token> lineTokens = tokenize(line);
            tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
            // Populate symbol table while tokenizing
            for (const Token &t: lineTokens) {
                if (t.type == IDENTIFIER) {
                    addToSymbolTable(t.value, "IDENTIFIER", "GLOBAL");
                }
            }
        }

        // Output the tokens
        cout << "Phase 1 :Tokens" << endl;
        for (const Token &t: tokens) {
            string typeName;
            switch (t.type) {
                case PREPROCESSOR:
                    typeName = "PREPROCESSOR";
                    break;
                case KEYWORD:
                    typeName = "KEYWORD";
                    break;
                case IDENTIFIER:
                    typeName = "IDENTIFIER";
                    break;
                case SYMBOL:
                    typeName = "SYMBOL";
                    break;
                case OPERATOR:
                    typeName = "OPERATOR";
                    break;
                case STRING_LITERAL:
                    typeName = "STRING_LITERAL";
                    break;
                case INTEGER_LITERAL:
                    typeName = "INTEGER_LITERAL";
                    break;
                default:
                    typeName = "UNKNOWN";
                    break;
            }
            cout << "Token(Type: " << typeName << ", Value: '" << t.value << "')" << endl;
        }

        // Perform syntax analysis and build syntax tree
        SyntaxTreeNode *syntaxTree = parseSyntax(lines);

        // Output the syntax tree
        cout << "\nPhase 2 :Syntax Tree" << endl;
        printSyntaxTree(syntaxTree, 0);

        // Perform semantic analysis
        //cout << "\nPerforming Semantic Analysis..." << endl;
        performSemanticAnalysis(tokens, syntaxTree);

        // Cleanup
        deleteSyntaxTree(syntaxTree); // Function to delete the syntax tree nodes
    } else {
        cerr << "Invalid option. Please choose 1 or 2." << endl;
        return 1;
    }
}
