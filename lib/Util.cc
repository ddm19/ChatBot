#include "Util.h"

using namespace std;

const int KSIZE=10;

const string greetings[KSIZE]={
  "Hola, ¿en qué puedo ayudarte?",
  "¿Qué puedo hacer hoy por ti?",
  "Oh, gran prócer de los bits, ¿cuál es tu deseo?",
  "Buenas",
  "¿Qué quieres?",
  "¿Otra vez necesitas ayuda?",
  "¡Hola! Soy Megabot 3000. ¿En qué puedo ayudarte?",
  "Bonito día para ser Megabot 3000",
  "Pregunta sin miedo",
  "Soy todo oídos"
};

void Util::error(Error e){
  switch(e){
    case ERR_OPTION:
      cout << "ERROR: wrong menu option" << endl;
      break;
    case ERR_INTENT:
      cout << "ERROR: wrong intent name" << endl;
      break;
    case ERR_EXAMPLE:
      cout << "ERROR: wrong example id" << endl;
      break;
    case ERR_RESPONSE:
      cout << "Lo siento, pero no tengo respuesta para eso" << endl;
      break;
    case ERR_FILE:
      cout << "ERROR: cannot open file" << endl;
      break;
    case ERR_THRESHOLD:
      cout << "ERROR: the threshold value must be between 0 and 1" << endl;
      break;
    case ERR_SIMILARITY:
      cout << "ERROR: valid values are \"jc\" (Jaccard) and \"ng\" (n-grams)" << endl;
      break;
    case ERR_EMPTY:
      cout << "ERROR: empty strings are not permited" << endl;
      break;
    case ERR_MAXEXAMPLES:
      cout << "ERROR: cannot add more examples" << endl;
  }
}

string Util::welcome()	
{
	int position = rand()%KSIZE;
	string saludo;
	
	saludo=greetings[position];

	return saludo;
}

string Util::Donlimpio(string palabra)
{
	string tempo="";
	
	
	
	for(unsigned i = 0; i<=palabra.length(); i++)
	{
	
	
	
		if(isupper(palabra[i]))
		{
				palabra[i]=tolower(palabra[i]);
			
		}
		
		if(isalnum(palabra[i]))
		{
				tempo=tempo+palabra[i];
			
		}
	
	
	
	}
	if(tempo[tempo.length()-1]=='s' || tempo[tempo.length()-1]=='s')
	{
	
		tempo.erase(tempo.length()-1);
	
	}

	return tempo;
}

vector <string> Util::extractTokens(string text)
{
	vector <string> tokens;
	unsigned ntokens=0;
	string palabra=text,tokensuma="";
	
	for(unsigned i = 0; i<palabra.length();i++)	// suma carácteres hasta formar una palabra (encontrar espacios)
		{
				if(palabra[i]!=' ')
					tokensuma=tokensuma+palabra[i];
					
				if((palabra[i]==' ' && palabra[i+1]!=' ') || (palabra[i]!=' ' && palabra[i+1]=='\0'))
				{	
					tokensuma=cleanString(tokensuma);				//quito eñes y cosas que molestan en los tokens (mayus . _ ?...)			
					tokensuma=Donlimpio(tokensuma);
					
					if(tokensuma.length()>=1)
					{
						ntokens++;
						tokens.push_back(tokensuma);		
						tokensuma="";	
					}
					
				}
				
		}
	
	
	
	
	return tokens;
}


