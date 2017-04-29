#ifndef _emulate_arduino_h
#define _emulate_arduino_h

#define F(x) x			// redefine macro from arduino

// class simulate arduino Serial class
class Ser
{
	public:
	void ini(void);
	void set_send_function(int (*fnc)(const char*, const int)); // can set function for send line etc. by tcp socket, if is NULL no print
	void print(void);
	void println(void);
	void print(const char* s );
	void println(const char* s );
	void print(int n );
	void print(long n );
	void println(long n );
	void write(const char* s );
	void write(const char s);
	
	void println(unsigned long n, int i);
	void println(unsigned long* n, int i);

	private:
	void enter(); // function who call function set in set_send_function()
	
};

// prototype other function from arduino
long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif

