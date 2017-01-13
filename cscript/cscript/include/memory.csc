unsigned long long alloc(int size){
	static pointer_t<byte> address = 2;
	static function<unsigned long long(int)> function_object = address;
	return function_object(size);
}

void dealloc(unsigned long long address_value){
	static pointer_t<byte> address = 10;
	static function<void(unsigned long long)> function_object = address;
	function_object(address_value);
}

unsigned long long realloc(unsigned long long address_value, int size){
	static pointer_t<byte> address = 18;
	static function<unsigned long long(unsigned long long, int)> function_object = address;
	return function_object(address_value, size);
}

bool is_allocated(unsigned long long address_value){
	static pointer_t<byte> address = 26;
	static function<bool(unsigned long long)> function_object = address;
	return function_object(address_value);
}

bool is_valid_address(unsigned long long address_value){
	static pointer_t<byte> address = 34;
	static function<bool(unsigned long long)> function_object = address;
	return function_object(address_value);
}

bool is_block_head(unsigned long long address_value){
	static pointer_t<byte> address = 42;
	static function<bool(unsigned long long)> function_object = address;
	return function_object(address_value);
}

unsigned int get_block_size(unsigned long long address_value){
	static pointer_t<byte> address = 50;
	static function<unsigned int(unsigned long long)> function_object = address;
	return function_object(address_value);
}

any get_block_object(unsigned long long address_value){
	static pointer_t<byte> address = 58;
	static function<any(unsigned long long)> function_object = address;
	return function_object(address_value);
}

decltype(int) get_block_type(unsigned long long address_value){
	typedef decltype(int) return_type;
	static pointer_t<byte> address = 66;
	static function<return_type(unsigned long long)> function_object = address;
	return function_object(address_value);
}

void copy(unsigned long long source, unsigned int size, unsigned long long destination){
	static pointer_t<byte> address = 74;
	static function<void(unsigned long long, unsigned int, unsigned long long)> function_object = address;
	function_object(source, size, destination);
}

void set(unsigned long long destination, unsigned int size, int value){
	static pointer_t<byte> address = 82;
	static function<void(unsigned long long, unsigned int, int)> function_object = address;
	function_object(destination, size, value);
}

any move(ref any object){
	static pointer_t<byte> address = 90;
	static function<any(any)> function_object = address;
	return function_object(object);
}

void watch(unsigned long long destination, unsigned int size, int value){
	static pointer_t<byte> address = 98;
	static function<bool(unsigned long long)> function_object = address;
	return function_object(value);
}

void unwatch(unsigned long long destination, unsigned int size, int value){
	static pointer_t<byte> address = 106;
	static function<bool(unsigned long long)> function_object = address;
	return function_object(value);
}

pointer_t<char> string_new(int size){
	static pointer_t<byte> address = 114;
	static function<pointer_t<char>(int)> function_object = address;
	return function_object(size);
}

pointer_t<char> string_resize(unsigned long long address_value, int size){
	static pointer_t<byte> address = 122;
	static function<pointer_t<char>(unsigned long long, int)> function_object = address;
	return function_object(address_value, size);
}

pointer_t<char> string_insert(unsigned long long address_value, int index, const pointer_t<char> value){
	static pointer_t<byte> address = 130;
	static function<pointer_t<char>(unsigned long long, int, pointer_t<char>)> function_object = address;
	return function_object(address_value, index, value);
}

pointer_t<char> string_erase(unsigned long long address_value, int index, unsigned int count = -1){
	static pointer_t<byte> address = 138;
	static function<pointer_t<char>(unsigned long long, int, unsigned int)> function_object = address;
	return function_object(address_value, index, count);
}

pointer_t<char> string_clear(unsigned long long address_value){
	static pointer_t<byte> address = 146;
	static function<void(unsigned long long)> function_object = address;
	return function_object(address_value);
}

/*namespace memory{
	unsigned long long alloc(int size){
		static address = pointer_t<byte>(2);
		static function<unsigned long long(int)> function_object = address;
		return function_object(size);
	}

	void dealloc(unsigned long long value){
		static address = pointer_t<byte>(10);
		static function<void(unsigned long long)> function_object = address;
		function_object(value);
	}

	unsigned long long realloc(unsigned long long value, int size){
		static address = pointer_t<byte>(18);
		static function<unsigned long long(unsigned long long, int)> function_object = address;
		return function_object(value, size);
	}

	bool is_allocated(unsigned long long value){
		static address = pointer_t<byte>(24);
		static function<bool(unsigned long long)> function_object = address;
		return function_object(value);
	}

	bool is_valid_address(unsigned long long value){
		static address = pointer_t<byte>(32);
		static function<bool(unsigned long long)> function_object = address;
		return function_object(value);
	}
}*/