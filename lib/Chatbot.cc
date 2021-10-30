#include "Chatbot.h"
#include <cstring>
#include "Util.h"

const string t_intent="Intent name: ";
const string t_response="New response: ";
const string t_confirm="Confirm [Y/N]?:";
const string t_newexample="New example: ";
const string t_idexample="Example id:";
const string t_algorithm = "Enter algorithm: ";
const string t_threshold = "Enter threshold: ";
const string t_similarity = "Similarity: ";

Chatbot::Chatbot(float threshold,const char similarity[])
{
	if(threshold < 0 || threshold>1)
		throw(ERR_THRESHOLD);
	else
		this->threshold=threshold;
	if(strcmp(similarity,"jc")!=0 && strcmp(similarity,"ng")!=0)
		throw(ERR_SIMILARITY);
	else
		strcpy(this->similarity,similarity);
	
}

float Chatbot::getThreshold() const { return threshold; }

string Chatbot::getSimilarity() const {	return similarity; }

int Chatbot::searchIntent(string name) const 
{
	int nintent=-1;
	
	
	for(unsigned i = 0; i<intents.size();i++)
	{
		
		if(intents[i]->getName()==name)
		{
			nintent=i;
		}
	}
	
	return nintent;
	
}
bool Chatbot::addIntent(Intent* pIntent)
{
	bool conseguido=false;
	if(searchIntent(pIntent->getName())!=-1)
		throw(ERR_INTENT);
	else
	{
		intents.push_back(pIntent);
		conseguido=true;
	}
		
	return conseguido;
}
bool Chatbot::deleteIntent(string name, bool askConfirm)
{
	bool conseguido=false;
	string tempo=name;
	int nintent=-1;
	char confirmar='y';
	
	if(tempo=="")	//Preguntar Nombre si vacío
	{
		cout<<t_intent;
		getline(cin,tempo);
	}
	nintent=searchIntent(tempo);
	if(nintent==-1)	// Si no existe lanzar error
		Util::error(ERR_INTENT);
	else 									// Si existe, comenzar borrado
	{
		if(askConfirm)
		{
			do
			{
				cout<<t_confirm;cin>>confirmar;		//Pregunto confirmación
				confirmar=tolower(confirmar);
			}while(confirmar!='y' && confirmar!='n');
		}
		if(confirmar=='y')
		{
			intents.erase(intents.begin()+nintent);
			conseguido=true;
		}
	}
	
	return conseguido;	
}
bool Chatbot::addExample(string name)
{
	bool conseguido=false;
	string tempo=name,texto="";	
	vector <string> tokens;
	int nintent=-1;
	if(tempo=="")	//Preguntar name si vacío
	{
		cout<<t_intent;
		getline(cin,tempo);
	}
	nintent=searchIntent(tempo);
	if(nintent==-1)
		Util::error(ERR_INTENT);
	else
	{
		while(texto!="q")
		{
			cout<<t_newexample;
			getline(cin,texto);
			if(texto!="q")
			{	
				try // Constructor Example con Excepción si vacío + Intent.AddExample() con excepción si MaxExamples
				{
					Example tempExample(texto);
					tokens=tempExample.getTokens();
					intents[nintent]->addExample(tempExample);
					conseguido=true;
				}
				catch(Error e)
				{
					Util::error(e);
				}
			}
		}
	}
	return conseguido;
	
	
	
}

bool Chatbot::deleteExample(int id)
{
	bool conseguido=false;
	unsigned temptam,temptam2;
	int tempo=id;
	
	if(tempo==0)
	{
		cout<<t_idexample;
		cin>>tempo;
		cin.ignore();
	}
	for(unsigned i = 0 ; i<intents.size();i++)
	{
		try
		{
			if(intents[i]->getNumExamples()>0)
			{ 
				temptam=intents[i]->getNumExamples();
				intents[i]->deleteExample(tempo);
				temptam2=intents[i]->getNumExamples();
				if(temptam!=temptam2)
					conseguido=true;
			}
		}
		catch(Error ERR_EXAMPLE)
		{
			
		}
	}
	
	if(!conseguido)
		{
			Util::error(ERR_EXAMPLE);
		}

		
	return conseguido;
}

bool Chatbot::addResponse(string name,string response)
{
	string tempo=name,tempresponse=response;
	bool conseguido=false;
	int nintent;
	
	if(tempo=="")	//Preguntar name si vacío
	{
		cout<<t_intent;
		getline(cin,tempo);
	}
	nintent=searchIntent(tempo);

	if(nintent==-1)
		Util::error(ERR_INTENT);
	else
	{
		if(tempresponse=="")	//Preguntar response si vacío
		{
			cout<<t_response;
			getline(cin,tempresponse);
		}
		intents[nintent]->addResponse(tempresponse);
		conseguido=true;
	}
	
	return conseguido;
	
}

string Chatbot::bestResponse(string query,bool debug) const
{
	float mejor=0,tempo=0;
	string respuesta="",similitudstr=getSimilarity();
	char similarity[3];
	unsigned intentwinner=-1;
	
	strcpy(similarity,similitudstr.c_str());
	
	for(unsigned i = 0; i<intents.size() ; i++)
	{
		
		tempo=intents[i]->calculateSimilarity(query,similarity);
		
		
		if(tempo>mejor)
		{
			mejor=tempo;
			intentwinner=i;
		}
	}

	if(mejor<getThreshold())
		throw(ERR_RESPONSE);
	else
	{
		//cout<<mejor<<" "<<getThreshold()<<endl;
		respuesta=intents[intentwinner]->getResponse();
		if(debug)
			respuesta="("+to_string(mejor)+")"+respuesta;
	}	
	return respuesta;
}

void Chatbot::test(bool debug) const
{	
	string comando="",respuesta;
	cout<<">> "<<Util::welcome()<<endl;
	do
	{
		cout<<"<<";
		getline(cin,comando);
			
		if(comando!="q")
		{
			try
			{
				respuesta=bestResponse(comando,debug);
				cout<<" >> "<<respuesta<<endl;
			}
			catch(Error e)
			{
				Util::error(e);
			}
		}
	}while(comando!="q");
}

void Chatbot::configure(float threshold,const char similarity[])
{
	float max=1.000,min=0.000;
	string similitud=similarity;
	string jacc="jc",ngrams="ng";
	
	if(threshold==-1)
	{
		cout<<t_threshold;
		cin>>threshold;			//si thresh vacío lo pido
		cin.get();
	}
	if(threshold<min || threshold>max )
		Util::error(ERR_THRESHOLD);			//error si está fuera de los límites
	else
		this->threshold=threshold;			
		
	if(string(similarity).length()==0)	//si similarity vacía la pido
	{
		cout<<t_algorithm;
		getline(cin,similitud);
	}
	
	if(similitud!=jacc && similitud!=ngrams)
		Util::error(ERR_SIMILARITY);
	else
		strcpy(this->similarity,similitud.c_str());
	
}

ostream& operator <<(ostream& os, const Chatbot chatbot)
{
	string similar="Jaccard";
	float nexamples=0;
	
	os<<t_similarity;
	if(strcmp(chatbot.getSimilarity().c_str(),"jc")==0)
		similar="Jaccard";
	else if(strcmp(chatbot.getSimilarity().c_str(),"ng")==0)
		similar="N-grams";
	os<<similar<<endl;
	os<<"Threshold: ";
	os<<chatbot.getThreshold()<<endl;
	
	for(unsigned i = 0; i<chatbot.intents.size() ; i++)
	{	
		os<<*chatbot.intents[i]<<endl;
		nexamples=nexamples+chatbot.intents[i]->getNumExamples();
	}
	
	os<<"Total intents: "<<chatbot.intents.size()<<endl;
	os<<"Total examples: "<<nexamples<<endl;
	if(nexamples!=0 && chatbot.intents.size()!=0)
		os<<"Examples per intent: "<<nexamples/chatbot.intents.size()<<endl;
	else
		os<<"Examples per intent: "<<0<<endl;
	
	
	
	return os;
}
