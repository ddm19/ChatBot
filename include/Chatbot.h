#ifndef _Chatbot_
#define _Chatbot_
#include <iostream>
#include "Intent.h"
using namespace std;



class Chatbot{
	
	protected:
		float threshold;
		char similarity[3];
		int searchIntent(string name) const;
		vector <Intent*> intents;
		string bestResponse(string query,bool debug) const;
	public:
		Chatbot(float threshold=0.25,const char similarity[]="jc");
		float getThreshold() const;
		string getSimilarity() const;
		bool addIntent(Intent* pIntent);
		bool deleteIntent(string name = "",bool askConfirm=true);
		bool addExample(string name="");
		bool deleteExample(int id=0);
		bool addResponse(string name= "",string response="");
		void test(bool debug=false) const;
		void configure(float threshold=-1,const char similarity[]="");
		friend ostream& operator <<(ostream& os, const Chatbot chatbot);
		
		
		
	

};




#endif
