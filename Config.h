#include <string>
#include <fstream>
#include <map>

class Config {
public:
	Config(std::string file_name) : m_file_name(std::move(file_name)) {}
	
	void Init() {
		std::ifstream ifs(m_file_name.c_str());
		if (!ifs.is_open()) {
			return;
		}
		std::string line;
		while (std::getline(ifs, line)) {
			auto mid_idx = line.find('=');
			auto key = line.substr(0, mid_idx);
			auto value = line.substr(mid_idx+1);
			m_cnf_map[key] = value;
		}
		ifs.close();
	}
	
	std::string operator[] (std::string key) {
		if (m_cnf_map.find(key) != m_cnf_map.end()) {
			return m_cnf_map[key];
		} else {
			return "";
		}
	}
private:
	std::string m_file_name;
	std::map<std::string, std::string> m_cnf_map;
};

/*
class Config {
public:
	Config(std::string file_name) : m_file_name(std::move(file_name)) {}
	
	void Init() {
		std::ifstream ifs(m_file_name.c_str());
		if (!ifs.is_open()) {
			return;
		}
		std::string line;
		char key[20], value[20];
		while (std::getline(ifs, line)) {
			sscanf(line.c_str(), "%[^=]=%[^\n]\n", key, value);
			m_cnf_map[key] = value;
		}
		ifs.close();
	}
	
	std::string operator[] (std::string key) {
		if (m_cnf_map.find(key) != m_cnf_map.end()) {
			return m_cnf_map[key];
		} else {
			return "";
		}
	}
private:
	std::string m_file_name;
	std::map<std::string, std::string> m_cnf_map;
};
*/
