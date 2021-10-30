#include "Intent.h"
#include "Example.h"
#include "Util.h"
#include <cstring>

const char jacc[3]="jc";
const char ng[3]="ng";
const string t_intent="Intent: ";
const string t_response="Response: ";

Intent::Intent(string name)
{
	if(name=="\0")
		throw(ERR_EMPTY);
	this->name=name;
}

vector <Example> Intent::getExamples() const { return examples; }

string Intent::getName() const {return name;}

unsigned Intent::getNumExamples() const { return examples.size(); }

string Intent::getResponse() const { return response; }

unsigned Intent::BuscaExamples(int id , vector<Example> examples ,bool &encontrado)
{
	unsigned i = 0;
	encontrado=false;
	
	while(encontrado== false && i<examples.size())
	{
		if( id == examples[i].getId() )
			encontrado=true;
		else
		{
			i++;
		}
	}
	if(!encontrado)
		throw(ERR_EXAMPLE);
	return i;
}

void Intent::addExample (const Example &example)
{
	if(getNumExamples()>KMAXEXAMPLES)
		throw(ERR_MAXEXAMPLES);
	else
	{
		examples.push_back(example);
	}
}

void Intent::deleteExample (int id)
{
	int idaborrar;
	bool encontrado=false;
	
	idaborrar=BuscaExamples(id,getExamples(),encontrado);
	
	if(encontrado)
		examples.erase(examples.begin()+idaborrar);
	else
	{
		throw(ERR_EXAMPLE);
	}
}

void Intent::addResponse(string response)
{
	this->response=response;
}

float Intent::calculateSimilarity(string text,char similarity[])
{
	float similitud=0;
	
	if(string(similarity)==string(jacc))
		similitud=SimilarityJC(text);
	else if (string(similarity)==string(ng))
		similitud=SimilarityNG(text);
	else 
		throw(ERR_SIMILARITY);
		
	
	return similitud;
}	

float Intent::SimilarityNG(string text) const
{
	float similitud=0;
	vector <Example> ejemplos= getExamples();
	
	for(unsigned i = 0 ; i<getNumExamples() ; i++)
		{
			if(ejemplos[i].ngramSimilarity(text)>similitud)
			{
				similitud=ejemplos[i].ngramSimilarity(text);
				//cout<<ejemplos[i].getText()<<similitud<<endl;
			}
		}
	
	
	return similitud;
	
}

float Intent::SimilarityJC(string text) const
{
	float similitud=0;
	
	vector <Example> ejemplos= getExamples();
	
	for(unsigned i = 0 ; i<getNumExamples() ; i++)
		{
			if(ejemplos[i].jaccardSimilarity(text)>similitud)
			{
				similitud=ejemplos[i].jaccardSimilarity(text);
				//cout<<ejemplos[i].getText()<<similitud<<endl;
			}
		}
	
	
	return similitud;
	
	
}

ostream& operator <<(ostream& os,const Intent &intent)
{
	vector<Example> examples = intent.getExamples();
	os<<t_intent<<intent.getName()<<endl;
	os<<t_response<<intent.getResponse()<<endl;
	for(unsigned i = 0; i<intent.getNumExamples(); i++)
	{
		os<<examples[i];
		
	}
	
	
	return os;
}
