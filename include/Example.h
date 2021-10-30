#ifndef _Example_
#define _Example_

#include <iostream>
#include <vector>
using namespace std;

class Example
{
	protected:
		int id;
		string text;
		vector <string> tokens;
		static int nextid;
		unsigned Comparador(vector <string> tokens1,vector <string> tokens2) const;
		vector <string> DiviGramas(string palabra) const;
		bool AntiDuplicados(vector <string> palabras, string adicionar) const;
		vector <string> SoloTriGramas(vector <string> gramas) const; 
	public:
		Example(string t); // Constructor de clase
		~Example();			//Destructor (BORRAR)
		static void resetNextId();
		int getId() const;
		string getText() const;
		vector <string> getTokens() const;
		float jaccardSimilarity(string text)const;
		float ngramSimilarity(string text)const;
		friend ostream& operator<<(ostream&,const Example &example);
	
	
	
};

#endif
