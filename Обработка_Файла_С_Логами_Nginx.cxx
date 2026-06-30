#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <algorithm>
using namespace std;


int main(int argc, char *argv[]) // argc - количество аргуметов, argv[] - массив с аргументами из командной строки
{
	fstream fs; // fstream - поток для чтения-записи
	ifstream in; // ifstream - поток для записи
	
	string name = "*Введите имя файла аргументом командной строки";
	if (argv[1] != nullptr){
	    name = argv[1];
	    } else cout << name << endl;
	
	string line;
	vector<string> file = {};
	vector<string> status_arr = {};
	unordered_map<string, string> status_code_hash = {
	    {" 200 ", "OK"},
	    {" 201 ", "Created"},
	    {" 202 ",  "Accepted"},
	    {" 206 ", "Partial Content"},
	    {" 301 ", "Moved Permanently"},
	    {" 302 ",  "Found"},
	    {" 304 ", "Not Modified"},
	    {" 400 ",  "Bad Request"},
	    {" 403 ",  "Forbidden"},
	    {" 404 ",  "Not Found"}
	    };
	string first_valid_time = "";
	string last_valid_time = "";   
	
	bool booler1 = true;
	bool booler2 = true; 
	
	float NotFoundCounter = 0;    
	int invTime_counter = 0;
	int counter = 0;
	
	in.open(name);
	if (in.is_open()){
	    while(getline(in, line) && counter < 2000){
	           //file += line;
	           file.push_back(line); // line - это как-бы одна строка
	           counter++;
	           
	           auto text = line;
	           //cout << text << endl << endl;
	
	           //regex_match() - возвращает                 true только когда совпадает                   вся входная            последовательность
	           //regex_search() - возвращает                 true даже если только часть                   последовательности               соответствует регулярному выражению
	
	 const auto r_time = regex(R"([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+ - - |[ ]["].+)");
	string time = regex_replace(text ,r_time, "*"); // работает как re.sub в python
	time = time.replace(22, 30, 1, ']');
	time = time.replace(0, 2, 1, '[');
	
	const auto r2 = regex(R"(\[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}\])");
	if (!regex_match(time, r2)){ 
	time = "INVALID_TIME";
	invTime_counter++;}
	
	if (regex_match(time, r2) && booler1){     first_valid_time = time;
	booler1 = false;}
	if(regex_match(time, r2)) last_valid_time = time;
	
	
	const auto r_status = regex(R"(.+1\.[01]"[ ]|"http.+|"-".+)");
	string status = regex_replace(text, r_status, "@");
	status = status.replace(4, 12, 1, ' ');
	status = status.replace(0, 1,  1, ' ');
	//cout << status << endl;
	
	const auto r3 = regex(R"([ ][0-9]{3}[ ])");
	
	if(!regex_match(status, r3)) status = "UNKNOWN";
	
	// Далее наверное надо будет создать unordered_map
	
    for (auto el : status_code_hash){
        if (el.first == status){
          status = el.second;
          }
        }
     cout << status  << endl;
     status_arr.push_back(status);
	        }
	}
	in.close();
	
	cout << "----------------СВОДКА ИНФОРМАЦИИ О .log ФАЙЛЕ--------------" << endl;
	
	
	cout << "Всего строк:" << counter << endl; 
	
	for (auto el : status_code_hash){
	    cout << el.second << ":" << count(status_arr.begin(), status_arr.end(), el.second)  << endl;
	    if (el.second == "Not Found") NotFoundCounter = count(status_arr.begin(), status_arr.end(), el.second);
	}
	cout << "INVALID_TIME:" << invTime_counter << endl;
	
	if (first_valid_time != "" && last_valid_time != ""){
	cout << "first_valid_time: " << first_valid_time << "\n" << "last_valid_time: " << last_valid_time << endl;}
	else cout << "No valid timestaps" << endl;
	
	cout << "Число ошибок: " << NotFoundCounter << "\t" << "Процент от всех строк: ";
	printf("%.1f", NotFoundCounter / float(counter));
	cout << endl;
	
}