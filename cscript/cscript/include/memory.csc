unsigned long long alloc(int size){
	static pointer_t<byte> address = 2;
	static function<unsigned long long(int)> function_object = address;
	return function_object(size);
}

void dealloc(unsigned long long value){
	static pointer_t<byte> address = 10;
	static function<void(unsigned long long)> function_object = address;
	function_object(value);
}

unsigned long long realloc(unsigned long long value, int size){
	static pointer_t<byte> address = 18;
	static function<unsigned long long(unsigned long long, int)> function_object = address;
	return function_object(value, size);
}

bool is_allocated(unsigned long long value){
	static pointer_t<byte> address = 26;
	static function<bool(unsigned long long)> function_object = address;
	return function_object(value);
}

bool is_valid_address(unsigned long long value){
	static pointer_t<byte> address = 34;
	static function<bool(unsigned long long)> function_object = address;
	return function_object(value);
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