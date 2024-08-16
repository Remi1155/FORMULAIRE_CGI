#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

// Fonction pour décoder l'URL
std::string urlDecode(const std::string &src);

int main()
{
    // Affichage de l'entête HTTP
    std::cout << "Content-type: text/html\n\n";

    // Récupération de la chaîne de requête transmise via GET
    std::string queryString;
    char *queryStr = getenv("QUERY_STRING");
    if (queryStr != nullptr)
    {
        queryString = queryStr;
    }

    // Extraction des valeurs des champs login et password du GET
    std::string extractedLogin, extractedPassword;
    size_t loginPos = queryString.find("login=");
    size_t passwordPos = queryString.find("&password=");

    if (loginPos != std::string::npos && passwordPos != std::string::npos)
    {
        extractedLogin = urlDecode(queryString.substr(loginPos + 6, passwordPos - (loginPos + 6)));
        extractedPassword = urlDecode(queryString.substr(passwordPos + 10));
    }

    // Lecture du fichier contenant les informations de login et password corrects
    std::vector<std::string> logins, passwords;
    std::ifstream credentialsFile("loginPassword.txt");
    if (credentialsFile.is_open())
    {
        std::string line;
        while (getline(credentialsFile, line))
        {
            std::string login, password;
            std::stringstream ss(line);
            if (ss >> login >> password)
            {
                logins.push_back(login);
                passwords.push_back(password);
            }
        }
        credentialsFile.close();
    }
    else
    {
        std::cerr << "Erreur lors de la lecture du fichier contenant le login et le password";
    }

    // Génération de la réponse HTML
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<title>Inscription CGI</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";

    // Vérification si les informations de login et de password sont correctes
    bool isAuthenticated = false;
    for (size_t i = 0; i < logins.size(); ++i)
    {
        if (extractedLogin == logins[i] && extractedPassword == passwords[i])
        {
            isAuthenticated = true;
            break;
        }
    }

    if (isAuthenticated)
    {
        std::cout << "<h1>Welcome " << extractedLogin << "!</h1>\n";
        std::cout << "<p>User: " << extractedLogin << "</p>\n";
        std::cout << "<p>Password: " << extractedPassword << "</p>\n";
    }
    else
    {
        // Affichage du formulaire de connexion si les informations sont incorrectes
        std::cout << "<h2>Inscription</h2>\n";
        std::cout << "<form method='get' action='/cgi-bin/login_form.cgi'>";
        std::cout << "<label for='login'>Login:</label>";
        std::cout << "<input type='text' id='login' name='login' value='" << extractedLogin << "'><br><br>";
        std::cout << "<label for='password'>Password:</label>";
        std::cout << "<input type='password' id='password' name='password' value='" << extractedPassword << "'><br><br>";
        std::cout << "<input type='submit' value='Submit'>";
        std::cout << "</form>";
    }

    std::cout << "</body>\n";
    std::cout << "</html>\n";

    return 0;
}

// Fonction pour décoder les caractères encodés dans l'URL (par exemple, %20 pour un espace)
std::string urlDecode(const std::string &src)
{
    std::string decodedString;
    char decodedChar;
    int hexValue;
    for (size_t i = 0; i < src.length(); i++)
    {
        if (src[i] == '%')
        {
            sscanf(src.substr(i + 1, 2).c_str(), "%x", &hexValue);
            decodedChar = static_cast<char>(hexValue);
            decodedString += decodedChar;
            i += 2; // Passage aux deux caractères suivants (hexadécimal)
        }
        else
        {
            decodedString += src[i];
        }
    }
    return decodedString;
}