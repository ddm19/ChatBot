#ifndef _Intent_
#define _Intent_

#include <iostream>
#include "Example.h"


using namespace std;

class Intent
{
	protected:
		string name;
		string response;
		vector <Example> examples;
		unsigned BuscaExamples(int id , vector<Example> examples, bool &encontrado);
		float SimilarityNG(string text) const;
		float SimilarityJC(string text) const;
		
	public:
		const unsigned KMAXEXAMPLES = 10;
		Intent (string name);
		string getName() const;
		string getResponse() const;
		vector<Example> getExamples() const;
		unsigned getNumExamples () const;
		void addExample (const Example &example);
		void deleteExample (int id);
		void addResponse(string response);
		float calculateSimilarity(string text,char similarity[]);
		friend ostream& operator <<(ostream&,const Intent &intent);
	
};


#endif
