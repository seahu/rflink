#include "EmulateArduino.h"
#include <sstream>      // std::ostringstream
#include <pthread.h> // for mutex

int (*send_function)(const char*, const int);  // define pointer to send_function (this my by set for function send line by socket)

// print lilne mutex (safe, that print line will not break print from another threader)
pthread_mutex_t print_line_mutex;

// output string buffer
std::ostringstream outBuf;

	/*
	 * add end line for Ser::prints functions and send this line throught socket by pointer into send_function
	 */
	void Ser::enter(){
		pthread_mutex_lock(&print_line_mutex); // lock mutex

		outBuf << "\r\n";
		if ( send_function!=NULL ) (*send_function)(outBuf.str().c_str(),outBuf.str().size());
		outBuf.str(""); // erase  buffer for nex use

		pthread_mutex_unlock(&print_line_mutex); // unlock mutex
	}

	void Ser::set_send_function(int (*fnc)(const char*, const int)=NULL){
		send_function=fnc;
	}

	void Ser::ini(void){ // ini mutext for print line
		pthread_mutex_init(&print_line_mutex, NULL);
		send_function=NULL;
	}


	void Ser::print(void){
		return;
	}

	void Ser::println(void){
		return Ser::enter();
	}

	void Ser::print(const char* s ){
		outBuf << s << std::flush;
		return;
	}

	void Ser::println(const char* s ){
		outBuf << s;
		return Ser::enter();
	}

	void Ser::print(int n ){
		outBuf << n;
		return;
	}

	void Ser::print(long n ){
		outBuf << n;
		return;
	}

	void Ser::println(long n ){
		//std::cout << n << "\n" << std::flush;
		outBuf << n ;
		return Ser::enter();
	}

	void Ser::write(const char* s ){
		outBuf << s;
		return;
	}
	void Ser::write(const char s ){
		outBuf << s ;
		return;
	}

	void Ser::println(unsigned long n,  int i){
		if (i==1) { //hex print number
			outBuf << std::hex << n << std::dec; 
		}
		else {
			outBuf << n;
		}
		return Ser::enter();
	}

	void Ser::println(unsigned long* n,  int i){
		if (i==1) { //hex print number
			outBuf << std::hex << n << std::dec;
		}
		else {
			outBuf << n;
		}
		return Ser::enter();
	}


// map function from arduino
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

