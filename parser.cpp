#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <fstream>
#include "lazycsv.hpp"
#include "json.hpp"

using json = nlohmann::json;

//This function finds a substring in a mainstring
//This is used for finding
    bool findSubstring(const std::string& mainString, const std::string& subString) {
    size_t found = mainString.find(subString);
    if (found != std::string::npos) {
        return true; // Return the starting index of the substring
    } else {
        return false; // Substring not found
    }
}


int main() {
    
    //KEYWORDS TO RECOGNIZE CATEGORIES

    /*
    To add new categories:
        - Add <CATEGORY>keyWords vector with any key words found in description
        - Add <CATEGORY>keyWords vector to the keyWords vector
        - Add <CATEGORY> to categories vector
        - Add .JSON file to jsonNames vector
    */
    std::vector<std::string> foodKeyWords = {"DOORDASH", "PIZZA", "PIZZERIA", "POPEYES"};
    std::vector<std::string> creditCardKeyWords = {"CRCARDPMT"};
    std::vector<std::string> cannabisKeyWords = {"STRAIN"};
    std::vector<std::string> nicotineKeyWords = {"SMOKE", "CONVEN"};
    std::vector<std::string> subscriptionKeyWords = {"OPENAI", "NINTENDO", "aws.amazon", "SQSP", "Patreon", "TWITCH", "PARAMOUNT", "MIDJOURNEY"};
    std::vector<std::string> transportationKeyWords = {"MTA*NYCT", "MTA*LIRR"};
    std::vector<std::string> groceryKeyWords = {"STOP & SHOP", "CVS", "WHOLEFDS"};
    std::vector<std::string> entertainmentKeyWords = {"LEGO", "GUITAR", "Steam", "MUJI"};

    std::vector<std::vector<std::string>> keyWords = {
        foodKeyWords, 
        creditCardKeyWords, 
        cannabisKeyWords, 
        nicotineKeyWords, 
        subscriptionKeyWords, 
        transportationKeyWords, 
        groceryKeyWords, 
        entertainmentKeyWords
    };

    std::vector<std::string> categories = {
        "food", 
        "creditCard", 
        "cannabis", 
        "nicotine", 
        "subscriptions", 
        "transportation", 
        "grocery", 
        "entertainment"
    };

    std::unordered_map<std::string, json> category_map;

    const auto start = std::chrono::system_clock::now();


    // Create a parser object for example.CSV
    lazycsv::parser parser{ "example.CSV" };

    //initialization of empty json arrays for each category
    for(int i = 0; i < categories.size(); i++) {
        category_map[categories[i]] = json::array();
    }
    
    // Iterate through each row in the CSV
    for(const auto row : parser) {
      

        //collect data for each row 
        const auto [type, date, description, amount] = row.cells(0, 1, 2, 3);
        
        //iterate through each batch of keywords
        for(int i = 0; i < keyWords.size(); i++) {
            //iterate through each word in each batch
            for(int j = 0; j < keyWords[i].size(); j++) {
                //look for keyword in the current row's description
                bool x = findSubstring(std::string(description.raw()), keyWords[i][j]);

                //if keyword is found
                if(x) {
                    
                    //initialize json object
                    json object;

                    //add in values to json object
                    object["description"] = description.raw();
                    object["amount"] = amount.raw();
                    object["date"] = date.raw();

                    //push object back onto category map (previously initialized json arrays)
                    category_map[categories[i]].push_back(object);

                }
            }
        }
    }

    std::vector<std::string> jsonNames = {
        "json_data/food.json", 
        "json_data/creditCard.json", 
        "json_data/cannabis.json", 
        "json_data/nicotine.json", 
        "json_data/subscriptions.json", 
        "json_data/transportation.json", 
        "json_data/grocery.json", 
        "json_data/entertainment.json"
    };


    //iterate through categories
   for(int i = 0; i < categories.size(); i++) {
        //initialize output file
        std::ofstream outfile(jsonNames[i]);

        //insert json objects from category_map into json file
        outfile << category_map[categories[i]].dump(4);
   }

    
    const auto end = std::chrono::system_clock::now();
    const auto duration = end - start;

    std::cout<<std::endl<<"Program lasted: "<<duration<<std::endl;


    return 0;
}
