#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <limits.h>
#include <iomanip>
#include <algorithm>
#include <ctype.h>
#include <assert.h>

enum NumberType {
	Hex,
	Octal,
	Binary,
	Decimal
};
enum ValueType {
	Any,
	Number,
	String,
};

bool is_value_type_allowed (ValueType restricter, ValueType val) {
	return restricter == ValueType::Any || restricter == val;
}

bool is_octal_digit (char c) {
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' ||
		c == '6' || c == '7';
}

uint8_t parse_octal_digit (char c) {
	if (c == '0') return 0;
	else if (c == '1') return 1;
	else if (c == '2') return 2;
	else if (c == '3') return 3;
	else if (c == '4') return 4;
	else if (c == '5') return 5;
	else if (c == '6') return 6;
	else if (c == '7') return 7;
	else {
		std::cout << "Offending char: '" << c << "'" << std::endl;
		throw std::runtime_error("Invalid octal digit.");
	}
}

bool is_binary_digit (char c) {
	return c == '0' || c == '1';
}

char to_octal_digit (uint8_t v) {
	if (v == 0) return '0';
	else if (v == 1) return '1';
	else if (v == 2) return '2';
	else if (v == 3) return '3';
	else if (v == 4) return '4';
	else if (v == 5) return '5';
	else if (v == 6) return '6';
	else if (v == 7) return '7';
	else {
		// TODO: throw error
		std::cout << "Offending value: '" << unsigned(v) << "'" << std::endl;
		throw std::runtime_error("Invalid octal number.");
	}
}
uint8_t parse_decimal_digit (char c) {
	if (is_octal_digit(c)) return parse_octal_digit(c);
	else if (c == '8') return 8;
	else if (c == '9') return 9;
	else {
		// TODO: throw error
		std::cout << "Invalid Decimal Digit: '" << c << "'" << std::endl;
		return 0;
	}
}
char to_decimal_digit (uint8_t v) {
	if (v <= 7) return to_octal_digit(v);
	else if (v == 8) return '8';
	else if (v == 9) return '9';
	else {
		// TODO: throw error
		std::cout << "Invalid Decimal Number: '" << unsigned(v) << "'" << std::endl;
		return 0;
	}
}
uint8_t parse_hex_digit (char c) {
	if (std::isdigit(c)) return parse_decimal_digit(c);
	else if (c == 'a' || c == 'A') return 10;
	else if (c == 'b' || c == 'B') return 11;
	else if (c == 'c' || c == 'C') return 12;
	else if (c == 'd' || c == 'D') return 13;
	else if (c == 'e' || c == 'E') return 14;
	else if (c == 'f' || c == 'F') return 15;
	else {
		// TODO: throw error
		std::cout << "Invalid Hex Digit: '" << c << "'" << std::endl;
		return 0;
	}
}
char to_hex_digit (uint8_t v) {
	if (v <= 9) return to_decimal_digit(v);
	else if (v == 10) return 'a';
	else if (v == 11) return 'b';
	else if (v == 12) return 'c';
	else if (v == 13) return 'd';
	else if (v == 14) return 'e';
	else if (v == 15) return 'f';
	else {
		// TODO: throw error
		std::cout << "Invalid Hex Number: '" << unsigned(v) << "'" << std::endl;
		return 0;
	}
}
uint8_t parse_two_hex_digits (char a, char b) {
	return (parse_hex_digit(a) * 16) + parse_hex_digit(b);
}

// This function was written by Kainui
bool is_str_odd(std::string x){
  if(x[x.length()-1]%2 == 1){
    return true;
  }
  return false;
}

// This function was written by Kainui
bool is_str_zero(std::string x){
  return (x.length() == 1) && x[0] == '0';
}

// This function was written by Kainui
//returns x/2
std::string str_rshift(std::string x){
  	if(is_str_odd(x)){
    	x[x.length()-1]-=1;
	}
	std::string result = "";
	char temp = '0';
	for(size_t i = 0; i < x.length(); i++) {
		temp = x[i] - '0';
		if(temp % 2 != 0){
			temp--;
			x[i+1] += 10;
		}

		result += temp / 2 + '0';
	}
	if(result[0] == '0'){
		if(result.length() == 1){
			return "0";
		}
		result.erase(0,1);
	}
	return result;
}

unsigned long sim_pow (unsigned long base, int exponent) {
	unsigned long res = 1;

	for (int i = 0; i < exponent; i++) {
		res *= base;
	}

	return res;
}
// This function was written by Kainui
std::vector<uint8_t> decimalStringToByteVector(std::string x){
	//std::vector<uint8_t> byteVector(floor(x.length() / log10(16)) - 1);
	std::vector<uint8_t> byteVector;
	uint8_t currentByte;
	while(!is_str_zero(x)){
		currentByte = 0;
		for(int i = 0; i < 8; i++){
			if(is_str_odd(x)){
				currentByte += 1 << i;
			}
			x = str_rshift(x);
		}
		byteVector.insert(byteVector.begin(), currentByte);
	}


	return byteVector;
}

std::vector<uint8_t> parse_dec_string_to_bytes (std::string str) {
	char* endptr;
	const char* nstr = str.c_str();
	unsigned long res_v = strtoul(nstr, &endptr, 10);

	if (endptr == nstr && res_v == 0) {
		throw std::runtime_error("Failed to parse decimal data, perhaps it was too large. The handling of decimal is simplistic and limited by system.");
	}

	std::vector<uint8_t> res;

	while (res_v != 0) {
		std::uint8_t byte = res_v & 0xFF;
		res.insert(res.begin(), byte);

		res_v >>= 8;
	}

	return res;
}
std::vector<uint8_t> parse_hex_string_to_bytes (std::string str) {
	std::vector<uint8_t> res;
	if (str == "0" || str == "00") {
		res.push_back(0);
		return res;
	}

	if (str.length() % 2 == 1) {
		str = '0' + str;
	}
	for (size_t i = 0; i < str.length();) {
		if ((i+1) < str.length()) { // there's two bytes
			res.insert(res.begin(), parse_two_hex_digits(str[i], str[i + 1]));
			i += 2;
		} else {
			res.insert(res.begin(), parse_two_hex_digits('0', str[i]));
			i++;
		}
	}
	std::reverse(res.begin(), res.end());
	return res;
}
std::string repeat_char (char c, size_t amount) {
	std::string res;
	for (size_t j = 0; j < amount; j++) {
		res += c;
	}
	return res;
}
uint8_t parse_binary_byte (std::string str) {
	uint8_t res = 0;
	for (size_t i = 0; i < str.length(); i++) {
		uint8_t v = 0;
		if (str[i] == '0') {
			v = 0;
		} else {
			v = 1;
		}

		unsigned long pow_res = sim_pow(2, (str.length()-1-i));

		res += pow_res * v;
	}
	return res;
}
std::vector<uint8_t> parse_bin_string_to_bytes (std::string str) {
	std::vector<uint8_t> res;

	if (str == "0" || str.length() == 0) {
		res.push_back(0);
		return res;
	}

	if (str.length() % 8 != 0) {
		str = repeat_char('0', 8 - (str.length() % 8)) + str;
	}

	for (size_t i = 0; i < str.length();) {
		res.push_back(parse_binary_byte(str.substr(i, 8)));
		i += 8;
	}
	return res;
}

template<typename T>
T bytes_to_n (std::vector<uint8_t> bytes) {
	T res = 0;

	size_t mult = 0;
	for (size_t i = bytes.size(); i > 0; i--) {
		// Each half-byte is increased by another *16 (exponents)
		T msbs = static_cast<unsigned long>(((bytes[i-1] >> 4) & 0x0F));
		T lsbs = static_cast<unsigned long>(bytes[i-1] & 0xF);
		T lpow = sim_pow(16, mult);
		T mpow = sim_pow(16, mult+1);
		res += (lsbs * lpow) + (msbs * mpow);
		mult += 2;
	}
	return res;
}

void minimum_byte_count (std::string& str, size_t min_byte_count) {
	if (str.length() % 2 == 1) {
		str = "0" + str;
	}
	while ((str.length()/2) < min_byte_count) {
		str = "00" + str;
	}
}
std::string u8_to_hex_string (uint8_t n) {
	// This works since it doesn't round up
	uint8_t a = n / 16;
	uint8_t b = n - (a*16);
	std::string res;
	res += to_hex_digit(a);
	res += to_hex_digit(b);
	return res;
}

template<typename T>
void concat_vectors (std::vector<T>& dest, std::vector<T> src) {
	for (T item : src) {
		dest.push_back(item);
	}
}

enum ExtendStyle {
	Start,
	End
};
// append_to == -1, means to start, == 1 to the end, otherwise, error
template<typename T>
void extend_vector_to_min (std::vector<T>& dest, size_t min_size, T empty_value, ExtendStyle style = ExtendStyle::Start) {
	if (dest.size() < min_size) {
		size_t amount = min_size - dest.size();
		for (size_t i = 0; i < amount; i++) {
			if (style == ExtendStyle::Start) {
				dest.insert(dest.begin(), empty_value);
			} else if (style == ExtendStyle::End) {
				dest.push_back(empty_value);
			}
		}
	}
}

enum DataType {
	None,
	Block,
	Direct
};

class DataBuffer {
	public:
	std::vector<uint8_t> vec;
	DataType type = DataType::None;

	DataBuffer () {}
	DataBuffer (DataType t_type) {
		type = t_type;
	}
	~DataBuffer () {
		empty();
	}

	bool is_valid () {
		return type != DataType::None;
	}

	void empty () {
		vec.clear();
		type = DataType::None;
	}
};

class HexSyn {
	public:
	std::ifstream file;
	std::ofstream out_file;

	HexSyn () {}
	~HexSyn () {

	}

	bool parse_files (std::string filename, std::string out_filename) {
		file.open(filename);
		if (file.fail()) {
			std::cout << "File failed to open." << std::endl;
			return false;
		}
		out_file.open(out_filename);
		if (out_file.fail()) {
			std::cout << "Out file failed to open." << std::endl;
			return false;
		}

		return parse();
	}

	private:
	bool started_parsing = false;
	std::string line = "";

	DataBuffer write_buffer = DataType();
	std::vector<DataBuffer> temp_buffers;
	DataBuffer last_buffer;
	size_t i = 0; // index

	DataBuffer& get_current_buffer () {
		if (temp_buffers.size() == 0) {
			return write_buffer;
		}
		return temp_buffers.at(temp_buffers.size() - 1);
	}

	void write_data () {
		for (uint8_t item : write_buffer.vec) {
			out_file.put(item);
		}
	}

	void store_current_bytes () {
		flush_last_stored_bytes();

		DataBuffer data = temp_buffers.at(temp_buffers.size() - 1);
		temp_buffers.pop_back();
		last_buffer = data;
	}
	void store_bytes (std::vector<uint8_t> vec) {
		flush_last_stored_bytes();
		last_buffer.vec = vec;
		last_buffer.type = DataType::Direct;
	}
	void flush_last_stored_bytes () {
		if (last_buffer.type == DataType::Block || last_buffer.type == DataType::Direct) {
			concat_vectors(get_current_buffer().vec, last_buffer.vec);
			last_buffer.empty();
		}
	}
	void flush_all () {
		flush_last_stored_bytes();
		while (temp_buffers.size() != 0) {
			DataBuffer item = temp_buffers.at(temp_buffers.size() - 1);
			temp_buffers.pop_back();
			if (item.type == DataType::Block || item.type == DataType::Direct) {
				concat_vectors(get_current_buffer().vec, item.vec);
			} else {
				std::cout << "\tCan't flush buffer of unknown type of: " << item.type << std::endl;
			}
		}
	}


	bool is_ending_character (char c) {
		return std::isspace(c) || c == '{' || c == '}' || c == ';' || c == '_';
	}
	bool in_range (size_t index) {
		return index < line.length();
	}
	bool in_range () {
		return in_range(i);
	}

	bool parse () {
		started_parsing = true;

		while (!file.eof()) {
			std::getline(file, line);

			for (i = 0; in_range();) {
				parse_char();
			}
		}

		flush_all();
		write_data();

		return true;
	}

	void parse_char () {
		if (std::isspace(line[i])) {
			i++;
		} else if (line[i] == ';') {
			// If it's beyond the length it's not gonna do anything more
			i = line.length()+1;
		} else if (line[i] == '{') {
			parse_start_block();
		} else if (line[i] == '}') {
			parse_end_block();
		} else if (try_parse_number_expression()) {
			// Do nothing, it's been handled in above function
		} else if (line[i] == '*') {
			i++;
			while (in_range() && std::isspace(line[i])) {
				// Consume whitespace
				i++;
			}

			if (!in_range()) {
				throw std::runtime_error("Found * operator but file ended before a number-expression was found");
			}

			std::vector<uint8_t> bytes = do_parse_number_expression();

			if (bytes.size() == 0) {
				throw std::runtime_error("Unknown error from trying to parse numbers.. perhaps there wasn't a number after the * operator?");
			}

			size_t times = bytes_to_n<size_t>(bytes);

			if (last_buffer.type == DataType::Block || last_buffer.type == DataType::Direct) {
				std::vector<uint8_t> temp_bytes = last_buffer.vec;
				last_buffer.vec.clear();
				for (size_t j = 0; j < times; j++) {
					concat_vectors(last_buffer.vec, temp_bytes);
				}
			} else {
				throw std::runtime_error("* operator not operating on valid block");
			}
		}
	}

	// 'try' since it may not succeed in wanting to do anything at all
	bool try_parse_number_expression () {
		std::vector<uint8_t> bytes = do_parse_number_expression();

		if (bytes.size() == 0) {
			return false;
		}

		store_bytes(bytes);
		return true;
	}

	std::vector<uint8_t> do_parse_number_expression () {
		std::vector<uint8_t> bytes;
		if (std::isxdigit(line[i])) {
			bytes = parse_number(NumberType::Hex);
		} else if (line[i] == '$') {
			bytes = parse_special();
		}
		return bytes;
	}

	std::vector<uint8_t> parse_special (ValueType allowed_type = ValueType::Any) {
		if (!in_range(i + 1)) { // over the length
			throw std::runtime_error("Found special sign ($) but was at end of line, and did not find");
		} else if (line[i] != '$') {
			throw std::runtime_error("Parse_special was called but the starting character was not a $.");
		}

		i++; // Skip $

		size_t min_byte_count = 0;
		std::vector<uint8_t> bytes;
		// $d{dec-number} or $D{min-byte-count}_{dec-number?}
		if ((line[i] == 'd' || line[i] == 'D') && is_value_type_allowed(allowed_type, ValueType::Number)) {
			// capitalized means it has a min-size of bytes
			bool found_value = false;

			i++; // skip 'D'|'d'
			if (line[i-1] == 'D') { // it has a min byte count in dec
				std::vector<uint8_t> t_bytes = parse_number(NumberType::Decimal);
				min_byte_count = bytes_to_n<size_t>(t_bytes);

				if (!in_range(i) || std::isspace(line[i])) { // Empty afterwards, so we assume that it wanted that to be all zeros
					found_value = true;
					for (size_t x = 0; x < min_byte_count; x++) {
						bytes.push_back(0x00);
					}
				} else if (line[i] == '_') {
					i++;
				} else {
					throw std::runtime_error("Unknown character after reading decimal and it's custom size. Note that capital D means the number you specify is it's min byte size, if you don't want that then a lowercase 'd'.");
				}
			}

			if (found_value) {
				return bytes;
			}

			bytes = parse_number(NumberType::Decimal, min_byte_count);
		} else if ((line[i] == 'o' || line[i] == 'O') && is_value_type_allowed(allowed_type, ValueType::Number)) {
			throw std::runtime_error("Sorry but octal is not currently implemented :(");
		} else if ((line[i] == 'b' || line[i] == 'B') && is_value_type_allowed(allowed_type, ValueType::Number)) {
			// capitalized means it has a min-size of bytes
			bool found_value = false;

			i++; // skip 'B'|'b'
			if (line[i-1] == 'B') { // it has a min byte count in dec
				std::vector<uint8_t> t_bytes = parse_number(NumberType::Decimal);
				min_byte_count = bytes_to_n<size_t>(t_bytes);

				if (!in_range(i) || std::isspace(line[i])) { // Empty afterwards, so we assume that it wanted that to be all zeros
					found_value = true;
					for (size_t x = 0; x < min_byte_count; x++) {
						bytes.push_back(0x00);
					}
				} else if (line[i] == '_') {
					i++;
				} else {
					throw std::runtime_error("Unknown character after reading decimal and it's custom size. Note that capital D means the number you specify is it's min byte size, if you don't want that then a lowercase 'd'.");
				}
			}

			if (found_value) {
				return bytes;
			}

			bytes = parse_number(NumberType::Binary, min_byte_count);
		} else if ((line[i] == 's' || line[i] == 'S') && is_value_type_allowed(allowed_type, ValueType::String)) {
			// raw string
			// Has various options that are toggled by characters after it. If it finds a " it will parse the string
			// By default string ends with a null byte, since that seems common
			bool ends_with_null_byte = true;
			bool starts_with_null_byte = false;

			bool split_bytes = false;
			// TODO: allow user to customize this
			uint8_t byte_to_split = 0x00;

			// TODO: add a 'length counter' which will be put before or after string, and can be enabled to (ex/in)clude the null byte

			i++;
			for (; in_range();) {
				if (std::isspace(line[i])) {
					throw std::runtime_error("Unexpected space whilst attempting to parse string!");
				} else if (line[i] == 'z') { // don't end with a null byte
					ends_with_null_byte = false;
					i++;
				} else if (line[i] == 'Z') {
					starts_with_null_byte = true;
					i++;
				} else if (line[i] == 'x') { //
					split_bytes = true;
					i++;
				} else if (line[i] == '"') {
					i++;
					for (; in_range();) {
						if (line[i] == '"') {
							i++;
							break;
						} else if (line[i] == '\\') {
							if (in_range(i+1)) {
								if (line[i+1] == 'n') {
									bytes.push_back(static_cast<uint8_t>('\n'));
								} else if (line[i+1] == 't') {
									bytes.push_back(static_cast<uint8_t>('\t'));
								} else {
									// This is so you can use escape sequences which aren't implemented
									// But may be in the future.
									// TODO: add a warning message that this is happening
									bytes.push_back(static_cast<uint8_t>(line[i+1]));
								}
								i += 2;
							} else {
								throw std::runtime_error("Unexpected issue in parsing, found backslash but nothing after it.");
							}
						} else {
							bytes.push_back(static_cast<uint8_t>(line[i]));
							i++;
						}
					}
					break;
				} else {
					std::cout << "Offending char: " << line[i] << std::endl;
					throw std::runtime_error("Unknown character whilst trying to parse string flags");
				}
			}

			if (split_bytes) {
				std::vector<uint8_t> old_bytes = bytes;
				bytes.clear();
				for (size_t j = 0; j < old_bytes.size(); j++) {
					bytes.push_back(old_bytes[j]);
					if (j < (old_bytes.size()-1)) {
						bytes.push_back(byte_to_split);
					}
				}
			}

			if (starts_with_null_byte) {
				bytes.insert(bytes.begin(), 0x00);
			}

			if (ends_with_null_byte) {
				bytes.push_back(static_cast<uint8_t>(0));
			}
		}

		return bytes;
	}

	void parse_start_block () {
		flush_last_stored_bytes();

		temp_buffers.push_back(DataBuffer(DataType::Block));
		i++;
	}
	void parse_end_block () {
		if (temp_buffers.size() == 0) { 
			throw std::runtime_error("Found closing bracket but there was no matching opening bracket");
		} else if (temp_buffers.at(temp_buffers.size() - 1).type != DataType::Block) {
			throw std::runtime_error("Found closing bracket but encountered type that was not block, it was: " + temp_buffers.at(temp_buffers.size() - 1).type);
		}
		store_current_bytes();
		i++;
	}

	std::vector<uint8_t> parse_number (NumberType type, size_t min_byte_count = 0) {
		size_t digit_count = 0;
		size_t start = i;

		for (size_t j = i; j < line.length(); j++) {
			if (
				(type == NumberType::Hex && std::isxdigit(line[j])) ||
				(type == NumberType::Decimal && std::isdigit(line[j])) ||
				(type == NumberType::Octal && is_octal_digit(line[j])) ||
				(type == NumberType::Binary && is_binary_digit(line[j]))
			) {
				digit_count++;
			} else if (is_ending_character(line[j])) {
				break;
			} else {
				std::cout << "Char: " << line[j] << std::endl;
				throw std::runtime_error("Found non-digit when a valid digit was expected.");
			}
		}

		std::string num = line.substr(start, digit_count);
		std::vector<uint8_t> bytes;

		if (type == NumberType::Hex) {
			bytes = parse_hex_string_to_bytes(num);
		} else if (type == NumberType::Octal) {
			// TODO: make octal
		} else if (type == NumberType::Binary) {
			bytes = parse_bin_string_to_bytes(num);
		} else if (type == NumberType::Decimal) {
			bytes = parse_dec_string_to_bytes(num);
		} else {
			throw std::runtime_error("Unhandled number type!");
		}

		if (min_byte_count != 0) {
			extend_vector_to_min(bytes, min_byte_count, static_cast<uint8_t>(0));
		}

		i = start + digit_count;
		return bytes;
	}
};


#ifdef M_TEST
// #region TESTING
void test_concat_vector () {
	std::vector<uint8_t> dest1;
	dest1.push_back(5);

	std::vector<uint8_t> src1;
	src1.push_back(10);
	src1.push_back(42);

	concat_vectors(dest1, src1);

	assert(dest1.size() == 3);
	assert(dest1.at(0) == 5);
	assert(dest1.at(1) == 10);
	assert(dest1.at(2) == 42);
	// Doesn't modify original
	assert(src1.size() == 2);
	assert(src1.at(0) == 10);
	assert(src1.at(1) == 42);
}

void test_parse_dec_string_to_bytes () {
	std::vector<uint8_t> res1 = decimalStringToByteVector("4204");
	assert(res1.size() == 2);
	assert(res1[1] == 0x6c);
	assert(res1[0] == 0x10);
	// TODO: add more
	/*
	for (unsigned long i = 0; i < ULONG_MAX; i++) {
		char res_str[10];
		snprintf(res_str, 10, "%lu", i);
		std::vector<uint8_t> res = decimalStringToByteVector(res_str);
		std::cout << "Str val: " << res_str <<  ", Hex bytes: [";
		for (uint8_t item : res) {
			std::cout << u8_to_hex_string(item) << ", ";
		}
		std::cout << "]";
		unsigned long gen = bytes_to_n<unsigned long>(res);
		std::cout << ", gen: " << gen << std::endl;

		assert(gen == i);
	}*/
}

void test_bytes_to_n () {
	std::vector<uint8_t> test1;
	test1.push_back(0x10);
	test1.push_back(0x6c);

	assert(bytes_to_n<unsigned long>(test1) == 4204);
}

// TODO: Add more tests
// #endregion
#endif

int main (int argc, char** argv) {
	#ifdef M_TEST
	test_concat_vector();
	test_parse_dec_string_to_bytes();
	test_bytes_to_n();
	#endif

	if (argc < 3) {
		std::cout << "Sorry, but this requires two arguments. Usage: ./program [file_in] [file_out]" << std::endl;
		return 1;
	}

	char* file_in = argv[1];
	char* file_out = argv[2];

	std::cout << "Starting.." << std::endl;

	HexSyn hex;
	hex.parse_files(file_in, file_out);

	std::cout << "Done." << std::endl;

	return 0;
}