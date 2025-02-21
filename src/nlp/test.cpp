//
// Created by Bhargav Guntupalli on 2/21/25.
//

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// A simple struct to hold configuration, such as endpoint URL
struct SbertClientConfig {
    std::string endpointUrl; // e.g. "http://localhost:8000/compare"
};

// A helper for capturing libcurl response data in a std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Our class that sends text to the local SBERT API and gets the result
class SbertClient {
public:
    // Constructor that takes a config struct (with endpoint URL, etc.)
    explicit SbertClient(const SbertClientConfig& config)
            : endpointUrl_(config.endpointUrl)
    {
        // Initialize CURL once per program (best practice)
        curl_global_init(CURL_GLOBAL_ALL);
    }

    // Destructor cleans up global resources
    ~SbertClient() {
        curl_global_cleanup();
    }

    // Sends the text to the SBERT API and returns the closest keyword
    // or an empty string on error.
    std::string getClosestKeyword(const std::string& text) {
        // Prepare JSON body: {"text": "user's sentence"}
        nlohmann::json requestBody;
        requestBody["text"] = text;
        std::string jsonString = requestBody.dump();

        // We'll store the response here
        std::string responseStr;

        // Initialize a CURL handle
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize curl handle.\n";
            return "";
        }

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, endpointUrl_.c_str());

        // Set POST request
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Provide POST fields
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)jsonString.size());

        // Set headers (JSON content type)
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // WriteFunction callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            return "";
        }

        // Parse JSON response
        // Expecting something like:
        // {
        //   "input_sentence": "...",
        //   "scores": [[0.7, 0.3]],
        //   "closest_keyword": "exit"
        // }
        try {
            nlohmann::json responseJson = nlohmann::json::parse(responseStr);

            // The "closest_keyword" field should contain either "exit" or "help"
            if (responseJson.contains("closest_keyword")) {
                return responseJson["closest_keyword"].get<std::string>();
            } else {
                std::cerr << "Response JSON missing 'closest_keyword' field.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << "\n";
        }
        return "";
    }

private:
    std::string endpointUrl_;
};

int main() {
    // 1. Configure the client
    SbertClientConfig config;
    config.endpointUrl = "http://localhost:8000/compare";
    // If you're using Flask default port: "http://localhost:5000/compare"

    // 2. Create client object
    SbertClient client(config);

    // 3. Interactive loop
    while (true) {
        std::cout << "\nEnter a sentence (type 'quit' to exit): ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            // End if EOF or input stream error
            break;
        }

        if (input == "quit") {
            // Stop the loop
            std::cout << "Exiting...\n";
            break;
        }

        // 4. Send the input to the SBERT API
        std::string result = client.getClosestKeyword(input);

        // 5. Print the result
        if (!result.empty()) {
            std::cout << "The sentence's meaning is closer to: " << result << "\n";
        } else {
            std::cout << "Could not determine closest keyword.\n";
        }
    }

    return 0;
}
