#include <iostream>
#include <fstream>
#include <string>
#include <csv.h>
#include <curl/curl.h>

using namespace std;

public string generateUrl(string stockName, stockKey) {
    string functionName = "TIME_SERIES_INTRADAY_EXTENDED";
    string symbol = stockName;
    string interval = "5min";
    string adjusted = "true";
    
    int month = 1;

    string slice = "year1month" + to_string(month);
    

    return "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY_EXTENDED&symbol=" + symbol + "&interval=" + interval 
        + "&slice=" + slice + "&adjusted=" + adjusted + "&apikey=" + stockKey; 
}

public void generateStocks(string stockName, string stockKey) {

    string url = generateUrl(stockName, stockKey);

    // Curl request and CSV generation was made with openai
    CURL *curl;
    CURLcode res;
    string responseData;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return 1;
        }
        curl_easy_cleanup(curl)
    }

    vector<map<string, string>> data;
    io::CSVReader<0> header_reader(responseData);
    map<string, string> row;
    while (header_reader.read_row()) {
        stringstream ss;
        ss << header_reader.get_file().get();
        io::CSVReader<0> reader(ss.str());
        reader.read_header(io::ignore_extra_column, row);
        reader.read_row(row);
        data.push_back(row);
    }

    String outputCsv = stockName + ".csv";
    io::CSVWriter<0> writer(outputCsv);
    writer.write_header(data[0]);
    for (const auto& row : data) {
        writer.write_row(row);
    }
    




    cout << api;
    cout << "\n";
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, string *data) {
    size_t bytes = size * nmemb;
    data->append(ptr, bytes);
    return bytes;
}

string getStockKey() {
    fstream newFile;
    newFile.open("StockKey.txt", ios::in);
    if(newFile.is_open()) {
        std::string key;
        getline(newFile, key);
        newFile.close();
        return key;
    } else {
        return "";
    }
    
}

int main(int argc, char *argv[]) {

    // Get Stock Key
    string stockKey = getStockKey();
    if (stockKey == ""){
        cout << "Could not find stock key in StockKey.txt";
        return 0;
    }
    cout << stockKey;

    for (int i = 1; i < argc; i++) {
        generateStocks(argv[i], stockKey);
    }
    return 0;
}